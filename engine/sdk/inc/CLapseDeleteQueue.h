#pragma once
#include "CRenderSpaceNode.h"
#include "CDynamicObject.h"
#include "CSceneMallocObject.h"
/*
ר�Ŵ�����ʱ��͸���ͷ����
*/

namespace sqr
{
	class CLapseDeleteQueue ;
	class CLapseDeleteNode 
		: public virtual CDynamicObject
		, public CSceneMallocObject
	{
		friend class CLapseDeleteQueue;
	public:
		CLapseDeleteNode(CRenderSpaceNode* pChild,uint32 LapseTime);
		virtual ~CLapseDeleteNode();

		//�Ƿ��͸ɾ���Ľӿ�
		//uint32	GetLapseTime(void);
		//void	SetLapseTime( uint32 tLapse );

	protected:
		CRenderSpaceNode*	m_pRenderChild;
		uint32			m_LapseTime;
		uint32			m_DeleteTime;
		float			m_LapseAlpha;
	};

	class CLapseDeleteQueue
	{
	protected:
		typedef SList<CLapseDeleteNode*> DeleteQueue;
	public:
		CLapseDeleteQueue();
		~CLapseDeleteQueue();

		void	InsertNode( CLapseDeleteNode* pNode );
		void	Update( uint32 uCurTime );
		void	Clear(void);

	protected:
		DeleteQueue		m_lDeleteQueue;
		uint32			m_CurTime;
	};
}