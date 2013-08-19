#ifndef __PLUGIN_I_H__
#define __PLUGIN_I_H__ 1

#ifdef __cplusplus
#define PBool bool
extern "C" {
#else
#define PBool _Bool
#endif

typedef struct plugin_interface {
    // Host Provided Callbacks
    void* host;
    int (*register_func)(void* host, int id, const char* name, const char* sig);
    int (*register_func_c)(void* host, const char* name, const char* sig, void (*)());

    // Plugin Provided Callbacks
    void* (*begin_call)(int id, const char* name);
    void (*end_call)(void* call);

    void (*arg_bool)(void* call, PBool v);
    void (*arg_char)(void* call, signed char v);
    void (*arg_uchar)(void* call, unsigned char v);
    void (*arg_short)(void* call, short v);
    void (*arg_ushort)(void* call, unsigned short v);
    void (*arg_int)(void* call, int v);
    void (*arg_uint)(void* call, unsigned int v);
    void (*arg_long)(void* call, long v);
    void (*arg_ulong)(void* call, unsigned long v);
    void (*arg_long_long)(void* call, long long v);
    void (*arg_ulong_long)(void* call, unsigned long long v);
    void (*arg_float)(void* call, float v);
    void (*arg_double)(void* call, double v);
    void (*arg_pointer)(void* call, void* v);
    void (*arg_c_str)(void* call, const char* v);

    void (*call_void)(void* call);
    PBool (*call_bool)(void* call);
    char (*call_char)(void* call);
    unsigned char (*call_uchar)(void* call);
    short (*call_short)(void* call);
    unsigned short (*call_ushort)(void* call);
    int (*call_int)(void* call);
    unsigned int (*call_uint)(void* call);
    long (*call_long)(void* call);
    unsigned long (*call_ulong)(void* call);
    long long (*call_long_long)(void* call);
    unsigned long long (*call_ulong_long)(void* call);
    float (*call_float)(void* call);
    double (*call_double)(void* call);
    void* (*call_pointer)(void* call);
    const char* (*call_c_str)(void* call);
} plugin_interface_t;

#undef PBool
#ifdef __cplusplus
}
#endif

#endif
