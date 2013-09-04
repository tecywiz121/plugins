#include <memory>
#include <iostream>
#include "plugin.hpp"
#include "plugin_library.hpp"
#include "plugin_manager.hpp"
#include "function.hpp"

plugin::plugin(const plugin_library& library, int newid)
    : _id(newid), _library(library)
{
}

void plugin::start()
{
    if (_library._start(_id) == 0) {
        throw std::exception();
    }
}

void plugin::argument(std::string key, std::string value)
{
    if (_library._argument(_id, key.c_str(), value.c_str()) == 0) {
        throw std::exception();
    }
}

void plugin::register_function(std::unique_ptr<function> func)
{
    _functions[func->name()] = std::move(func);
}

function& plugin::get_function(const std::string& name)
{
    return *_functions.at(name);
}

plugin::~plugin()
{
    _library._destroy(_id);
}
