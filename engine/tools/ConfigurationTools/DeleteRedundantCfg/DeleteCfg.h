#pragma once
#include "stdafx.h"
#include "TSingleton.h"
#include <iterator>
#include "common.h"
class CSplitInfo;
class CDeleteCfg : public TSingleton<CDeleteCfg>
{
public:
	CDeleteCfg();
	~CDeleteCfg();

	void StartProcess();
	//����src������Ҫ��ֵı��¼����
	static FTW_RESULT MakeFilePath( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	//����temp��src���бȶ�
	static FTW_RESULT CheckTemp( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	//����src�����б��¼����
	static FTW_RESULT MakeAllFilePath( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	//����tgt��src���бȶ�
	static FTW_RESULT CheckTgt( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	//����lang��src�ȶ�
	static FTW_RESULT CheckLanguage( const wchar_t* szFileName, const struct _stat *,FTW_FLAG eFlag, void* );
	//��srcĿ¼��Ҫ��ֵı����map
	void Add2Map(string& strFileName);
	//��srcĿ¼�����б����map
	void AddAll2Map(string& strFileName);
	//���temp�ļ���
	void CheckTempFile(string& strFileName);
	//���tgt�ļ���
	void CheckTgtFile(string& strFileName);
	//���lang�ļ���
	void CheckLanguageFile(string& strFileName);
	//ǿ��ɾ���ļ�
	bool ForceDeleteFile(const string& strFile);
	//ɾ���������ñ�
	void DeleteCfg(vector<string> vec);

private:
	CSplitInfo* m_pSplitInfo;
	string strSrc;
	string strTgt;
	string strTemp;
	string strLangFile;
    map<string,string> m_mFilePath;
	map<string,string> m_mAllFilePath;
	vector<string> m_vFile;
	vector<string> m_vAllFile;
	vector<string> m_vLangFile;

};
