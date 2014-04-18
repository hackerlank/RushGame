#pragma once
#include <iostream>
using namespace std;

typedef int ENUM_ID_TYPE;
typedef CString ENUM_STR_TYPE;
#define ERROR_ID		-111111    //��ʹ����� id ����Ҳ������
#define ERROR_STR		""
#define ENUM_DEF_BEGIN(enumName) \
	std::vector<ENUM_STR_TYPE> enum_vec##enumName##Str;\
	std::vector<ENUM_ID_TYPE> enum_vec##enumName##ID;\
	ENUM_STR_TYPE Get##enumName##Str (ENUM_ID_TYPE n)\
	{\
		int count =  enum_vec##enumName##Str.size();\
		for (int i= 0; i< count; i++)\
		{\
			if (n == enum_vec##enumName##ID[i])\
			{\
				return enum_vec##enumName##Str[i];\
			}\
		}\
		return ERROR_STR;\
	}\
	ENUM_ID_TYPE Get##enumName##ID (ENUM_STR_TYPE s)\
	{\
		int count =  enum_vec##enumName##Str.size();\
		for (int i= 0; i< count; i++)\
		{\
			if (s == enum_vec##enumName##Str[i])\
			{\
				return enum_vec##enumName##ID[i];\
			}\
		}\
		return ERROR_ID;\
	}\
	void Add##enumName##ALL()\
	{

#define ADD_ENUM_ITEM(enumName,str,id) \
		enum_vec##enumName##Str.push_back(str);\
		enum_vec##enumName##ID.push_back(id);

#define ENUM_DEF_END \
	}

#define ENUM_REGISTER(enumName) Add##enumName##ALL()


class CEnumMgr
{
public:
	CEnumMgr(void)
	{
		ENUM_REGISTER(MoveType);
		ENUM_REGISTER(MoveState);
		ENUM_REGISTER(IdleType);
		ENUM_REGISTER(ColonyType);
	}
public:
	ENUM_DEF_BEGIN(MoveType)
		ADD_ENUM_ITEM(MoveType,"�̶�",		0);
		ADD_ENUM_ITEM(MoveType,"ѭ��Ѳ��",	1)
		ADD_ENUM_ITEM(MoveType,"����Ѳ��",	2)
		ADD_ENUM_ITEM(MoveType,"���",		3);
		ADD_ENUM_ITEM(MoveType,"����Ѳ��",  4);
		ADD_ENUM_ITEM(MoveType,"�ȵ��ߺ����",  5);
		ADD_ENUM_ITEM(MoveType,"���·��Ѳ��",  6);
	ENUM_DEF_END
	

	ENUM_DEF_BEGIN(MoveState)
		ADD_ENUM_ITEM(MoveState,"����",	2);
		ADD_ENUM_ITEM(MoveState,"��",	3);
	ENUM_DEF_END
	
	ENUM_DEF_BEGIN(IdleType)
		ADD_ENUM_ITEM(IdleType,"һ�Ŵ�������",	0);
		ADD_ENUM_ITEM(IdleType,"���Ŵ�������",	1);
		ADD_ENUM_ITEM(IdleType,"���Ŵ�������",	2);
		ADD_ENUM_ITEM(IdleType,"��������",	3);
	ENUM_DEF_END
	
	ENUM_DEF_BEGIN(ColonyType)
		ADD_ENUM_ITEM(ColonyType,"�޹���ʿ",	0);
		ADD_ENUM_ITEM(ColonyType,"BOSS",		1);
		ADD_ENUM_ITEM(ColonyType,"С��",		2);
	ENUM_DEF_END

	~CEnumMgr(void){};
	static CEnumMgr& Inst()
	{
		static CEnumMgr instance;
		return instance;
	}
};
