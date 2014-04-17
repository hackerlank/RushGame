#pragma once

//////////////////////////////////////////////////////////////////////////
//summary:Md5 digest�㷨��ʵ����
//////////////////////////////////////////////////////////////////////////
#include "CommonDefs.h"

#ifndef _WIN32
#include "BaseTypes.h"
#endif

namespace sqr
{
	//<summary>md5 digester�ࡣ��md5ǩ���㷨��</summary>
	class COMMON_API CMd5Digester
	{
	private:
		uint	m_pCount[2];		/* message length in bits, lsw first */
		uint	m_pDigestBuffer[4];	/* digest buffer */
		uint8	m_pBuffer[64];		/* accumulate block */

		void Process(const uint8 *pData);
	public:

		//<summary>��ʼ�׽���ǩ������</summary>
		void Begin();


		//<summary>����Ҫ�׽���ǩ���������ݡ�</summary>
		//<param name="pData">��Ҫ���׽������ݡ�</param>
		//<param name="uLength">���ݵĳ��ȡ�</param>


		void Append(const uint8 *pData,uint uLength);
		//<summary>�����׽�����ȡ���ݵ�ǩ��ֵ��</summary>
		//<param name="pDigest">�����Ƶ�ǩ��ֵ��</param>
		void End(uint8 pDigest[16]);

		// ���õ���ԭʼMD5��ת��ΪBASE16��ʽ���ַ���
		void GetMD5Base16Str( char* pBase16 );

	};
}

