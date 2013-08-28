#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "plugin_description.hpp"

bad_parse::bad_parse(const std::string msg) throw()
{
    _msg = msg;
}

const char* bad_parse::what() const throw()
{
    return _msg.c_str();
}

std::string& plugin_description::module()
{
    return _module;
}

std::unordered_map<std::string, std::string>& plugin_description::arguments()
{
    return _arguments;
}

plugin_description::plugin_description(std::string path) throw (bad_parse)
{
    std::ifstream fdesc(path);

    int state = 0;
    std::stringstream ss;
    std::string key;
    std::string value;

    if (fdesc.is_open())
    {
        while (fdesc.good())
        {
            char c = fdesc.get();
            switch (state)
            {
            case 0:
                if ('[' == c)
                {
                    state = 1;
                    ss.str(std::string());
                }
                else
                {
                    throw bad_parse("first character of file must be '['");
                }
                break;
            case 1:
                // Read until "]"
                if (']' == c)
                {
                    state = 2;
                    _module = ss.str();
                    ss.str(std::string());
                }
                else if ('\n' == c)
                {
                    throw bad_parse("unexpected newline in header");
                }
                else
                {
                    ss << c;
                }
                break;
            case 2:
                // Read until '='
                if ('=' == c)
                {
                    state = 3;
                    key = ss.str();
                    ss.str(std::string());
                }
                else if ('\n' == c && ss.tellp() != 0)
                {
                    throw bad_parse("unexpected newline in argument name");
                }
                else if ('\n' != c)
                {
                    ss << c;
                }
                break;
            case 3:
                // Read until end of line
                if ('\n' == c)
                {
                    state = 2;
                    value = ss.str();
                    ss.str(std::string());

                    _arguments.insert(make_pair(key, value));
                }
                else
                {
                    ss << c;
                }
                break;
            }
        }
        if (2 != state && 3 != state)
        {
            throw bad_parse("incomplete file");
        }
    }
    else
    {
        throw bad_parse("could not open file");
    }
}

plugin_description::~plugin_description()
{

}
