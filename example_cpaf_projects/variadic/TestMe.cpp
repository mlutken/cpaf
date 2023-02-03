#include "TestMe.h"
#include <iostream>


using namespace std;

// --------------
// --- TestMe ---
// --------------


TestMe::~TestMe()
{
    cout << "Destructor TestMe: (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
}

TestMe::TestMe()
{
    cout << "Default constructor TestMe(): (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
}

TestMe::TestMe(const TestMe& source)
    : m_i(source.m_i), m_name (source.m_name)
{
    cout << "COPY Constructor TestMe(int): (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
}

TestMe::TestMe(TestMe&& source)
    : m_i(std::move(source.m_i)), m_name (std::move(source.m_name))
{
    cout << "MOVE Constructor TestMe(int): (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
}

TestMe& TestMe::operator=(const TestMe& source)
{
    m_i = source.m_i;
    m_name = source.m_name;
    cout << "assignment operator TestMe: (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
    return *this;
}

TestMe& TestMe::operator=(TestMe&& source)
{
    m_i = std::move(source.m_i);
    m_name = std::move(source.m_name);
    cout << "MOVE assignment operator TestMe: (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
    return *this;
}

TestMe::TestMe(int i)
    : m_i(i)
{
    cout << "Constructor TestMe(int): (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
}

TestMe::TestMe(const string& name)
    : m_name(name)
{
    cout << "Constructor TestMe(string): (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
}

TestMe::TestMe(int i, const string& name)
    : m_i(i), m_name(name)
{
    cout << "Constructor TestMe(int, string): (i, name) : (" << m_i << " , '" << m_name << "')" << endl;
}

ostream& operator<<(ostream& os, const TestMe& source)
{
    os << "(i, name): (" << source.m_i << " , '" << source.m_name << "')";
    return os;
}

// --------------
// --- TestUs ---
// --------------


TestUs::TestUs()
{
    cout << "Default Constructor TestUs" << endl;
}

TestUs::~TestUs()
{
    cout << "Destructor TestUs: (f, i, name) : (" << m_f << ", " << m_me.m_i << " , '" << m_me.m_name << "')" << endl;
}

TestUs::TestUs(const TestUs& source)
    : m_f(source.m_f), m_me(source.m_me)
{
    cout << "COPY Constructor TestUs: (f, i, name) : (" << m_f << ", " << m_me.m_i << " , '" << m_me.m_name << "')" << endl;
}

TestUs::TestUs(TestUs&& source)
    : m_f(std::move(source.m_f)), m_me(std::move(source.m_me))
{
    cout << "MOVE Constructor TestUs: (f, i, name) : (" << m_f << ", " << m_me.m_i << " , '" << m_me.m_name << "')" << endl;
}

TestUs& TestUs::operator=(const TestUs& source)
{
    m_f = source.m_f;
    m_me = source.m_me;
    cout << "Assignement operator TestUs: (f, i, name) : (" << m_f << ", " << m_me.m_i << " , '" << m_me.m_name << "')" << endl;
    return *this;
}

TestUs& TestUs::operator=(TestUs&& source)
{
    m_f = std::move(source.m_f);
    m_me = std::move(source.m_me);
    cout << "Assignement operator TestUs: (f, i, name) : (" << m_f << ", " << m_me.m_i << " , '" << m_me.m_name << "')" << endl;
    return *this;
}

TestUs::TestUs(float f, const TestMe& me)
    : m_f(f), m_me(me)
{
    cout << "Constructor TestUs(float, TestMe): (f, i, name) : (" << m_f << ", " << m_me.m_i << " , '" << m_me.m_name << "')" << endl;
}

TestUs::TestUs(float f, TestMe&& me)
    : m_f(f), m_me(std::move(me))
{
    cout << "Constructor TestUs(float, TestMe&&): (f, i, name) : (" << m_f << ", " << m_me.m_i << " , '" << m_me.m_name << "')" << endl;
}


ostream& operator<<(ostream& os, const TestUs& source)
{
    os << "f; me: " << source.m_f << " ; " << source.m_me;
    return os;
}

