//////////////////////////////////////////////////////////////////////////
//summary:sha1 digest�㷨��ʵ����
//author:���滳
//////////////////////////////////////////////////////////////////////////
#ifndef _CSHA1DIGESTER_H
#define _CSHA1DIGESTER_H

#include "CommonDefs.h"

namespace sqr
{
namespace Digester
{
//<summary>sha1 digester�ࡣ��sha1ǩ���㷨��</summary>
// typedef should not be placed inside a namespace.  flying commented.
#ifndef _WIN32
typedef unsigned int		uint;
#endif
class COMMON_API CSha1Digester
{
private:
	uint	m_pState[5];
	uint	m_pCount[2];
	uint8	m_pBuffer[64];

	void Process(uint8 pBuffer[64]);
public:

	//<summary>��ʼ�׽���ǩ������</summary>
	void Begin();


	//<summary>����Ҫ�׽���ǩ���������ݡ�</summary>
	//<param name="pData">��Ҫ���׽������ݡ�</param>
	//<param name="uLength">���ݵĳ��ȡ�</param>
	void Append(uint8* pData,uint uLength);


	//<summary>�����׽�����ȡ���ݵ�ǩ��ֵ��</summary>
	//<param name="pDigest">�����Ƶ�ǩ��ֵ��</param>
	void End(uint8 pDigest[20]);
};
}
}

::Digester;

#endif /* _CSHA1DIGESTER_H */
