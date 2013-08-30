#ifndef FUNCTION_H
#define FUNCTION_H 1
#include <exception>
#include "dyncall.h"
#include "util.h"
#include "plugin.hpp"
#include "invocation.hpp"

typedef void (*fptr)(void);
class invocation;
template<typename TReturn, typename First, typename... Rest> class invoker;

class function {
    template<typename TReturn, typename First, typename... Rest>
    friend class invoker;

private:
    const std::string _name;
    const std::string _signature;
    const std::string _arguments;
    const char _return;
    void check_signature(int fromend, char type) const;
    void check_return(char type) const;
    char parse_return(const std::string& sig) const;
    std::string parse_args(const std::string& sig) const;
protected:
    virtual std::unique_ptr<invocation> begin_call() const =0;
public:
    function(const std::string& name, const std::string& signature);
    template<typename TReturn, typename... Arguments>
    TReturn invoke(Arguments... args);
    const std::string& name() const;
    const std::string& signature() const;
    virtual ~function();
};

class dyncall_function : public function {
    friend class dyncall_invocation;
private:
    void* _func;
protected:
    virtual std::unique_ptr<invocation> begin_call() const;
public:
    dyncall_function(const std::string& name, const std::string& sig,
                     fptr func);
    virtual ~dyncall_function();
};

class plugin_function : public function {
    friend class plugin_invocation;
private:
    struct plugin_interface* _interface;
    const int _id;
protected:
    virtual std::unique_ptr<invocation> begin_call() const;
public:
    plugin_function(std::string& name, std::string& sig, plugin& plugin);
    virtual ~plugin_function();
};

#include "invoker.hpp"

template<typename TReturn, typename... Arguments>
TReturn function::invoke(Arguments... args)
{
    if (sizeof...(args) != _arguments.size()) {
        throw std::exception();
    }
    std::unique_ptr<invocation> inv = begin_call();
    return invoker<TReturn, Arguments...>::invoke(*inv, args...);
}

#endif
