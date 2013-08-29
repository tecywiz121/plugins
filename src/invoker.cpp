#ifndef IN_INVOKER_H
#   error __FILE__ should not be included directly
#endif

template<typename TReturn>
TReturn invoker<TReturn, void>::invoke(invocation& inv)
{
    static_assert(sizeof(TReturn) < 0, "unsupported return type");
    return TReturn();
}

template<>
inline void invoker<void, void>::invoke(invocation& inv)
{
    inv.of().check_return('v');
    inv.ret_void();
}

template<typename TReturn, typename First, typename... Rest>
inline TReturn invoker<TReturn, First, Rest...>::invoke(invocation& inv, First first, Rest... rest)
{
    static_assert(sizeof(First) < 0, "argument type not supported");
    return invoker<TReturn, Rest...>::invoke(inv, rest...);
}

#define INVOKER_SPECIALIZATION(type, ch, ff) template<typename TReturn, typename... Rest>           \
inline TReturn invoker<TReturn, type, Rest...>::invoke(invocation& inv, type first, Rest... rest)   \
{                                                                                                   \
    const unsigned short int argc = sizeof...(rest) + 1;                                            \
    inv.of().check_signature(argc, ch);                                                             \
    inv.arg_##ff(first);                                                                            \
    return invoker<TReturn, Rest...>::invoke(inv, rest...);                                         \
}                                                                                                   \
                                                                                                    \
template<>                                                                                          \
inline type invoker<type, void>::invoke(invocation& inv)                                            \
{                                                                                                   \
    inv.of().check_return(ch);                                                                      \
    return inv.ret_##ff();                                                                          \
}


INVOKER_SPECIALIZATION(bool, 'b', bool)

INVOKER_SPECIALIZATION(char, 'c', char)
INVOKER_SPECIALIZATION(unsigned char, 'C', uchar)

INVOKER_SPECIALIZATION(short, 's', short)
INVOKER_SPECIALIZATION(unsigned short, 'S', ushort)

INVOKER_SPECIALIZATION(int, 'i', int)
INVOKER_SPECIALIZATION(unsigned int, 'I', uint)

INVOKER_SPECIALIZATION(long, 'j', long)
INVOKER_SPECIALIZATION(unsigned long, 'J', ulong)

INVOKER_SPECIALIZATION(long long, 'l', longlong)
INVOKER_SPECIALIZATION(unsigned long long, 'L', ulonglong)

INVOKER_SPECIALIZATION(float, 'f', float)
INVOKER_SPECIALIZATION(double, 'd', double)

INVOKER_SPECIALIZATION(void*, 'p', pointer)
INVOKER_SPECIALIZATION(const char*, 'Z', c_str)
