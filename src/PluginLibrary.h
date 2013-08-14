#ifndef __PLUGINLIBRARY_H__
#define __PLUGINLIBRARY_H__ 1

#include <memory>
#include <string>
#include "Plugin.h"

class Plugin;

typedef int (*create_fptr)(void);
typedef void (*destroy_fptr)(int);
typedef void (*argument_fptr)(int, const char*, const char*);
typedef void (*start_fptr)(int);

class PluginLibrary
{
    friend class Plugin;

    private:
        void* _libptr;

    protected:
        // Plugin API
        create_fptr _create;
        destroy_fptr _destroy;
        argument_fptr _argument;
        start_fptr _start;

    public:
        void load(std::string& path);
        std::unique_ptr<Plugin> create();

        virtual ~PluginLibrary();
};

#endif
