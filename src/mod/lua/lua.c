#include <stdlib.h>
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "khash.h"
#include "util.h"
#include "plugin_i.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct instance {
    lua_State* state;
    const char* path;
} instance_t;

static instance_t *instance_init(lua_State* state)
{
    instance_t *inst = (instance_t *)malloc(sizeof(instance_t));
    inst->state = state;
    return inst;
}

static void instance_destroy(instance_t *inst)
{
    lua_close(inst->state);
    free(inst);
}

// Plugin Interface
plugin_interface_t plugin_interface;

// Initialize the hashmap
KHASH_MAP_INIT_INT(m32, instance_t *)
khash_t(m32) *states = 0;

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
// The API expected by the loader
//

int create(void)
{
    if (!states)
    {
        states = kh_init(m32);
    }

    lua_State* l = lua_open();
    openlualibs(l);

    instance_t *inst = instance_init(l);

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
        instance_t* inst = kh_val(states, key);
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
            instance_t* inst = kh_val(states, key);
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
        instance_t* inst = kh_val(states, key);
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

#ifdef __cplusplus
}
#endif
