#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <exception>
#include <stdexcept>
#include "util.h"
#include "plugin_i.h"
#include "dynload.h"
#include "plugin_library.hpp"
#include "plugin_description.hpp"
#include "function.hpp"
#include "plugin_manager.hpp"
#include "invocation.hpp"

void* plugin_library::_begin_outgoing_call(void* host, const char* name)
{
    plugin_library* lib = static_cast<plugin_library*>(host);
    std::string sName(name);
    return &lib->begin_outgoing_call(sName);
}

void plugin_library::_arg_bool(void* inv, bool v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_bool(v);
}

void plugin_library::_arg_char(void* inv, signed char v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_char(v);
}

void plugin_library::_arg_uchar(void* inv, unsigned char v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_uchar(v);
}

void plugin_library::_arg_short(void* inv, short v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_short(v);
}

void plugin_library::_arg_ushort(void* inv, unsigned short v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_ushort(v);
}

void plugin_library::_arg_int(void* inv, int v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_int(v);
}

void plugin_library::_arg_uint(void* inv, unsigned int v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_uint(v);
}

void plugin_library::_arg_long(void* inv, long v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_long(v);
}

void plugin_library::_arg_ulong(void* inv, unsigned long v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_ulong(v);
}

void plugin_library::_arg_long_long(void* inv, long long v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_longlong(v);
}

void plugin_library::_arg_ulong_long(void* inv, unsigned long long v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_ulonglong(v);
}

void plugin_library::_arg_float(void* inv, float v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_float(v);
}

void plugin_library::_arg_double(void* inv, double v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_double(v);
}

void plugin_library::_arg_pointer(void* inv, void* v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_pointer(v);
}

void plugin_library::_arg_c_str(void* inv, const char* v)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->arg_c_str(v);
}

void plugin_library::_ret_void(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    invoke->ret_void();
}

bool plugin_library::_ret_bool(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_bool();
}

signed char plugin_library::_ret_char(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_char();
}

unsigned char plugin_library::_ret_uchar(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_uchar();
}

short plugin_library::_ret_short(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_short();
}

unsigned short plugin_library::_ret_ushort(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_ushort();
}

int plugin_library::_ret_int(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_int();
}

unsigned int plugin_library::_ret_uint(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_uint();
}

long plugin_library::_ret_long(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_long();
}

unsigned long plugin_library::_ret_ulong(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_ulong();
}

long long plugin_library::_ret_long_long(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_longlong();
}

unsigned long long plugin_library::_ret_ulong_long(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_ulonglong();
}

float plugin_library::_ret_float(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_float();
}

double plugin_library::_ret_double(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_double();
}

void* plugin_library::_ret_pointer(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_pointer();
}

const char* plugin_library::_ret_c_str(void* inv)
{
    invocation* invoke = static_cast<invocation*>(inv);
    return invoke->ret_c_str();
}

void plugin_library::_end_call(void* h, void* inv)
{
    plugin_library *host = static_cast<plugin_library*>(h);
    std::unique_ptr<invocation> invoke(static_cast<invocation*>(inv));
    host->_outgoing_invocations.erase(invoke);
    invoke.release();
}

int plugin_library::_register_plugin_func(void* host, int id, const char* name,
                                          const char* sig)
{
    plugin_library* lib = static_cast<plugin_library*>(host);
    std::string sName(name);
    std::string sSig(sig);
    return lib->register_plugin_func(id, sName, sSig);
}

int plugin_library::_register_plugin_func_c(void* host, int id,
                                            const char* name, const char* sig,
                                            fptr func)
{
    plugin_library* lib = (plugin_library*)host;
    std::string sName(name);
    std::string sSig(sig);
    return lib->register_plugin_func(id, sName, sSig, func);
}

plugin_library::plugin_library(plugin_manager& manager)
    : _manager(manager)
{

}

invocation& plugin_library::begin_outgoing_call(std::string& name)
{
    const function& func = _manager.get_function(name);
    std::unique_ptr<invocation> inv = func.begin_call();
    invocation& retval = *inv;
    _outgoing_invocations.insert(std::move(inv));
    return retval;
}

int plugin_library::register_plugin_func(int id, std::string& name,
                                         std::string& sig)
{
    try {
        plugin& plug = *_plugins.at(id);
        plugin_function *func = new plugin_function(name, sig, plug);
        std::unique_ptr<plugin_function> func_ptr(func);
        _manager.register_function(*func_ptr);
        plug.register_function(move(func_ptr));
        return 1;
    } catch (std::out_of_range& e) {
        return 0;
    }
}

