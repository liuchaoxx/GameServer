// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "BaseType.h"
#include "MemoryPool.h"
#include "Thread.h"

#include <iostream>

using namespace std;

class TestClass 
{ 
	UINT m_PoolID;
public:
	void SetPoolID (UINT poolID) 
	{
		m_PoolID = poolID;
	}

	UINT GetPoolID () const
	{
		return this->m_PoolID;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	MemoryPool<TestClass, MutexThread> pool ( 1000 );
	TestClass *tc = pool.Alloc ();
	if ( tc != NULL )
	{
		cout << " add pool size is " << pool.Size ( ) << endl;
		pool.Free ( tc );
		cout << " free pool size is " << pool.Size ( ) << endl;
	}
	
	while (true)
	{
		Sleep ( 10000 );
	}
	return 0;
}

