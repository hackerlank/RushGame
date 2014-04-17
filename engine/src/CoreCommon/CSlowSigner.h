#pragma once
#include "GuidTypes.h"

namespace sqr
{

	class CSlowSigner
	{
	public:
		CSlowSigner(const guid_t& guidConn);
		~CSlowSigner(void);

		template<typename CmdType>
		void SlowSignOnData(const void* pData,size_t stDataSize,CmdType* pCmd);

		template<typename CmdType>
		void SlowSignOnMediatorCmd(CmdType* pCmd);		

	private:
		guid_t		m_guidConn;
		uint8		m_uModSignCounter;			//ר�Ÿ���ModSign�㷨���ܵ�������Ϣ�õ�Counter
	};

}

