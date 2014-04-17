#pragma once

extern "C"
{
#include "lua/lua.h"
#include "lua/lauxlib.h"
}

#include "CScript.h"

struct lua_State;

namespace sqr
{
	class CClass;
	class CScriptDebuger;
	class CBaseType;

	int32  ToUint32( lua_State* L );
	int32  ToInt32( lua_State* L );
	int32  ToUint16( lua_State* L );
	int32  ToInt16( lua_State* L );
	int32  ToUint8( lua_State* L );
	int32  ToInt8( lua_State* L );
	int32  BitAnd( lua_State* L );
	int32  BitOr( lua_State* L );
	int32  BitNot( lua_State* L );
	int32  BitXor( lua_State* L );

	int ASLoadFile( lua_State*L, const char* szFileName );

	CScript&		_S( lua_State* pState );
	lua_State*		_L( CScript& Script );

	//extern const char AS_GLOBAL_REFERENCE_TABLE_NAME[];
	//extern const char AS_REF_NUM_FILED_NAME[];
	//extern const char AS_REF_DATA_FILED_NAME[];

	void		MapObjectToUserdata( lua_State* pState, void* pUserData );
	void		GetObjectByUserdata( lua_State* pState, void* pUserData );

	void		MapObjectToID( lua_State* pState, uint32 uID );
	void		GetObjectByID( lua_State* pState, uint32 uID );


	//��ջ��table����cpp obj ptr��field���ó�pObj  obj["__CppObjPtr"] = pObj
	void SetCppObjPtr(lua_State* pState, void* pObj);

	//�õ�ջ��table����cpp obj ptr��field��ֵ��ѹ��lua��ջ  return obj["__CppObjPtr"]
	void GetCppObjPtr(lua_State* pState);

	//��ȫ��table���obj tbl��field����Ϊָ���table  _G["__ObjTable"] = objtbl
	void SetGlobalObjTable(lua_State* pState);

	//��ȫ��table��õ�obj tbl��field��ֵ��ѹ��lua��ջ return _G["__ObjTable"]
	void GetGlobalObjTable(lua_State* pState);

	//����cpp mem leak table
	void SetCppMemLeakTable(lua_State* pState);

	//��ȡcpp mem leak table
	void GetCppMemLeakTable(lua_State* pState);

	//����ĳһ��table�������е�key value�����������Ӧ�ļ�
	int TraverseTable(lua_State* pState);

	void OnScriptError(lua_State* pState, int32 nErrorCode);
}
