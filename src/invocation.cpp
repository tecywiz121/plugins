#include "invocation.hpp"
#include "function.hpp"

// invocation

invocation::invocation(const function& of)
    : _of(of)
{

}
invocation::~invocation()
{

}

// dyncall_invocation

dyncall_invocation::dyncall_invocation(const dyncall_function& of)
    : invocation(of), _vm(dcNewCallVM(4096), dcFree), _func(of._func)
{
    dcMode(_vm.get(), DC_CALL_C_DEFAULT);
    dcReset(_vm.get());
}

dyncall_invocation::~dyncall_invocation()
{
}

void dyncall_invocation::arg_bool(bool v)
{
    dcArgBool(_vm.get(), v);
}

void dyncall_invocation::arg_char(char v)
{
    dcArgChar(_vm.get(), v);
}

void dyncall_invocation::arg_uchar(unsigned char v)
{
    dcArgChar(_vm.get(), v);
}

void dyncall_invocation::arg_short(short v)
{
    dcArgShort(_vm.get(), v);
}

void dyncall_invocation::arg_ushort(unsigned short v)
{
    dcArgShort(_vm.get(), v);
}

void dyncall_invocation::arg_int(int v)
{
    dcArgInt(_vm.get(), v);
}

void dyncall_invocation::arg_uint(unsigned int v)
{
    dcArgInt(_vm.get(), v);
}

void dyncall_invocation::arg_long(long v)
{
    dcArgLong(_vm.get(), v);
}

void dyncall_invocation::arg_ulong(unsigned long v)
{
    dcArgLong(_vm.get(), v);
}

void dyncall_invocation::arg_longlong(long long v)
{
    dcArgLongLong(_vm.get(), v);
}

void dyncall_invocation::arg_ulonglong(unsigned long long v)
{
    dcArgLongLong(_vm.get(), v);
}

void dyncall_invocation::arg_float(float v)
{
    dcArgFloat(_vm.get(), v);
}

void dyncall_invocation::arg_double(double v)
{
    dcArgDouble(_vm.get(), v);
}

void dyncall_invocation::arg_pointer(void* v)
{
    dcArgPointer(_vm.get(), v);
}

void dyncall_invocation::arg_c_str(const char* v)
{
    dcArgPointer(_vm.get(), const_cast<char*>(v));
}

void dyncall_invocation::ret_void()
{
    dcCallVoid(_vm.get(), _func);
}

bool dyncall_invocation::ret_bool()
{
    return dcCallBool(_vm.get(), _func);
}

char dyncall_invocation::ret_char()
{
    return dcCallChar(_vm.get(), _func);
}

unsigned char dyncall_invocation::ret_uchar()
{
    return dcCallChar(_vm.get(), _func);
}

short dyncall_invocation::ret_short()
{
    return dcCallShort(_vm.get(), _func);
}

unsigned short dyncall_invocation::ret_ushort()
{
    return dcCallShort(_vm.get(), _func);
}

int dyncall_invocation::ret_int()
{
    return dcCallInt(_vm.get(), _func);
}

unsigned int dyncall_invocation::ret_uint()
{
    return dcCallInt(_vm.get(), _func);
}

long dyncall_invocation::ret_long()
{
    return dcCallLong(_vm.get(), _func);
}

unsigned long dyncall_invocation::ret_ulong()
{
    return dcCallLong(_vm.get(), _func);
}

long long dyncall_invocation::ret_longlong()
{
    return dcCallLongLong(_vm.get(), _func);
}

unsigned long long dyncall_invocation::ret_ulonglong()
{
    return dcCallLongLong(_vm.get(), _func);
}

float dyncall_invocation::ret_float()
{
    return dcCallFloat(_vm.get(), _func);
}

double dyncall_invocation::ret_double()
{
    return dcCallDouble(_vm.get(), _func);
}

void* dyncall_invocation::ret_pointer()
{
    return dcCallPointer(_vm.get(), _func);
}

const char* dyncall_invocation::ret_c_str()
{
    return static_cast<const char*>(dcCallPointer(_vm.get(), _func));
}

// plugin_invocation

