#pragma once
#include "stdafx.h"
#include "TSingleton.h"
#include "common.h"
using namespace std;
class CCount:public TSingleton<CCount>
{
public:
	CCount();
	~CCount(){};
	static FTW_RESULT CountWord_MakeOldFileList( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	static FTW_RESULT CountWord_MakeNewFileList( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* );
	//������txt�ļ��б�
	void MakeOldFileList(string strFileName);
	void MakeNewFileList(string strFileName);
	//ͳ�������ļ��ͼ����ļ�
	void FileNumber();
	//�����ļ���ʼ����
	void FileLoad();
	void start();
	//�����������ļ�
	void WordCountNew(vector<vector<string>> vTemp);
	//����һ����ļ�
	void WordCount(vector<string> vNewTemp,vector<string> vOldTemp);
	//���txt�ļ��ĵ�һ��
	vector<string> GetIndex(vector<string> vec);
	//����ĳһ����������
	void CountWord(vector<string> vecNewIndex,vector<string> vecNew,vector<string> vecOldIndex,vector<string> vecOld);
	//��ȡUTF16txt�ļ�
	void ReadUtf16Txt(string& strFileName,vector<vector<wstring> >& vecRead);
	void ReadUtf16Txt(string& strFileName,vector<vector<string> >& vecRef);
	//ͳ���������
	uint32 SignNumNew(string str);
	//ͳ�Ʊ仯�ı��
	uint32 SignNumCha(string str);
	//�ж��Ƿ��Ƿ����ֵ����ķ��ţ�������λ������ж�
	bool isChineseSign(const string& str);
private:
	//�����԰��ļ��б�
	set<string> m_OldFileList;
	//�����԰��ļ��б�
	set<string> m_NewFileList;
	//�����԰��൱�����԰������ļ��б�
	set<string> m_NewFileListInc;
	//�����԰���������԰����ٵ��ļ���
	uint32 m_FileNumberDes;
	//�����԰���������԰����ӵ��ļ���
	uint32 m_FileNumberInc;
	//�����ֽ���
	uint32 m_WordCountNew;
	//�仯�ֽ���
	uint32 m_WordCountCha;
	//�仯�ķ�����
	uint32 m_SignNumCha;
	//�仯�����ķ�����
	uint32 m_SignChNumCha;
	//�仯��Ansi������
	uint32 m_SignAnNumCha;
	//����������
	uint32 m_SignNumNew;
	//���������ķ�����
	uint32 m_SignChNumNew;
	//������Ansi������
	uint32 m_SignAnNumNew;
	//����Ӣ���������
	uint32 m_EnglishNewNum;
	//�仯��Ӣ���������
	uint32 m_EnglishChNum;
	//ԭ���԰��ļ���
	string m_OldLanFolder;
	//�����԰��ļ���
	string m_NewLanFolder;
	//���ķ��ż���
	set<string> m_sChineseSign;
	//Ansi���ż���
	set<string> m_sAnsiSign;
	//Ӣ����ĸ�����ּ���
	set<string> m_sEnglish;
};