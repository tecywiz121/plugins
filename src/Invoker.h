#ifndef __INVOKER_H__
#define __INVOKER_H__ 1
#define __IN_INVOKER_H__ 1

template<typename TReturn, typename First=void, typename... Rest>
class Invoker
{
public:
    static TReturn invoke(Function& func, First first, Rest... rest);
};

template<typename TReturn>
class Invoker<TReturn, void>
{
public:
    static TReturn invoke(Function& func);
};

#define INVOKER_SPECIALIZATION_H(type) template<typename TReturn, typename... Rest> \
class Invoker <TReturn, type, Rest...>                                              \
{                                                                                   \
public:                                                                             \
    static TReturn invoke(Function& func, type first, Rest... rest);                \
};

INVOKER_SPECIALIZATION_H(bool)

INVOKER_SPECIALIZATION_H(char)
INVOKER_SPECIALIZATION_H(unsigned char)

INVOKER_SPECIALIZATION_H(short)
INVOKER_SPECIALIZATION_H(unsigned short)

INVOKER_SPECIALIZATION_H(int)
INVOKER_SPECIALIZATION_H(unsigned int)

INVOKER_SPECIALIZATION_H(long)
INVOKER_SPECIALIZATION_H(unsigned long)

INVOKER_SPECIALIZATION_H(long long)
INVOKER_SPECIALIZATION_H(unsigned long long)

INVOKER_SPECIALIZATION_H(float)
INVOKER_SPECIALIZATION_H(double)

INVOKER_SPECIALIZATION_H(void*)
INVOKER_SPECIALIZATION_H(const char*)
#include "Invoker.cpp"
#undef __IN_INVOKER_H__
#endif
