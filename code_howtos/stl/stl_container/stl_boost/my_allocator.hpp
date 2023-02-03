#ifndef _MY_ALLOCATOR_HPP_
#define _MY_ALLOCATOR_HPP_

// ****************************************************************************
// CREATED_ON	: 09-07-03
// CREATED_BY	: Martin Lutken
// ****************************************************************************

// NOTE: This is taken from: http://www.roguewave.com/support/docs/leif/toolsug/12-6.html
// ©2002 Copyright Rogue Wave Software, Inc. All Rights Reserved. Rogue Wave and 
// .h++ are registered trademarks of Rogue Wave Software, Inc. SourcePro, Stingray 
// Studio and XML Link are trademarks of Rogue Wave Software, Inc. Solution Services, 
// Assessment Service, Consulting Services, Project Success Service, Upgrade Service 
// and Education Services are trademarks of Rogue Wave Software, Inc. All other 
// trademarks are the property of their respective owners. 
// Contact Rogue Wave about documentation or support issues.

//#include <rw/tvdlist.h>
//#include <rw/cstring.h>
#include <iostream>

template <class T>
class my_allocator
{
public:
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

	my_allocator() {}
	my_allocator(const my_allocator&) {}



	/** Allocates storage for n values of T. Uses the value of hint to optimize 
		storage placement, if possible. */
	pointer   allocate(	size_type n, 
						const void * = 0) 
	{
		T* t = (T*) malloc(n * sizeof(T));
		std::cout
		<< "  used my_allocator to allocate   at address "
		<< t << " (+)" << std::endl;
		return t;
	}

	/** Deallocates storage obtained by a call to allocate. */
	void      deallocate(void* p, 
						 size_type) 
	{
		if (p) 
		{
			free(p);
			std::cout
			<< "  used my_allocator to deallocate at address "
			<< p << " (-)" << 
			std::endl;
		} 
	}

	/** Returns the address of x as a pointer type. This function and the following 
		function are used to convert references to pointers. */
	pointer           address(reference x) const 
	{ 
		return &x; 
	}

	/** Returns the address of x as a const_pointer type. */
	const_pointer     address(const_reference x) const 
	{
		return &x; 
	}

	my_allocator<T>&  operator=(const my_allocator&) 
	{ 
		return *this; 
	}

	/** Constructs an object of type T at the location of p, using the value of 
		val in the call to the constructor for T. */
	void              construct(pointer p, const T& val) 
	{ 
		new ((T*) p) T(val); 
	}

	/** Calls the destructor on the value pointed to by p. */
	void              destroy(pointer p)			///< [in]	Pointer to object to destroy
	{ 
		p->~T(); 
	}

	/** Returns the largest possible storage available through a call to allocate. */
	size_type         max_size() const { return size_t(-1); }


	// The rebind member allows a container to construct an allocator for some 
	// arbitrary data type. The allocator type is determined by the template parameter. 
	// For example, a container may need to allocate types other than T (i.e. list 
	// nodes or hash buckets). In this case the container could obtain correct type, 
	// typically with a typedef: typedef A::rebind<Node>::other  Node_Allocator;
	template <class U>
	struct rebind { typedef my_allocator<U> other; };

	template <class U>
	my_allocator(const my_allocator<U>&) {}

	template <class U>
	my_allocator& operator=(const my_allocator<U>&) 
	{ 
		return *this; 
	}

};

#endif




/*

int main()
{
  const int numItems = 100;
  std::cout << "\nCreating a RWTValDlist with a default allocator"
            << std::endl;

  RWTValDlist<RWCString> regular;


  std::cout << "\nInserting " << numItems
            << " items" << std::endl;

  for (int i = 0; i < numItems; ++i) {
    regular.insert(RWCString('a' + i, i));
  }


  std::cout << "\n\nCreating a RWTValDlist with my_allocator type"
            << std::endl;

  RWTValDlist<RWCString, my_allocator<RWCString> > custom;

  std::cout << "\nInserting " << numItems
            << " items\n" << std::endl;

  for (int i = 0; i < numItems; ++i) {
    custom.insert(RWCString('a' + i, i));
  }
  
  return 0;
}

Program Output

When compiled with the MSVC7 compiler and the Rogue Wave implementation of the C++ Standard Library, the above example would result in this output:

    Creating a RWTValDlist with a default allocator

    Inserting 100 items


    Creating a RWTValDlist with my_allocator type
      used my_allocator to allocate   at address 0080ABD0 (+)
      used my_allocator to allocate   at address 0080AC08 (+)

    Inserting 100 items

      used my_allocator to allocate   at address 0080AC40 (+)
      used my_allocator to allocate   at address 0080AC78 (+)
      used my_allocator to allocate   at address 0080C6F0 (+)
      used my_allocator to allocate   at address 0080C728 (+)
      used my_allocator to allocate   at address 0080FB28 (+)
      used my_allocator to allocate   at address 00820068 (+)
      used my_allocator to deallocate at address 00820068 (-)
      used my_allocator to deallocate at address 0080FB28 (-)
      used my_allocator to deallocate at address 0080C728 (-)
      used my_allocator to deallocate at address 0080C6F0 (-)
      used my_allocator to deallocate at address 0080AC78 (-)
      used my_allocator to deallocate at address 0080AC40 (-)
      used my_allocator to deallocate at address 0080AC08 (-)
      used my_allocator to deallocate at address 0080ABD0 (-)

In the output listed above, it can be seen that, when no custom allocator was used, the RWTValDist was created and 100 items were inserted. However, when the list was instantiated with my_allocator, instances of this class were used in 8 allocations and deallocations of heap memory. This proves that the example used my_allocator for memory management.



--------------------------------------------------------------------------------

*/