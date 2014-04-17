#pragma once 
#include "ModuleDefs.h"

namespace sqr
{

	#define _LOG_USE_ENCRYPT   //�Ƿ�ʹ�ü��ܣ������ڿ����ڣ�ʵ����Ӫ��ʱ��log���Ի���ܵ�

	//errlog�ļ��ܽ�����
	class CErrLogEncrypt
	{
	public:
		CErrLogEncrypt();
		~CErrLogEncrypt();

		static CErrLogEncrypt& Inst();

		//��������ִ����м��������
		void Encode(char* buf, uint32 uLen);
		void Decode(char* buf, uint32 uLen);

		
	private:
		uint32 m_uEncodeNum;
		uint32 m_uDecodeNum;
	};

}

