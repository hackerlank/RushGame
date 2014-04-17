#pragma once


namespace sqr
{
	class CThreadAllocPool;
	class CMemCallStackInfo;

#pragma pack(push,1)
	struct CMemoryCookie
	{
		//���union���������ǰ�棬��Ϊ��m_pNext��ԭ�Ӳ���Ҫ�����8�ֽڣ�32bit�����롣
		//dlmalloc����������ڴ���׵�ַ�������Ҫ��������Щ����Ҫ�����׵�ַ��λ��
		union
		{
			CMemoryCookie*volatile		m_pNext;//���ָ����ڿ��߳��ͷ��ڴ��ʱ��ʹ��
			CMemCallStackInfo*			m_pStackInfo;
			size_t 						m_uCheckSum;			
		};

		size_t				m_stSize;
		CThreadAllocPool*	m_pPool;
	};
#pragma pack(pop)

}
