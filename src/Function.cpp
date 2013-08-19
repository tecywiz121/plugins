#include <iostream>
#include <sstream>
#include "Function.h"
#include "util.h"
#include "dyncall.h"

using namespace std;

Function::Function(const string& name, const string& sig)
    : _name(name), _signature(sig)
{
    stringstream ss;
    bool ret = false;

    for (char c : sig)
    {
        if (ret)
        {
            _return = c;
        }
        else if (c == ')')
        {
            ret = true;
        }
        else
        {
            ss << c;
        }
    }

    if (!ret)
    {
        _return = 'v';
    }

    _arguments = ss.str();
}

const string& Function::name()
{
    return _name;
}

void Function::check_signature(int fromend, char type)
{
    if (_arguments[_arguments.size() - fromend] != type)
    {
        end_call();
        throw exception();
    }
}

void Function::check_return(char type)
{
    if (_return != type)
    {
        end_call();
        throw exception();
    }
}

DynCallFunction::DynCallFunction(string& name, string& sig, fptr func)
    : Function(name, sig)
{
    _func = *reinterpret_cast<void**>(&func);
}

void DynCallFunction::begin_call()
{
    if (_vm)
    {
        // Can't have two at the same time
        throw exception();
    }

    _vm = dcNewCallVM(4096); // TODO: Can probably estimate this much more accurately since we have the function signature
    dcMode(_vm, DC_CALL_C_DEFAULT);
    dcReset(_vm);
}

void DynCallFunction::end_call()
{
    if (_vm)
    {
        dcFree(_vm);
        _vm = 0;
    }
}

void DynCallFunction::arg_bool(bool v)
{
    dcArgBool(_vm, v);
}

void DynCallFunction::arg_char(char v)
{
    dcArgChar(_vm, v);
}

void DynCallFunction::arg_uchar(unsigned char v)
{
    dcArgChar(_vm, v);
}

void DynCallFunction::arg_short(short v)
{
    dcArgShort(_vm, v);
}

void DynCallFunction::arg_ushort(unsigned short v)
{
    dcArgShort(_vm, v);
}

void DynCallFunction::arg_int(int v)
{
    dcArgInt(_vm, v);
}

void DynCallFunction::arg_uint(unsigned int v)
{
    dcArgInt(_vm, v);
}

void DynCallFunction::arg_long(long v)
{
    dcArgLong(_vm, v);
}

void DynCallFunction::arg_ulong(unsigned long v)
{
    dcArgLong(_vm, v);
}

void DynCallFunction::arg_longlong(long long v)
{
    dcArgLongLong(_vm, v);
}

void DynCallFunction::arg_ulonglong(unsigned long long v)
{
    dcArgLongLong(_vm, v);
}

void DynCallFunction::arg_float(float v)
{
    dcArgFloat(_vm, v);
}

void DynCallFunction::arg_double(double v)
{
    dcArgDouble(_vm, v);
}

void DynCallFunction::arg_pointer(void* v)
{
    dcArgPointer(_vm, v);
}

void DynCallFunction::arg_c_str(const char* v)
{
    dcArgPointer(_vm, const_cast<char*>(v));
}

void DynCallFunction::ret_void()
{
    dcCallVoid(_vm, _func);
}

bool DynCallFunction::ret_bool()
{
    return dcCallBool(_vm, _func);
}

char DynCallFunction::ret_char()
{
    return dcCallChar(_vm, _func);
}

unsigned char DynCallFunction::ret_uchar()
{
    return dcCallChar(_vm, _func);
}

short DynCallFunction::ret_short()
{
    return dcCallShort(_vm, _func);
}

unsigned short DynCallFunction::ret_ushort()
{
    return dcCallShort(_vm, _func);
}

int DynCallFunction::ret_int()
{
    return dcCallInt(_vm, _func);
}

unsigned int DynCallFunction::ret_uint()
{
    return dcCallInt(_vm, _func);
}

long DynCallFunction::ret_long()
{
    return dcCallLong(_vm, _func);
}

unsigned long DynCallFunction::ret_ulong()
{
    return dcCallLong(_vm, _func);
}

long long DynCallFunction::ret_longlong()
{
    return dcCallLongLong(_vm, _func);
}

