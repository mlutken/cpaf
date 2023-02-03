#ifndef TESTME_H
#define TESTME_H

#include <string>
#include <iostream>

class TestMe
{
public:
    ~TestMe();
    TestMe();
    TestMe(const TestMe& source);
    TestMe(TestMe&& source);
    TestMe& operator=(const TestMe& source);
    TestMe& operator=(TestMe&& source);
    explicit TestMe(int i);
    TestMe( const std::string& name );
    TestMe( int i, const std::string& name );

    int         m_i     = 0;
    std::string m_name  = "";
};

std::ostream& operator<<(std::ostream& os, const TestMe& source);


class TestUs
{
public:
    TestUs();
    ~TestUs();
    TestUs(const TestUs& source);
    TestUs(TestUs&& source);
    TestUs& operator=(const TestUs& source);
    TestUs& operator=(TestUs&& source);
    TestUs(float f, const TestMe& me);
    TestUs(float f, TestMe&& me);

    float m_f = 1.5f;
    TestMe m_me;
};

#endif // TESTME_H
