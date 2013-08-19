#include <iostream>
#include "util.h"
#include "PluginManager.h"

#include "Function.h"

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

    PluginManager manager;

    for (int ii = 1; ii < argc; ii++)
    {
        Plugin& plugin = manager.load_plugin(argv[ii]);
        plugin.start();

        PluginFunction& banana = plugin.get_function("banana");
        cout << "Returned: " << banana.invoke<int>() << endl;
    }

    return 0;
}
