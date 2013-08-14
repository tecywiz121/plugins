#ifndef __PLUGINDESCRIPTION_H__
#define __PLUGINDESCRIPTION_H__ 1

#include <string>
#include <exception>
#include <unordered_map>

class bad_parse : public std::exception
{
    private:
        std::string _msg;
    public:
        bad_parse(std::string msg) throw ();

        virtual const char* what() const throw ();
};

class PluginDescription
{
    private:
        std::string _module;
        std::unordered_map<std::string, std::string> _arguments;

    public:
        PluginDescription(std::string path) throw (bad_parse);

        std::string& module();
        std::unordered_map<std::string, std::string>& arguments();

        virtual ~PluginDescription();
};

#endif
