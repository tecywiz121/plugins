#ifndef __PLUGIN_H__
#define __PLUGIN_H__ 1

class PluginLibrary;

#include <memory>
#include <unordered_map>
#include "PluginLibrary.h"

class PluginFunction;

class Plugin
{
    friend class PluginLibrary;
    friend class PluginFunction;
    private:
        const PluginLibrary& _library;
        std::unordered_map<std::string, std::unique_ptr<PluginFunction> > _functions;

        void register_plugin_function(std::unique_ptr<PluginFunction> func);

    public:
        Plugin(const PluginLibrary& library, int id);
        const int id;
        void start();
        void argument(std::string key, std::string value);
        PluginFunction& get_function(const std::string& name);

        virtual ~Plugin();
};

#endif
