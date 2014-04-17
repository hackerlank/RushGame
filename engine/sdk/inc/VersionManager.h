#pragma once


#include "IAsyncUpdateHandler.h"
#include "BaseTypes.h"
#include <string>
#include <list>
#include <set>
#include <map>
#include <fstream>
#include <cassert>

namespace sqr
{
	struct ReleaseVersion
	{
		uint16 uReserve;       // δʹ��
		uint16 uSequelVersion; // ����Ƭ�汾��(���ɽ���Patch����)
		uint16 uBranchVersion; // ��֧�汾��
		uint16 uRevision;      // ��֧���޶��汾��
		uint16 uSVersion;	   // С�汾��
	};

	struct UpdatePacket
	{
		ReleaseVersion  ver;
		std::string     url;
	};

	// �������ݲ���û���������ܱ����̵߳���
	class VersionManager
	{
	public:
		typedef  std::list<UpdatePacket>  PatchList_t;

	public:
		VersionManager(IAsyncUpdateHandler* pUpadteHandler);
		~VersionManager();
		bool GetVersionList(const wchar_t* szFile, const char* szTargetVersion);
		uint32 GetLocalSequel();
		uint32 GetLocalBranch();
		uint32 GetLocalRevision();
		uint32 GetLocalSmallVersion();
		uint32 GetOfficialSequel();
		uint32 GetOfficialBranch();
		uint32 GetOfficialRevision();
		uint32 GetOfficialSmallVersion();
		std::string PopUrl();
		PatchList_t& GetPatchList();
		void GetPatchListInfo(const wchar_t* wszFile);
		void GetVersionString(const wchar_t* wszFile, char* szVersion);
		void PatchSmallVersion(const char* szSrc, const char* szTgt);
		void PatchRevision(const char* szSrc, const char* szTgt);
		char* GetBranchPatchVersion(uint32 uSequelVersion, uint32 uBranchVersion);
		void PatchBranchVersion(const char* szSrc, const char* szTgt);
		void PatchReVersion(const char* szSrc, const char* szTgt);
		void GetPatchType(const char* szSrc, const char* szTgt);

	private:
		IAsyncUpdateHandler*  m_UpadteHandler;
		PatchList_t           m_PatchList; // ��Ҫ�ϲ������б����һ��Ϊ�ٷ����°汾
		ReleaseVersion        m_LocalVersion;
		ReleaseVersion		  m_TgtVersion;
		string				  m_strPatchPath;
		map<string, char*> m_mapPatchList; // PatchList��¼�ڶ�λ�汾��������·
		typedef void (VersionManager::* pVersionManagerMemfun)(const char*, const char*);
		pVersionManagerMemfun m_pMemFun;
	};

}

