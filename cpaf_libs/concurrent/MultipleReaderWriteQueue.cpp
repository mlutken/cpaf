// MultipleReaderWriteQueue.cpp : Defines the entry point for the console application.
//

//#ifdef WIN32
#include "stdafx.h"
//#endif

#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/cstdint.hpp>

#include "SingleWriterSingleReaderLocklessFifo.h"
#include "MultipleWriterSingleReaderFifo.h"
#include "ModuloFifo.h"

typedef boost::shared_mutex                         MutexT;
typedef boost::unique_lock< boost::shared_mutex >   UniqueLockT;
//typedef DC::Shared::CriticalSection               MutexT;
//typedef DC::Shared::CriticalSection::Lock         UniqueLockT;

// ---------------------------
// --- atomic_operations.h ---
// ---------------------------






// ---------------
// --- Testing ---
// ---------------
#include <iostream>

using namespace std;


struct MyRec
{
  MyRec() : m_uic(0), m_val(0) {}
  MyRec( unsigned int uic, unsigned int val ) : m_uic(uic), m_val(val) {}

  boost::uint32_t m_uic;
  boost::uint32_t m_val;
};

struct MyRecModuloPolicy
{
  unsigned int  getModuloVal( const MyRec& val ) const { return val.m_uic; }
};


std::ostream& operator<< ( std::ostream& os, const MyRec& myRec  ) 
{ 
  os << myRec.m_uic << " " << myRec.m_val; 
  return os;
}


typedef SingleWriterSingleReaderLocklessFifo<MyRec> Fifo1_t;
typedef MutipleWriterSingleReaderFifo<MyRec>        Fifo2_t;
typedef ModuloFifo<MyRec, SingleWriterSingleReaderLocklessFifo, MyRecModuloPolicy> ModuloFifo1_t;
typedef ModuloFifo<MyRec, MutipleWriterSingleReaderFifo, MyRecModuloPolicy> ModuloFifo2_t;



void SingleWriterSingleReaderLocklessFifo_test1()
{
  Fifo2_t fifo1(4);
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.push(MyRec(1,1));
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.push(MyRec(2,2));
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.push(MyRec(3,3));
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.push(MyRec(4,4));
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.pop();
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.pop();
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.pop();
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
  fifo1.pop();
  cout << "fifo1.empty(): " << fifo1.empty() << " fifo1.full(): " << fifo1.full() << "  front(): " << fifo1.front() << endl;
}


void ModuloFifo_test1()
{
  ModuloFifo2_t fifo1(4, 2);
  cout << "fifo1.empty(0): " << fifo1.empty(0) << " fifo1.full(0): " << fifo1.full(0) << "  front(0): " << fifo1.front(0) << endl;
  cout << "fifo1.empty(1): " << fifo1.empty(1) << " fifo1.full(1): " << fifo1.full(1) << "  front(1): " << fifo1.front(1) << endl << endl;
  fifo1.push(MyRec(1,1));
  cout << "fifo1.empty(0): " << fifo1.empty(0) << " fifo1.full(0): " << fifo1.full(0) << "  front(0): " << fifo1.front(0) << endl;
  cout << "fifo1.empty(1): " << fifo1.empty(1) << " fifo1.full(1): " << fifo1.full(1) << "  front(1): " << fifo1.front(1) << endl << endl;
  fifo1.push(MyRec(2,2));
  fifo1.push(MyRec(3,3));
  cout << "fifo1.empty(0): " << fifo1.empty(0) << " fifo1.full(0): " << fifo1.full(0) << "  front(0): " << fifo1.front(0) << endl;
  cout << "fifo1.empty(1): " << fifo1.empty(1) << " fifo1.full(1): " << fifo1.full(1) << "  front(1): " << fifo1.front(1) << endl << endl;
  fifo1.push(MyRec(4,4));
  fifo1.push(MyRec(5,5));
  cout << "fifo1.empty(0): " << fifo1.empty(0) << " fifo1.full(0): " << fifo1.full(0) << "  front(0): " << fifo1.front(0) << endl;
  cout << "fifo1.empty(1): " << fifo1.empty(1) << " fifo1.full(1): " << fifo1.full(1) << "  front(1): " << fifo1.front(1) << endl << endl;

  fifo1.pop(0);
  fifo1.pop(1);
  cout << "fifo1.empty(0): " << fifo1.empty(0) << " fifo1.full(0): " << fifo1.full(0) << "  front(0): " << fifo1.front(0) << endl;
  cout << "fifo1.empty(1): " << fifo1.empty(1) << " fifo1.full(1): " << fifo1.full(1) << "  front(1): " << fifo1.front(1) << endl << endl;

  fifo1.pop(0);
  fifo1.pop(1);
  cout << "fifo1.empty(0): " << fifo1.empty(0) << " fifo1.full(0): " << fifo1.full(0) << "  front(0): " << fifo1.front(0) << endl;
  cout << "fifo1.empty(1): " << fifo1.empty(1) << " fifo1.full(1): " << fifo1.full(1) << "  front(1): " << fifo1.front(1) << endl << endl;

  fifo1.pop(0);
  fifo1.pop(1);
  cout << "fifo1.empty(0): " << fifo1.empty(0) << " fifo1.full(0): " << fifo1.full(0) << "  front(0): " << fifo1.front(0) << endl;
  cout << "fifo1.empty(1): " << fifo1.empty(1) << " fifo1.full(1): " << fifo1.full(1) << "  front(1): " << fifo1.front(1) << endl << endl;
}

