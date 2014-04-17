#pragma once
//---------------------------------------------------------Script--------------------------------------------------------
//
// �ű���c++�Ľӿ�
//
// ScriptFunction : c++��������ֵΪ�ű���������ֵ����
// szFormat :		��д��ĸ�޷��ţ�Сд�з���
//					t/T				size_t			//һ�����ڰ�ָ���ֵ���ݸ�luaʹ��
//					z:				int64			//����ӵ��������ͣ�ֻ��֧��(-1e+15,1e+15)��Χ�ڵ����ݣ����������Χ�����
//					Z:				uint64          //ͬ�ϣ�ֻ�Ǵ�0��ʼ
//					i:				int32
//					I:				uint32
//					h:				int16
//					H:				uint16
//					c:				int8
//					C:				uint8
//					b:				bool
//					f:				float
//					d:				double
//					s:				const char *
//					m:				Memory				//Script�ṩ�ķ��������ڴ����
//					(CSomeClass):	CSomeClass *		//Ŀǰ����ȷ���Ƿ������ڷ���ֵ�Լ��ص�
//					a:              CVariant            //֧�������������
//					l:              vector<CVariant*>   //֧�ֿɱ�����Ļ�������
//
//					RunScript ��szFormat = "ii:ss"��ʽ����ʾ���õĺ�������Ϊ��������������������Ϊ�����ַ���
// ��lua��������޸ģ�
//					@.�޸���global_State�ṹ��ĩβ������һ��void* context���������ڱ��������
//					@.�޸���luaFile��һϵ�к�����ʹ���ļ����ԴӰ��ļ�ϵͳ����
//					@.�ر���sethook��gethook����������ע�ᣬʹ���޷���lua�����޸�hook����
// ע�⣺			�ӽű��л�ȡ���ݣ�����RunScript�Ľ����GetArgFromHandle��õĲ��������봫���ݵ�ַ���÷�����scanf��
//					�������ݴ����ű�������RunScript�Ĳ�����PushResultToHandle����Ľ����ֻ�ܴ���ֵ���÷�����printf��
//
//---------------------------------------------------------------------------------------------------------------------------

/*
�ű�����Ҫע�����

1��ע������ʱ��д�����˳������c++��������˳��һ�¡�
2������ͬʱ���м̳й�ϵ����ע��Ϊ�Լ����ࡣ
3��ע��ʱ���������μ̳С�
4��Unsafe�಻������c++���洴������֮����luaʹ�á�
5��ע��ʱ������Unsafe�������Ҳ��Unsafe��
6����lua���������c++�࣬��ֹʹ����������������ֻ�ÿյ�����������ԭ���������������� �����ջ��������Ƶ��ã�ִ��ʱ������ȷ�������ײ������ѽ����bug��������������Ӧ����һ ��Release()�����У���lua��ʽ���á�
7����c++�д������󲢴��ݸ�luaʹ�õ�c++�࣬����ʹ��������
*/

#include "ScriptDef.h"
#include "ThreadTypes.h"

struct lua_State;

namespace sqr
{
	enum EScriptHelper {eSH_None,eSH_Debuger,eSH_Profiler};

	class CSqrAllocPool;

	class SCRIPT_API CScript
	{
	public:
		CScript(EScriptHelper eScriptHelper,
			float fCallbackGCRatio=1,float fNormalUnpackGCRatio=1,float fSerialUnpackGCRatio=1);
		~CScript();

		static int Compile(int nArgc,char* aryArgv[]);


		EScriptHelper GetHelperType()const;

		//�õ�Script����
		CScriptData* GetData() const;
	
		//���ýű��ļ�
		std::string RunFile( const char* szScript, const char* szFormat=NULL, ... );
		//ִ��һ�δ���
		std::string	RunString( const char* szString, const char* szFormat=NULL, ... );
		//ִ��һ������
		std::string	RunFunction( const char* szFunction, const char* szFormat=NULL, ... );

		//����Script��AliasPath�Լ�Path
		void SetLoadPath(const char* szPathAlias, const char* szPath);
		const char* GetLoadPath(const char* szPathAlias);

		//ע�ᵥ�����ű����õĺ���
		template<typename Func>
		void RegCallFunc(const char* szArg, Func pFunc, const char* FunctionName );

		// ע�����Ա����
		template<typename Func>
		void RegClassCallFunc(const char* szArg, Func pFunc, const char* szClass, const char* FunctionName);

		template<typename Func>
		void RegClassCallback(const char* szArg, Func pFunc, const char* szClass, const char* szFunName, bool bPureCallBackFunc);

