#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__ 1
#include "PluginLibrary.h"
#include "Plugin.h"
#include <vector>
#include <memory>
#include <unordered_map>

class PluginManager
{
    private:
        std::unordered_map<std::string, std::unique_ptr<PluginLibrary> > _libraries;
        std::vector<std::unique_ptr<Plugin> > _plugins;
        PluginLibrary& load_library(std::string path);
    public:
        PluginManager();
        Plugin& load_plugin(std::string path);

        virtual ~PluginManager();
};

#endif
