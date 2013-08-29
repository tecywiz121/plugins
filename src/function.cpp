#include <iostream>
#include <sstream>
#include "function.hpp"
#include "util.h"
#include "dyncall.h"

function::function(const std::string& name, const std::string& sig)
    : _name(name), _signature(sig)
{
    std::stringstream ss;
    bool ret = false;

    for (char c : sig) {
        if (ret) {
            _return = c;
        } else if (c == ')') {
            ret = true;
        } else {
            ss << c;
        }
    }

    if (!ret) {
        _return = 'v';
    }

    _arguments = ss.str();
}

const std::string& function::name()
{
    return _name;
}

void function::check_signature(int fromend, char type)
{
    if (_arguments[_arguments.size() - fromend] != type) {
        end_call();
        throw std::exception();
    }
}

void function::check_return(char type)
{
    if (_return != type) {
        end_call();
        throw std::exception();
    }
}

dyncall_function::dyncall_function(std::string& name, std::string& sig, fptr func)
    : function(name, sig)
{
    _func = *reinterpret_cast<void**>(&func);
}

void dyncall_function::begin_call()
{
    if (_vm) {
        // Can't have two at the same time
        throw std::exception();
    }

    _vm = dcNewCallVM(4096); // TODO: Can probably estimate this much more accurately since we have the function signature
    dcMode(_vm, DC_CALL_C_DEFAULT);
    dcReset(_vm);
}

void dyncall_function::end_call()
{
    if (_vm) {
        dcFree(_vm);
        _vm = 0;
    }
}

void dyncall_function::arg_bool(bool v)
{
    dcArgBool(_vm, v);
}

void dyncall_function::arg_char(char v)
{
    dcArgChar(_vm, v);
}

void dyncall_function::arg_uchar(unsigned char v)
{
    dcArgChar(_vm, v);
}

void dyncall_function::arg_short(short v)
{
    dcArgShort(_vm, v);
}

void dyncall_function::arg_ushort(unsigned short v)
{
    dcArgShort(_vm, v);
}

void dyncall_function::arg_int(int v)
{
    dcArgInt(_vm, v);
}

void dyncall_function::arg_uint(unsigned int v)
{
    dcArgInt(_vm, v);
}

void dyncall_function::arg_long(long v)
{
    dcArgLong(_vm, v);
}

void dyncall_function::arg_ulong(unsigned long v)
{
    dcArgLong(_vm, v);
}

void dyncall_function::arg_longlong(long long v)
{
    dcArgLongLong(_vm, v);
}

void dyncall_function::arg_ulonglong(unsigned long long v)
{
    dcArgLongLong(_vm, v);
}

void dyncall_function::arg_float(float v)
{
    dcArgFloat(_vm, v);
}

void dyncall_function::arg_double(double v)
{
    dcArgDouble(_vm, v);
}

void dyncall_function::arg_pointer(void* v)
{
    dcArgPointer(_vm, v);
}

void dyncall_function::arg_c_str(const char* v)
{
    dcArgPointer(_vm, const_cast<char*>(v));
}

void dyncall_function::ret_void()
{
    dcCallVoid(_vm, _func);
}

bool dyncall_function::ret_bool()
{
    return dcCallBool(_vm, _func);
}

char dyncall_function::ret_char()
{
    return dcCallChar(_vm, _func);
}

unsigned char dyncall_function::ret_uchar()
{
    return dcCallChar(_vm, _func);
}

short dyncall_function::ret_short()
{
    return dcCallShort(_vm, _func);
}

unsigned short dyncall_function::ret_ushort()
{
    return dcCallShort(_vm, _func);
}

int dyncall_function::ret_int()
{
    return dcCallInt(_vm, _func);
}

unsigned int dyncall_function::ret_uint()
{
    return dcCallInt(_vm, _func);
}

long dyncall_function::ret_long()
{
    return dcCallLong(_vm, _func);
}

unsigned long dyncall_function::ret_ulong()
{
    return dcCallLong(_vm, _func);
}

long long dyncall_function::ret_longlong()
{
    return dcCallLongLong(_vm, _func);
}

unsigned long long dyncall_function::ret_ulonglong()
{
    return dcCallLongLong(_vm, _func);
}

float dyncall_function::ret_float()
{
    return dcCallFloat(_vm, _func);
}

double dyncall_function::ret_double()
{
    return dcCallDouble(_vm, _func);
}

void* dyncall_function::ret_pointer()
{
    return dcCallPointer(_vm, _func);
}

