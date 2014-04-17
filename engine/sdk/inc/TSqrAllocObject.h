#pragma once

namespace sqr
{
	/*
		TΪʵ�ʵ�����
		stCapacityΪ��pool�Ĵ�С
		uThreadΪ��pool�������̷߳��ʣ�uThread�����Զ���չ
	*/

	class CSqrAllocPool;

	template<typename AllocObject>
	class TSqrAllocObject
	{
	public:
		static void *operator new( size_t stAllocateBlock);
		static void* operator new( size_t , void* ptr) { return ptr; }
		static void *operator new[]( size_t stAllocateBlock);
		static void operator delete(void* pMem);
		static void operator delete(void* , void* ) { }
		static void operator delete[](void* pMem);
	};


#define DeclareAllocObject(ClassName) \
	class ClassName : public TSqrAllocObject<ClassName> \
	{\
	public:\
		static CSqrAllocPool* GetPool();\
	};
}

