#pragma once
#include "TSingleton.h"

#ifndef _WIN32
#include <dirent.h>
#define  MAX_PATH  PATH_MAX
#endif

namespace sqr
{
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4275)	//dll linkage
#pragma warning(disable:4251)	//dll linkage
#endif

	class CError;

	class CErrInfoData;

	class COpearationLog : public Singleton<COpearationLog>
	{
	public:
		COpearationLog();
		~COpearationLog();
		void Realese();

		void WriteOperationLog(const char* szOperationLog);
		void WriteOperationLog(const int nGridIndx, const char* szOperationLog);

		void SetLogFileName(const char* szMapName, const char* szFilePath);

		//�õ�err log��Ŀ¼
		const char* GetLogFilePath() const;

		void Save(const string& path);
	private:
		void WriteInfoLog(const char* szOperationInfo);

	private:
		FILE*			m_fdOperationInfo;      //��¼������ϸ��Ϣ
		char			m_szOperationFilePath[MAX_PATH];
		char			m_szLogPath[MAX_PATH];
		char			m_szMapName[MAX_PATH];
		bool			m_bStart;
	};


#ifdef _WIN32
#pragma warning(pop)
#endif
}