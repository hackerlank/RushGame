#include "stdafx.h"
#include "DataPack.h"
#include "CScriptData.h"
#include "CBaseType.h"
#include "ErrLogHelper.h"
#include "ScriptHelper.h"
#include "CLogOwner.h"
#include "TSqrAllocator.inl"


//-----------------------------------------------------------------------
// �������
//-------------------------------------------------------------------------

int32 CPackFunction::PackFromLua( lua_State* pState )
{
	CPackFunction& Function = *(CPackFunction*)lua_touserdata(pState, lua_upvalueindex(1));
	SQR_TRY
	{
		//��gas2gacɾ��
		lua_remove( pState, 1 );
		//�����һ��������ΪCallContext
		void* pCallContext = NULL;
		if( Function.m_pCallContext )
		{
			Function.m_pCallContext->GetFromLuaStack( pState, (char*)&pCallContext, 1 );
			lua_remove(pState, 1);
		}

		//�����������ݣ�׼�����ͳ�ȥ
		char* pDataBuf = (char*)alloca( Function.GetParamBufSize() );
		size_t nSize = Function.GetParamFromLuaStack( pState, pDataBuf, true );
		char* pParamBuf = (char*)alloca( nSize );
		Function.FillParamToCppStack( pDataBuf, pParamBuf, (int32)nSize, true );
		lua_pop( pState, (int32)Function.m_pData->m_vecParam.size() );
		//printf("Pack %s %d\n", Function.GetFunctionName(), nSize);
		//����ð��Ĳ��������ص�����
		bool re = ( *Function.m_pGroup->m_pCallBackFun )( Function.m_uID, Function.m_pGroup->m_pCppContext, pCallContext, pParamBuf, nSize );
		lua_pushboolean( pState, re );
	}
	SQR_CATCH(exp)
	{
		LogExp(exp);
	}
	SQR_TRY_END;
	return 1;
}

int32 CUnPackCallback::UnPackBuf( char* pDataBuf, char* pBuf, size_t size )
{
	CallBaseData::VecType_t::reverse_iterator iter,end;
	iter = m_pData->m_vecParam.rbegin();
	end = m_pData->m_vecParam.rend();

	int32 nPos = 0;
	for( ; iter!= end; ++iter )
	{
		int32 nSize = (*iter)->UnPack( pDataBuf, pBuf + nPos, size - nPos );
		if( nSize < 0 )
			return -1;
		if( !nSize )
			return -2;
		nPos += nSize;
	}

	return nPos;
}

//----------------------------------------------------------------------------
// CPackMgr
//-----------------------------------------------------------------------------
bool CPackMgr::OnUnPackError( lua_State* pState, const char* szTableName, int32 nCode, 
							 void* pContext, const char *szFunName )
{
	/*
	-1 ����ʾid����
	-2 ����ʾ��֤�����
	-3 ����ʾ���ݸ�ʽ����
	-4 ����ʾû���ҵ�������
	-5 ����ʾ���صĴ���
	>0 ����ʾ�û��Լ����صĲ���
	*/

	luaL_checkstack(pState, 10, "too many arguments in OnUnpack Error");

	int32 nTop = lua_gettop(pState);

	int32 nErrFunIndex=0;
	lua_pushcfunction(pState,&CScriptData::ErrorHandler );
	nErrFunIndex=lua_gettop(pState);

	lua_pushstring( pState, szTableName );
	lua_rawget( pState, LUA_GLOBALSINDEX );
	if (!lua_istable(pState, -1))
	{
		lua_pop( pState, 1 );
		return false;
	}

	lua_getfield( pState, -1, "OnError" );
	//printf("Error rpc Func is: %s\n", szFunName);
	if( !lua_isfunction( pState, -1 ) )
	{
		lua_pop( pState, 2 );
		return false;
	}

	lua_insert( pState, -2 );
	lua_pushinteger( pState, nCode );

	if (*szFunName == '\0')
	{
		lua_pushstring(pState, "no rpc fun ");
	}
	else
	{
		lua_pushstring(pState, szFunName);
	}

	int nErrCode = lua_pcall( pState, 3, 1, nErrFunIndex );
	if(nErrCode == LUA_ERRRUN || nErrCode == LUA_ERRMEM
		|| nErrCode == LUA_ERRERR)
	{
		string strError = lua_tostring(pState, -1);
		CScriptData* pData= CScript::Inst()->GetData();

		lua_settop(pState, nTop);	

		CError error(strError, pData->m_sStackInfo.c_str());
		LogExp(error, dynamic_cast<CLogOwner*>((CBaseObject*)pContext), false);
		OnScriptError(pState, nErrCode);
		return false;
	}
	
	bool bSuc = (lua_toboolean(pState, -1) == 1);
	if(!bSuc)
	{
		lua_settop(pState, nTop);
		return false;
	}

	lua_settop(pState, nTop);
	return true;
}

