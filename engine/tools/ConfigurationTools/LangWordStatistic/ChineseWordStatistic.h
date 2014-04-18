#pragma once
#include "TSingleton.h"
#include "CSplitInfo.h"

class CChineseWordStatistic : public TSingleton<CChineseWordStatistic>
{
public:
	CChineseWordStatistic();
	~CChineseWordStatistic();

	static FTW_RESULT ChineseWordStatistic( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	bool BeLangDif( string& strFileName );
	void DoStatistic( string& strFileName );
	void PrintWordStatisticResult();
	void WriteAllWordToTxt();

	//����ӣ�ͳ�Ʒ��������
	void StaticsLanaguage(string strFileName);
	void PrintTStaticResult();

	void StartProcess();
	void FolderCompare(string& strOne,string& strAnother);
	 static FTW_RESULT RecordFilelist( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	void RecordFile(string& strFile);
	void StaticsLanaguage(string strFileName,int& staticOne,int& staticTwo);

private:
	static int32 s_nCount;
	CSplitInfo* m_pSplitInfo;
	// 1û��ȥ���ظ� 2ȥ�������ظ� 3ȥ��ȫ���ظ�
	static ofstream s_ofsWordCount1;
	static ofstream s_ofsWordCount2;
	static ofstream s_ofsWordCount3;
	static hash_map<string, ofstream*> s_hmapOfstream1;
	static hash_map<string, ofstream*> s_hmapOfstream2;
	static hash_map<string, ofstream*> s_hmapOfstream3;
	static hash_set<string> s_hsetWordCount2;
	static hash_set<string> s_hsetWordCount3;
	static map<string, hash_set<string> > s_mapWordCount2;
	static map<string, hash_set<string> > s_mapWordCount3;

	//�����
	int32	m_wordNumWithSign;
	int32   m_wordNumWithoutSign;

	vector<string> m_vecCompareOne;
	vector<string> m_vecCompareAnother;
	vector<string> m_vecTemp;
};
