#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>

#include "PluginDescription.h"
#include "PluginManager.h"
#include "PluginLibrary.h"
#include "Plugin.h"

using namespace std;

PluginManager::PluginManager()
{

}

PluginLibrary& PluginManager::load_library(string path)
{
    try
    {
        return *_libraries.at(path);
    }
    catch (out_of_range& e)
    {
        unique_ptr<PluginLibrary> lib(new PluginLibrary());
        PluginLibrary& result = *lib;
        lib->load(path);
        _libraries[path] = move(lib);
        return result;
    }
}

Plugin& PluginManager::load_plugin(string path)
{
    PluginDescription desc(path);

    PluginLibrary& library = load_library(desc.module());
    unique_ptr<Plugin> plugin = library.create();

    for (pair<string, string> p : desc.arguments())
    {
        plugin->argument(p.first, p.second);
    }

    Plugin& result = *plugin;
    _plugins.push_back(move(plugin));
    return result;
}

PluginManager::~PluginManager()
{
}