//template <typename T, template <typename T, class Allocator> class FifoClass, class ModuloPolicy, class Allocator = std::allocator<T> >
//class ModuloFifo



// ------------------------
// --- Threaded Testing ---
// ------------------------
const boost::uint32_t g_packetsToSendPerWriteThreadCount    = 3;
const boost::uint32_t g_writerThreadCount                   = 4;
const boost::uint32_t g_packetsToSendCount                  = g_writerThreadCount*g_packetsToSendPerWriteThreadCount;
const boost::uint32_t g_moduloBase                          = 2;      // Same as number of reader threads
const boost::uint32_t g_maxQueueSize                        = 4;
MutexT                g_incSequencenumberGuard;

ModuloFifo2_t g_fifo(g_maxQueueSize, g_moduloBase);

#ifdef WIN32
__declspec( align( 4 ) )boost::uint32_t g_sequenceNumber = 0;
#else
boost::uint32_t g_sequenceNumber = 0;
#endif

boost::uint32_t getNextSequenceNumber()
{
  UniqueLockT lock(g_incSequencenumberGuard);
  boost::uint32_t oldVal = g_sequenceNumber;
  ++g_sequenceNumber;
  return oldVal;
}

void writerThread()
{
  for (boost::uint32_t i= 0; i < g_packetsToSendPerWriteThreadCount; ++i ) {
    boost::uint32_t val = getNextSequenceNumber();
    boost::uint32_t uic = val;
    MyRec rec(uic, val);
    if ( !g_fifo.push(rec) ) {
      printf("Err queue full: %d\n", val);
    }

    printf("Write thread: %d,  %d\n", i, val);
    boost::posix_time::seconds workTime(3);  
    boost::this_thread::sleep(workTime);  
  }
}

void ModuloFifo_threadTest1()
{
  //typedef boost::shared_ptr<boost::thread> ThreadPtr_t;
  //typedef std::vector<ThreadPtr_t>        ThreadVec_t;

  //ThreadVec_t writerThreads;

  //for ( unsigned int i = 0; i < g_writerThreadCount; ++i )
  //{
  //  ThreadPtr_t writerThread ( ThreadPtr_t( newwriterThread);  
  //  writerThreads.push_back(writerThread);
  //}

  //std::cout << "main: waiting for thread" << std::endl;  

  //for ( unsigned int i = 0; i < g_writerThreadCount; ++i )
  //{
  //  writerThreads[i].join();
  //}

  std::cout << "main: done" << std::endl;  


}

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
{
  cout << "hello" << endl;
  //SingleWriterSingleReaderLocklessFifo_test1();
  //ModuloFifo_test1();
  ModuloFifo_threadTest1();
  return 0;
}
#endif

