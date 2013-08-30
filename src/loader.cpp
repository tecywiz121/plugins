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


        try {
            function& banana = plugin.get_function("banana");
            std::cout << "banana returned: "
                      << banana.invoke<int>() << std::endl;
        } catch (std::out_of_range& e) {
            std::cout << "banana not found :-(" << std::endl;
        }

        try {
            function& do_something = plugin.get_function("do_something");
            std::cout << "do_something returned: "
                      << do_something.invoke<int>(4.0f, 2.0f) << std::endl;
        } catch (std::out_of_range& e) {
            std::cout << "do_something not found :-(" << std::endl;
        }
    }

    return 0;
}
