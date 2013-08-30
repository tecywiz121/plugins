#ifndef __PLUGIN_I_H__
#define __PLUGIN_I_H__ 1

#ifdef __cplusplus
#define PBool bool
extern "C" {
#else
#define PBool _Bool
#endif

/******************************************************************************
 * The Plugin Interface                                                       *
 * --------------------                                                       *
 * This plugin interface is designed to be as flexible as possible to ease    *
 * the communication between dynamic languages like Lua and Python and static *
 * languages like C++ and C.                                                  *
 *                                                                            *
 * Both plugins and the host can expose functions by registering them with a  *
 * plugin manager.  Each function has a unique name, and a signature string,  *
 * which describes the arguments and return type.                             *
 *                                                                            *
 * Invoking functions is fairly simple.  First you begin a function call,     *
 * providing the name of the function you wish to invoke.  Next, arguments    *
 * are pushed onto a virtual "stack" from left to right.  The type of each    *
 * argument is checked against the signature string.  The function is then    *
 * invoked, and finally the call is ended.                                    *
 ******************************************************************************/

struct invoke_interface {
    void (*arg_bool)(void* invocation, PBool v);
    void (*arg_char)(void* invocation, signed char v);
    void (*arg_uchar)(void* invocation, unsigned char v);
    void (*arg_short)(void* invocation, short v);
    void (*arg_ushort)(void* invocation, unsigned short v);
    void (*arg_int)(void* invocation, int v);
    void (*arg_uint)(void* invocation, unsigned int v);
    void (*arg_long)(void* invocation, long v);
    void (*arg_ulong)(void* invocation, unsigned long v);
    void (*arg_long_long)(void* invocation, long long v);
    void (*arg_ulong_long)(void* invocation, unsigned long long v);
    void (*arg_float)(void* invocation, float v);
    void (*arg_double)(void* invocation, double v);
    void (*arg_pointer)(void* invocation, void* v);
    void (*arg_c_str)(void* invocation, const char* v);

    void (*ret_void)(void* invocation);
    PBool (*ret_bool)(void* invocation);
    char (*ret_char)(void* invocation);
    unsigned char (*ret_uchar)(void* invocation);
    short (*ret_short)(void* invocation);
    unsigned short (*ret_ushort)(void* invocation);
    int (*ret_int)(void* invocation);
    unsigned int (*ret_uint)(void* invocation);
    long (*ret_long)(void* invocation);
    unsigned long (*ret_ulong)(void* invocation);
    long long (*ret_long_long)(void* invocation);
    unsigned long long (*ret_ulong_long)(void* invocation);
    float (*ret_float)(void* invocation);
    double (*ret_double)(void* invocation);
    void* (*ret_pointer)(void* invocation);
    const char* (*ret_c_str)(void* invocation);

    void (*end_call)(void* call);
};

struct plugin_interface {
    // Host Provided Callbacks
    void* host;
    int (*register_func)(void* host, int id, const char* name, const char* sig);
    int (*register_func_c)(void* host, int id, const char* name,
                           const char* sig, void (*)());

    const char* (*describe_func)(void* host, const char* name);

    struct invoke_interface host_invoke;
    void* (*begin_host_call)(void* host, const char* name);

    // Plugin Provided Callbacks
    struct invoke_interface plugin_invoke;
    void* (*begin_plugin_call)(int id, const char* name);
};

#undef PBool
#ifdef __cplusplus
}
#endif

#endif
