#pragma once
#include "TSqrAllocObject.h"
#include "TSqrAllocator.h"

namespace sqr
{
	/*
		���ǰ�tick�ڲ����ڴ��������һ�����н��й���
	*/

	DeclareAllocObject(CMirrorMallocObject);

	DeclarePoolAllocator(CMirrorMallocAllocator, CMirrorMallocObject);
}
