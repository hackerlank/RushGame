#pragma once 

namespace sqr
{
	enum ETableFileType
	{
		eTFT_Txt = 0,
		eTFT_Row,
		eTFT_None,
		eTFT_TypeError,
	};

	enum ETableFileFieldType
	{
		eTFFT_String = 0,
		eTFFT_Number,
		eTFFT_Empty,
	};

	enum ERowTableNodeType
	{
		eRTNT_Tree = 0,
		eRTNT_LeafNoData,	//leaf�ڵ㣬û���κ�����
		eRTNT_LeafOneData,  //leaf�ڵ㣬ֻ��һ������
		eRTNT_LeafMoreData,	//leaf�ڵ㣬�ж������
	};

	//��ȡtablefile�����ͣ�Ϊtxt����row
	ETableFileType GetTableFileType(const char* alias, const char* szFileName);

	void SetOriLangPath(const string& strOriLangPath);
	const string& GetOriLangPath();
	void SetCurLangPath(const string& strOriLangPath);
	const string& GetCurLangPath();
}