size_t CPackMgr::UnPack( uint32 uID, CUnPackGroup* pGroup, void* pCallContext, char* pBuf, size_t uBufSize)
{
	lua_State* pState = _L( *pGroup->m_pScript );
	
	int32 nTop = lua_gettop(pState);

	int32 nErrFunIndex=0;
	lua_pushcfunction(pState,&CScriptData::ErrorHandler );
	nErrFunIndex=lua_gettop(pState);

	//����idУ�飬��ֹ��Ϣ��λ
	if( uint16(uID) != (*(uint16*)pBuf ^ 55555 ) )
	{
		OnUnPackError( pState, pGroup->m_sTableName.c_str(), -2, pCallContext );
		lua_settop(pState, nTop);
		return static_cast<size_t>(-1);
	}

	pBuf += 2;
	uBufSize -= 2;

	//����id�ҳ�UnPacker
	CUnPackGroup::MapUnPackCallBack_t::iterator itUnPack = pGroup->m_mapUnPackInfo.find( uID );
	if( itUnPack == pGroup->m_mapUnPackInfo.end() )
	{
		OnUnPackError( pState, pGroup->m_sTableName.c_str(), -1, pCallContext );
		lua_settop(pState, nTop);
		return static_cast<size_t>(-1);
	}	

	uint32 stSrcSize = *(uint32*)pBuf;
	pBuf += sizeof(uint32);
	uBufSize -= sizeof(uint32);

	//���buffer�ĳ���С��rpc��Ҫ����ĳ��ȣ����Ƿ��صȴ����յ����������ڴ���
	if (stSrcSize > uBufSize)
	{
		lua_settop(pState, nTop);
		return static_cast<size_t>(-2);
	}

	//��pipe���������ݣ����н���
	CUnPackCallback& Info = *itUnPack->second;
	char* pInfoBuf = (char*)alloca( Info.GetParamBufSize() );
	size_t nUnPackSize = Info.UnPackBuf( pInfoBuf, pBuf, uBufSize );

	const char *szFunName = Info.GetFunctionName();

	if( nUnPackSize == (size_t)-2 )
	{
		lua_settop(pState, nTop);
		return static_cast<size_t>(-2);
	}

	if( nUnPackSize < 0 )
	{
		OnUnPackError( pState, pGroup->m_sTableName.c_str(), -3,pCallContext, szFunName );
		lua_settop(pState, nTop);
		return static_cast<size_t>(-1);
	}

	//printf("UnPack %s %d %d\n", szFunName, nUnPackSize, stSrcSize);

	lua_pushlstring( pState, pGroup->m_sTableName.c_str(), pGroup->m_sTableName.size() );
	lua_rawget( pState, LUA_GLOBALSINDEX );
	if (!lua_istable(pState, -1))
	{
		lua_pop( pState, 1 );
		OnUnPackError( pState, pGroup->m_sTableName.c_str(), -5, pCallContext );
		lua_settop(pState, nTop);
		return static_cast<size_t>(-1);
	}

	lua_pushlstring(pState, szFunName, Info.GetFunctionNameSize());
	lua_gettable(pState, -2);
	if( !lua_isfunction( pState, -1 ) )
	{
		lua_pop( pState, 2 );
		bool bNeedPopData = OnUnPackError( pState, pGroup->m_sTableName.c_str(), -4, pCallContext, szFunName );
		/*
			����������ѹ�����Ե�ʱ�򣬲������е�rpc�����������ˣ�����Ϊ����ʹ����һֱ���У�
			�����ڳ������ʱ�����Ƿ�����Ҫpop�������ݣ��������д�����
		*/
		lua_settop(pState, nTop);

		if(bNeedPopData)
			return nUnPackSize;
	
		return static_cast<size_t>(-1);
	}

	lua_insert( pState, -2 );
	if( Info.m_pCallContext )
		Info.m_pCallContext->Push2LuaStack( pState, (char*)&pCallContext );
	
	int ParamNum = (int32)Info.m_pData->m_vecParam.size() + ( Info.m_pCallContext ? 2 : 1 );
	int StackSize = ParamNum + 2;
	//unpack�Ĳ���������࣬���ܵ���lua��ջ�����������һ��
	luaL_checkstack(pState, StackSize, "too many arguments in unpack");
	
	int32 nParamTop = lua_gettop(pState);
	Info.PushParam2LuaStack( pState, pInfoBuf );

	if ( stSrcSize != (uint32)nUnPackSize)
	{
		ostringstream strm;

		strm << "FuncID: " << uID;
		strm << " GroupName: " << pGroup->m_sTableName;
		strm << " FuncName: " << szFunName
			<< " �������: " << stSrcSize 
			<< " �������: " << nUnPackSize;

		int32 n = lua_gettop(pState);

		for (int32 i = nParamTop + 1; i <= n; i++)
		{
			strm << "\n����" << i - nParamTop << ": ";
			int32 type = lua_type(pState, i);
			switch(type)
			{
			case LUA_TNUMBER:
				strm << lua_tonumber(pState, i);
				break;
			case LUA_TBOOLEAN:
				strm << lua_toboolean(pState, i);
				break;
			case LUA_TSTRING:
				strm << lua_tostring(pState, i);
				break;
			default:
				break;
			}
		}

		LogErr("UnPack���Ȳ���ȷ", strm.str(), 
			dynamic_cast<CLogOwner*>((CBaseObject*)pCallContext));
		lua_settop(pState, nTop);
		return static_cast<size_t>(-1);
	}

	CScriptData* pData= CScript::Inst()->GetData();

	pData->m_stNormalUnpackCounter+=10;
	int32 nErrCode = lua_pcall( pState, ParamNum, 1, nErrFunIndex );
	if(nErrCode == LUA_ERRRUN || nErrCode == LUA_ERRMEM
		|| nErrCode == LUA_ERRERR)
	{
		string strError = lua_tostring(pState, -1);	
		
		lua_settop(pState, nTop);	

		CError error(strError, pData->m_sStackInfo.c_str());
		LogExp(error, dynamic_cast<CLogOwner*>((CBaseObject*)pCallContext), false);
		OnScriptError(pState, nErrCode);
		return nUnPackSize;
	}

	if( !lua_isnil( pState, -1 ) )
	{
		int32 nCode = (int32)lua_tointeger( pState, -1 );
		lua_pop( pState, 1 );
		if( nCode > 0 )
			OnUnPackError( pState, pGroup->m_sTableName.c_str(), nCode, pCallContext, szFunName );
		lua_settop(pState, nTop);
		return static_cast<size_t>(-1);
	}

	//lua_pop( pState, 1 );

	lua_settop(pState, nTop);

	return nUnPackSize;
}

