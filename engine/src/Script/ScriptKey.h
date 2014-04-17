#pragma once

namespace sqr
{
	/*
		��Ϊ��ǰ��������lua�����һЩ������ͨ��
		�ַ������ʵģ�lua�����ַ����Ĵ�����Ȼ�ܸ�Ч������lua�����ⲿ������ַ�����
		���Ȼ����һ��hashƥ����ң�ͬ���Ĵ���
		lua_getfield(L, t, "field") //ȡ��t������Ӧtable����field�ֶεĶ���

		����
		static int field; //����������field
		lua_pushlightuserdata(L, &key)
		lua_gettable(L, t)
		
		Ч��Ҫ��һ�㣬�������ǿ���ʹ��һЩkey������string���ֶ�

		������Щ���������ṩһЩ�ؼ��ַ�����key
	*/

	inline void* GetCppObjPtrKey()  //���ڶ��������cpp obj ptr
	{
		static int ls_CppObjPtr = 0;
		return &ls_CppObjPtr;
	}

	
	inline void* GetGlobalObjTblKey()  //����global table�����obj table
	{
		static int ls_ObjTbl = 0;
		return &ls_ObjTbl;
	}

	inline const char* GetGlobalObjTblKeyName()
	{
		return "GlobalObjTbl";
	}

	inline void* GetClassPtrKey()
	{
		static int ls_ClassPtr = 0;
		return &ls_ClassPtr;
	}

	inline void* GetCppMemLeakTblKey()
	{
		static int ls_CppMemLeakTbl = 0;
		return &ls_CppMemLeakTbl;
	}

	inline const char* GetCppMemLeakTblKeyName()
	{
		return "CppObjMemLeakTbl";
	}

	inline void* GetSerialRefTblKey()
	{
		static int ls_SerialRefTbl = 0;
		return &ls_SerialRefTbl;
	}

	inline void* GetGlobalRefTableKey()
	{
		static int ls_RefTableKey = 0;
		return &ls_RefTableKey;
	}

	inline const char* GetGlobalRefTableKeyName()
	{
		return "__global_reference_table";;
	}
}
