#pragma once

namespace sqr
{

#define IDSTACK_INVALID_INDEX -1

	/*
		ID Stack������
		���������ʹ�õ�id
	*/

	//IDContainerֻ��ʹ��vector����dequeʵ�֣�Ĭ����vector

	template<typename IDType, class IDContainer = vector<IDType> >
	class TIDStack
	{
	public:
		TIDStack();
		TIDStack(size_t stCount);
		~TIDStack();

		//��ȡһ������id
		IDType WithdrawIdleID();
		//�黹�Ѿ�ʹ�õ�id
		void RestoreUsedID(IDType ID);
		//�Ƿ�Ϊ����id
		bool IsIdleID(IDType ID);
		//�ӿ���id���Ƴ�ָ��id
		void RemoveIdleID(IDType ID);
		//�����е�id���������չ��MaxID
		void ExpandIdleID(IDType MaxID);
		//�鿴ID�Ƿ���stack�����
		bool IsStackID(IDType ID);
		//�鿴��ǰ���ж���ʣ��idle id
		size_t GetIdleIDSize();
		//�õ���ʱstack�����ID����
		IDType GetIDSize(); 
		//�ж��Ƿ��Ϊ��
		bool Empty();

	private:
		IDContainer	m_IDStack;  //
		IDContainer m_IDIndex;  //ָ����ID��stack��������
		size_t	m_CurIdlePos;

	};
}