//ע�ắ���������������
HPACKAGE CPackMgr::RegPackPakage( CScript& Script, PACK_FUNCTION pPackFun, void* pCppContext, const char* szTableName )
{
	m_listPackGroup.push_back( CPackGroup() );
	CPackGroup& Group = *m_listPackGroup.rbegin();
	Group.m_pCallBackFun = pPackFun;
	Group.m_pCppContext = pCppContext;
	Group.m_szTableName = szTableName;

	lua_State* pState = Script.GetData()->m_pState;
	lua_pushstring( pState, szTableName );
	lua_rawget( pState, LUA_GLOBALSINDEX );
	if( lua_isnil( pState, -1 ) )
	{
		lua_pop( pState, 1 );

		// ���ڴ�Ź�Զ�̵��õĺ���
		lua_newtable(pState);
		lua_setglobal(pState, szTableName);
	}
	else
	{
		Ast( lua_istable( pState, -1 ) );
		lua_pop( pState, 1 );
	}

	return &Group;
}

//ע�ắ�������������������ĺ���
void CPackMgr::RegPackFunction( CScript& Script, HPACKAGE hPakageHandle,
							   const char* szFunction, uint32 uID, bool bFirstArgAsCallContext, const char* szFormat )
{
	CPackGroup& Group = *(CPackGroup*)hPakageHandle;
	CPackFunction* pPackScript = new CPackFunction( Script, szFormat, bFirstArgAsCallContext, szFunction );
	pPackScript->m_uID = uID; //Э���id
	pPackScript->m_pGroup = &Group; //pPackFun������pCppContext

	lua_State* pState = Script.GetData()->m_pState;
	lua_pushstring( pState, Group.m_szTableName.c_str() );
	lua_rawget( pState, LUA_GLOBALSINDEX );
	Ast( lua_istable( pState, -1 ) );

	// ��upvalue��Э���ʽ���ݰ󶨵��������
	lua_pushlightuserdata( pState, pPackScript );
	lua_pushcclosure( pState, reinterpret_cast<lua_CFunction>( CPackFunction::PackFromLua ), 1 );
	lua_setfield( pState, -2, szFunction );
	lua_pop( pState, 1 );

	Group.m_listPackInfo.push_back( pPackScript );
}

