#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "kvec.h"
#include "khash.h"
#include "util.h"
#include "plugin_i.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// struct call_info - A structure containing a single call to a lua function
//
struct call_info {
    lua_State* state;
    const char* function_name;
    int n_args;
    int called;
};

static struct call_info *call_info_init(lua_State* state, const char* function_name)
{
    struct call_info *inst = (struct call_info *)malloc(sizeof(struct call_info));
    inst->state = state;
    inst->function_name = function_name;
    inst->n_args = 0;
    inst->called = 0;
    return inst;
}

static void call_info_destroy(struct call_info *inst)
{
    if (!inst->called)
    {
        // For whatever reason, the function was not called, so we have to clean up the stack
        lua_pop(inst->state, inst->n_args);
    }

    free(inst);
}

//
// struct instance - An instance of the lua intepreter and the path to its script
//
struct instance {
    lua_State* state;
    const char* path;
    kvec_t(struct call_info *) callstack;
};

static struct instance *instance_init(lua_State* state)
{
    struct instance *inst = (struct instance *)malloc(sizeof(struct instance));
    inst->state = state;
    kv_init(inst->callstack);
    return inst;
}

static void instance_destroy(struct instance *inst)
{
    lua_close(inst->state);
    kv_destroy(inst->callstack);
    free(inst);
}

struct plugin_interface plugin_interface;

// Initialize the hashmap
KHASH_MAP_INIT_INT(m32, struct instance *)
static khash_t(m32) *states = 0;

// Stores the next id to assign to a lua plugin
static int _nextid = 1;

// Libraries to load in each lua state
static const luaL_reg lualibs[] =
{
    { "base",       luaopen_base },
    { NULL,         NULL }
};

// A function to open up all the Lua libraries you declared above.
static void openlualibs(lua_State *l)
{
    const luaL_reg *lib;

    for (lib = lualibs; lib->func != NULL; lib++)
    {
        lib->func(l);
        lua_settop(l, 0);
    }
}

//
// Lua-C Interface
//
static int reg_plugin_func(lua_State *l)
{
    const char* name = lua_tostring(l, 1);
    const char* sig = lua_tostring(l, 2);


    // Find the matching id for the given state
    for (khiter_t k = kh_begin(states); k != kh_end(states); ++k)
    {
        if (kh_exist(states, k))
        {
            int key = kh_key(states, k);
            struct instance *val = kh_val(states, k);

            if (val->state == l)
            {
                if (plugin_interface.register_func(plugin_interface.host, key, name, sig))
                {
                    // Registered properly
                    return 0;
                }
                else
                {
                    // Register failed in host
                    lua_pushstring(l, "host could not register function");
                    return 1;
                }
            }
        }
    }

    // The given lua state isn't in our states hashtable
    lua_pushstring(l, "could not find plugin to match lua state");
    return 1;
}

// Register functions available to the script
static void regluafuncs(lua_State *l)
{
    lua_pushcfunction(l, reg_plugin_func);
    lua_setglobal(l, "register_function");
}


//
// Loader Interface
//
int create(void)
{
    if (!states)
    {
        states = kh_init(m32);
    }

    lua_State* l = lua_open();
    openlualibs(l);
    regluafuncs(l);

    struct instance *inst = instance_init(l);

    // Find an unused key
    int ret = 0;
    int id = -1;
    khint_t key;
    do
    {
        id = _nextid++;
        key = kh_put(m32, states, id, &ret);
    } while (0 == ret);

    if (ret > 0)
    {
        kh_value(states, key) = inst;
        return id;
    }
    else
    {
        return 0;
    }
}

int destroy(int id)
{
    khint_t key = kh_get(m32, states, id);
    if (key != kh_end(states)) // check thet key exists
    {
        struct instance* inst = kh_val(states, key);
        kh_del(m32, states, key);
        instance_destroy(inst);
        return 1;
    }
    else
    {
        return 0;
    }
}

