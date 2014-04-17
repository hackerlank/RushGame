#include "stdafx.h"
#include "CErrLog.h"
#include "jenkins.h"
#include "CError.h"
#include "ErrLogHelper.h"
#include <time.h>
#include "CErrLogCompress.h"
#include "CErrLogEncrypt.h"
#include "UtfCvs.h"
#include "CErrLogMgr.h"

namespace sqr{

struct ErrInfo: public CMallocObject
{
public:
	ErrInfo(uint32 uErrId)
		: m_uErrId(uErrId)
		, m_uErrNum(0)
	{}
	uint32	m_uErrId;       //������
	uint32	m_uErrNum;		//����������
};



class CErrInfoData : public CMallocObject
{
public:
	uint32 ErrInfo2ID(const char* str, uint32 len)
	{
		if (str == NULL)
			return 0;
		return sqr::hash::jenkins::hash((uint8*)str, len, len);
	}
	ErrInfo* GetErrInfo(const char* szErrInfo)
	{
		uint32 uLen = strlen(szErrInfo);
		uint32 uErrId = ErrInfo2ID(szErrInfo, uLen);

		MapErrInfo_t::iterator iter = m_mapErrInfo.find(uErrId);

		if(iter == m_mapErrInfo.end())
		{
			ErrInfo* pErrInfo = new ErrInfo(uErrId);
			m_mapErrInfo.insert(make_pair(uErrId, pErrInfo));
			return pErrInfo;
		}

		return iter->second;
	}
	typedef set<uint32, less<uint32>, TMallocAllocator<uint32> >	SetErrInfo_t;
	SetErrInfo_t			m_setErrInfo;
private:
	typedef map<uint32, ErrInfo*, less<uint32>, TMallocAllocator<pair<uint32, ErrInfo*> > >	MapErrInfo_t;
	MapErrInfo_t			m_mapErrInfo;
};
}

CErrLog::CErrLog()
:m_fdErrComm(NULL)
,m_fdErrInfo(NULL)
{
	m_pErrInfoData = new CErrInfoData();
}

CErrLog::~CErrLog()
{
	delete m_pErrInfoData;
	CloseLogFile();
}

void CErrLog::CreateLogFile(const wchar_t* szErrFileName,const char* szDeviceInfo)
{
	wstring strErrFileName = szErrFileName;
#ifdef _WIN32 
	m_fdErrComm = _wfopen((strErrFileName+L".comm").c_str(),L"wb");
	m_fdErrInfo = _wfopen((strErrFileName+L".info").c_str(), L"wb");
#else
	char* strErrComm = U16_to_U8((strErrFileName+L".comm").c_str());
	char* strErrInfo = U16_to_U8((strErrFileName+L".info").c_str());
	m_fdErrComm = fopen(strErrComm, "wb");
	m_fdErrInfo = fopen(strErrInfo, "wb");
	FreeUtfCvsDest(strErrComm);
	FreeUtfCvsDest(strErrInfo);
#endif	
	if (m_fdErrInfo&&m_fdErrComm)
	{
		WriteHeadLog(m_fdErrInfo);
		WriteHeadLog(m_fdErrComm);
		HandleAndWriteLog(m_fdErrComm, szDeviceInfo);
	}
	else
	{
		CErrLogMgr::Inst().SetErrLogEnabled(false);
	}
}

void CErrLog::CloseLogFile()
{
	if (m_fdErrComm)
	{
		fclose(m_fdErrComm);
		m_fdErrComm = NULL;
	}

	if (m_fdErrInfo)
	{
		fclose(m_fdErrInfo);
		m_fdErrInfo = NULL;
	}
}

void CErrLog::WriteErrLog(const CError& exp,const char* szErrlogInfo)
{
	if (!m_fdErrInfo||!m_fdErrComm)
	{
		return;
	}

	const char* szErrorType = exp.ErrorTitle();
	const char* szStackInfo = exp.StackInfo();

	CLogStringStream strKey;
	strKey << szErrorType << szStackInfo;

	//����ͨ��stack�������ж��Ƿ���ͬһ�����
	ErrInfo* pErrInfo = m_pErrInfoData->GetErrInfo(strKey.str().c_str());

	uint32 uErrId = pErrInfo->m_uErrId;
	uint32 uErrNum = (++pErrInfo->m_uErrNum);

	//���Ƿ��ǵ�һ�β��룬����ǣ����¼info��Ϣ
	bool bSuc = m_pErrInfoData->m_setErrInfo.insert(uErrId).second;

	//����ǵ�һ�μ�¼����Ѵ�����ϸ��ջ��Ϣ��¼��info�ļ�����
	if (bSuc)
	{
		CLogStringStream str;
		str << "���: " << uErrId << endl;
		str << "��Ϣ: " << szErrorType << endl;

		if(szStackInfo[0] != '\0')
		{
			str << "��ջ: " << endl;
			FilterStackInfo(szStackInfo, str);
			str << endl;					
		}

		const char* szExpandInfo = exp.ExpandInfo();
		if(szExpandInfo[0] != '\0')
		{
			str << "������Ϣ: " << endl;
			str << szExpandInfo << endl;
		}

		const char* szCategory = exp.Category();
		if(szCategory[0] != '\0')
		{
			str << "����: " << szCategory << endl;
		}

		str << endl;

		HandleAndWriteLog(m_fdErrInfo, str.str().c_str());
	}

	CLogStringStream sCommMsg;
	sCommMsg << exp.ErrorMsg();

	const char* sMemInfo = exp.MemInfo();
	if (sMemInfo[0] != '\0')
	{
		sCommMsg << "\tMemInfo:" << sMemInfo;
	}

	HandleAndWriteLog(m_fdErrComm, szErrlogInfo);
	WriteCommLog(uErrId, uErrNum, sCommMsg.str().c_str());
}

