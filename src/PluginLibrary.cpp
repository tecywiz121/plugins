#include <memory>
#include <string>
#include <iostream>
#include "SDL2/SDL_loadso.h"
#include "PluginLibrary.h"
#include "PluginDescription.h"

using namespace std;

void PluginLibrary::load(string& path)
{
    _libptr = SDL_LoadObject(path.c_str());

    if (!_libptr)
    {
        throw exception();
    }

    // Get around object-to-function pointer restrictions (http://stackoverflow.com/a/1096349)
    *reinterpret_cast<void**>(&_create) = SDL_LoadFunction(_libptr, "create");
    *reinterpret_cast<void**>(&_destroy) = SDL_LoadFunction(_libptr, "destroy");
    *reinterpret_cast<void**>(&_argument) = SDL_LoadFunction(_libptr, "argument");
    *reinterpret_cast<void**>(&_start) = SDL_LoadFunction(_libptr, "start");

    if (!(_create && _destroy && _argument && _start))
    {
        throw exception();
    }
}

unique_ptr<Plugin> PluginLibrary::create()
{
    int id = _create();

    if (id < 0)
    {
        throw exception();
    }

    Plugin* inst = new Plugin(*this, id);
    return unique_ptr<Plugin>(inst);
}

PluginLibrary::~PluginLibrary()
{
    if (_libptr)
    {
        SDL_UnloadObject(_libptr);
    }
}
