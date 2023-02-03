// ****************************************************************************
// CREATED_ON	: 09-07-03
// CREATED_BY	: Martin Lutken
// ****************************************************************************

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#include "../my_container.hpp"
#include "../my_allocator.hpp"

using namespace std;


struct my_type
{
	int		m_iData;
	my_type()
	{
		m_iData = 0;
		printf("DEFAULT my_type::constructor, Data: %d\n", GetData());
	}
	my_type(const int i)
	{
		m_iData = i;
		//		printf("my_type::constructor. Data: %d\n", GetData());
	}
	int		GetData() const {return m_iData;}
	void	SetData(const int iData){m_iData = iData;}

	~my_type()
	{
		printf("my_type::destructor, data: %d\n", GetData());
	}

	my_type		operator+(const my_type& o)
	{
		return my_type(m_iData += o.m_iData);
	}

	bool		operator==(const my_type& o)
	{
		return (m_iData == o.m_iData);
	}


	friend ostream&		operator <<(ostream& os, const my_type& o)
	{
		os<<o.GetData();
		return os;
	}
};





/** Test const version my_container. */
void test_my_container_const()
{
	typedef int T;
//	typedef my_type T;

//	typedef my_container<T>				Con;
//	typedef my_container<const T>		Con;
	typedef const my_container<T>		Con;
//	typedef const my_container<const T>	Con;

//	typedef list<T>						Con;
//	typedef list<const T>				Con;
//	typedef const list<const T>		Con;


	printf("test_my_container_const():\n\n");
	//	typedef my_container<int>	Container;
	Con c(3, 22);
//	Con c(5);
//	Con c;
	
//	c.front() = T(12);

//	c.push_front(T(2));	// This is ONLY possible on NON const containers !!!
						// Their element type is allowed to be const though!

	Con::const_iterator it = c.begin();
//	Con::iterator it = c.begin();
	for (it = c.begin(); it != c.end(); ++it)
	{
//		cout<<"C (*it): "<<*it<<", it->GetData(): "<<it->GetData()<<endl;
//		(*it) = 4;
		cout<<"C (*it): "<<*it<<endl;
//		it->SetData(34);	// Error! Trying to call non const member method using a const_iterator!
	}
}


/** Test mutable (most common use) of my_container. */
void test_my_container()
{
	// ---------------------
	// --- Iterator test ---
	// ---------------------

	typedef int T;
//	typedef my_type T;
//	typedef my_container<T>	Con;
	typedef list<T>	Con;

	printf("test_my_container():\n\n");
	//	typedef my_container<int>	Container;
	Con c;
	c.push_front(T(3));
	Con::iterator itM1( c.begin() );	
	Con::const_iterator itC1( c.begin() );	
	cout<<"c.front(): "<<c.front()<<endl;

	c.push_front(T(2));
	Con::iterator itM2( c.begin() );	Con::const_iterator itC2( c.begin() );	
	cout<<"c.front(): "<<c.front()<<endl;

	c.push_front(T(1));
	Con::iterator itM3( c.begin() );	Con::const_iterator itC3( c.begin() );	
	cout<<"c.front(): "<<c.front()<<endl;

	Con::iterator it = c.begin();
	T i = T(0);
	for (it = c.begin(); it != c.end(); ++it)
	{
//		it->SetData(34);
//		cout<<"C (*it): "<<*it<<", it->GetData(): "<<it->GetData()<<endl;
		cout<<"M (*it): "<<*it<<endl;
		i = i + T(10);
		(*it) = i;
	}


	// ------------------------------
	// --- Compare iterators test ---
	// ------------------------------
	if ( itM1 == itM2) cout<<"==\n";
	if ( itC1 == itC2) cout<<"==\n";
	if ( itC1 == itM1) cout<<"==\n";
	if ( itM1 == itC1) cout<<"==\n";

	if ( itM1 != itM2) cout<<"!=\n";
	if ( itC1 != itC2) cout<<"!=\n";
	if ( itC1 != itM1) cout<<"!=\n";
	if ( itM1 != itC1) cout<<"!=\n";

	// ---------------------------
	// --- Const iterator test ---
	// ---------------------------

	Con::iterator itM( c.begin() );			// Just to test general copy constructor
	Con::const_iterator itC( c.begin() );	// Just to test general copy constructor
//	Con::iterator itCErr2( itC );			// Error! Initializing iterator with const_iterator! 
	itC = it;								// Just to test general assignment
//	Con::iterator itErr = itC;				// Error! Initializing iterator with const_iterator! 
	for (itC = c.begin(); c.end() != itC; itC++)
//	for (itC = c.begin(); itC != c.end(); ++itC)
	{
//		cout<<"C (*itC): "<<*itC<<", itC->GetData(): "<<itC->GetData()<<endl;
		cout<<"C (*itC): "<<*itC<<endl;
//		(*itC) = i;			// Error! Trying to change pointee object using const_iterator!
//		itC->SetData(34);	// Error! Trying to call non const member method using a const_iterator!
	}


//	c.remove(T(10));
	c.push_front(T(9));
	c.push_front(T(8));

	cout<<"\nErase insert test:\n";
	for ( it = c.begin(); it != c.end(); ++it ) cout<<"(*it): "<<*it<<endl; cout<<endl;// show
	
	Con::iterator itR = find(c.begin(), c.end(), 10);
	if (itR != c.end()) c.erase(itR);
	for ( it = c.begin(); it != c.end(); ++it ) cout<<"(*it): "<<*it<<endl; cout<<endl;// show

}



void test_my_container_insert_iterator()
{
	typedef int T;
//	typedef my_type T;
	typedef my_container<T>	Con;
//	typedef list<T>	Con;

	printf("test_my_container():\n\n");
	//	typedef my_container<int>	Container;
	Con c;
	c.push_front(T(3));
	c.push_front(T(2));
	c.push_front(T(1));
	c.push_front(T(9));
	c.push_front(T(8));

	Con::iterator it;
	for ( it = c.begin(); it != c.end(); ++it ) cout<<"(*it): "<<*it<<endl; cout<<endl;// show
	
	const int aInts[] = {1,2,3,4};

	front_insert_iterator<Con> ii(c);
	*ii++ = 0;
	*ii++ = 1;
	*ii++ = 2;
	copy(c.begin(), c.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;
	for ( it = c.begin(); it != c.end(); ++it ) cout<<"(*it): "<<*it<<endl; cout<<endl;// show

}

void test_my_container_my_allocator()
{
	typedef int T;
//	typedef my_type T;
	typedef my_container<T, my_allocator<T> >	Con;
//	typedef list<T>	Con;

	printf("test_my_container():\n\n");
	//	typedef my_container<int>	Container;
	Con c;
	c.push_front(T(3));
	c.push_front(T(2));
	c.push_front(T(1));
	c.push_front(T(9));
	c.push_front(T(8));

	Con::iterator it;
	for ( it = c.begin(); it != c.end(); ++it ) cout<<"(*it): "<<*it<<endl; cout<<endl;// show
	

}