		// ע����
		void RegClass( int32 eClassType, REG_CREATE_FUN pCreateFun, REG_DELETE_FUN pDeleteFun, size_t sizeOfClass, const char* szTypeClass, const char* szClass, ... );
		
		//ע��isolated class
		void RegIsolatedClass( REG_CREATE_FUN pCreateFun, REG_DELETE_FUN pDeleteFun, size_t sizeOfClass, const char* szTypeClass, const char* szClass );
		
		// ע�����Ա����
		void RegClassMember( const char* szArg, int32 nOffset, const char* szClass, const char* szMemberName );
		
		//ע�ắ���������������
		HPACKAGE RegPackPakage( PACK_FUNCTION pPackFun, void* pCppContext, const char* szTableName );
		
		//ע�ắ�������������������ĺ���
		void RegPackFunction( HPACKAGE hPakageHandle, const char* szFunction, uint32 uID, bool bFirstArgAsCallContext, const char* szFormat );
		
		//ע�ắ���������������
		HPACKAGE RegUnPackPakage( const char* szTableName, UNPACK_FUNCTION* pUnPackFun );
		
		//ע�ắ����������������Ľ������
		void RegUnPackFunction( HPACKAGE hPakageHandle, const char* szFunction, uint32 uID, bool bFirstArgAsCallContext, const char* szFormat );
		
		//ע��serial���������������
		void RegSerialPackFun(const char* szFunName, POST_PACKFUNCTION pPostPackFun);
		
		//ע��serial���������������
		void RegSerialUnPackFun(ON_UNPACKFUNCTION* pOnUnPackFun);

		//ע�᳣��
		void RegConstant( const char* szTableName, const char* szFeild, int32 nValue );
		//�õ�ע�������Ϣ
		CClass* GetClassInfo( const char* szClassName );
		//ͨ��rtti��name�õ�������Ϣ
		CClass*	GetClassInfoWithType( const char* szClassType );
		
		//�������ü���
		void RefScriptObj( void* pUserData );
		void RefScriptObj( CBaseObject* pObject );
		
		//�������ü���
		void UnrefScriptObj( void* pUserData );
		void UnrefScriptObj( CBaseObject* pObject );
		
		//ȡ�������script�����ӹ�ϵ
		void UnlinkCppObj( void* pObj );
		void UnlinkCppObj( CBaseObject* pObject );

		//��������
		void GCAll();

		//��ʱ������������		
		void GCStep(uint32 uMinStep);

		//����gc������
		void SetGCStepMul(int stepmul);
		void SetGCPause(int pause);

		//�õ�lua���ڴ�ռ��
		int	GCCount();

		//ȡ�ûص���������
		void* GetCallBackData(CBaseObject* pObject, const char* szFunName);
		//ȡ�ûص����������Ϣ
		void GetCallBackAttr(void* pCall, size_t& stBufSize, size_t*& pAyParamOffset, 
			size_t& iRetOffset);
		//���ûص�����
		bool CallClassCallBack(void* pCall, CBaseObject* pObject, char* pDataBuf);

		//��ȡ��ǰ��lua��ջ
		static void	GetStack(ostream& strm);

		static void Init(EScriptHelper eScriptHelper,
			float fCallbackGCRatio=1,float fNormalUnpackGCRatio=1,float fSerialUnpackGCRatio=1);
		static void Unit();
		static CScript* Inst();

		void AstInSameThread();

		void SetRunningThread(HTHREADID id);
		void DetachRunningThread();

		void CallBackProfileBegin(const char* szFunName);
		void CallBackProfileEnd(const char* szFunName);

	private:
		bool InSameThread();
		
		void _RegCallFunc(void* pContext, const char* szFunctionName, int32 (*pFunc)(lua_State*));
		void _RegClassCallFunc(void *pContext, const char* szClassName, const char* szFunctionName, int32 (*pFunc)(lua_State*));
		void _RegClassCallback(void* pContext);

		CClass* CreateClass(REG_CREATE_FUN pCreateFun, REG_DELETE_FUN pDeleteFun, 
			size_t sizeOfClass, const char* szTypeClass, 
			const char* szClass, int32 eClassType);

		
	
	private:

		CScriptData*		m_pData;
		CCallBackProfiler*	m_pCallBackProfiler;

		/*
		�ű�������Ҫ���ж��̲߳���ˣ�����ms_pInstֻ�������̵߳Ķ���
		�������̵߳�script��������ʹ��tls�洢
		*/
		static CScript*	ms_pInst;

		static HTLSKEY	ms_tlsKey;

		HTHREADID			m_hThreadID;
	};
}


using namespace sqr;
