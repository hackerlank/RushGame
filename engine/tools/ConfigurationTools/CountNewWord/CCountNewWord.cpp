#include "stdafx.h"
#include "CCountNewWord.h"
#include "CodeCvs.h"
#include "common.h"
#include "tinyxml/tinyxml.h"
using namespace std;
ofstream f("CountNewWord.txt");
string ChineseSign[100] = {"��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","\0"};
string AnsiSign[100] = {",",".","/","<",">","?",";","\"","\\","[","]","{","}","!","@","~","#","$","%","^","&","*","(",")","-","=","\0"};
string English[100] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
						"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
						"1","2","3","4","5","6","7","8","9","0","\0"};
CCount::CCount():m_FileNumberDes(0),
			 m_FileNumberInc(0),
			 m_WordCountNew(0),
			 m_WordCountCha(0),
			 m_SignNumNew(0),
			 m_SignNumCha(0),
			 m_SignChNumCha(0),
			 m_SignChNumNew(0),
			 m_SignAnNumCha(0),
			 m_SignAnNumNew(0),
			 m_EnglishNewNum(0),
			 m_EnglishChNum(0)
{
	TiXmlDocument* pDoc = new TiXmlDocument();
	if(!pDoc->LoadFile("��������Ƚ�.xml"))
	{
		MessageBox(NULL,NULL,"�����ļ�����������Ƚ�.xml��ʧ�ܡ�",0);
	}
	m_OldLanFolder = pDoc->RootElement()->FirstChild("OldLanFolder")->ToElement()->GetText();
	m_NewLanFolder = pDoc->RootElement()->FirstChild("NewLanFolder")->ToElement()->GetText();
	delete pDoc;
	pDoc = NULL;


	int an=0;
	while(*(AnsiSign+an)!="\0")
	{
		m_sAnsiSign.insert(*(AnsiSign+an));
		an++;
	}
	int ch=0;
	while(*(ChineseSign+ch)!="\0")
	{
		m_sChineseSign.insert(*(ChineseSign+ch));
		ch++;
	}
	int en=0;
	while(*(English+en)!="\0")
	{
		m_sEnglish.insert(*(English+en));
		en++;
	}
}
void CCount::start()
{
	FileTreeWalk(gbk_to_utf16(m_OldLanFolder).c_str(),CountWord_MakeOldFileList,NULL);
	FileTreeWalk(gbk_to_utf16(m_NewLanFolder).c_str(),CountWord_MakeNewFileList,NULL);
	FileNumber();
	FileLoad();
	printf("ͳ�ƽ��������CountNewWord.txt��");
	
	cout<<endl<<"�仯�ֽ���"<<m_WordCountCha<<endl;
	cout<<"�����ֽ���"<<m_WordCountNew<<endl;
	cout<<"���ֽ���"<<m_WordCountCha+m_WordCountNew<<endl;
	cout<<"Ansi�����ֽ���"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
	cout<<"Ansi������"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
	cout<<"���ķ����ֽ���"<<(m_SignChNumNew+m_SignChNumCha)*2<<endl;
	cout<<"���ķ�����"<<m_SignChNumNew+m_SignChNumCha<<endl;
	cout<<"Ӣ����ĸ+�����޸��ֽ���"<<m_EnglishChNum<<endl;
	cout<<"Ӣ����ĸ+�����޸���"<<m_EnglishChNum<<endl;
	cout<<"Ӣ����ĸ+���������ֽ���"<<m_EnglishNewNum<<endl;
	cout<<"Ӣ����ĸ+����������"<<m_EnglishNewNum<<endl;
	cout<<"�����ֽ���"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)-(m_SignChNumNew+m_SignChNumCha)*2-(m_EnglishNewNum+m_EnglishChNum)<<endl;
	cout<<"��������"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)/2-(m_SignChNumNew+m_SignChNumCha)-(m_EnglishChNum+m_EnglishNewNum)/2<<endl;
	cout<<"�����޸��ֽ���"<<(m_WordCountCha-m_SignAnNumCha)-(m_SignChNumCha)*2-(m_EnglishChNum)<<endl;
	cout<<"�����޸�����"<<(m_WordCountCha-m_SignAnNumCha)/2-(m_SignChNumCha)-(m_EnglishChNum)/2<<endl;
	cout<<"���������ֽ���"<<(m_WordCountNew-m_SignAnNumNew)-(m_SignChNumNew)*2-(m_EnglishNewNum)<<endl;
	cout<<"������������"<<(m_WordCountNew-m_SignAnNumNew)/2-(m_SignChNumNew)-(m_EnglishNewNum)/2<<endl;

	f<<endl<<"-------------����Ϊ������ͳ�ƽ��-------------------"<<endl;
	f<<"�仯�ֽ���"<<m_WordCountCha<<endl;
	f<<"�����ֽ���"<<m_WordCountNew<<endl;
	f<<"���ֽ���"<<m_WordCountCha+m_WordCountNew<<endl;
	f<<"Ansi�����ֽ���"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
	f<<"Ansi������"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
	f<<"���ķ����ֽ���"<<(m_SignChNumNew+m_SignChNumCha)*2<<endl;
	f<<"���ķ�����"<<m_SignChNumNew+m_SignChNumCha<<endl;
	f<<"Ӣ����ĸ+�����޸��ֽ���"<<m_EnglishChNum<<endl;
	f<<"Ӣ����ĸ+�����޸���"<<m_EnglishChNum<<endl;
	f<<"Ӣ����ĸ+���������ֽ���"<<m_EnglishNewNum<<endl;
	f<<"Ӣ����ĸ+����������"<<m_EnglishNewNum<<endl;
	f<<"�����ֽ���"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)-(m_SignChNumNew+m_SignChNumCha)*2-(m_EnglishNewNum+m_EnglishChNum)<<endl;
	f<<"��������"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)/2-(m_SignChNumNew+m_SignChNumCha)-(m_EnglishChNum+m_EnglishNewNum)/2<<endl;
	f<<"�����޸��ֽ���"<<(m_WordCountCha-m_SignAnNumCha)-(m_SignChNumCha)*2-(m_EnglishChNum)<<endl;
	f<<"�����޸�����"<<(m_WordCountCha-m_SignAnNumCha)/2-(m_SignChNumCha)-(m_EnglishChNum)/2<<endl;
	f<<"���������ֽ���"<<(m_WordCountNew-m_SignAnNumNew)-(m_SignChNumNew)*2-(m_EnglishNewNum)<<endl;
	f<<"������������"<<(m_WordCountNew-m_SignAnNumNew)/2-(m_SignChNumNew)-(m_EnglishNewNum)/2<<endl;
	f.close();


}
FTW_RESULT CCount::CountWord_MakeOldFileList( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* )
{
	size_t nLen = wcslen( szFileName );
	if( !_wcsicmp( szFileName + nLen - 4, L".svn" ) && eFlag == _FTW_DIR )
		return _FTW_IGNORE;
	if( eFlag != _FTW_FILE || nLen <= 4 )
		return _FTW_CONTINUNE;
	if((_wcsicmp( szFileName + nLen - 4,L".txt")))
		return _FTW_CONTINUNE;
	CCount* pManager = CCount::GetInst();
	string strFileName = utf16_to_gbk(szFileName);
    pManager->MakeOldFileList(strFileName);
	return _FTW_CONTINUNE;
}
void CCount::MakeOldFileList(string strFileName)
{
	m_OldFileList.insert(strFileName);
}
FTW_RESULT CCount::CountWord_MakeNewFileList( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* )
{
	size_t nLen = wcslen( szFileName );
	if( !_wcsicmp( szFileName + nLen - 4, L".svn" ) && eFlag == _FTW_DIR )
		return _FTW_IGNORE;
	if( eFlag != _FTW_FILE || nLen <= 4 )
		return _FTW_CONTINUNE;
	if((_wcsicmp( szFileName + nLen - 4,L".txt")))
		return _FTW_CONTINUNE;
	CCount* pManager = CCount::GetInst();
	string strFileName = utf16_to_gbk(szFileName);
    pManager->MakeNewFileList(strFileName);
	return _FTW_CONTINUNE;
}
void CCount::MakeNewFileList(string strFileName)
{
	m_NewFileList.insert(strFileName);
}
void CCount::FileNumber()
{
	
	for(set<string>::iterator iter=m_OldFileList.begin();iter!=m_OldFileList.end();iter++)
	{
		string str = *iter;
		replace_all_distinct(str,m_OldLanFolder,m_NewLanFolder,0);
		if(m_NewFileList.find(str)==m_NewFileList.end())
			m_FileNumberDes++;
	}
	for(set<string>::iterator iter=m_NewFileList.begin();iter!=m_NewFileList.end();iter++)
	{
		string str=*iter;
		replace_all_distinct(str,m_NewLanFolder,m_OldLanFolder,0);
		if(m_OldFileList.find(str)==m_OldFileList.end())
		{
			m_FileNumberInc++;
			m_NewFileListInc.insert(*iter);
		}
	}
	
}
void CCount::FileLoad()
{

	for(set<string>::iterator iter=m_NewFileListInc.begin();iter!=m_NewFileListInc.end();iter++)
	{
		vector<string> Temp;
		////////////////////////////////////////
		vector<vector<string>> vec;
		ReadUtf16Txt(*iter,vec);
		WordCountNew(vec);
		cout<<"��������"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)/2-(m_SignChNumNew+m_SignChNumCha)-(m_EnglishChNum+m_EnglishNewNum)/2<<endl;
		printf("�����ֽ���%d\n",m_WordCountNew);
		printf("�޸��ֽ���%d\n�ļ�%s\n",m_WordCountCha,(*iter).c_str());

		f<<"�ļ���"<<(*iter).c_str()<<endl;
		f<<"---------���½����Ϊ�ۼ�ֵ---------"<<endl;
		f<<"�仯�ֽ���"<<m_WordCountCha<<endl;
		f<<"�����ֽ���"<<m_WordCountNew<<endl;
		f<<"���ֽ���"<<m_WordCountCha+m_WordCountNew<<endl;
		f<<"Ansi�����ֽ���"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
		f<<"Ansi������"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
		f<<"���ķ����ֽ���"<<(m_SignChNumNew+m_SignChNumCha)*2<<endl;
		f<<"���ķ�����"<<m_SignChNumNew+m_SignChNumCha<<endl;
		f<<"Ӣ����ĸ+�����޸��ֽ���"<<m_EnglishChNum<<endl;
		f<<"Ӣ����ĸ+�����޸���"<<m_EnglishChNum<<endl;
		f<<"Ӣ����ĸ+���������ֽ���"<<m_EnglishNewNum<<endl;
		f<<"Ӣ����ĸ+����������"<<m_EnglishNewNum<<endl;
		
		f<<"�����ֽ���"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)-(m_SignChNumNew+m_SignChNumCha)*2-(m_EnglishNewNum+m_EnglishChNum)<<endl;
		f<<"��������"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)/2-(m_SignChNumNew+m_SignChNumCha)-(m_EnglishChNum+m_EnglishNewNum)/2<<endl;
		f<<"�����޸��ֽ���"<<(m_WordCountCha-m_SignAnNumCha)-(m_SignChNumCha)*2-(m_EnglishChNum)<<endl;
		f<<"�����޸�����"<<(m_WordCountCha-m_SignAnNumCha)/2-(m_SignChNumCha)-(m_EnglishChNum)/2<<endl;
		f<<"���������ֽ���"<<(m_WordCountNew-m_SignAnNumNew)-(m_SignChNumNew)*2-(m_EnglishNewNum)<<endl;
		f<<"������������"<<(m_WordCountNew-m_SignAnNumNew)/2-(m_SignChNumNew)-(m_EnglishNewNum)/2<<endl<<endl;
		
	}
	for(set<string>::iterator iter=m_NewFileList.begin();iter!=m_NewFileList.end();iter++)
	{
		string str = *iter;
		if(m_NewFileListInc.find(str)!=m_NewFileListInc.end())
			continue;
		vector<string> OldTemp;
		vector<string> NewTemp;
/////////////////////////////////////////////
		vector<vector<string>> vec;
		ReadUtf16Txt(str,vec);
		for(uint32 i=0;i<vec.size();i++)
		{
			string str1="";
			for(uint32 j=0;j<vec[i].size();j++)
			{
				if(j==0)
				{
					str1+=vec[i][j];
				}
				else
				{
					str1+="\t"+vec[i][j];
				}
			}
			NewTemp.push_back(str1);
		}
		replace_all_distinct(str,m_NewLanFolder,m_OldLanFolder,0);
		//////////////////////////////////////////
		vector<vector<string>> vec1;
		ReadUtf16Txt(str,vec1);
		for(uint32 i=0;i<vec1.size();i++)
		{
			string str1="";
			for(uint32 j=0;j<vec1[i].size();j++)
			{
				if(j==0)
				{
					str1+=vec1[i][j];
				}
				else
				{
					str1+="\t"+vec1[i][j];
				}
			}
			OldTemp.push_back(str1);
		}
		//���һ��Ϊ���У�ɾ��
		NewTemp.erase(NewTemp.end()-1);
		OldTemp.erase(OldTemp.end()-1);
		WordCount(NewTemp,OldTemp);
		printf("�����ֽ���%d\n",m_WordCountNew);
		printf("�޸��ֽ���%d\n�ļ�%s\n",m_WordCountCha,(*iter).c_str());
		
		f<<"�ļ���"<<(*iter).c_str()<<endl;
		f<<"---------���½����Ϊ�ۼ�ֵ---------"<<endl;
		f<<"�仯�ֽ���"<<m_WordCountCha<<endl;
		f<<"�����ֽ���"<<m_WordCountNew<<endl;
		f<<"���ֽ���"<<m_WordCountCha+m_WordCountNew<<endl;
		f<<"Ansi�����ֽ���"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
		f<<"Ansi������"<<m_SignAnNumNew+m_SignAnNumCha<<endl;
		f<<"���ķ����ֽ���"<<(m_SignChNumNew+m_SignChNumCha)*2<<endl;
		f<<"���ķ�����"<<m_SignChNumNew+m_SignChNumCha<<endl;
		f<<"Ӣ����ĸ+�����޸��ֽ���"<<m_EnglishChNum<<endl;
		f<<"Ӣ����ĸ+�����޸���"<<m_EnglishChNum<<endl;
		f<<"Ӣ����ĸ+���������ֽ���"<<m_EnglishNewNum<<endl;
		f<<"Ӣ����ĸ+����������"<<m_EnglishNewNum<<endl;
		
		f<<"�����ֽ���"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)-(m_SignChNumNew+m_SignChNumCha)*2-(m_EnglishNewNum+m_EnglishChNum)<<endl;
		f<<"��������"<<(m_WordCountNew+m_WordCountCha-m_SignAnNumNew-m_SignAnNumCha)/2-(m_SignChNumNew+m_SignChNumCha)-(m_EnglishChNum+m_EnglishNewNum)/2<<endl;
		f<<"�����޸��ֽ���"<<(m_WordCountCha-m_SignAnNumCha)-(m_SignChNumCha)*2-(m_EnglishChNum)<<endl;
		f<<"�����޸�����"<<(m_WordCountCha-m_SignAnNumCha)/2-(m_SignChNumCha)-(m_EnglishChNum)/2<<endl;
		f<<"���������ֽ���"<<(m_WordCountNew-m_SignAnNumNew)-(m_SignChNumNew)*2-(m_EnglishNewNum)<<endl;
		f<<"������������"<<(m_WordCountNew-m_SignAnNumNew)/2-(m_SignChNumNew)-(m_EnglishNewNum)/2<<endl<<endl;
	}
}
void CCount::WordCountNew(vector<vector<string>> vTemp)
{
	//�ӵڶ��п�ʼͳ��
	for(uint32 i=1;i<vTemp.size();i++)
	{
		//�ӵڶ��п�ʼͳ��
		for(uint32 j=1;j<vTemp[i].size();j++)
		{
			m_WordCountNew+=vTemp[i][j].size();
			SignNumNew(vTemp[i][j]);
		}
	}
}
void CCount::WordCount(vector<string> vNewTemp,vector<string> vOldTemp)
{
	map<string,vector<string>> mapNewTemp;
	map<string,vector<string>> mapOldTemp;
	vector<string> NewIndex = GetIndex(vNewTemp);
	vector<string> OldIndex = GetIndex(vOldTemp);
	//�ӵڶ��п�ʼͳ��
	for(uint32 i=1;i<vNewTemp.size();i++)
	{
		basic_string <char>::size_type pos = vNewTemp[i].find_first_of("\t",0);
		static const basic_string <char>::size_type npos = -1;
		string index = vNewTemp[i];
		index = index.substr(0,pos);
		vector<string> content;
		while(true)
		{
			size_t endpos = vNewTemp[i].find_first_of("\t",pos+1); 
			if(endpos==npos)
			{
				string str = vNewTemp[i];
				str = str.substr(pos+1,str.size()-pos-1);
				content.push_back(str);
				break;
			}
			string str = vNewTemp[i];
			str = str.substr(pos+1,endpos-pos-1);
			content.push_back(str);
			pos = endpos;
		}
		mapNewTemp.insert(make_pair(index,content));
	}
	//�ӵڶ��п�ʼͳ��
	for(uint32 i=1;i<vOldTemp.size();i++)
	{
		basic_string <char>::size_type pos = vOldTemp[i].find_first_of("\t",0);
		static const basic_string <char>::size_type npos = -1;
		string index = vOldTemp[i];
		index = index.substr(0,pos);
		vector<string> content;
		while(true)
		{
			size_t endpos = vOldTemp[i].find_first_of("\t",pos+1); 
			if(endpos==npos)
			{
				string str = vOldTemp[i];
				str = str.substr(pos+1,str.size()-pos-1);
				content.push_back(str);
				break;
			}
			string str = vOldTemp[i];
			str = str.substr(pos+1,endpos-pos-1);
			content.push_back(str);
			pos = endpos;
		}
		mapOldTemp.insert(make_pair(index,content));
	}
	map<string,vector<string>>::iterator iter=mapNewTemp.begin();
	for(;iter!=mapNewTemp.end();iter++)
	{
		bool Mark = false;
		map<string,vector<string>>::iterator iter1=mapOldTemp.begin();
		for(;iter1!=mapOldTemp.end();iter1++)
		{
			if(iter->first==iter1->first)
			{
				//��ĳһ���������н��д���
				CountWord(NewIndex,iter->second,OldIndex,iter1->second);
				Mark = true;
				break;
			}
		}
		if(Mark==false)
		{
			//���������н�������ͳ��
			vector<string> vec = iter->second;
			vector<string>::iterator iterTemp = vec.begin();
			for(;iterTemp!=vec.end();iterTemp++)
			{
				m_WordCountNew+=(*iterTemp).size();
				SignNumNew(*iterTemp);
			}
		}
	}

}
//��vec�л�õ�һ�У�Ҳ��������
vector<string> CCount::GetIndex(vector<string> vec)
{
	vector<string> Index;
	basic_string <char>::size_type pos = 0;
	static const basic_string <char>::size_type npos = -1;
	while(true)
	{
		size_t endpos = vec[0].find_first_of("\t",pos+1); 
		if(endpos==npos)
		{
			string str = vec[0];
			str = str.substr(pos,str.size()-pos);
			Index.push_back(str);
			break;
		}
		string str = vec[0];
		str = str.substr(pos,endpos-pos);
		Index.push_back(str);
		pos = endpos;
	}
	return Index;
}
void CCount::CountWord(vector<string> vecNewIndex,vector<string> vecNew,vector<string> vecOldIndex,vector<string> vecOld)
{
	for(uint32 i=0;i<vecNew.size();i++)
	{
		string Index = vecNewIndex[i+1];
		bool mark = false;
		//���ϵ�vecindex�в���Ҫ������һ���򰴸ı���������㣬��û���ϵ��������ҵ��������ļ���
		for(uint32 j=1;j<vecOldIndex.size();j++)
		{
			if(Index==vecOldIndex[j])
			{
				if(vecNew[i]!=vecOld[j-1])
				{
					m_WordCountCha+=vecNew[i].size();
					SignNumCha(vecNew[i]);
					mark = true;
					break;
				}
				mark = true;
				break;
			}
		}
		//�ж��Ƿ���ڣ�trueΪ���ڡ�
		if(mark==false)
		{
			m_WordCountNew+=vecNew[i].size();
			SignNumNew(vecNew[i]);
		}
	}
}

