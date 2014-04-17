#include "stdafx.h"
#include "CClassCallBack.h"
#include "CScript.h"
#include "CClass.h"
#include "CDataType.h"
#include "ScriptCallBaseData.h"
#include "CScriptData.h"
#include "ScriptCommon.h"
#include "CBaseObject.h"
#include "ErrLogHelper.h"
#include "CCallBackProfiler.h"

CClassCallBack::CClassCallBack(CScript& Script, const char* szClassName, const char* szFunName, 
							   const char* szFormat, bool bPureCallBackFunc)
: ParentType(Script, szFormat, szFunName)
, m_sClassName(szClassName)
, m_sFunName(szFunName)
, m_szFormat(szFormat)
, m_bPureCallBackFunc(bPureCallBackFunc)
{	
	CClass* pClass = Script.GetClassInfo( szClassName );
	if( ! pClass )
	{
		ostringstream strm;
		strm<<"\""<<szClassName<<"\" not exists.";
		GenErr(strm.str());
	}

	if (m_pData->m_bRetArgList)
	{
		GenErr("�ɱ��������������callback�ķ���ֵ");
	}

	pClass->RegCallBackFunc(szFunName, this);

}

CClassCallBack::~CClassCallBack()
{
}


void* CClassCallBack::GetCallBackData(CBaseObject* pObject, const char* szFunName)
{
	//��������ɽű��ⴴ���ģ���ô�ǲ�������callback�ص���
	if(!pObject->CreatedByScript())
		return NULL;

	CScript* pScript = CScript::Inst();	

	if(!pScript)
		return NULL;

	//pScript->AstInSameThread();

	const char* szClassName = typeid(*pObject).name();
	if (!szClassName)
	{
		char buffer[100];
		sprintf(buffer, "����%s��c++����û����ȷ��rtti��Ϣ", szFunName);
		GenErr(buffer);
	}

	CClass* pClass = pScript->GetClassInfoWithType(szClassName);

	if(!pClass)
	{
		char buffer[100];
		sprintf(buffer, "����%s��c++�����rtti����%sû��ע��", szFunName, szClassName);
		GenErr(buffer);
	}

	if(!pClass->IsCallBack())
		return NULL;
	
	void* pCall = pClass->GetCallBackFunc(szFunName);
	
	return pCall;
}


bool CClassCallBack::CallClassCallBack(CBaseObject* pObject, char* pDataBuf)
{
	CScript::Inst()->CallBackProfileBegin(m_sFunName.c_str());

	CScriptData* pData=CScript::Inst()->GetData();
	lua_State* pState;

	if(pData->m_pRunningState)
	{	 
		//��ǰ����ִ�е�coroutine����NULL,˵����ǰcallback�Ǳ�ĳ��lua���õ�c���������ģ���ôӦ��ʹ�������ǰlua��coroutine
		pState=pData->m_pRunningState;
	}
	else
	{
		//��ǰִ��coroutine��NULL��˵��Ŀǰ��ջ��û���κ�lua������
		pState=m_pDefaultState;
	}

	//_S(pState).AstInSameThread();

	int32 nTop = lua_gettop(pState);

	int32 nErrFunIndex=0;
	lua_pushcfunction(pState,&CScriptData::ErrorHandler );
	nErrFunIndex=lua_gettop(pState);

	int32 nErrCode = 0;

	GetGlobalObjTable(pState);
	
	EBaseObjectType eType = pObject->GetBaseObjectType();

	if (eType == eBOT_Share)
	{
		lua_pushnumber(pState, pObject->GetBaseObjectID());
	}
	else
	{
		lua_pushlightuserdata(pState, pObject);
	}

	lua_gettable(pState, -2);

	if(lua_isnil(pState, -1))
	{
		lua_settop(pState, nTop);
		return false;
	}

	lua_pushlstring(pState, m_sFunName.c_str(), m_sFunName.size());
	lua_gettable(pState, -2);		//err func, __objtable, obj, func	
	if(lua_isnil(pState, -1))
	{
		if (!m_bPureCallBackFunc)
		{
			lua_settop(pState, nTop);
			return false;
		}
		else
		{
			ostringstream strm;
			strm<<"��: "<< m_sClassName.c_str()<<" ��CallBack����: "<< m_sFunName.c_str()<<" ������lua�����غ����ʹ�ã�"<<endl;
			GenErr(strm.str());
		}
	}

	int32 nArg = (int32)m_pData->m_vecParam.size();
	int StackSize = nArg + m_pData->m_nReturnNum + 1;
	//callback�Ĳ������࣬���ܵ���lua��ջ�����������һ��
	luaL_checkstack(pState, StackSize, "too many arguments in callback");

	lua_insert( pState, -2 );	//err, objtable, func, obj
	
	int32 nArgBegin = lua_gettop(pState);
	PushParam2LuaStack( pState, pDataBuf );

	int32 nArgEnd = lua_gettop(pState);	

	++pData->m_stCallbackCounter;
	nErrCode = lua_pcall( pState, nArgEnd - nArgBegin + 1, m_pData->m_nReturnNum, nErrFunIndex );

	if(nErrCode == LUA_ERRRUN || nErrCode == LUA_ERRMEM
		|| nErrCode == LUA_ERRERR)
	{
		const char* szError = lua_tostring(pState, -1);

		lua_settop(pState, nTop);	

		ostringstream str;
		str << "��" << m_sClassName.c_str() << "::" << m_sFunName.c_str() 
			<< " �������ص�����" << endl;

		CError error(szError);
		error.AppendInfo(str.str().c_str());
		error.AppendStackInfo(pData->m_sStackInfo.c_str());

		LogExp(error, NULL, false);

		OnScriptError(pState, nErrCode);
		return false;
	}


	if( m_pData->m_pResult )
		m_pData->m_pResult->GetFromLuaStack( pState, pDataBuf, -1 );

	//����lua��ջ��Ϊ����ǰ��С
	lua_settop(pState, nTop);

	CScript::Inst()->CallBackProfileEnd(m_sFunName.c_str());

	return true;
}
