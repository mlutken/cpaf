#ifndef SINGLE_READER_SINGLE_WRITER_LOCKLESS_FIFO_H
#define SINGLE_READER_SINGLE_WRITER_LOCKLESS_FIFO_H

#include <vector>
#include <boost/cstdint.hpp>
#include <align_macros.h>

// ----------------------------------------------
// --- SingleWriterSingleReaderLocklessFifo.h ---
// ----------------------------------------------

namespace cpaf { namespace concurrent {


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
  typedef boost::uint32_t                       size_type;
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

}} //END namespace cpaf::concurrent


#endif // SINGLE_READER_SINGLE_WRITER_LOCKLESS_FIFO_H
