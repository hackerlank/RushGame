#pragma once


#include "FindPathDefs.h"
#include "CFindPathMallocObject.h"


namespace sqr
{

	class CNodeState;

	class CMapInfoManager
		:public CFindPathMallocObject
	{
	public:
		CMapInfoManager(uint32 uWidthInRegion,uint32 uHeightInRegion);
		~CMapInfoManager();

		inline uint32 GetQuarWidth();
		inline uint32 GetQuarHeight();
		inline uint32 GetWidth();
		inline uint32 GetHeight();
		inline uint32 IndexError(uint32 x, uint32 y);
		uint32 GetIndex(uint32 x, uint32 y);
		inline bool CheckXYInRegion(int32 x, int32 y);

	private:
		uint32 m_uWidth,m_uHeight;           // ��ͼӵ�еĳ��Ϳ�(Grid����)
		uint32 m_uQuarterWidth;              // ��ͼӵ�еĳ��Ϳ�(1/4����Grid����)
		uint32 m_uQuarterHeight;
	};

}