void CCount::ReadUtf16Txt(string& strFileName,vector<vector<wstring> >& vecRead)
{
	ifstream inFile(strFileName.c_str(),ios_base::binary | ios_base::in);
	wchar_t wcharTemp;
	wstring wstr;
	vecRead.push_back(vector<wstring>());
	inFile.seekg(2,ios::beg);
	bool bInQuot = false;
	while (inFile.peek() != EOF)
	{
		inFile.read((char*)(&wcharTemp),2);
		if (wcharTemp == 0x0009 && !bInQuot)
		{
			if (vecRead[0].empty() && wstr.empty())
			{
				string strMsg = "���ñ���Ϊ:" + strFileName;
				GenErr("���ñ��ʽ����,��һ����Ϊ��",strMsg.c_str());
			}
			vecRead.back().push_back(wstr);
			wstr = L"";
		}
		else if (wcharTemp == 0x000D)
		{
			vecRead.back().push_back(wstr);
			wstr = L"";
			inFile.seekg(2,ios::cur);
			vecRead.push_back(vector<wstring>());
			bInQuot = false;
		}
		else if (wcharTemp == 0x0022)
		{
			wstr.append(1,wcharTemp);
			bInQuot = !bInQuot;
		}
		else
		{
			wstr.append(1,wcharTemp);
		}
	}
	inFile.close();
}

