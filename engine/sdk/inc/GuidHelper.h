#pragma once
#include "ModuleDefs.h"
#include "GuidTypes.h"

namespace sqr
{	
	//����һ���guid���ַ�����ʾ��linux�º�win���涼��ʹ��formatΪ��%08x-%04x-%04x-%04x-%012x��

	//guid ��һ��128bit���ֶΣ��������ǿ�����unsigned char 16�洢
	MODULE_API void CreateGuid(guid_t guid);
	
	MODULE_API void CreateStringGuid(str_guid_t sz_guid);

	//��guidת���ɶ�Ӧ���ַ���
	MODULE_API void GuidToString(const guid_t guid, str_guid_t sz_guid);

	//��guid�ִ�ת����Ӧ��guid
	MODULE_API void StringToGuid(const str_guid_t sz_guid, guid_t guid);

}

