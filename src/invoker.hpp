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

#define INVOKER_SPEC_H(type) template<typename TReturn, typename... Rest>   \
class invoker <TReturn, type, Rest...> {                                    \
public:                                                                     \
    static TReturn invoke(invocation& inv, type first, Rest... rest);       \
};

INVOKER_SPEC_H(bool)

INVOKER_SPEC_H(char)
INVOKER_SPEC_H(unsigned char)

INVOKER_SPEC_H(short)
INVOKER_SPEC_H(unsigned short)

INVOKER_SPEC_H(int)
INVOKER_SPEC_H(unsigned int)

INVOKER_SPEC_H(long)
INVOKER_SPEC_H(unsigned long)

INVOKER_SPEC_H(long long)
INVOKER_SPEC_H(unsigned long long)

INVOKER_SPEC_H(float)
INVOKER_SPEC_H(double)

INVOKER_SPEC_H(void*)
INVOKER_SPEC_H(const char*)
#include "invoker.cpp"
#undef IN_INVOKER_H
#endif
