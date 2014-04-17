#pragma once
#include "ModuleDefs.h"

namespace sqr
{
	//�����⼸�������ڵ���֮�󷵻ص��ִ��������FreeUtfCvsDest�����ڴ��ͷ�
	MODULE_API char* U16_to_U8(const wchar_t* wzSrc);
	MODULE_API wchar_t* U8_to_U16(const char* szSrc);
	
	MODULE_API char* U16_to_MB(const wchar_t* wzSrc);

	MODULE_API void FreeUtfCvsDest(void* dest);
}
