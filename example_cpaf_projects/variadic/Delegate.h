#ifndef DELEGATE_H
#define DELEGATE_H
#include <iostream>


template <typename RetType, class... ArgTypes>
class Delegate
{
public:
    typedef RetType return_type_t;
    typedef return_type_t (*StaticFunT)(ArgTypes...);

    class GenericClass;
    typedef return_type_t (GenericClass::* GenericClassMemberFunction)(ArgTypes...);

    void addCallback(StaticFunT fun)
    {
        m_fun = fun;
    }

    return_type_t invoke(ArgTypes... args)
    {
        if (m_fun) return m_fun(args...);
        else return invokeMemberFun(args...);
    }


    template <typename ClassType, typename ClassMemberFunctionType>
    void addCallback(ClassType c, ClassMemberFunctionType member_function )
    {
        m_class = reinterpret_cast<GenericClass*>(c);
        m_member_function = reinterpret_cast<GenericClassMemberFunction>(member_function);
    }

private:
    StaticFunT                  m_fun = nullptr;
    GenericClass*               m_class;
    GenericClassMemberFunction  m_member_function;

    return_type_t invokeMemberFun(ArgTypes... args)
    {
        return (m_class->*m_member_function)(args...);
    }
};


int Delegate_test();


#endif // DELEGATE_H
