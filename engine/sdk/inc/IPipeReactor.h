#pragma once

/*

��ͬʱ����������
��������������ڲ�ͬ��Reactor֮��ת��
��������������ж�
����Ҫ�յ����ӳɹ���ʧ�ܵ�֪ͨ

��Reactor֮����Դ���Pipe
��Disconnect�Ĵ�����PipeReadBuffer����ղ���ͬһ������
*/
#include "PipeTypes.h"
#include "IPipe.h"

namespace sqr
{
	class IPipeReactor
		:public virtual CDynamicObject
	{
	public:
		virtual IMulticast* CreateMulticast()=0;

		virtual void DestroyMulticast(IMulticast*)=0;

		virtual IPipe* CreateIPipe(IPipeHandler* pHandler,
			unsigned int uSendBufferSize,
			unsigned int uRecvBufferSize,
			uint8 uFlag)=0;

		virtual size_t GetPipeNum()const=0;

		virtual void CancelBlock()=0;

		virtual uint32 GetEvent(unsigned uWaitTime=0) = 0;

		virtual bool Release()=0;

		virtual ~IPipeReactor(){}
	};

}

