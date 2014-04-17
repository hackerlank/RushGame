#pragma once
#include "CSyncVariantClientHolder.h"
#include "CCypherVariantStream.h"
#include "TVariantStream.inl"

template<typename CmdType>
bool CSyncVariantClientHolder::Decode(const CmdType* pCmd, bool bCheckDecodeTimes, const char* szVariantTypeName)
{
#ifdef CACHE_VARIANT
	if (m_bDecodeBlock)
		return true;
#endif

	bool bResult;

	if ( m_uDecodeTimes == 0 )
	{
		bResult = true;
		m_uDecodeTimes = pCmd->GetEncodeTimes();
	}
	else
	{
		++m_uDecodeTimes;
		bResult = pCmd->GetEncodeTimes() == m_uDecodeTimes;
		if( !bResult )
		{
			bResult = bCheckDecodeTimes ? false : true;
			if ( !bResult )
			{
				ostringstream strm;
				strm << szVariantTypeName << "����������˳��ƥ��" << endl
					<< "����ʱ������" << (uint32)pCmd->GetEncodeTimes() << endl
					<< "����ʱ������" << (uint32)m_uDecodeTimes << endl;		
				LogErr("����������˳��ƥ��", strm.str());
			}
			m_uDecodeTimes = pCmd->GetEncodeTimes();
		}
	}
	++m_uRealDecodeTimes;

	CCypherVariantStream strm;
	strm.Write( reinterpret_cast<const char*>(pCmd+1), pCmd->GetPayloadSize() );
	CCypherVariantHolderForDecode::Decode(strm);

	return bResult;
}

