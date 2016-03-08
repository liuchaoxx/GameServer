#ifndef _THREAD_H
#define _THREAD_H

#include "BaseType.h"

#if defined(__WINDOWS__)
class Mutex
{
	CRITICAL_SECTION mtx_;
public:
	Mutex ( ) { InitializeCriticalSection ( &mtx_ ); };
	~Mutex ( ) { DeleteCriticalSection ( &mtx_ ); };
	VOID	Lock ( ) { EnterCriticalSection ( &mtx_ ); };
	VOID	Unlock ( ) { LeaveCriticalSection ( &mtx_ ); };
};
#elif defined(__LINUX__)
class Mutex
{
	pthread_mutex_t 	mtx_;
public:
	Mutex () { pthread_mutex_init ( &mtx_, NULL ); };
	~Mutex () { pthread_mutex_destroy ( &mtx_ ); };
	VOID	Lock () { pthread_mutex_lock ( &mtx_ ); };
	VOID	Unlock () { pthread_mutex_unlock ( &mtx_ ); };
};
#endif

//单线程模型
class SingleThread
{
protected:
	class Lock
	{
	public:
		explicit Lock () {}
		explicit Lock ( const SingleThread& ) {}
	};
};

//多线程模型
class MutexThread
{
protected:
	mutable Mutex mtx_;

	class Lock
	{
		const MutexThread& host_;
	public:
		explicit Lock ( const MutexThread& host ): host_ ( host ) { host_.mtx_.Lock ( ); }
		~Lock ( ) { host_.mtx_.Unlock ( ); }
	};
};

class Thread
{
public:
protected:
private:
};

#endif