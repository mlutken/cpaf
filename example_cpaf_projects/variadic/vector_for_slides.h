#ifndef VECTOR_FOR_SLIDES_H
#define VECTOR_FOR_SLIDES_H

template<class T>
class MyVector {
public:
    MyVector(int nMaxElements)
        : m_pData( malloc(nMaxElements*sizeof(T)) )
    {}

    ~MyVector()
    {
        for ( int i = size()-1; i >= 0; --i) {
            char* buffer = (char*)m_pData + i*sizeof(T);
            T* e = reinterpret_cast<T*>(buffer);
            e->~T();
        }
        free (m_pData);
    }

    void push_back(const T& data) {
        char* buffer = (char*)m_pData + ++m_currentSize*sizeof(T);
        new(buffer) T(data);
    }

    void push_back(T&& data) {
        char* buffer = (char*)m_pData + ++m_currentSize*sizeof(T);
        new(buffer) T(std::move(data));
    }

    template <class... ArgTypes>
    void emplace_back(ArgTypes&&... args) {
        char* buffer = (char*)m_pData + ++m_currentSize*sizeof(T);
        new(buffer) T(std::forward<ArgTypes>(args)...);
    }
};




#endif // VECTOR_FOR_SLIDES_H
