#pragma once
#include <deque>

namespace sqr
{

	template<typename ObjType, template<typename>class TAlloc >
	class TObjectBank
	{
	public:
		TObjectBank(uint32 uSize=64);
		TObjectBank(uint32 uSize,bool bPrepare);
		~TObjectBank();

		void Push(ObjType*);
		ObjType* Pop();

		//���������Ѿ��գ���newһ������.
		//����������޲����Ĺ��캯��

		//��Ҫ��ͼ�����������֧�ֶ��ֲ������캯���Ĺ��ܣ���Ϊ��������������������ζ��ʹ��ObjectBank����������Ч�ʡ�
		//ʹ�ö�����Ĺ��캯��˵����������޷�ͨ�������������ݳ�ʼ�����
		ObjType* PopOrNew();
	private:
		typedef TAlloc<ObjType*> DeqAlloc_t;
		deque<ObjType*,DeqAlloc_t >	m_deqObj;

		uint32		m_uSize;
	};
}