const char* dyncall_function::ret_c_str()
{
    return static_cast<const char*>(dcCallPointer(_vm, _func));
}

plugin_function::plugin_function(std::string& name, std::string& sig,
                                plugin& plug)
    : function(name, sig), _interface(plug._library._interface), _id(plug.id)
{
}

void plugin_function::begin_call()
{
    if (_call_data) {
        // Only one call at a time
        throw std::exception();
    }
    _call_data = _interface->begin_plugin_call(_id, _name.c_str());
}

void plugin_function::end_call()
{
    if (_call_data) {
        _interface->plugin_invoke.end_call(_call_data);
        _call_data = 0;
    }
}

void plugin_function::arg_bool(bool v)
{
    _interface->plugin_invoke.arg_bool(_call_data, v);
}

void plugin_function::arg_char(char v)
{
    _interface->plugin_invoke.arg_char(_call_data, v);
}

void plugin_function::arg_uchar(unsigned char v)
{
    _interface->plugin_invoke.arg_uchar(_call_data, v);
}

void plugin_function::arg_short(short v)
{
    _interface->plugin_invoke.arg_short(_call_data, v);
}

void plugin_function::arg_ushort(unsigned short v)
{
    _interface->plugin_invoke.arg_ushort(_call_data, v);
}

void plugin_function::arg_int(int v)
{
    _interface->plugin_invoke.arg_int(_call_data, v);
}

void plugin_function::arg_uint(unsigned int v)
{
    _interface->plugin_invoke.arg_uint(_call_data, v);
}

void plugin_function::arg_long(long v)
{
    _interface->plugin_invoke.arg_long(_call_data, v);
}

void plugin_function::arg_ulong(unsigned long v)
{
    _interface->plugin_invoke.arg_ulong(_call_data, v);
}

void plugin_function::arg_longlong(long long v)
{
    _interface->plugin_invoke.arg_long_long(_call_data, v);
}

void plugin_function::arg_ulonglong(unsigned long long v)
{
    _interface->plugin_invoke.arg_ulong_long(_call_data, v);
}

void plugin_function::arg_float(float v)
{
    _interface->plugin_invoke.arg_float(_call_data, v);
}

void plugin_function::arg_double(double v)
{
    _interface->plugin_invoke.arg_double(_call_data, v);
}

void plugin_function::arg_pointer(void* v)
{
    _interface->plugin_invoke.arg_pointer(_call_data, v);
}

void plugin_function::arg_c_str(const char* v)
{
    _interface->plugin_invoke.arg_c_str(_call_data, v);
}

void plugin_function::ret_void()
{
    _interface->plugin_invoke.ret_void(_call_data);
}

bool plugin_function::ret_bool()
{
    return _interface->plugin_invoke.ret_bool(_call_data);
}

char plugin_function::ret_char()
{
    return _interface->plugin_invoke.ret_char(_call_data);
}

unsigned char plugin_function::ret_uchar()
{
    return _interface->plugin_invoke.ret_uchar(_call_data);
}

short plugin_function::ret_short()
{
    return _interface->plugin_invoke.ret_short(_call_data);
}

unsigned short plugin_function::ret_ushort()
{
    return _interface->plugin_invoke.ret_ushort(_call_data);
}

int plugin_function::ret_int()
{
    return _interface->plugin_invoke.ret_int(_call_data);
}

unsigned int plugin_function::ret_uint()
{
    return _interface->plugin_invoke.ret_uint(_call_data);
}

long plugin_function::ret_long()
{
    return _interface->plugin_invoke.ret_long(_call_data);
}

unsigned long plugin_function::ret_ulong()
{
    return _interface->plugin_invoke.ret_ulong(_call_data);
}

long long plugin_function::ret_longlong()
{
    return _interface->plugin_invoke.ret_long_long(_call_data);
}

unsigned long long plugin_function::ret_ulonglong()
{
    return _interface->plugin_invoke.ret_ulong_long(_call_data);
}

float plugin_function::ret_float()
{
    return _interface->plugin_invoke.ret_float(_call_data);
}

double plugin_function::ret_double()
{
    return _interface->plugin_invoke.ret_double(_call_data);
}

void* plugin_function::ret_pointer()
{
    return _interface->plugin_invoke.ret_pointer(_call_data);
}

const char* plugin_function::ret_c_str()
{
    return _interface->plugin_invoke.ret_c_str(_call_data);
}
