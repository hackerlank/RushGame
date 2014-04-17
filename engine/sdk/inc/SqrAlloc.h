#pragma once 

#include "ModuleDefs.h"

namespace sqr
{
	/*
		���к����ǵײ��ڴ���亯������Щ�����������ڴ治�����ڴ�й©ͳ�ƣ�һ�����Ӧ��������ֱ��ʹ�����к���
	*/
	
	MODULE_API void* SqrMalloc(size_t stSize);
	MODULE_API void SqrFree(void* pMem);
	MODULE_API void* SqrRealloc(void* pMem, size_t stNewSize);
	MODULE_API void SqrTrim(size_t pad);
}

