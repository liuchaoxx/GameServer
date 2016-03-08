#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include "BaseType.h"

#define EACH_ARRAY_SIZE 256

template< class T, class ThreadModel, class ResizePolicy >
class MemoryPool : public ThreadModel
{
	struct TArray
	{
		T **m_array;

		TArray ()
		{
			this->m_array = new T*[EACH_ARRAY_SIZE];
			for ( int i = 0; i < EACH_ARRAY_SIZE; i++ )
			{
				m_array[i] = NULL;
			}
		}

		~TArray ()
		{
			for ( int i = 0; i < EACH_ARRAY_SIZE; i++ )
			{
				if ( m_array[i] != NULL )
				{
					SAFE_DELETE ( m_array[i] );
				}
			}
			SAFE_DELETE_ARRAY ( m_array );
		}
	};

public:
	MemoryPool ( UINT maxSize ) 
	{
		this->m_MaxSize = maxSize;
		this->m_PosIndex= 0;
		this->m_ArrayIndex = 0;
		this->m_ArraySize = m_MaxSize / EACH_ARRAY_SIZE;
		if ( m_MaxSize % EACH_ARRAY_SIZE > 0 )
		{
			this->m_ArraySize = this->m_ArraySize + 1;
		}

		this->m_tArrays = new TArray*[m_ArraySize];
		for ( int i = 0; i < m_ArraySize; i++ )
		{
			m_tArrays[i] = NULL;
		}
	}

	~MemoryPool ()
	{
		for ( int i = 0; i < m_ArraySize; i++ )
		{
			if( m_tArrays[i] != NULL )
			{
				SAFE_DELETE ( m_tArrays[i] );
			}
		}
		SAFE_DELETE_ARRAY ( m_tArrays );
	}

	T* Alloc ( VOID )
	{
		typename Lock lock (*this);

		if ( m_PosIndex >= EACH_ARRAY_SIZE )
		{
			//ÐèÒªresize
			if ( m_ArrayIndex >= m_ArraySize )
				return NULL;

			m_ArrayIndex++;
			m_PosIndex = 0;
		}

		TArray *pArray = m_tArrays[m_ArrayIndex];
		if ( pArray == NULL )
		{
			pArray = new TArray();
			m_tArrays[m_ArrayIndex] = pArray;
		}
			
		T *tobj = pArray->m_array[m_PosIndex];
		if (tobj == NULL)
		{
			tobj = new T;
			pArray->m_array[m_PosIndex] = tobj;
		}

		tobj->SetPoolID ( m_ArrayIndex * EACH_ARRAY_SIZE + m_PosIndex );
		m_PosIndex++;

		return tobj;
	}


	VOID Free ( T* pObj )
	{
		typename Lock lock ( *this );

		if ( pObj == NULL )
			return;

		UINT del_pool = pObj->GetPoolID ();
		if ( del_pool < 0 || del_pool > m_ArrayIndex * EACH_ARRAY_SIZE + m_PosIndex )
			return;

		UINT _del_pos_index = del_pool % EACH_ARRAY_SIZE;
		UINT _del_array_index = del_pool / EACH_ARRAY_SIZE;

		TArray *pArray = m_tArrays[_del_array_index];
		if ( pArray == NULL )
			return;

		TArray *tArray = m_tArrays[m_ArrayIndex];
		if ( tArray == NULL )
			return;

		m_PosIndex--;

		pArray->m_array[_del_pos_index] = tArray->m_array[m_PosIndex];
		tArray->m_array[m_PosIndex] = pObj;

		pArray->m_array[_del_pos_index]->SetPoolID ( del_pool );
		tArray->m_array[m_PosIndex]->SetPoolID ( INVALID_ID );
		
		if ( m_PosIndex == 0 && m_ArrayIndex > 0 )
		{
			m_ArrayIndex--;
			m_PosIndex = EACH_ARRAY_SIZE;
		}
	}

	UINT Size ()const
	{
		typename Lock lock ( *this );
		return m_ArrayIndex * EACH_ARRAY_SIZE + m_PosIndex;
	}

private:

	void Resize ()
	{

	}

	TArray **m_tArrays;
	UINT m_MaxSize;
	UINT m_ArraySize;
	UINT m_PosIndex;
	UINT m_ArrayIndex;
};

#endif