#ifndef PLUGIN_H
#define PLUGIN_H 1

class plugin_library;

#include <memory>
#include <unordered_map>
#include "plugin_library.hpp"

class function;

class plugin {
    friend class plugin_library;
    friend class plugin_function;
    private:
        const int _id;
        const plugin_library& _library;
        std::unordered_map<std::string, std::unique_ptr<function> > _functions;

        void register_function(std::unique_ptr<function> func);

    public:
        plugin(const plugin_library& library, int id);
        int id() const { return _id; }
        void start();
        void argument(std::string key, std::string value);
        function& get_function(const std::string& name);

        virtual ~plugin();
};

#endif
