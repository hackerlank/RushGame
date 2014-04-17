#pragma once

namespace sqr
{
	class CCoreServerResult
	{
	public:
		inline static void* operator new(size_t stSize);
		inline static void operator delete(void* pMem){}

		//����result����
		inline void Add();

		inline virtual ~CCoreServerResult(){}

		inline void* AllocMem(size_t stSize);
		inline char* CloneString(const char* szStr);
		inline void* CloneData(const void* pData, size_t stSize);
	};
}
