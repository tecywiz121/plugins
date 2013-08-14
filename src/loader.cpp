#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_loadso.h"
#include "util.h"
#include "PluginManager.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        string me = "main";
        if (argc == 1)
        {
            me = string(argv[0]);
        }

        cout << me << ": plugin1 [plugin2...]" << endl;
        return 1;
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    PluginManager manager;

    for (int ii = 1; ii < argc; ii++)
    {
        Plugin& plugin = manager.load_plugin(argv[ii]);
        plugin.start();
    }

    SDL_Quit();
    return 0;
}
