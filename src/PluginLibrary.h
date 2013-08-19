#ifndef __PLUGINLIBRARY_H__
#define __PLUGINLIBRARY_H__ 1

#include <memory>
#include <string>
#include <unordered_map>
#include "plugin_i.h"
#include "dynload.h"
#include "Plugin.h"

class Plugin;
class PluginFunction;

typedef void (*fptr)();

typedef int (*create_fptr)(void);
typedef int (*destroy_fptr)(int);
typedef int (*argument_fptr)(int, const char*, const char*);
typedef int (*start_fptr)(int);

extern "C" typedef int register_plugin_func_t(void*, int, const char*, const char*);
extern "C" typedef int register_plugin_func_c_t(void*, const char*, const char*, fptr);

class PluginLibrary
{
    friend class Plugin;
    friend class PluginFunction;

    private:
        DLLib* _libptr;
        std::unordered_map<int, std::unique_ptr<Plugin> > _plugins;
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
        plugin_interface_t* _interface;

    public:
        void load(std::string& path);
        Plugin& create();
        void destroy(Plugin& plugin);

        virtual ~PluginLibrary();
};

#endif