int plugin_library::register_plugin_func(int id, std::string& name,
                                         std::string& sig, fptr func)
{
    try {
        plugin& plug = *_plugins.at(id);
        dyncall_function *pfunc = new dyncall_function(name, sig, func);
        std::unique_ptr<dyncall_function> func_ptr(pfunc);
        _manager.register_function(*func_ptr);
        plug.register_function(move(func_ptr));
        return 1;
    } catch (std::out_of_range& e) {
        return 0;
    }
}

void plugin_library::load(std::string& path)
{
    _libptr = dlLoadLibrary(path.c_str());

    if (!_libptr) {
        throw std::exception();
    }

    // Get around object-to-function pointer restrictions
    // From: http://stackoverflow.com/a/1096349
    *reinterpret_cast<void**>(&_create) = dlFindSymbol(_libptr, "create");
    *reinterpret_cast<void**>(&_destroy) = dlFindSymbol(_libptr, "destroy");
    *reinterpret_cast<void**>(&_argument) = dlFindSymbol(_libptr, "argument");
    *reinterpret_cast<void**>(&_start) = dlFindSymbol(_libptr, "start");

    _interface =
        static_cast<struct plugin_interface*>(dlFindSymbol(_libptr,
                                                           "plugin_interface"));
    if (!(_interface && _create && _destroy && _argument && _start)) {
        throw std::exception();
    }

    _interface->host = this;
    _interface->register_func = &_register_plugin_func;
    _interface->register_func_c = &_register_plugin_func_c;
    _interface->begin_host_call = &_begin_outgoing_call;
    _interface->end_host_call = &_end_call;

    _interface->host_invoke.arg_bool = &_arg_bool;
    _interface->host_invoke.arg_char = &_arg_char;
    _interface->host_invoke.arg_uchar = &_arg_uchar;
    _interface->host_invoke.arg_short = &_arg_short;
    _interface->host_invoke.arg_ushort = &_arg_ushort;
    _interface->host_invoke.arg_int = &_arg_int;
    _interface->host_invoke.arg_uint = &_arg_uint;
    _interface->host_invoke.arg_long = &_arg_long;
    _interface->host_invoke.arg_ulong = &_arg_ulong;
    _interface->host_invoke.arg_long_long = &_arg_long_long;
    _interface->host_invoke.arg_ulong_long = &_arg_ulong_long;
    _interface->host_invoke.arg_float = &_arg_float;
    _interface->host_invoke.arg_double = &_arg_double;
    _interface->host_invoke.arg_pointer = &_arg_pointer;
    _interface->host_invoke.arg_c_str = &_arg_c_str;

    _interface->host_invoke.ret_void = &_ret_void;
    _interface->host_invoke.ret_bool = &_ret_bool;
    _interface->host_invoke.ret_char = &_ret_char;
    _interface->host_invoke.ret_uchar = &_ret_uchar;
    _interface->host_invoke.ret_short = &_ret_short;
    _interface->host_invoke.ret_ushort = &_ret_ushort;
    _interface->host_invoke.ret_int = &_ret_int;
    _interface->host_invoke.ret_uint = &_ret_uint;
    _interface->host_invoke.ret_long = &_ret_long;
    _interface->host_invoke.ret_ulong = &_ret_ulong;
    _interface->host_invoke.ret_long_long = &_ret_long_long;
    _interface->host_invoke.ret_ulong_long = &_ret_ulong_long;
    _interface->host_invoke.ret_float = &_ret_float;
    _interface->host_invoke.ret_double = &_ret_double;
    _interface->host_invoke.ret_pointer = &_ret_pointer;
    _interface->host_invoke.ret_c_str = &_ret_c_str;
}

plugin& plugin_library::create()
{
    int id = _create();

    if (id == 0) {
        throw std::exception();
    }

    plugin* inst = new plugin(*this, id);
    std::unique_ptr<plugin> ptr(inst);
    _plugins[id] = std::move(ptr);
    return *inst;
}

void plugin_library::destroy(plugin& plug)
{
    _plugins.erase(plug.id());
}

plugin_library::~plugin_library()
{
    _plugins.clear();
    if (_libptr) {
        dlFreeLibrary(_libptr);
        _create = 0;
        _destroy = 0;
        _argument = 0;
        _start = 0;
        _libptr = 0;
    }
}
