#include <iostream>
#include "Plugin.h"
#include "PluginLibrary.h"

using namespace std;

Plugin::Plugin(const PluginLibrary& library, int id)
    : _id(id), _library(library)
{
}

void Plugin::start()
{
    _library._start(_id);
}

void Plugin::argument(string key, string value)
{
    _library._argument(_id, key.c_str(), value.c_str());
}

Plugin::~Plugin()
{
    _library._destroy(_id);
}
