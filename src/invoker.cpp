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
inline TReturn invoker<TReturn, First, Rest...>::invoke(invocation& inv,
                                                        First first,
                                                        Rest... rest)
{
    static_assert(sizeof(First) < 0, "argument type not supported");
    return invoker<TReturn, Rest...>::invoke(inv, rest...);
}

#define INVOKER_SPEC(type, ch, ff)                                          \
template<typename TReturn, typename... Rest>                                \
inline TReturn invoker<TReturn, type, Rest...>::invoke(invocation& inv,     \
                                                       type first,          \
                                                       Rest... rest)        \
{                                                                           \
    const unsigned short int argc = sizeof...(rest) + 1;                    \
    inv.of().check_signature(argc, ch);                                     \
    inv.arg_##ff(first);                                                    \
    return invoker<TReturn, Rest...>::invoke(inv, rest...);                 \
}                                                                           \
                                                                            \
template<>                                                                  \
inline type invoker<type, void>::invoke(invocation& inv)                    \
{                                                                           \
    inv.of().check_return(ch);                                              \
    return inv.ret_##ff();                                                  \
}


INVOKER_SPEC(bool, 'b', bool)

INVOKER_SPEC(char, 'c', char)
INVOKER_SPEC(unsigned char, 'C', uchar)

INVOKER_SPEC(short, 's', short)
INVOKER_SPEC(unsigned short, 'S', ushort)

INVOKER_SPEC(int, 'i', int)
INVOKER_SPEC(unsigned int, 'I', uint)

INVOKER_SPEC(long, 'j', long)
INVOKER_SPEC(unsigned long, 'J', ulong)

INVOKER_SPEC(long long, 'l', longlong)
INVOKER_SPEC(unsigned long long, 'L', ulonglong)

INVOKER_SPEC(float, 'f', float)
INVOKER_SPEC(double, 'd', double)

INVOKER_SPEC(void*, 'p', pointer)
INVOKER_SPEC(const char*, 'Z', c_str)
