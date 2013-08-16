#ifndef __FUNCTION_H__
#define __FUNCTION_H__ 1

#include <iostream>

using namespace std;

class Function;

template<typename TReturn, typename First, typename... Rest>
TReturn _invoke(Function& func, First first, Rest... rest);

class Function
{
    template<typename TReturn, typename First, typename... Rest> friend TReturn _invoke(Function& func, First first, Rest... rest);
    template<typename TReturn> friend TReturn _invoke(Function& func);
private:
    std::string _signature;
    void check_signature(int fromend, char type);
public:
    Function(string& signature);
    template<typename TReturn, typename... Arguments>
    TReturn invoke(Arguments... args)
    {
        if (sizeof...(args) != _signature.size())
        {
            throw exception();
        }
        return _invoke<TReturn, Arguments...>(*this, args...);
    }

};

Function::Function(string& sig)
{
    _signature = sig;
}

void Function::check_signature(int fromend, char type)
{
    if (_signature[_signature.size() - fromend] != type)
    {
        throw exception();
    }
}

template<typename TReturn>
TReturn _invoke(Function& func)
{
    static_assert(sizeof(TReturn) < 0, "unsupported return type");
    return TReturn();
}

template<>
void _invoke <void> (Function& func)
{
    cout << "Returning: void" << endl;
}

template<>
int _invoke <int> (Function& func)
{
    cout << "Returning: int" << endl;
    return 564;
}

template<typename TReturn, typename First, typename... Rest>
TReturn _invoke(Function& func, First first, Rest... rest)
{
    const unsigned short int argc = sizeof...(rest) + 1;
    static_assert(sizeof(First) < 0, "argument type not supported");
    return _invoke<TReturn, Rest...>(func, rest...);
}

int main(int argc, char** argv)
{
    Function f;
    f.invoke<void>(1, 2, 3, 4, 5);

    return 0;
}

#endif
