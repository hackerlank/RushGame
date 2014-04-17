#pragma once
#include "TSqrAllocObject.h"
#include "Memory.h"
#include "CSqrAllocPool.h"
#include "ExpHelper.h"

namespace sqr
{

	template<typename AllocObject>
	void* TSqrAllocObject<AllocObject>::operator new(size_t stSize)
	{
		return SqrNew(stSize, AllocObject::GetPool());	
	}

	template<typename AllocObject>
	void* TSqrAllocObject<AllocObject>::operator new[](size_t stSize)
	{
		return SqrNew(stSize, AllocObject::GetPool());			
	}

	template<typename AllocObject>
	void TSqrAllocObject<AllocObject>::operator delete(void* pMem)
	{
		SqrDelete(pMem, AllocObject::GetPool());		
	}

	template<typename AllocObject>
	void TSqrAllocObject<AllocObject>::operator delete[](void* pMem)
	{
		SqrDelete(pMem, AllocObject::GetPool());
	}

	/*
	���ǵ�poolͨ��GetPool����ȡ�ã�Ϊ�˱�֤�̰߳�ȫ
	����������������������������һ��initpool�����������ʼ����ʱ��
	���ܹ���֤pool����ʼ��
	*/

#define DefineAllocObject(ClassName)\
	template class TSqrAllocObject<ClassName>; \
	CSqrAllocPool* ClassName::GetPool()\
	{\
	static CSqrAllocPool* ClassName##_Pool = new CSqrAllocPool(#ClassName); \
	return ClassName##_Pool;\
	}\
	class ClassName##InitPool \
	{ \
	public:\
	ClassName##InitPool() \
	{ \
	ClassName::GetPool(); \
	} \
	};\
	static ClassName##InitPool	ClassName##_InitPool;


}

