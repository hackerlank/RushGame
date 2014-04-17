#pragma once

#include "DataPack.h"
#include "CScriptSerial.h"
#include "CScriptAllocator.h"

namespace sqr
{

	class CClass;
	class CCompactAllocPool;

	class CVariant;

	//----------------------------------------------------
	// struct CScriptData
	//---------------------------------------------------
	class CScriptData : public CScriptMallocObject
	{
	public:
		CScriptData();
		~CScriptData();

		void PrepareFunctions();

		void InitHelper(EScriptHelper eScriptHelper);
		void UnitHelper();

		CPackMgr	m_PackMgr;
		lua_State*	m_pState;
		CScriptDebuger*	m_pDebuger;
		EScriptHelper	m_eScriptHelper;

		typedef basic_string<char, std::char_traits<char>, CScriptAllocator<char> > String_t;
		typedef map<String_t, CClass*, less<String_t>, 
			CScriptAllocator<pair<String_t, CClass*> > > MapClass_t;


		MapClass_t	m_mapClasses;

		typedef map<const char*, CClass*, less<const char*>, 
			CScriptAllocator<pair<const char*, CClass*> > > MapTypeClass_t;

		//��Ϊ���map�洢����class��rtti���֣���rtti�ǲ���ģ��������ǿ���ֱ�Ӵ洢��name��ָ��
		MapTypeClass_t	m_mapTypeClasses;

		//��������ǳ���Ҫ������Ҫʮ���˽�lua�ĵ��û���(c++�ǿ�����ʱ�������ĸ�coroutine��ִ�д������)����������������������á�
		//���ڼ�¼CClassCallBack��������ʹ�õ�lua_State��
		//���һ��CallBack�Ǳ�һ��CFunction��Ӵ����ģ���ôCallBackӦ������ʹ��CFunction����ʹ��lua_State������
		//����Ч������ĳ��coroutine����һ����������c��������ô���c���������Ļص�Ҳ������ͬ��coroutine��ִ�еġ�
		//����������Ϊnull����ôCallBack��������ע���ʱ���lua_State��Ϊ�Լ���ִ��coroutine��
		lua_State*	m_pRunningState;
	

		//�û�ע���c++������ȫ�б��˳���ʱ����Զ�ɾ�����б�ע���˵ĺ����Լ������Ϣ��
		typedef vector<CScriptProc*, CScriptAllocator<CScriptProc*> > VecFunction_t;
		VecFunction_t	m_vecFunctions;

		typedef map<String_t, int32, less<String_t>, 
			CScriptAllocator<pair<String_t, int32> > > MapSizeOfNum_t;

		MapSizeOfNum_t	m_mapSizeOfEnum;


		static int ErrorHandler(lua_State *pState);
		static int ExpLogHandler(lua_State *pState);

		String_t m_sStackInfo; //lua���������error handler��ʱ���¼��ջ

		typedef map<void*, void*, less<void*>, 
			CScriptAllocator<pair<void*, void*> > > MapContextStack_t;
		MapContextStack_t	m_mapContext2Stack;

		void SetContextStack(void* pContext, void* pStack);
		void* GetContextStack(void* pContext);
		void RemoveContextStack(void* pContext);

		CScriptSerial		m_ScriptSerial;

		CCompactAllocPool*		m_pAllocPool;

		typedef map<String_t, String_t, less<String_t>, 
			CScriptAllocator<pair<String_t, String_t> > > MapAliasPath_t;

		MapAliasPath_t		m_mapAliasToPath;

		CVariant* CreateVariant();
		void ReleaseVariant(CVariant* pVariant);
		void ClearStkVariant();

		typedef vector<CVariant*>	VecVariant_t;

		VecVariant_t* CreateVariantList();
		void ReleaseVariantList(VecVariant_t* pArgList);
		void ClearStkVariantList();

		typedef stack<CVariant*, 
			deque<CVariant*, CScriptAllocator<CVariant*> > >		StkVariant_t;
		typedef stack<VecVariant_t*, 
			deque<VecVariant_t*, CScriptAllocator<VecVariant_t*> > >		StkVariantList_t;

		StkVariant_t		m_stkVariant;
		StkVariantList_t	m_stkVariantList;

		bool	m_bCppMemLeakEnabled;	//�Ƿ���lua����c++������ڴ�й©

		uint32 GetGCStepAndResetCounter();

		size_t	m_stCallbackCounter;
		size_t	m_stNormalUnpackCounter;
		size_t	m_stSerialUnpackCounter;

		float	m_fCallbackGCRatio;
		float	m_fNormalUnpackGCRatio;
		float	m_fSerialUnpackGCRatio;
	};

}