void CCount::ReadUtf16Txt(string& strFileName,vector<vector<string> >& vecRef)
{
	vector<vector<wstring> > vecWData;
	ReadUtf16Txt(strFileName,vecWData);
	for (int i=0;i<vecWData.size();++i)
	{
		vecRef.push_back(vector<string>());
		for (int j=0;j<vecWData[i].size();++j)
		{
			string strContent  = utf16_to_gbk(vecWData[i][j]);
			vecRef[i].push_back(strContent);
		}
	}
}

uint32 CCount::SignNumNew(string str)
{
	uint32 len = str.size();
	int pos = 0;
	int npos = -1;
	for(int i = 0;i<len;i++)
	{
		if (str[i] > 128 || str[i] < 0)
		{
			string strThis = str.substr(i,2);
			if(isChineseSign(strThis))
			{
				m_SignChNumNew++;
				m_SignNumNew++;
			}
			i++;
		}
		else
		{
			string s;
			s+= str[i];
			if (m_sAnsiSign.find(s)!=m_sAnsiSign.end())
			{
				m_SignAnNumNew++;
				m_SignNumNew++;
			}
			if (m_sEnglish.find(s)!=m_sEnglish.end())
			{
				m_EnglishNewNum++;
			}
		}
	}
	return m_SignNumNew;
}
uint32 CCount::SignNumCha(string str)
{
	uint32 len = str.size();
	int pos = 0;
	int npos = -1;
	for(int i = 0;i<len;i++)
	{
		if (str[i] > 128 || str[i] < 0)
		{
			//�������ַ�
			string strThis = str.substr(i,2);
			if(isChineseSign(strThis))
			{
				m_SignChNumCha++;
				m_SignNumCha++;
			}
			i++;
		}
		else
		{
			//��Ansi�ַ�
			string s;
			s+= str[i];
			if (m_sAnsiSign.find(s)!=m_sAnsiSign.end())
			{
				m_SignAnNumCha++;
				m_SignNumCha++;
			}
			if (m_sEnglish.find(s)!=m_sEnglish.end())
			{
				m_EnglishChNum++;
			}
		}
	}
	return m_SignNumCha;
}
//�ж��Ƿ��Ƿ����ֵ����ķ��ţ�������λ������ж�
bool CCount::isChineseSign(const string& str)
{
	//�����ַ������������ֽ�
	Ast(str.size()==2);
	unsigned char first = str[0];
	unsigned char second = str[1];
	if(first==0xa1)
		return true;
	else if((first==0xa3)&&(second<=0xb0))
		return true;
	else if((first==0xa3)&&(second>0xb9)&&(second<0xc1))
		return true;
	else if((first==0xa3)&&(second>0xda)&&(second<0xe1))
		return true;
	else if((first==0xa3)&&(second>0xfa))
		return true;
	else if((first==0xa8)||(first==0xa9))
		return true;
	return false;
}