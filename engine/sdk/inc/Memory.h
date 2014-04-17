#pragma once
#include <new>
#include "ModuleDefs.h"


namespace sqr
{
	class CSqrAllocPool;

	MODULE_API void* SqrNew(size_t stSize, CSqrAllocPool* pPool=NULL);
	MODULE_API void* SqrChunkNew(size_t stSize, CSqrAllocPool* pPool=NULL);
	MODULE_API void* SqrRenew(void* pMem,size_t stSize, CSqrAllocPool* pPool=NULL);
	MODULE_API void* SqrChunkRenew(void* pMem,size_t stSize, CSqrAllocPool* pPool=NULL);
	MODULE_API void SqrDelete(void* pMem, CSqrAllocPool* pPool=NULL);

	//pMemָ���������SqrNew����SqrChunkNew���ص�ֵ���������ڵ������������������������Ϊδ����
	MODULE_API size_t SqrGetMemSize(void* pMem);
	
}

#ifndef _WIN32
#define CDECL
#define THROWBADDALLOC throw(std::bad_alloc)
#else

#ifndef CDECL
#define CDECL __cdecl
#endif

#define THROWBADDALLOC

#endif


#define DEFINITION_OF_OPERATOR_NEW \
	void* CDECL operator new(size_t stSize)THROWBADDALLOC\
{\
	return sqr::SqrNew(stSize);\
}\
	void* CDECL operator new(size_t stSize, const std::nothrow_t&)throw()\
{\
	return sqr::SqrNew(stSize);\
}\
	void* CDECL operator new[](size_t stSize)THROWBADDALLOC\
{\
	return sqr::SqrNew(stSize);\
}\
	void* CDECL operator new[](size_t stSize, const std::nothrow_t&)throw()\
{\
	return sqr::SqrNew(stSize);\
}\
	\
	\
	void CDECL operator delete(void *pMem)throw()\
{\
	sqr::SqrDelete(pMem);\
}\
	void CDECL operator delete(void *pMem, const std::nothrow_t&)throw()\
{\
	sqr::SqrDelete(pMem);\
}\
	void CDECL operator delete[](void *pMem)throw()\
{\
	sqr::SqrDelete(pMem);\
}\
	void CDECL operator delete[](void *pMem, const std::nothrow_t&)throw()\
{\
	sqr::SqrDelete(pMem);\
}
