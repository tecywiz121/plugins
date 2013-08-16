#include <iostream>
#include "Plugin.h"
#include "PluginLibrary.h"

using namespace std;

Plugin::Plugin(const PluginLibrary& library, int newid)
    : _library(library), id(newid)
{
}

void Plugin::start()
{
    if (_library._start(id) == 0)
    {
        throw exception();
    }
}

void Plugin::argument(string key, string value)
{
    if (_library._argument(id, key.c_str(), value.c_str()) == 0)
    {
        throw exception();
    }
}

Plugin::~Plugin()
{
    _library._destroy(id);
}
