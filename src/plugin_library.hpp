#ifndef __PLUGINLIBRARY_H__
#define __PLUGINLIBRARY_H__ 1

#include <memory>
#include <string>
#include <unordered_map>
#include "plugin_i.h"
#include "dynload.h"
#include "plugin.hpp"

class plugin;
class plugin_function;

typedef void (*fptr)();

typedef int (*create_fptr)(void);
typedef int (*destroy_fptr)(int);
typedef int (*argument_fptr)(int, const char*, const char*);
typedef int (*start_fptr)(int);

extern "C" typedef int register_plugin_func_t(void*, int, const char*, const char*);
extern "C" typedef int register_plugin_func_c_t(void*, const char*, const char*, fptr);

class plugin_library
{
    friend class plugin;
    friend class plugin_function;

    private:
        DLLib* _libptr;
        std::unordered_map<int, std::unique_ptr<plugin> > _plugins;
        int register_plugin_func(int id, std::string& name, std::string& sig);
        int register_plugin_func_c(std::string& name, std::string& sig, fptr func);
        static register_plugin_func_t _register_plugin_func;
        static register_plugin_func_c_t _register_plugin_func_c;

    protected:
        // Plugin API
        create_fptr _create;
        destroy_fptr _destroy;
        argument_fptr _argument;
        start_fptr _start;
        struct plugin_interface* _interface;

    public:
        void load(std::string& path);
        plugin& create();
        void destroy(plugin& plugin);

        virtual ~plugin_library();
};

#endif
