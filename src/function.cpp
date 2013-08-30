#include <iostream>
#include <sstream>
#include "function.hpp"
#include "util.h"
#include "dyncall.h"

function::function(const std::string& name, const std::string& sig)
    : _name(name), _signature(sig), _arguments(parse_args(sig)),
    _return(parse_return(sig))
{
}

char function::parse_return(const std::string& sig) const
{
    bool ret = false;
    char rtype = 'v';

    for (char c : sig) {
        if (ret) {
            rtype = c;
            break;
        } else if (c == ')') {
            ret = true;
        }
    }

    return rtype;
}

std::string function::parse_args(const std::string& sig) const
{
    std::stringstream ss;

    for (char c : sig) {
        if (c == ')') {
            break;
        }
        ss << c;
    }

    return ss.str();
}

const std::string& function::name() const
{
    return _name;
}

void function::check_signature(int fromend, char type) const
{
    if (_arguments[_arguments.size() - fromend] != type) {
        throw std::exception();
    }
}

void function::check_return(char type) const
{
    if (_return != type) {
        throw std::exception();
    }
}

function::~function()
{

}

dyncall_function::dyncall_function(const std::string& name,
                                   const std::string& sig, fptr func)
    : function(name, sig)
{
    _func = *reinterpret_cast<void**>(&func);
}

std::unique_ptr<invocation> dyncall_function::begin_call() const
{
    std::unique_ptr<invocation> inv(new dyncall_invocation(*this));
    return inv;
}

dyncall_function::~dyncall_function()
{

}

plugin_function::plugin_function(std::string& name, std::string& sig,
                                plugin& plug)
    : function(name, sig), _interface(plug._library._interface), _id(plug.id)
{
}

std::unique_ptr<invocation> plugin_function::begin_call() const
{
    std::unique_ptr<invocation> inv(new plugin_invocation(*this));
    return inv;
}

plugin_function::~plugin_function()
{

}
