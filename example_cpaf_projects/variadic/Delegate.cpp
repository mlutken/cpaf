#include "Delegate.h"
#include <iostream>
#include <stdio.h>
#include <functional>

using namespace std;



class SomeClass {
public:
    virtual void someMemberFunc(int x)
    {
       cout << "In SomeClass: " << x << endl;
    }

    virtual int anotherMemberFunc(int x, string s)
    {
       cout << "anotherMemberFunc, x: " << x << ", s: " << s << endl;
       return 5;
    }

    static void staticMemberFunc(int x)
    {
       cout << "In SomeClass (static): " << x << endl;
    }

    static int anotherStaticMemberFunc(int x, string s)
    {
       cout << "anotherStaticMemberFunc, x: " << x << ", s: " << s << endl;
       return 15;
    }

};

class DerivedClass : public SomeClass {
public:
 // If you uncomment the next line, the code at line (*) will fail!
//    virtual void some_member_func(int x, char *p) { printf("In DerivedClass"); };
//    virtual void someMemberFunc(int x) override { cout << "In DerivedClass: " << x << endl;   }
};

template <class LambdaType>
class LambdaHolder
{
public:
    LambdaHolder(LambdaType lambda_fun)
        : m_lambda(lambda_fun)
    {
    }

    LambdaType m_lambda;
};

template <class LambdaType>
LambdaHolder<LambdaType> make_lambda_holder(LambdaType lambda_fun)
{
    return LambdaHolder<LambdaType>(lambda_fun);
}

int Delegate_test()
{
    cout << "\n--- Delegate_test() ---" << endl;

    auto l1 = [](int i) {
        cout <<  "LamdaHolder test: " << i << endl;
    };


    auto lh1 = make_lambda_holder(l1);
    lh1.m_lambda(77);
    typedef decltype(lh1) LambdaTypeT;
    cout << "sizeof(LambdaTypeT)            : " << sizeof(LambdaTypeT) << endl;
    cout << "sizeof(lh1)                    : " << sizeof(lh1) << endl;
    cout << "sizeof(int)                    : " << sizeof(int) << endl;
    cout << "sizeof(Delegate::StaticFunT)   : " << sizeof(Delegate<int, int, string>::StaticFunT) << endl;

    SomeClass someClass;

//    Delegate<void, int> del1;

//    del1.addCallback(&someClass, &SomeClass::someMemberFunc);
//    del1.invoke(7);

//    del1.addCallback( [](int i) {
//        cout <<  "Test!: " << i << endl;
//    });

//    del1.invoke(45);

    Delegate<int, int, string> del2;

    del2.addCallback(&someClass, &SomeClass::anotherMemberFunc);
    int r1 = del2.invoke(7, "seven");
    cout << "r1: " << r1 << endl;

    del2.addCallback( [](int i, string s) -> int {
        cout <<  "Test2, i : " << i  << "  s: " << s << endl;
        return 12;
    });

    int r2 = del2.invoke(22, "ged");
    cout << "r2: " << r2 << endl;

//    del2.addCallback(SomeClass::anotherStaticMemberFunc);

//    int r3 = del2.invoke(44, "hest");
//    cout << "r3: " << r3 << endl;

//    auto myType = [](int i, string s) {
//        cout <<  "Test2, i : " << i  << "  s: " << s << endl;
//        return 12;
//    };
//    typedef decltype(myType) MyOtherTypeT;
//    MyOtherTypeT myOtherType = myType;


    return 0;
}


//    template <typename CallbackType>
//    void addCallback(CallbackType cb)
//    {
////        auto capture_cb = cb;
//        std::cout << "addCallback (CallbackType)" << std::endl;
////        cb(56);
//    }

//    float fHest = 12.4;
//del1.addCallback([fHest](int i)
//{
//    cout <<  "Test!: " << i << endl;
//});

//del1.invoke(45);

//std::function<void (int)> fn = [fHest](int i)
//{
//    cout <<  "Test2: " << i << " fHest: " << fHest << endl;
//};

//fn(19);

//void Delegate::lambdaTest()
//{
//    cout << "Delegate::lambdaTest: " << endl;
//    m_fun_impl = [](void* p, int i)
//    {
//        int *pi = (int*)p;
//        cout << "Delegate: Test!: " << i << ", m_iTest: " << *pi << endl;
//    };

//    m_fun_impl(&m_iTest,4);
//    //m_fun(4);

//}

