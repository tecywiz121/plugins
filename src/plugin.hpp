#ifndef PLUGIN_H
#define PLUGIN_H 1

class plugin_library;

#include <memory>
#include <unordered_map>
#include "plugin_library.hpp"

class plugin_function;

class plugin {
    friend class plugin_library;
    friend class plugin_function;
    private:
        const plugin_library& _library;
        std::unordered_map<std::string, std::unique_ptr<plugin_function> >
                                                                    _functions;

        void register_plugin_function(std::unique_ptr<plugin_function> func);

    public:
        plugin(const plugin_library& library, int id);
        const int id;
        void start();
        void argument(std::string key, std::string value);
        plugin_function& get_function(const std::string& name);

        virtual ~plugin();
};

#endif
