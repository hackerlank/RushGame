#pragma once

struct lua_State;

namespace sqr
{
	//���н�������ƵĽű�
	void RegDefClass(lua_State* pState);

	//���д���DebugPrint�����Ľű�
	void RegDebugPrint(lua_State* pState);

	//���б������ṩ��lua��չ������
	void RegExtend(lua_State* pState);

}
