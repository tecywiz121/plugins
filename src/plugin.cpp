#include <memory>
#include <iostream>
#include "plugin.hpp"
#include "plugin_library.hpp"
#include "function.hpp"

plugin::plugin(const plugin_library& library, int newid)
    : _library(library), id(newid)
{
}

void plugin::start()
{
    if (_library._start(id) == 0) {
        throw std::exception();
    }
}

void plugin::argument(std::string key, std::string value)
{
    if (_library._argument(id, key.c_str(), value.c_str()) == 0) {
        throw std::exception();
    }
}

void plugin::register_plugin_function(std::unique_ptr<plugin_function> func)
{
    _functions[func->name()] = std::move(func);
}

plugin_function& plugin::get_function(const std::string& name)
{
    return *_functions[name];
}

plugin::~plugin()
{
    _library._destroy(id);
}
