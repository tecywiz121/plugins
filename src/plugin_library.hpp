#ifndef PLUGINLIBRARY_H
#define PLUGINLIBRARY_H 1

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "plugin_i.h"
#include "dynload.h"
#include "plugin.hpp"
#include "invocation.hpp"

class plugin;
class plugin_function;
class plugin_manager;

typedef void (*fptr)();

typedef int (*create_fptr)(void);
typedef int (*destroy_fptr)(int);
typedef int (*argument_fptr)(int, const char*, const char*);
typedef int (*start_fptr)(int);

extern "C" typedef void ext_arg_bool(void* inv, bool v);
extern "C" typedef void ext_arg_char(void* inv, signed char v);
extern "C" typedef void ext_arg_uchar(void* inv, unsigned char v);
extern "C" typedef void ext_arg_short(void* inv, short v);
extern "C" typedef void ext_arg_ushort(void* inv, unsigned short v);
extern "C" typedef void ext_arg_int(void* inv, int v);
extern "C" typedef void ext_arg_uint(void* inv, unsigned int v);
extern "C" typedef void ext_arg_long(void* inv, long v);
extern "C" typedef void ext_arg_ulong(void* inv, unsigned long v);
extern "C" typedef void ext_arg_long_long(void* inv, long long v);
extern "C" typedef void ext_arg_ulong_long(void* inv, unsigned long long v);
extern "C" typedef void ext_arg_float(void* inv, float v);
extern "C" typedef void ext_arg_double(void* inv, double v);
extern "C" typedef void ext_arg_pointer(void* inv, void* v);
extern "C" typedef void ext_arg_c_str(void* inv, const char* v);

extern "C" typedef void ext_ret_void(void* inv);
extern "C" typedef bool ext_ret_bool(void* inv);
extern "C" typedef signed char ext_ret_char(void* inv);
extern "C" typedef unsigned char ext_ret_uchar(void* inv);
extern "C" typedef short ext_ret_short(void* inv);
extern "C" typedef unsigned short ext_ret_ushort(void* inv);
extern "C" typedef int ext_ret_int(void* inv);
extern "C" typedef unsigned int ext_ret_uint(void* inv);
extern "C" typedef long ext_ret_long(void* inv);
extern "C" typedef unsigned long ext_ret_ulong(void* inv);
extern "C" typedef long long ext_ret_long_long(void* inv);
extern "C" typedef unsigned long long ext_ret_ulong_long(void* inv);
extern "C" typedef float ext_ret_float(void* inv);
extern "C" typedef double ext_ret_double(void* inv);
extern "C" typedef void* ext_ret_pointer(void* inv);
extern "C" typedef const char* ext_ret_c_str(void* inv);
extern "C" typedef void ext_end_call(void* host, void* inv);

extern "C" typedef const char* ext_describe_func(void*, const char*);
extern "C" typedef void* ext_begin_outgoing_call(void*, const char*);
extern "C" typedef int ext_register_plugin_func(void*, int, const char*,
                                                const char*);
extern "C" typedef int ext_register_plugin_func_c(void*, int, const char*,
                                                  const char*, fptr);

class plugin_library {
    friend class plugin;
    friend class plugin_function;

    private:
        static ext_describe_func _describe_func;
        static ext_register_plugin_func _register_plugin_func;
        static ext_register_plugin_func_c _register_plugin_func_c;
        static ext_begin_outgoing_call _begin_outgoing_call;

        static ext_arg_bool _arg_bool;
        static ext_arg_char _arg_char;
        static ext_arg_uchar _arg_uchar;
        static ext_arg_short _arg_short;
        static ext_arg_ushort _arg_ushort;
        static ext_arg_int _arg_int;
        static ext_arg_uint _arg_uint;
        static ext_arg_long _arg_long;
        static ext_arg_ulong _arg_ulong;
        static ext_arg_long_long _arg_long_long;
        static ext_arg_ulong_long _arg_ulong_long;
        static ext_arg_float _arg_float;
        static ext_arg_double _arg_double;
        static ext_arg_pointer _arg_pointer;
        static ext_arg_c_str _arg_c_str;

        static ext_ret_void _ret_void;
        static ext_ret_bool _ret_bool;
        static ext_ret_char _ret_char;
        static ext_ret_uchar _ret_uchar;
        static ext_ret_short _ret_short;
        static ext_ret_ushort _ret_ushort;
        static ext_ret_int _ret_int;
        static ext_ret_uint _ret_uint;
        static ext_ret_long _ret_long;
        static ext_ret_ulong _ret_ulong;
        static ext_ret_long_long _ret_long_long;
        static ext_ret_ulong_long _ret_ulong_long;
        static ext_ret_float _ret_float;
        static ext_ret_double _ret_double;
        static ext_ret_pointer _ret_pointer;
        static ext_ret_c_str _ret_c_str;
        static ext_end_call _end_call;

        plugin_manager& _manager;
        DLLib* _libptr;
        std::unordered_map<int, std::unique_ptr<plugin> > _plugins;
        std::unordered_set<std::unique_ptr<invocation> > _outgoing_invocations;

        invocation& begin_outgoing_call(std::string& name);
        int register_plugin_func(int id, std::string& name, std::string& sig);
        int register_plugin_func(int id, std::string& name, std::string& sig,
                                   fptr func);
        const std::string& describe_func(std::string& name);

    protected:
        // Plugin API
        create_fptr _create;
        destroy_fptr _destroy;
        argument_fptr _argument;
        start_fptr _start;
        struct plugin_interface* _interface;

    public:
        plugin_library(plugin_manager& manager);
        void load(std::string& path);
        plugin& create();
        void destroy(plugin& plugin);

        virtual ~plugin_library();
};

#endif