//ע�ắ���������������
HPACKAGE CPackMgr::RegUnPackPakage( CScript& Script, const char* szTableName, UNPACK_FUNCTION* pUnPackFun )
{
	m_listUnPackGroup.push_back( CUnPackGroup() );
	CUnPackGroup& Group = *m_listUnPackGroup.rbegin();
	Group.m_sTableName = szTableName;
	Group.m_pScript = &Script;

	lua_State* pState = Script.GetData()->m_pState;
	lua_pushstring( pState, szTableName );
	lua_rawget( pState, LUA_GLOBALSINDEX );
	if( lua_isnil( pState, -1 ) )
	{
		lua_pop( pState, 1 );

		// ���ڴ�Ź�Զ�̵��õĺ���
		lua_newtable(pState);
		lua_setglobal(pState, szTableName);
	}
	else
	{
		Ast( lua_istable( pState, -1 ) );
		lua_pop( pState, 1 );
	}

	*pUnPackFun = (UNPACK_FUNCTION)UnPack;
	return &Group;
}

//ע�ắ����������������Ľ������
void CPackMgr::RegUnPackFunction( CScript& Script, HPACKAGE hPakageHandle,
								 const char* szFunction, uint32 uID, bool bFirstArgAsCallContext, const char* szFormat )
{
	CUnPackGroup& Group = *(CUnPackGroup*)hPakageHandle;
	Ast( Group.m_mapUnPackInfo.find( uID ) == Group.m_mapUnPackInfo.end() );
	Group.m_mapUnPackInfo[uID] = new CUnPackCallback( Script, szFormat, szFunction, bFirstArgAsCallContext );
}
