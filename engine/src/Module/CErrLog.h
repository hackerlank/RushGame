#pragma once
#include "TMallocAllocator.h"
#include "ErrLogTypes.h"

namespace sqr
{
	class CErrInfoData;
	class CError;
	class CErrLog
	{
	public:
		CErrLog();
		~CErrLog();
		static CErrLog& Inst()
		{
			static CErrLog ls_Inst;
			return ls_Inst;
		}
		void CreateLogFile(const wchar_t* strErrFileName,const char* szDeviceInfo);
		void WriteErrLog(const CError& exp,const char* szErrlogInfo);
		void CloseLogFile();
	private:
		void HandleAndWriteLog(FILE* fd, const char* szErrLog);
		void WriteHeadLog(FILE* fd);
		void FilterStackInfo(const char* szName, CLogStringStream& strm);
		void WriteCommLog(uint32 uErrId, uint32 uErrNum, const char* szErrMsg);
		void FilterErrorMsg(string& strMsg);

	private:
		FILE*			m_fdErrComm;       //��¼����һ����Ϣ
		FILE*			m_fdErrInfo;      //��¼������ϸ��Ϣ

		CErrInfoData*			m_pErrInfoData;
	};
}

