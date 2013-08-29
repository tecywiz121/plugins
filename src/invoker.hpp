#ifndef INVOKER_H
#define INVOKER_H 1
#define IN_INVOKER_H 1
#include "invocation.hpp"


template<typename TReturn, typename First=void, typename... Rest>
class invoker {
public:
    static TReturn invoke(invocation& func, First first, Rest... rest);
};

template<typename TReturn>
class invoker<TReturn, void> {
public:
    static TReturn invoke(invocation& func);
};

#define INVOKER_SPECIALIZATION_H(type) template<typename TReturn, typename... Rest> \
class invoker <TReturn, type, Rest...> {                                            \
public:                                                                             \
    static TReturn invoke(invocation& inv, type first, Rest... rest);               \
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
#include "invoker.cpp"
#undef IN_INVOKER_H
#endif
