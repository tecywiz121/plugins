#ifndef __FUNCTION_H__
#define __FUNCTION_H__ 1
#include <exception>
#include "dyncall.h"
#include "util.h"
#include "plugin.hpp"

typedef void (*fptr)(void);

template<typename TReturn, typename First, typename... Rest> class invoker;

class function
{
    template<typename TReturn, typename First, typename... Rest> friend class invoker;
private:
    void check_signature(int fromend, char type);
    void check_return(char type);
protected:
    const std::string _name;
    const std::string _signature;
    std::string _arguments;
    char _return;
    virtual void begin_call() =0;
    virtual void end_call() =0;
    virtual void arg_bool(bool v) =0;
    virtual void arg_char(char v) =0;
    virtual void arg_uchar(unsigned char v) =0;
    virtual void arg_short(short v) =0;
    virtual void arg_ushort(unsigned short v) =0;
    virtual void arg_int(int v) =0;
    virtual void arg_uint(unsigned int v) =0;
    virtual void arg_long(long v) =0;
    virtual void arg_ulong(unsigned long v) =0;
    virtual void arg_longlong(long long v) =0;
    virtual void arg_ulonglong(unsigned long long v) =0;
    virtual void arg_float(float v) =0;
    virtual void arg_double(double v) =0;
    virtual void arg_pointer(void* v) =0;
    virtual void arg_c_str(const char* v) =0;

    virtual void ret_void() =0;
    virtual bool ret_bool() =0;
    virtual char ret_char() =0;
    virtual unsigned char ret_uchar() =0;
    virtual short ret_short() =0;
    virtual unsigned short ret_ushort() =0;
    virtual int ret_int() =0;
    virtual unsigned int ret_uint() =0;
    virtual long ret_long() =0;
    virtual unsigned long ret_ulong() =0;
    virtual long long ret_longlong() =0;
    virtual unsigned long long ret_ulonglong() =0;
    virtual float ret_float() =0;
    virtual double ret_double() =0;
    virtual void* ret_pointer() =0;
    virtual const char* ret_c_str() =0;
public:
    function(const std::string& name, const std::string& signature);
    template<typename TReturn, typename... Arguments>
    TReturn invoke(Arguments... args);
    const std::string& name();
    const std::string& signature();
};

class dyncall_function : public function
{
private:
    DCCallVM *_vm = 0;
    void* _func;
protected:
    virtual void begin_call();
    virtual void end_call();
    virtual void arg_bool(bool v);
    virtual void arg_char(char v);
    virtual void arg_uchar(unsigned char v);
    virtual void arg_short(short v);
    virtual void arg_ushort(unsigned short v);
    virtual void arg_int(int v);
    virtual void arg_uint(unsigned int v);
    virtual void arg_long(long v);
    virtual void arg_ulong(unsigned long v);
    virtual void arg_longlong(long long v);
    virtual void arg_ulonglong(unsigned long long v);
    virtual void arg_float(float v);
    virtual void arg_double(double v);
    virtual void arg_pointer(void* v);
    virtual void arg_c_str(const char* v);

    virtual void ret_void();
    virtual bool ret_bool();
    virtual char ret_char();
    virtual unsigned char ret_uchar();
    virtual short ret_short();
    virtual unsigned short ret_ushort();
    virtual int ret_int();
    virtual unsigned int ret_uint();
    virtual long ret_long();
    virtual unsigned long ret_ulong();
    virtual long long ret_longlong();
    virtual unsigned long long ret_ulonglong();
    virtual float ret_float();
    virtual double ret_double();
    virtual void* ret_pointer();
    virtual const char* ret_c_str();
public:
    dyncall_function(std::string& name, std::string& sig, fptr func);
};

class plugin_function : public function
{
private:
    struct plugin_interface* _interface;
    const int _id;
    void* _call_data = 0;
protected:
    virtual void begin_call();
    virtual void end_call();
    virtual void arg_bool(bool v);
    virtual void arg_char(char v);
    virtual void arg_uchar(unsigned char v);
    virtual void arg_short(short v);
    virtual void arg_ushort(unsigned short v);
    virtual void arg_int(int v);
    virtual void arg_uint(unsigned int v);
    virtual void arg_long(long v);
    virtual void arg_ulong(unsigned long v);
    virtual void arg_longlong(long long v);
    virtual void arg_ulonglong(unsigned long long v);
    virtual void arg_float(float v);
    virtual void arg_double(double v);
    virtual void arg_pointer(void* v);
    virtual void arg_c_str(const char* v);

    virtual void ret_void();
    virtual bool ret_bool();
    virtual char ret_char();
    virtual unsigned char ret_uchar();
    virtual short ret_short();
    virtual unsigned short ret_ushort();
    virtual int ret_int();
    virtual unsigned int ret_uint();
    virtual long ret_long();
    virtual unsigned long ret_ulong();
    virtual long long ret_longlong();
    virtual unsigned long long ret_ulonglong();
    virtual float ret_float();
    virtual double ret_double();
    virtual void* ret_pointer();
    virtual const char* ret_c_str();
public:
    plugin_function(std::string& name, std::string& sig, plugin& plugin);
};

#include "invoker.hpp"

template<typename TReturn, typename... Arguments>
TReturn function::invoke(Arguments... args)
{
    if (sizeof...(args) != _arguments.size())
    {
        throw std::exception();
    }
    begin_call();
    return invoker<TReturn, Arguments...>::invoke(*this, args...);
}

#endif
