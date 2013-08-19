#ifndef __IN_INVOKER_H__
#   error __FILE__ should not be included directly
#endif

template<typename TReturn>
TReturn Invoker<TReturn, void>::invoke(Function& func)
{
    static_assert(sizeof(TReturn) < 0, "unsupported return type");
    return TReturn();
}

template<>
inline void Invoker<void, void>::invoke(Function& func)
{
    func.check_return('v');
    func.ret_void();
    func.end_call();
}

template<typename TReturn, typename First, typename... Rest>
inline TReturn Invoker<TReturn, First, Rest...>::invoke(Function& func, First first, Rest... rest)
{
    static_assert(sizeof(First) < 0, "argument type not supported");
    return Invoker<TReturn, Rest...>::invoke(func, rest...);
}

#define INVOKER_SPECIALIZATION(type, ch, ff) template<typename TReturn, typename... Rest>           \
inline TReturn Invoker<TReturn, type, Rest...>::invoke(Function& func, type first, Rest... rest)    \
{                                                                                                   \
    const unsigned short int argc = sizeof...(rest) + 1;                                            \
    func.check_signature(argc, ch);                                                                 \
    func.arg_##ff(first);                                                                           \
    return Invoker<TReturn, Rest...>::invoke(func, rest...);                                        \
}                                                                                                   \
                                                                                                    \
template<>                                                                                          \
inline type Invoker<type, void>::invoke(Function& func)                                             \
{                                                                                                   \
    func.check_return(ch);                                                                          \
    type retval = func.ret_##ff();                                                                  \
    func.end_call();                                                                                \
    return retval;                                                                                  \
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
