#pragma once
#include "VariantTypes.h"
#include "TVariantTraits.h"
#include "CommonDefs.h"
#include "CVariantMallocObject.h"

namespace sqr
{
	template <typename ImpVariant>
	class COMMON_API TBaseVariant
		:public CVariantMallocObject
	{
	public:
		TBaseVariant();
		TBaseVariant(const TBaseVariant&);
		virtual ~TBaseVariant();

		//��õ�ǰֵ������

		//ValueTypeֻ֧��c++�ڽ���ֵ����
		//����ǰֵ��ΪValueType���ͻ�ȡ��ֵ�������ǰֵ����ValueType���ͣ�������쳣��
		template<typename ValueType>ValueType GetNumber()const;
		template<typename ValueType>const ValueType* GetNumberPtr()const;

		//ValueTypeֻ֧��c++�ڽ���ֵ����
		//����ǰֵ���ó�һ��ValueType��ֵ
		template<typename ValueType>
		void SetNumber(ValueType Value,ostream* strm=NULL);

		//����ǰֵ��Ϊ�ַ������ͻ�ȡ��ֵ�������ǰֵ�����ַ�����������쳣��
		const char* GetString()const;
		//����ǰֵ����Ϊһ���ַ�����������\0��β��֧��\0��β�Լ۱ȱȽϵͣ���
		void SetString(const char* szValue,ostream* strm=NULL);

		//����ǰֵ���óɿձ�
		void SetEmptyTable(ostream* pStrm=NULL);

		bool IsTable()const;
		bool IsEmptyTable() const;
		//szName�Ǳ������������������й���
		//1����������������ĸ��ͷ
		//2��������ֻ������ĸ�����ֺ��»�����ɣ������ܰ����ո񡢱����š���������������š�


		//����ǰֵ��Ϊһ������ʹ�ã������ǰֵ���Ǳ����Ƚ�����һ���ձ���������һ������ΪszName��\0��β���Ŀյı��Ա��
		ImpVariant* SetMember(const char* szName,ostream* strm=NULL);

		//����ǰֵ��Ϊ�����Ͳ���ȡ��ĳ����Ա�������ǰֵ���Ǳ�������쳣������ɆT�����ڣ��t����NULL��
		ImpVariant* GetMember(const char* szName)const;

		//����ǰֵ��Ϊһ������ʹ�ã������ǰֵ���Ǳ�������쳣������ɾ�������ΪszName��\0��β���ĳ�Ա��
		//return: true��ʾɾ���ɹ���false��ʾû���ҵ���Ա
		bool DelMember(const char* szName,ostream* strm=NULL);

		bool EnumMember(const void* pContext,bool(*)(const void* pContext,const char* szKey,ImpVariant& Member))const;

	protected:
		char* IntGetString();
		void IntFreeString();

		template<typename ValueType>
		ValueType IntGetNumber() const;

		template<typename ValueType>
		const ValueType* IntGetNumberPtr() const;

	private:
		bool IsType(EVariantType eType) const;
		void SetType(EVariantType eType);
		
	protected:
		void*			m_pData;
		char			m_szNumBuffer[sizeof(uint64)];
		bool			m_bDataNotInit;
	};

}
