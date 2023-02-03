#ifndef SINGLE_READER_SINGLE_WRITER_LOCKLESS_FIFO_H
#define SINGLE_READER_SINGLE_WRITER_LOCKLESS_FIFO_H

#include <vector>
#include <cstdint>
#include <align_macros.h>

// ----------------------------------------------
// --- SingleWriterSingleReaderLocklessFifo.h ---
// ----------------------------------------------
namespace cpaf { namespace concurrent {
/// TODO: Must update to use atomics, and new C++11 align features!
template <typename T, class Allocator = std::allocator<T> >
class SingleWriterSingleReaderLocklessFifo
{
private:
    typedef std::vector<T, Allocator >    QueueVec_t;
public:
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    typedef T                                     value_type;
    typedef std::uint32_t                         size_type;
    typedef Allocator                             allocator_type;
    typedef typename QueueVec_t::difference_type  difference_type;
    typedef typename QueueVec_t::reference        reference;
    typedef typename QueueVec_t::const_reference  const_reference;
    typedef typename QueueVec_t::pointer          pointer;
    typedef typename QueueVec_t::const_pointer 	const_pointer;

    explicit SingleWriterSingleReaderLocklessFifo ( size_type queueSize )
        : m_writeIndex(0)
        , m_queue(queueSize)
        , m_readIndex(0)
    {

    }

    SingleWriterSingleReaderLocklessFifo ( size_type queueSize, const allocator_type& allocator )
        : m_writeIndex(0)
        , m_queue(queueSize, allocator)
        , m_readIndex(0)
    {

    }

    bool  push ( T&& v )
    {
        size_type nextWriteIndex = incIndex(m_writeIndex);
        if ( nextWriteIndex != m_readIndex ) {
            m_queue[m_writeIndex] = std::move(v);
            m_writeIndex = nextWriteIndex;
            return true;
        }
        return false;
    }

    bool  push ( const T& v )
    {
        size_type nextWriteIndex = incIndex(m_writeIndex);
        if ( nextWriteIndex != m_readIndex ) {
            m_queue[m_writeIndex] = v;
            m_writeIndex = nextWriteIndex;
            return true;
        }
        return false;
    }


    bool  pop ()
    {
        if ( empty () ) return false;
        m_readIndex = incIndex (m_readIndex);
        return true;
    }

    T&  front ()
    {
        return m_queue[m_readIndex];
    }

    bool full () const
    {
        size_type writeCheck = incIndex(m_writeIndex);
        return (writeCheck == m_readIndex);
    }

    bool empty () const
    {
        return (m_writeIndex == m_readIndex);
    }

    size_type size() const
    {
        int64_t sz = static_cast<int64_t>(m_writeIndex) - static_cast<int64_t>(m_readIndex);
        return static_cast<size_t>(sz >= 0 ? sz : - sz -1);
    }

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------
    size_type    incIndex ( size_type index ) const { return (index +1) % m_queue.size(); }

    // ------------------------
    // PRIVATE: Member data ---
    // ------------------------
    _ALIGNED_TYPE(size_type, 64) aligned_size_type_t;

    volatile aligned_size_type_t  m_writeIndex;   // Aligning to avoid "false sharing"
    QueueVec_t                    m_queue;
    volatile aligned_size_type_t  m_readIndex;    // Aligning to avoid "false sharing"

};

}} // END namespaces cpaf concurrent

#endif // SINGLE_READER_SINGLE_WRITER_LOCKLESS_FIFO_H
