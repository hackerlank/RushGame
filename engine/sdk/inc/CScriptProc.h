#pragma once 
#include "EFuncParamType.h"
#include "CScriptMallocObject.h"
#include "CScriptAllocator.h"

struct lua_State;

namespace sqr
{

	class CBaseType;
	
	struct CallBaseData;

	class  CScript;

	class CScriptProc : public CScriptMallocObject
	{
	public:
		CScriptProc( CScript& Script, const char* szFormat, const char* szFunName);
		virtual ~CScriptProc();
		size_t GetParamBufSize();

	protected:
		CallBaseData*		m_pData;
	};

	class CFunction : public CScriptProc
	{
		friend class	CScript;
	public:
		CFunction( CScript& Script, const char* szFormat, const char* szFunName )
			: CScriptProc( Script, szFormat, szFunName ), m_bClass(false), m_sFunName( szFunName )
		{}
		const char* GetFunctionName(){ return m_sFunName.c_str(); }

		size_t GetFunctionNameSize() { return m_sFunName.size(); }

	protected:
		void	SetBeClassFunc(bool bClass) { m_bClass = bClass; }

		typedef basic_string<char, std::char_traits<char>, CScriptAllocator<char> > String_t;

		bool			m_bClass;
		String_t			m_sFunName;

		int32	GetParamFromLuaStack( lua_State* pState, char* pDataBuf, bool bPack = false );
		void	FillParamToCppStack( char* pDataBuf, char* pBuf, int32 nSize, bool bPack = false );
		void    FreeParamFromCppStack(char* pDataBuf);
		int32	PutResultToLuaStack( lua_State* pState, char* pDataBuf );
	};


	//CClassCallBack�Լ�CUnpackCallBack��������̳�����
	class CBaseCallBack : public CScriptProc
	{
	public:
		CBaseCallBack( CScript& Script, const char* szFormat, const char* szFunName );
		~CBaseCallBack();

	protected:
		typedef basic_string<char, std::char_traits<char>, CScriptAllocator<char> > String_t;

		String_t		m_sFunName;
		//���ﱣ����CCallBack��ע��ʱ��ʹ�õ�coroutine,��Callback��Ӧ������ʹ�ø�courtine��Ϊ�Լ���ִ��coroutine��
		//Ӧ������ʹ�õ�ǰ����ִ�е�coroutine��Ϊ�Լ���ִ��coroutine��
		lua_State*	m_pDefaultState;
		String_t		m_sArgBuf;
		void*		m_pObj;

		void GetFromCppStack( char* pDataBuf );
		void PushParam2LuaStack( lua_State* pState, char* pDataBuf );
	};


	template <typename BaseType>
	class CBaseFuncCall : public BaseType
	{
	public:
		typedef vector<size_t, CScriptAllocator<size_t> > VecParamOffset_t;
		typedef vector<int32, CScriptAllocator<int32> > VecParamType_t;
		typedef vector<size_t, CScriptAllocator<size_t> > VecParamLen_t;

		CBaseFuncCall(CScript& Script, const char* szFormat, const char* szFunName);
		~CBaseFuncCall(){}

		inline const VecParamOffset_t& GetParamOffset() const { return m_vecParamOffset; }
		inline VecParamOffset_t& GetParamOffset() { return m_vecParamOffset; }
		inline size_t GetRetOffset() const { return m_stRetOffset; }
	
	
		inline const VecParamLen_t& GetParamLen() const { return m_vecParamLen; }
		inline VecParamLen_t& GetParamLen() { return m_vecParamLen; }
		inline size_t GetRetLen() const { return m_stRetLen; }

	
		inline const VecParamType_t& GetParamType() const { return m_vecParamType; }
		inline VecParamType_t& GetParamType() { return m_vecParamType; }
		inline int32 GetRetType() const { return m_iRetType; }

	private:
		VecParamOffset_t	m_vecParamOffset;  //���������ƫ����������ߵĲ����ڵ�һ���������
											   //���Ա�������ã���һ��Ϊthis��ƫ��
		size_t				m_stRetOffset;

		VecParamType_t		m_vecParamType;    //��������
		int32				m_iRetType;		   //����ֵ����

		VecParamLen_t		m_vecParamLen;     //�������ͳ���
		size_t				m_stRetLen;		   //������ͳ���
	};
}

