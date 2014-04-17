#pragma once
#include "CMallocObject.h"

namespace sqr
{
	//������Դ��������������ֻ��¼һ��
	
	class CArtErrorData;

	class CArtError : public CMallocObject
	{
	public:
		static CArtError& Inst();

		void ArtError(const std::string& sErrorType);
		void ArtError(const std::string& sErrorType, const std::string& sErrorMsg);

	private:
		CArtError();
		~CArtError();
	
		CArtErrorData*	m_pData;
	};

}

