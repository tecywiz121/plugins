#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>

#include "plugin_description.hpp"
#include "plugin_manager.hpp"
#include "plugin_library.hpp"
#include "plugin.hpp"
#include "function.hpp"

plugin_manager::plugin_manager()
{

}

void plugin_manager::register_function(const function& func)
{
    _functions[func.name()] = &func;
}

const function& plugin_manager::get_function(std::string& name) const
{
    return *_functions.at(name);
}

plugin_library& plugin_manager::load_library(std::string path)
{
    try {
        return *_libraries.at(path);
    } catch (std::out_of_range& e) {
        std::unique_ptr<plugin_library> lib(new plugin_library(*this));
        plugin_library& result = *lib;
        lib->load(path);
        _libraries[path] = std::move(lib);
        return result;
    }
}

plugin& plugin_manager::load_plugin(std::string path)
{
    plugin_description desc(path);

    plugin_library& library = load_library(desc.module());
    plugin& plug = library.create();

    for (std::pair<std::string, std::string> p : desc.arguments()) {
        plug.argument(p.first, p.second);
    }

    return plug;
}

plugin_manager::~plugin_manager()
{
    _libraries.clear();
}