int argument(int id, char* key, char* value)
{
    if (strcmp("script", key) == 0)
    {
        khint_t key = kh_get(m32, states, id);
        if (key != kh_end(states))
        {
            struct instance* inst = kh_val(states, key);
            inst->path = value;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

int start(int id)
{
    khint_t key = kh_get(m32, states, id);
    if (key != kh_end(states))
    {
        struct instance* inst = kh_val(states, key);
        printf("Starting script: %s\n", inst->path);
        if (luaL_dofile(inst->state, inst->path))
        {
            return 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

//
// Plugin Interface
//
static void* begin_call(int id, const char* name)
{
    khint_t key = kh_get(m32, states, id);
    if (key != kh_end(states))
    {
        // TODO: Keep track of which functions have been registered
        struct instance* inst = kh_val(states, key);
        struct call_info *info = call_info_init(inst->state, name);

        // Push the function to call onto the lua stack
        lua_getglobal(inst->state, name);
        info->n_args++;

        // Push our call_info onto our callstack
        kv_push(struct call_info*, inst->callstack, info);
        return inst;
    }
    else
    {
        return 0;
    }
}

static void end_call(void* call)
{
    struct instance *inst = (struct instance *)call;
    struct call_info *info = kv_pop(inst->callstack);
    call_info_destroy(info);
}

static void ret_void(void* call)
{
    struct instance *inst = (struct instance *)call;
    struct call_info *info = kv_A(inst->callstack, kv_size(inst->callstack) - 1);
    info->called = true;
    if (lua_pcall(info->state, info->n_args - 1, 0, 0))
    {
        printf("mod_lua: call to function `%s` failed\n", info->function_name);
    }
}

static void ret_not_implemented(void* call)
{
    struct instance *inst = (struct instance *)call;
    struct call_info *info = kv_A(inst->callstack, kv_size(inst->callstack) - 1);
    luaL_error(info->state, "return type not implemented yet");
}

#define RET_FUNC(name, type, lua_type)                                                      \
    static type ret_##name(void* call)                                                      \
    {                                                                                       \
        struct instance *inst = (struct instance *)call;                                    \
        struct call_info *info = kv_A(inst->callstack, kv_size(inst->callstack) - 1);       \
        info->called = true;                                                                \
        if (lua_pcall(info->state, info->n_args - 1, 1, 0))                                 \
        {                                                                                   \
            printf("mod_lua: call to function `%s` failed\n", info->function_name);         \
        }                                                                                   \
        if (!lua_is##lua_type(info->state, -1))                                             \
        {                                                                                   \
            luaL_error(info->state,                                                         \
                "function '%s' should return " #lua_type, info->function_name);             \
        }                                                                                   \
        return (type)lua_to##lua_type(info->state, -1);                                     \
    }

RET_FUNC(bool, bool, number)
RET_FUNC(char, signed char, number)
RET_FUNC(uchar, unsigned char, number)
RET_FUNC(short, short, number)
RET_FUNC(ushort, unsigned short, number)
RET_FUNC(int, int, number)
RET_FUNC(uint, unsigned int, number)
RET_FUNC(long, long, number)
RET_FUNC(ulong, unsigned long, number)
RET_FUNC(long_long, long long, number)
RET_FUNC(ulong_long, unsigned long long, number)
RET_FUNC(float, float, number)
RET_FUNC(double, double, number)

struct plugin_interface plugin_interface = {
    // My callbacks
    .begin_plugin_call = &begin_call,
    .end_plugin_call = &end_call,

    .plugin_invoke = {

        .ret_void = &ret_void,
        .ret_bool = &ret_bool,
        .ret_char = &ret_char,
        .ret_uchar = &ret_uchar,
        .ret_short = &ret_short,
        .ret_ushort = &ret_ushort,
        .ret_int = &ret_int,
        .ret_uint = &ret_uint,
        .ret_long = &ret_long,
        .ret_ulong = &ret_ulong,
        .ret_long_long = &ret_long_long,
        .ret_ulong_long = &ret_ulong_long,
        .ret_float = &ret_float,
        .ret_double = &ret_double,
        .ret_pointer = (void* (*)(void*))(&ret_not_implemented),
        .ret_c_str = (const char* (*)(void*))(&ret_not_implemented),
    },
};

#ifdef __cplusplus
}
#endif
