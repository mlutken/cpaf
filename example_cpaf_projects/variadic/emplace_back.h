#ifndef EMPLACE_BACK_H
#define EMPLACE_BACK_H

#include <iostream>

////        , m_pData (static_cast<T*> (::operator new (sizeof(T[nMaxElements]))) )

template<class T>
class MyVector
{
public:

    MyVector(int nMaxElements)
        : m_numElements(nMaxElements)
//        , m_pData( new char[nMaxElements*sizeof(T)])
        , m_pData( malloc(nMaxElements*sizeof(T)) )
        , m_currentSize(-1)
    {
    }

    ~MyVector()
    {
        using namespace std;
        cout << "~MyVector, size: " << m_currentSize << endl;
        for ( int i = size()-1; i >= 0; --i) {
//            cout << "[" << i << "]:  " << getElem(i) << endl;
            char* buffer = (char*)m_pData + i*sizeof(T);
            T* e = reinterpret_cast<T*>(buffer);
            e->~T();
        }
        //delete[] m_pData;
        free (m_pData);
    }



    int size() const
    {
        return m_currentSize + 1; // Add 1 as it is zero based index
    }


    void push_back(const T& data)
    {
        char* buffer = (char*)m_pData + ++m_currentSize*sizeof(T);
        new(buffer) T(data);
    }


    void push_back(T&& data)
    {
        char* buffer = (char*)m_pData + ++m_currentSize*sizeof(T);
        new(buffer) T(std::move(data));
    }

    template <class... ArgTypes>
    void emplace_back(ArgTypes&&... args)
    {
        char* buffer = (char*)m_pData + ++m_currentSize*sizeof(T);
        new(buffer) T(std::forward<ArgTypes>(args)...);
    }

    template <class... ArgTypes>
    void emplace_back1(ArgTypes&&... args)
    {
        char* buffer = (char*)m_pData + ++m_currentSize*sizeof(T);
        new(buffer) T(args...);
    }

    T& operator[] (int index)
    {
        return getElem(index);
    }

    const T& operator[] (int index) const
    {
        return getElem(index);
    }


    void display_vector() const
    {
        using namespace std;
        const int SIZE = size();
        cout << "display_vector, size: " << SIZE << endl;
        for(int i = 0; i < SIZE; i++)
        {
            cout << "[" << i << "] " << getElem(i);
            if(  i == size() - 1)
                cout << "\n\n";
            else
                cout << "\n";
        }
    }

private:
    T& getElem(int index)
    {
        char* const buffer = (char* const)m_pData + index*sizeof(T);
        T* const e = reinterpret_cast<T* const>(buffer);
        return *e;
    }

    const T& getElem(int index) const
    {
        const char* const buffer = (char*)m_pData + index*sizeof(T);
        const T* const e = reinterpret_cast<const T*>(buffer);
        return *e;
    }

    int m_numElements;
//    char* m_pData;
    void* m_pData;
    int m_currentSize;
};

int emplace_back_test();


#endif // EMPLACE_BACK_H
