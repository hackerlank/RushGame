#pragma once

namespace sqr
{

	class CCoreServerJob 
	{
	public:
		static void* operator new(size_t stSize);
		static void operator delete(void* pMem){};
		
		//�����job����job�������棬������׼��ֱ����CCoreServerJob�Ĺ��캯������������
		//���ǿ��ܳ�������������ڸ��๹����ϼ������֮�����෢���쳣����ʱ��������������
		//����һ�������ֵ��ֱ�ӻᵼ�º�������������е�ʱ������������ǻ����������Լ�������ʱ����
		void Add();

		virtual ~CCoreServerJob(){}

		static void* AllocMem(size_t stSize);
		static char* CloneString(const char* szStr);
		static char* CloneString(const std::string& szStr);
		static void* CloneData(const void* pData, size_t stSize);
	};

}
