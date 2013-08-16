#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include "util.h"
#include "plugin_i.h"
#include "dynload.h"
#include "PluginLibrary.h"
#include "PluginDescription.h"

using namespace std;

int PluginLibrary::_register_plugin_func(void* host, int id, const char* name, const char* sig)
{
    PluginLibrary* lib = (PluginLibrary*)host;
    string sName(name);
    string sSig(sig);
    return lib->register_plugin_func(id, sName, sSig);
}

int PluginLibrary::_register_plugin_func_c(void* host, const char* name, const char* sig, fptr func)
{
    PluginLibrary* lib = (PluginLibrary*)host;
    string sName(name);
    string sSig(sig);
    return lib->register_plugin_func_c(sName, sSig, func);
}

int PluginLibrary::register_plugin_func(UNUSED(int id), UNUSED(string& name), UNUSED(string& sig))
{
    return 1;
}

int PluginLibrary::register_plugin_func_c(UNUSED(string& name), UNUSED(string& sig), UNUSED(fptr func))
{
    return 1;
}

void PluginLibrary::load(string& path)
{
    _libptr = dlLoadLibrary(path.c_str());

    if (!_libptr)
    {
        throw exception();
    }

    // Get around object-to-function pointer restrictions (http://stackoverflow.com/a/1096349)
    *reinterpret_cast<void**>(&_create) = dlFindSymbol(_libptr, "create");
    *reinterpret_cast<void**>(&_destroy) = dlFindSymbol(_libptr, "destroy");
    *reinterpret_cast<void**>(&_argument) = dlFindSymbol(_libptr, "argument");
    *reinterpret_cast<void**>(&_start) = dlFindSymbol(_libptr, "start");

    _interface = static_cast<plugin_interface_t*>(dlFindSymbol(_libptr, "plugin_interface"));
    if (!(_interface && _create && _destroy && _argument && _start))
    {
        throw exception();
    }

    _interface->host = this;
    _interface->register_func = &_register_plugin_func;
    _interface->register_func_c = &_register_plugin_func_c;
}

Plugin& PluginLibrary::create()
{
    int id = _create();

    if (id == 0)
    {
        throw exception();
    }

    Plugin* inst = new Plugin(*this, id);
    unique_ptr<Plugin> ptr(inst);
    _plugins[id] = move(ptr);
    return *inst;
}

void PluginLibrary::destroy(Plugin& plugin)
{
    _plugins.erase(plugin.id);
}

PluginLibrary::~PluginLibrary()
{
    _plugins.clear();
    if (_libptr)
    {
        dlFreeLibrary(_libptr);
        _create = 0;
        _destroy = 0;
        _argument = 0;
        _start = 0;
        _libptr = 0;
    }
}
