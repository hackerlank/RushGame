#pragma once
#include "TSingleton.h"
#include <set>
#include <vector>
#include <iterator>
#include "tinyxml/tinyxml.h"
#include <stack>
class CSplitInfo;
class CDif2Txt : public TSingleton<CDif2Txt>
{
public:
	CDif2Txt();
	~CDif2Txt();

	void StartProcess();
	static FTW_RESULT Dif2Txt( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	void Change2Txt(string& strFileName);
	bool BeRequireToTxt(string& strFileName);
	//�����б���������ظ����
	bool CheckMultiTable(string& strFileName);
	//�������б�xml�ļ������ս��������map��
	void MultiTableXmlTree(TiXmlNode * node);

private:
	CSplitInfo* m_pSplitInfo;
	set<string> FIRSTROW;
	string m_strSrc;
	string m_strTgt;
	int m_iRecord;
	//���ڽ������б�xml�ļ�ʱ��ʱ�洢�ڵ���
	vector<string> m_vString;
	//���ڽ������б�xml�ļ�ʱ��ʱ�洢�ڵ���������ֵ���ݸ�m_vString
	stack<string> m_sString;
	//�������б�xml�ļ�ʱ��ʱ��m_sString�����ݽ����������ĳһҶ�ӵ���󳤶ȣ���m_sString���״̬
	stack<string> m_sMax;
	//�洢���б�xml�ļ��Ľ������
	map<vector<string>,int> m_mVectorInt;
	//���ж��Ƿ��ظ�ʱ���ڼ�¼ĳһ�е����ݺ��к�
	map<vector<string>,int> m_mMapVecBuf;
	//��¼���б��γ�Ψһ��������С����
	int m_iMax;


};
