#pragma once
#include <cstring>
#include "CDynamicObject.h"
#include "CompileTimeHelper.h"
#include <sstream>
#include "ExpHelper.h"

namespace sqr
{
	/*
		ע�⣬���ͷ�ļ���Ҫ����c++��lua֮�����ݴ��ݵ�
		����һ��Ļ������ͣ����ǿ��ԺܺõĴ������Ƕ��������
		�ڱ�����Ӧ�ĺ���֮ǰ����������ö��壬������ֻǰ������
		��Ϊ���ֻ��ǰ��������û�ж��壬����һ��safe�࣬�������ܿ���
		����ȥ���ö�Ӧ��CBaseObject���غ���
		Ϊ��ȷ�������ඨ���ˣ�����ʹ��sizeof�����������ж�
	*/

	/*
		���classû�б����壬��ôsizeof�õ���ֵΪ0
		���ǻṹ��һ��CompileTimeError<false>()�Ķ��󣬵���������󲻴���
		������ᱨ��
		Ϊʲô��ֱ��ʹ��char ay[sizeof(class)]�����Ĵ���
		��Ϊ��gcc���棬ay��һ��û��ʹ�õı��������warning��������warning�ᵱ��error����
	*/
	 

	template<typename T>
	struct TypeWrapper{};

	//----------------------------------------------------------------------------

	template<typename T>
	inline T* _ScriptGetValue(void* pData, void*)
	{
		return (T*)pData;
	}

	template<typename T>
	inline T* _ScriptGetValue(void* pData, CBaseObject*)
	{
		T* pObject = dynamic_cast<T*>((CBaseObject*)pData);
		if(pData && !pObject)
		{	
			ostringstream str;
			str << "��������Ϊ " << typeid(T).name(); 
			GenErr("Script ����Ķ���������Ͳ�ƥ�䣬��������Ѿ���", str.str().c_str());
		}
		return pObject;
	}

	template<typename T>
	inline T* _ScriptGetValue(void* pData, const void*)
	{
		return (T*)pData;
	}

	template<typename T>
	inline T* _ScriptGetValue(void* pData, const CBaseObject*)
	{
		T* pObject =  dynamic_cast<T*>((CBaseObject*)pData);
		if(pData && !pObject)
		{
			ostringstream str;
			str << "��������Ϊ " << typeid(T).name(); 
			GenErr("Script ����Ķ���������Ͳ�ƥ�䣬��������Ѿ���", str.str().c_str());
		}
		return pObject;
	}

	//----------------------------------------------------------------------------

	template<typename T>
	inline T ScriptGetValue(TypeWrapper<T>, const char* pDataBuf, size_t stOffset)
	{
		return *(T*)((void*)(pDataBuf + stOffset));
	}

	inline void ScriptGetValue(TypeWrapper<void>, const char* pDataBuf, size_t stOffset)
	{}

	inline void* ScriptGetValue(TypeWrapper<void*>, const char* pDataBuf, size_t stOffset)
	{
		return *(void**)((void*)(pDataBuf + stOffset));
	}

	inline const void* ScriptGetValue(TypeWrapper<const void*>, const char* pDataBuf, size_t stOffset)
	{
		return *(void**)((void*)(pDataBuf + stOffset));
	}

	template<typename T>
	inline T* ScriptGetValue(TypeWrapper<T*>, const char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		T* pType = NULL;
		return (_ScriptGetValue<T>(*(void**)(pDataBuf + stOffset), pType));
	}

	template<typename T>
	inline const T* ScriptGetValue(TypeWrapper<const T*>, const char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		const T* pType = NULL;
		return (const T*)(_ScriptGetValue<T>(*(void**)(pDataBuf + stOffset), pType));
	}

	template<typename T>
	inline T& ScriptGetValue(TypeWrapper<T&>, const char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		T* pType = NULL;
		return *(_ScriptGetValue<T>(*(void**)(pDataBuf + stOffset), pType));
	}

	template<typename T>
	inline const T& ScriptGetValue(TypeWrapper<const T&>, const char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		const T* pType = NULL;
		return *(_ScriptGetValue<T>(*(void**)(pDataBuf + stOffset), pType));
	}

	
	//-------------------------------------------------------------------------------
		
	inline void _ScriptSetValue(CBaseObject* pData, void* pBuf)
	{
		memcpy(pBuf, &pData, sizeof(void*));
	}

	inline void _ScriptSetValue(void* pData, void* pBuf)
	{
		memcpy(pBuf, &pData, sizeof(void*));
	}

	inline void _ScriptSetValue(const CBaseObject* pData, void* pBuf)
	{
		memcpy(pBuf, &pData, sizeof(void*));
	}

	inline void _ScriptSetValue(const void* pData, void* pBuf)
	{
		memcpy(pBuf, &pData, sizeof(void*));
	}

	//-------------------------------------------------------------------------------

	template<typename T>
	inline void ScriptSetValue(TypeWrapper<T>, T& Value, char* pDataBuf, size_t stOffset)
	{
		memcpy(pDataBuf + stOffset, (void*)&Value, sizeof(T));
	}

	template<typename T>
	inline void ScriptSetValue(TypeWrapper<T*>, T* Value, char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		_ScriptSetValue(Value, pDataBuf + stOffset);
	}

	inline void ScriptSetValue(TypeWrapper<void*>, void* Value, char* pDataBuf, size_t stOffset)
	{
		_ScriptSetValue(Value, pDataBuf + stOffset);
	}

	template<typename T>
	inline void ScriptSetValue(TypeWrapper<T&>, T& Value, char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		_ScriptSetValue(&Value, pDataBuf + stOffset);
	}

	template<typename T>
	inline void ScriptSetValue(TypeWrapper<const T*>, const T* Value, char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		_ScriptSetValue(Value, pDataBuf + stOffset);
	}

	template<typename T>
	inline void ScriptSetValue(TypeWrapper<const T&>, const T& Value, char* pDataBuf, size_t stOffset)
	{
		ASSERT_CLASS_DEFINED(T);

		_ScriptSetValue(&Value, pDataBuf + stOffset);
	}


	//-------------------------------------------------------------------------
	//for call back function
	template<typename RT>
	inline RT RetValue(TypeWrapper<RT>)
	{	return RT(); }

	template<typename RT>
	inline RT* RetValue(TypeWrapper<RT*>)
	{	return NULL; }

	template<typename RT>
	inline const RT* RetValue(TypeWrapper<const RT*>)
	{	return NULL; }
	
	template<typename RT>
	inline RT RetValue(TypeWrapper<RT&>)
	{	return RT(); }
	
	template<typename RT>
	inline RT RetValue(TypeWrapper<const RT&>)
	{	return RT(); }

	inline void RetValue(TypeWrapper<void>)
	{}
}
