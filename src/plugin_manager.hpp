#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__ 1
#include "plugin_library.hpp"
#include "plugin.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

class plugin_manager {
    private:
        std::unordered_map<std::string, std::unique_ptr<plugin_library> > _libraries;
        plugin_library& load_library(std::string path);
    public:
        plugin_manager();
        plugin& load_plugin(std::string path);

        virtual ~plugin_manager();
};

#endif
