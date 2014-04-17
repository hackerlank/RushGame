#pragma once

#include "CPos.h"
#include "ExpHelper.h"
#include "CFindPathMallocObject.h"

#pragma pack(push,1)

namespace sqr
{

	class CLinkRegionMgr;

	struct CBaseRegionNode
		:public CFindPathMallocObject
	{
		uint32 m_nIndex;
		uint32 m_nFatherRegionId;
	};

	struct CLinkInfo
		:public CFindPathMallocObject
	{
		uint32					m_nDestRegionId;
		float					m_fFare;
	};

	class CLinkRegionNode
		:public CFindPathMallocObject
	{
		friend class CLinkRegionMgr;
	public:
		CLinkRegionNode();

		typedef	 uint8		Num_t;
		~CLinkRegionNode(void);

		CPos GetKeyPos();
		inline uint8 GetLinkIndex(uint32 nDestIndex)
		{
			uint8 i = 0;
			for ( ; i < m_nLinkNum; ++i)
			{
				if (m_pLinkList[i].m_nDestRegionId == nDestIndex)
				{
					break;
				}
			}
			Ast(i < m_nLinkNum);
			return i;
		}
	private:
		uint16				m_nKeyPosX;
		uint16				m_nKeyPosY;
		uint32				m_nFatherRegionId;		//����ͨ����
		Num_t				m_nChildNum;
		uint32				m_nChildStartId;

		Num_t				m_nLinkNum;				//ͬ����ͨ�����������ͨ������������ͨ�������
		CLinkInfo*			m_pLinkList;			//��������ͨ������Ϣ�б�
	};
}


#pragma pack(pop)
