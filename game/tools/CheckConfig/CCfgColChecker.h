#pragma once
#include "CTxtTableFile.h"
#include "TConfigAllocator.h"

namespace sqr
{
	extern const wstring PATH_ALIAS_CFG;
}

typedef map<string, uint32>				MapEnum;

typedef bool (*FIELD_CHECKER) (float, float);
typedef bool (*FIELD_CHECKER_STRING) (const string&);

class CCfgCalc;

enum EErrorReactType
{
	eERT_AtOnce,
	eERT_AtEnd,
	eERT_Never,
};



//��Լ����������ռ�
namespace CfgChk
{
#define CANEMPTY	true
#define MUSTFILL	false
#define YES			true
#define NO			false
#define CHECKCFG

	//�����Ϣ�õ�ȫ�ֶ���
	extern bool g_bIsCheckingCfg;			//��ʾ���ڼ�����ñ�ʱ�ı�־λ
	extern CTxtTableFile* g_pTabFile;			//�����
	extern string g_sTabName;				//����
	extern string g_sSrcTabLineName;
	extern string g_sMagicEffName;
	extern int32 g_iLine;					//�к�
	extern string g_sTitle;					//�б���
	extern bool g_bCheckInReading;			//�Ƿ��ڶ���ʱ���
	extern EErrorReactType g_uErrReact;		//����Ӧ
	extern bool g_bExistError;				//���ڴ���
	extern uint64 g_uTimeForTest;				//������

	bool	ExistError();
	bool	ExpIsForbidden();
	bool	ExpAtOnce();
	bool	ExpAtEnd();
	void	GenExpInfo(const string& sExp);
	void	SetTabFile(CTxtTableFile& TabFile,  const string& TabName);		//�������ñ��ļ�
	void	ResetTab(CTxtTableFile& TabFile, const string& TabName);
	void	SetTabName(const string& TabName);
	void	SetTabNull();
	void	SetLineNo(int32 iLine);
	void	SetItemTitle(const TCHAR* sTitle);

	template<class EnumType>
	void	CreateMap(map<string, EnumType> &mapEnum, uint32 uCount, ...);

	template<class NodeType>
	void	CreateVector(vector<NodeType> &vecNode, uint32 uCount, ...);

	
	bool	GT(const float x, const float a);					//x > a
	bool	GE(const float x, const float a);					//x >= a
	bool	LT(const float x, const float a);					//x < a
	bool	LE(const float x, const float a);					//x <= a
	bool	EQ(const float x, const float a);					//x == a
	bool	NE(const float x, const float a);					//x != a
	bool	IsRate(const float x, const float a);				//x >= 0.0f && x <= 1.0f


	//��ȡ���ñ��һ�е�sTitle�tReader
	void ReadItem(uint32& tReader, const TCHAR* sTitle,
		FIELD_CHECKER pFunFieldChecker, uint32 fFieldCheckerParam);

	void ReadItem(uint32& tReader, const TCHAR* sTitle, bool bCanEmpty = false,
		uint32 tDefault = 0, FIELD_CHECKER pFunFieldChecker = NULL, uint32 fFieldCheckerParam = 0);

	void ReadItem(string& tReader, const TCHAR* sTitle, FIELD_CHECKER_STRING pFunFieldChecker);

	void ReadItem(string& tReader, const TCHAR* sTitle, bool bCanEmpty = false,
		const string& tDefault = "", FIELD_CHECKER_STRING pFunFieldChecker = NULL);

	void ReadItem(CCfgCalc*& tReader, const TCHAR* sTitle, FIELD_CHECKER pFunFieldChecker,
		float fFieldCheckerParam = 0.0f);

	void ReadItem(CCfgCalc*& tReader, const TCHAR* sTitle, bool bCanEmpty = false,
		float tDefault = 0.0f, FIELD_CHECKER pFunFieldChecker = NULL, float fFieldCheckerParam = 0.0f);
	
	void ReadMixedItem(CCfgCalc*& tReader, const TCHAR* sTitle, bool bCanEmpty = false, 
		const string& tDefault = "");

	template<class ReaderType>
	void ReadItem(ReaderType& tReader, const TCHAR* sTitle,
		FIELD_CHECKER pFunFieldChecker, ReaderType fFieldCheckerParam);

	template<class ReaderType>
	void ReadItem(ReaderType& tReader, const TCHAR* sTitle, bool bCanEmpty = false,
		ReaderType tDefault = 0, FIELD_CHECKER pFunFieldChecker = NULL, ReaderType fFieldCheckerParam = 0);

	template<class EnumType>
	void ReadItem(EnumType& tReader, const TCHAR* sTitle, map<string, EnumType, less<string>, 
		TConfigAllocator<pair<string, EnumType > > >& pFieldMap);

	template<class EnumType>
	void ReadItem(EnumType& tReader, const TCHAR* sTitle, bool bCanEmpty, EnumType tDefault, map<string, EnumType, less<string>, 
		TConfigAllocator<pair<string, EnumType > > >& pFieldMap);

	template<class EnumType1, class EnumType2>
	uint32 ReadItem(EnumType1& tReader1, EnumType2& tReader2, const TCHAR* sTitle,
		map<string, EnumType1>& pFieldMap1, map<string, EnumType2>& pFieldMap2);

	template<class EnumType1, class EnumType2>
	uint32 ReadItem(EnumType1& tReader1, EnumType2& tReader2, const TCHAR* sTitle,
		bool bCanEmpty, map<string, EnumType1>& pFieldMap1,
		map<string, EnumType2>& pFieldMap2);


	
	template<class ReaderType>
	void	GenExpInfo(const string& sExp, const ReaderType& tValue);



	//����Ƿ��Ϊ���Լ���Ĭ��ֵ
	template<class DefaultType>
	void CheckEmpty(CCfgCalc*& tReader, const string& sValue, bool bCanEmpty, const DefaultType& tDefault);

	template<class ReaderType>
	void CheckEmpty(ReaderType& tReader, const string& sValue, bool bCanEmpty, const ReaderType& tDefault);


	//��ֵ
	void SetValue(int32& tReader, const string& sValue);
	void SetValue(float& tReader, const string& sValue);
	void SetValue(bool& tReader, const string& sValue);
	void SetValue(string& tReader, const string& sValue);
	void SetValue(CCfgCalc*& pCalc, const string& sValue, bool bIsSingle = true);

	template<class EnumType>
	void SetValue(EnumType& tReader, const string& sValue, map<string, EnumType, less<string>, 
		TConfigAllocator<pair<string, EnumType > > >& pFieldMap);
	
	
	//���ֵ�Ƿ����uint32���ͣ������ڻ����0��
	void CheckUIntType(int32& tReader);


	//���ȡֵ��Χ
	void CheckField(string& sValue, FIELD_CHECKER_STRING pFunFieldChecker);

	template<class ReaderType>
	void CheckField(ReaderType& tReader, FIELD_CHECKER pFunFieldChecker, ReaderType fFieldCheckerParam);

	template<class EnumType>
	bool InField(string& sValue, map<string, EnumType>& pFieldMap);

	template<class EnumType>
	void CheckField(string& sValue, map<string, EnumType, less<string>, 
		TConfigAllocator<pair<string, EnumType > > >& pFieldMap);


	//�������ñ��һ�е����ñ�map
	template<class CfgNodeType>
	bool InsertMap(map<string, CfgNodeType*>& mapCfg, CfgNodeType* pCfgNode);

	template<class CfgNodeType>
	bool InsertMap(map<uint32, CfgNodeType*>& mapCfg, CfgNodeType* pCfgNode);

};
