#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <exception>
#include <stdexcept>
#include "util.h"
#include "plugin_i.h"
#include "dynload.h"
#include "plugin_library.hpp"
#include "plugin_description.hpp"
#include "function.hpp"
#include "plugin_manager.hpp"

int plugin_library::_register_plugin_func(void* host, int id, const char* name,
                                          const char* sig)
{
    plugin_library* lib = (plugin_library*)host;
    std::string sName(name);
    std::string sSig(sig);
    return lib->register_plugin_func(id, sName, sSig);
}

int plugin_library::_register_plugin_func_c(void* host, int id,
                                            const char* name, const char* sig,
                                            fptr func)
{
    plugin_library* lib = (plugin_library*)host;
    std::string sName(name);
    std::string sSig(sig);
    return lib->register_plugin_func(id, sName, sSig, func);
}

plugin_library::plugin_library(plugin_manager& manager)
    : _manager(manager)
{

}

int plugin_library::register_plugin_func(int id, std::string& name,
                                         std::string& sig)
{
    try {
        plugin& plug = *_plugins.at(id);
        plugin_function *func = new plugin_function(name, sig, plug);
        std::unique_ptr<plugin_function> func_ptr(func);
        _manager.register_function(*func_ptr);
        plug.register_function(move(func_ptr));
        return 1;
    } catch (std::out_of_range& e) {
        return 0;
    }
}

int plugin_library::register_plugin_func(int id, std::string& name,
                                         std::string& sig, fptr func)
{
    try {
        plugin& plug = *_plugins.at(id);
        dyncall_function *pfunc = new dyncall_function(name, sig, func);
        std::unique_ptr<dyncall_function> func_ptr(pfunc);
        _manager.register_function(*func_ptr);
        plug.register_function(move(func_ptr));
        return 1;
    } catch (std::out_of_range& e) {
        return 0;
    }
}

void plugin_library::load(std::string& path)
{
    _libptr = dlLoadLibrary(path.c_str());

    if (!_libptr) {
        throw std::exception();
    }

    // Get around object-to-function pointer restrictions
    // From: http://stackoverflow.com/a/1096349
    *reinterpret_cast<void**>(&_create) = dlFindSymbol(_libptr, "create");
    *reinterpret_cast<void**>(&_destroy) = dlFindSymbol(_libptr, "destroy");
    *reinterpret_cast<void**>(&_argument) = dlFindSymbol(_libptr, "argument");
    *reinterpret_cast<void**>(&_start) = dlFindSymbol(_libptr, "start");

    _interface =
        static_cast<struct plugin_interface*>(dlFindSymbol(_libptr,
                                                           "plugin_interface"));
    if (!(_interface && _create && _destroy && _argument && _start)) {
        throw std::exception();
    }

    _interface->host = this;
    _interface->register_func = &_register_plugin_func;
    _interface->register_func_c = &_register_plugin_func_c;
}

plugin& plugin_library::create()
{
    int id = _create();

    if (id == 0) {
        throw std::exception();
    }

    plugin* inst = new plugin(*this, id);
    std::unique_ptr<plugin> ptr(inst);
    _plugins[id] = std::move(ptr);
    return *inst;
}

void plugin_library::destroy(plugin& plug)
{
    _plugins.erase(plug.id());
}

plugin_library::~plugin_library()
{
    _plugins.clear();
    if (_libptr) {
        dlFreeLibrary(_libptr);
        _create = 0;
        _destroy = 0;
        _argument = 0;
        _start = 0;
        _libptr = 0;
    }
}
