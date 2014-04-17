#pragma once
#include "ScriptCommon.h"
#include <list>
#include "CScriptAllocator.h"

using std::list;

namespace sqr
{
	class CRunScript
	{
	public:
		CRunScript( CScript& Script, const char* szFormat, char* pStack );
		~CRunScript();
		bool RunScript( lua_State* pState, char* pDataBuf, int32 nErrFunIndex );
		size_t GetParamBufSize(){ return m_uBufSize; }
	protected:
		void GetFromCppStack( char* pDataBuf );
		void PushParam2LuaStack( lua_State* pState, char* pDataBuf );
		void ReSetCppStack( lua_State* pState, char* pDataBuf );
		
		typedef vector<CBaseType*, CScriptAllocator<CBaseType*> >	VecType_t;
		
		VecType_t			m_vecParam;
		VecType_t			m_vecResult;
		char*				m_pStack;
		size_t				m_ParamTypeLen;		//��Ϊ����lua�����Ĳ�����buf��С
		size_t				m_uBufSize;			//���������ͷ���ֵ��buf��С
	};
	
}

