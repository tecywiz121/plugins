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
    }

    std::string nbanana("banana");
    std::string nsomething("do_something");

    try {
        const function& banana = manager.get_function(nbanana);
        std::cout << "banana returned: "
                  << banana.invoke<int>() << std::endl;
    } catch (std::out_of_range& e) {
        std::cout << "banana not found :-(" << std::endl;
    }

    try {
        const function& do_something = manager.get_function(nsomething);
        std::cout << "do_something returned: "
                  << do_something.invoke<int>(4.0f, 2.0f) << std::endl;
    } catch (std::out_of_range& e) {
        std::cout << "do_something not found :-(" << std::endl;
    }

    return 0;
}