//���˶�ջ��Ϣ��ֻ�����ջ��ַ
void CErrLog::FilterStackInfo(const char* szName, CLogStringStream& strm)
{
	if(!ErrLogEncrypted())
	{
		strm << szName;
		return;
	}

	//��windows����£�����ֻ��¼��ջ��ַ������¼��ϸ��Ϣ��
	//��Ϊ�Ժ󲻻�����ṩmapfile���������ڼ�ʹ��mapfile������Ҳ����û��mapfile����
#ifdef _WIN32

	string strFilter;
	char* szStr = (char*)malloc(strlen(szName) + 1);
	strcpy(szStr, szName);

	char seps[] = "\n";
	char* token = strtok(szStr, seps);
	while(token != NULL)
	{
		//�����һ����Addr:,������һ��c++�Ķ�ջ�����Ǿ�Ҫ������
		if(strstr(token, "Addr:"))
		{
			string strToken(token);
			uint32 pos = strToken.find('\t');
			string subToken = strToken.substr(0, pos);
			strFilter.append(subToken).append("\n");
		}
		else
		{
			strFilter.append(token).append("\n");		
		}
		token = strtok(NULL, seps);
	}
	free(szStr);
	strm << strFilter.c_str();

#else
	strm << szName;
#endif
}

void CErrLog::WriteHeadLog(FILE* fd)
{
	if(!ErrLogEncrypted())
	{
		return;
	}

	static char buf[] = "ybtx_errlog";
	static uint32 uLen = strlen(buf);
	char* szHead = (char*)malloc(uLen);
	for(uint32 i = 0; i < uLen; i++)
	{
		szHead[i] = buf[i] ^ 3;
	}

	fwrite(szHead, sizeof(char), uLen, fd);  
	free(szHead);

	uint32 uVersion = GetCurrentRevision();
	uVersion ^= 0xeeee;
	fwrite(&uVersion, sizeof(uint32), 1, fd); //д��汾��
	fflush(fd);
}

void CErrLog::WriteCommLog(uint32 uErrId, uint32 uErrNum, const char* szMsg)
{
	//��ÿ��дcomm֮ǰ�����ǻ��һЩerr msg��¼��comm����

	//�Ѵ������������Լ�ʱ���¼��log�ļ�����
	time_t T;
	struct tm * timenow;

	time ( &T );
	timenow = localtime ( &T ); 

	ostringstream str;
	str << uErrId;
	str << " \t" << timenow->tm_year + 1900 << "-" << 
		timenow->tm_mon + 1<< "-" <<
		timenow->tm_mday << "-" <<
		timenow->tm_hour << ":" <<
		timenow->tm_min << ":" <<
		timenow->tm_sec;
	str << "\t" << uErrNum << "\t";		

	if(szMsg)
	{
		string strMsg(szMsg);
		FilterErrorMsg(strMsg);
		str << strMsg.c_str() << endl;
	}

	HandleAndWriteLog(m_fdErrComm, str.str().c_str());
}

void CErrLog::HandleAndWriteLog(FILE* fd, const char* szErrLog)
{
	if(!ErrLogEncrypted())
	{
		fprintf(fd, "%s", szErrLog);
		fflush(fd);
		return;
	}

	//first compress the buffer
	uint32 uSourceLen = strlen(szErrLog) + 1;

	//�õ����˵�dest��С
	uint32 uDestLen = (uint32)(uSourceLen * 1.01 + 640);
	char* szDest = (char*)malloc(uDestLen);

	CErrLogCompress::Inst().Compress(szDest, &uDestLen, szErrLog, uSourceLen);

	//then encode the buffer
	CErrLogEncrypt::Inst().Encode(szDest, uDestLen);

	//write the log

	//first write the source len and the compress len
	uint16 uSrc = (uint16)uSourceLen;    //�������ǵ�buffer�ĳ��Ȳ����ܳ���2^16��������uint16�洢
	uint16 uDest = (uint16)uDestLen;

	fwrite(&uSrc, sizeof(uint16), 1, fd); 
	fwrite(&uDest, sizeof(uint16), 1, fd);

	fwrite(szDest, sizeof(char), uDestLen, fd);
	fflush(fd);

	free(szDest);
}

void CErrLog::FilterErrorMsg(string& strMsg)
{
	size_t stSize = strMsg.size();
	for(size_t i = 0; i < stSize; i++)
	{
		if(strMsg[i] == '\n')
			strMsg[i] = ' ';
	}
}
