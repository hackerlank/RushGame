#pragma once

#include "ExpHelper.h"
#include "BaseDefs.h"

namespace sqr
{
	//------------------------------------------------------------------------------
	/**
	@class TSingleton

	��������.
	������ʽ����������, �ڵ�һ�ε���ʱ����, �����˳�ʱstatic�����Զ�����.
	*/
	template <class T>
	class TSingleton
	{
	protected:
		TSingleton ()
		{
		}

	public:
		/// ȡ��Ψһʵ��
		static T* GetInst()
		{
			static T Inst;
			return &Inst;
		}

	};


	//------------------------------------------------------------------------------
	/**
	@class Singleton

	��������.
	��Ҫ�ֶ��������������ڵ�ʵ��.
	�����ڶ������֮���й���/����˳�����������.
	*/
	template <class T>
	class Singleton
	{
		static T* ms_Singleton;

	protected:
		/// ���Ƽ�ֱ�ӹ���
		Singleton()
		{
			Ast(NULL == ms_Singleton);
			ms_Singleton = static_cast<T*>(this);
		}

		~Singleton()
		{
			Ast(NULL != ms_Singleton);
			ms_Singleton = NULL;
		}

	public:
		/// ȡ��Ψһʵ��
		static T* GetInst()
		{
			Ast(NULL != ms_Singleton);
			return ms_Singleton;
		}

		/// �Ƿ��Ѿ�����
		static bool HasInst()
		{
			return (NULL != ms_Singleton);
		}

		static void SetNull()
		{
			ms_Singleton = NULL;
		}

		/// ����
		static T* Create()
		{
			Ast(NULL == ms_Singleton);
			return new T();
		}

		/// ����
		static void Destroy()
		{
			Ast(NULL != ms_Singleton);
			delete ms_Singleton;
			SetNull();
		}
	};

	template <class T> T* Singleton<T>::ms_Singleton = NULL;
	//------------------------------------------------------------------------------
}