unsigned long long DynCallFunction::ret_ulonglong()
{
    return dcCallLongLong(_vm, _func);
}

float DynCallFunction::ret_float()
{
    return dcCallFloat(_vm, _func);
}

double DynCallFunction::ret_double()
{
    return dcCallDouble(_vm, _func);
}

void* DynCallFunction::ret_pointer()
{
    return dcCallPointer(_vm, _func);
}

const char* DynCallFunction::ret_c_str()
{
    return static_cast<const char*>(dcCallPointer(_vm, _func));
}

PluginFunction::PluginFunction(string& name, string& sig, Plugin& plug)
    : Function(name, sig), _interface(plug._library._interface), _id(plug.id)
{
}

void PluginFunction::begin_call()
{
    if (_call_data)
    {
        // Only one call at a time
        throw exception();
    }
    _call_data = _interface->begin_call(_id, _name.c_str());
}

void PluginFunction::end_call()
{
    if (_call_data)
    {
        _interface->end_call(_call_data);
        _call_data = 0;
    }
}

void PluginFunction::arg_bool(bool v)
{
    _interface->arg_bool(_call_data, v);
}

void PluginFunction::arg_char(char v)
{
    _interface->arg_char(_call_data, v);
}

void PluginFunction::arg_uchar(unsigned char v)
{
    _interface->arg_uchar(_call_data, v);
}

void PluginFunction::arg_short(short v)
{
    _interface->arg_short(_call_data, v);
}

void PluginFunction::arg_ushort(unsigned short v)
{
    _interface->arg_ushort(_call_data, v);
}

void PluginFunction::arg_int(int v)
{
    _interface->arg_int(_call_data, v);
}

void PluginFunction::arg_uint(unsigned int v)
{
    _interface->arg_uint(_call_data, v);
}

void PluginFunction::arg_long(long v)
{
    _interface->arg_long(_call_data, v);
}

void PluginFunction::arg_ulong(unsigned long v)
{
    _interface->arg_ulong(_call_data, v);
}

void PluginFunction::arg_longlong(long long v)
{
    _interface->arg_long_long(_call_data, v);
}

void PluginFunction::arg_ulonglong(unsigned long long v)
{
    _interface->arg_ulong_long(_call_data, v);
}

void PluginFunction::arg_float(float v)
{
    _interface->arg_float(_call_data, v);
}

void PluginFunction::arg_double(double v)
{
    _interface->arg_double(_call_data, v);
}

void PluginFunction::arg_pointer(void* v)
{
    _interface->arg_pointer(_call_data, v);
}

void PluginFunction::arg_c_str(const char* v)
{
    _interface->arg_c_str(_call_data, v);
}

void PluginFunction::ret_void()
{
    _interface->call_void(_call_data);
}

bool PluginFunction::ret_bool()
{
    return _interface->call_bool(_call_data);
}

char PluginFunction::ret_char()
{
    return _interface->call_char(_call_data);
}

unsigned char PluginFunction::ret_uchar()
{
    return _interface->call_uchar(_call_data);
}

short PluginFunction::ret_short()
{
    return _interface->call_short(_call_data);
}

unsigned short PluginFunction::ret_ushort()
{
    return _interface->call_ushort(_call_data);
}

int PluginFunction::ret_int()
{
    return _interface->call_int(_call_data);
}

unsigned int PluginFunction::ret_uint()
{
    return _interface->call_uint(_call_data);
}

long PluginFunction::ret_long()
{
    return _interface->call_long(_call_data);
}

unsigned long PluginFunction::ret_ulong()
{
    return _interface->call_ulong(_call_data);
}

long long PluginFunction::ret_longlong()
{
    return _interface->call_long_long(_call_data);
}

unsigned long long PluginFunction::ret_ulonglong()
{
    return _interface->call_ulong_long(_call_data);
}

float PluginFunction::ret_float()
{
    return _interface->call_float(_call_data);
}

double PluginFunction::ret_double()
{
    return _interface->call_double(_call_data);
}

void* PluginFunction::ret_pointer()
{
    return _interface->call_pointer(_call_data);
}

const char* PluginFunction::ret_c_str()
{
    return _interface->call_c_str(_call_data);
}