plugin_invocation::plugin_invocation(const plugin_function& of)
    : invocation(of), _interface(of._interface)
{
    _call_data = _interface->begin_plugin_call(of._id, of.name().c_str());
}

plugin_invocation::~plugin_invocation()
{
    if (_call_data) {
        _interface->plugin_invoke.end_call(_call_data);
        _call_data = 0;
    }
}

void plugin_invocation::arg_bool(bool v)
{
    _interface->plugin_invoke.arg_bool(_call_data, v);
}

void plugin_invocation::arg_char(char v)
{
    _interface->plugin_invoke.arg_char(_call_data, v);
}

void plugin_invocation::arg_uchar(unsigned char v)
{
    _interface->plugin_invoke.arg_uchar(_call_data, v);
}

void plugin_invocation::arg_short(short v)
{
    _interface->plugin_invoke.arg_short(_call_data, v);
}

void plugin_invocation::arg_ushort(unsigned short v)
{
    _interface->plugin_invoke.arg_ushort(_call_data, v);
}

void plugin_invocation::arg_int(int v)
{
    _interface->plugin_invoke.arg_int(_call_data, v);
}

void plugin_invocation::arg_uint(unsigned int v)
{
    _interface->plugin_invoke.arg_uint(_call_data, v);
}

void plugin_invocation::arg_long(long v)
{
    _interface->plugin_invoke.arg_long(_call_data, v);
}

void plugin_invocation::arg_ulong(unsigned long v)
{
    _interface->plugin_invoke.arg_ulong(_call_data, v);
}

void plugin_invocation::arg_longlong(long long v)
{
    _interface->plugin_invoke.arg_long_long(_call_data, v);
}

void plugin_invocation::arg_ulonglong(unsigned long long v)
{
    _interface->plugin_invoke.arg_ulong_long(_call_data, v);
}

void plugin_invocation::arg_float(float v)
{
    _interface->plugin_invoke.arg_float(_call_data, v);
}

void plugin_invocation::arg_double(double v)
{
    _interface->plugin_invoke.arg_double(_call_data, v);
}

void plugin_invocation::arg_pointer(void* v)
{
    _interface->plugin_invoke.arg_pointer(_call_data, v);
}

void plugin_invocation::arg_c_str(const char* v)
{
    _interface->plugin_invoke.arg_c_str(_call_data, v);
}

void plugin_invocation::ret_void()
{
    _interface->plugin_invoke.ret_void(_call_data);
}

bool plugin_invocation::ret_bool()
{
    return _interface->plugin_invoke.ret_bool(_call_data);
}

char plugin_invocation::ret_char()
{
    return _interface->plugin_invoke.ret_char(_call_data);
}

unsigned char plugin_invocation::ret_uchar()
{
    return _interface->plugin_invoke.ret_uchar(_call_data);
}

short plugin_invocation::ret_short()
{
    return _interface->plugin_invoke.ret_short(_call_data);
}

unsigned short plugin_invocation::ret_ushort()
{
    return _interface->plugin_invoke.ret_ushort(_call_data);
}

int plugin_invocation::ret_int()
{
    return _interface->plugin_invoke.ret_int(_call_data);
}

unsigned int plugin_invocation::ret_uint()
{
    return _interface->plugin_invoke.ret_uint(_call_data);
}

long plugin_invocation::ret_long()
{
    return _interface->plugin_invoke.ret_long(_call_data);
}

unsigned long plugin_invocation::ret_ulong()
{
    return _interface->plugin_invoke.ret_ulong(_call_data);
}

long long plugin_invocation::ret_longlong()
{
    return _interface->plugin_invoke.ret_long_long(_call_data);
}

unsigned long long plugin_invocation::ret_ulonglong()
{
    return _interface->plugin_invoke.ret_ulong_long(_call_data);
}

float plugin_invocation::ret_float()
{
    return _interface->plugin_invoke.ret_float(_call_data);
}

double plugin_invocation::ret_double()
{
    return _interface->plugin_invoke.ret_double(_call_data);
}

void* plugin_invocation::ret_pointer()
{
    return _interface->plugin_invoke.ret_pointer(_call_data);
}

const char* plugin_invocation::ret_c_str()
{
    return _interface->plugin_invoke.ret_c_str(_call_data);
}
