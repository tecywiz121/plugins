#include <iostream>
#include "util.h"
#include "plugin_manager.hpp"

#include "function.hpp"

int main(int argc, char** argv)
{
    if (argc <= 1) {
        std::string me = "main";
        if (argc == 1) {
            me = std::string(argv[0]);
        }

        std::cout << me << ": plugin1 [plugin2...]" << std::endl;
        return 1;
    }

    plugin_manager manager;

    for (int ii = 1; ii < argc; ii++) {
        plugin& plugin = manager.load_plugin(argv[ii]);
        plugin.start();

        plugin_function& banana = plugin.get_function("banana");
        std::cout << "Returned: " << banana.invoke<int>() << std::endl;
    }

    return 0;
}
