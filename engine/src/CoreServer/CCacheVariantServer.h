#pragma once

#include <sstream>

#include "BaseDefs.h"
#include "CCypherVariant.h"

namespace sqr
{
	class CCypherVariantHolder;
	class CSyncVariantServer;
	class CSyncVariantServerHolder;
	class CCacheVariantServerHolder;

	class CCacheVariantServer
		:protected CCypherVariant
	{
		friend class CSyncVariantServer;
		friend class CSyncVariantServerHolder;
		friend class CCacheVariantServerHolder;
		friend class CCypherVariantHolder;
	public:

		const char* GetString()const;

		template<typename ValueType>
		ValueType GetNumber() const;
		bool IsEmptyTable() const;
		//����ǰֵ��Ϊ�����Ͳ���ȡ��ĳ����Ա�������ǰֵ���Ǳ�������쳣������ɆT�����ڣ��t����NULL��
		CCacheVariantServer* GetMember(const char* szName)const;

		inline CCacheVariantServer* GetParent()const
		{
			return static_cast<CCacheVariantServer*>( CCypherVariant::GetParent() );
		}
	private:
		explicit CCacheVariantServer(CCacheVariantServerHolder *pHolder);
		~CCacheVariantServer(void);
	
		inline CCacheVariantServerHolder* GetHolder()const
		{
			return m_pHolder;
		}

		//ValueTypeֻ֧��c++�ڽ���ֵ����
		//����ǰֵ���ó�һ��ValueType��ֵ
		template<typename ValueType>
		void SetNumber(ValueType Value);
		//����ǰֵ����Ϊһ���ַ�����������\0��β��֧��\0��β�Լ۱ȱȽϵͣ���
		void SetString(const char* szValue);
		//����ǰֵ��Ϊһ������ʹ�ã������ǰֵ���Ǳ����Ƚ�����һ���ձ���������һ������ΪszName��\0��β���Ŀյı��Ա��
		CCacheVariantServer* SetMember(const char* szName);
		//����ǰֵ���óɿձ�
		void SetEmptyTable();

		//����ǰֵ��Ϊһ������ʹ�ã������ǰֵ���Ǳ�������쳣������ɾ�������ΪszName��\0��β���ĳ�Ա��
		//return: true��ʾɾ���ɹ���false��ʾû���ҵ���Ա
		bool DelMember(const char* szName);

		virtual CCypherVariant* CreateVariant()const;
		virtual void ResetDataMember();
		virtual void RecycleSubTree();
		//void InitAllMember();

		CCacheVariantServerHolder *m_pHolder;

		//void SetSyncVariantServer(CSyncVariantServer* pSyncVariantServer);
		//CSyncVariantServer* GetSyncVariantServer()const;
		//CSyncVariantServer* m_pSyncVariant;
	};
}
