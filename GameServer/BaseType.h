#ifndef _BASE_TYPE_H
#define _BASE_TYPE_H

#define __WINDOWS__

#if defined(__WINDOWS__)
#pragma warning ( disable : 4786 )
#include <Windows.h>
#include "crtdbg.h"
#elif defined(__LINUX__)
#include <sys/types.h>
#include <pthread.h>
#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <setjmp.h>
#include <sys/epoll.h>
#endif

typedef unsigned char	UCHAR;			//标准无符号CHAR
typedef char			CHAR;			//标准CHAR
typedef unsigned int	UINT;			//标准无符号INT
typedef int				INT;			//标准INT
typedef unsigned short	USHORT;			//标准无符号short
typedef short			SHORT;			//标准short
typedef unsigned long	ULONG;			//标准无符号LONG(不推荐使用)
typedef long			LONG;			//标准LONG(不推荐使用)
typedef float			FLOAT;			//标准float
typedef INT				BOOL;
typedef UCHAR			BYTE;

#define INVALID_ID -1


//根据指针值删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif
//根据指针值删除数组类型内存
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif
//根据指针调用free接口
#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif
//根据指针调用Release接口
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif


#endif