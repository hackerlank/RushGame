#pragma once
#include "CRenderBatch.h"
#include "CRenderOperation.h"
#include "CRenderTarget.h"
#include "CElementManager.h"
#include "CRenderStyle.h"
namespace sqr
{
	class ITexture;

	class CSkyMgr 
		: public CRenderBatch
		, public CElementManager
		//, public CElementNode
	{
	public:
		CSkyMgr(CRenderTarget* pTarget);
		~CSkyMgr();

		virtual FilterNode* Filter(FilterNode* fn)
		{
			return NULL;
		};

		virtual void Remove(const FilterNode* fn) {};
		virtual	void Refresh(CRenderFilter* newFilter) {};
		virtual void SetLightIndex(uint index) {};

		void Render();
		void SetCameraDest( const CVector3f& dest );
		void SetSkyBoxTextureName( const string& name );

		void ResetSkyBoxWnd();

	public: //��ʱ�ŵ�����
		void	SetMoveEnable( bool bMove )			{ m_bMove = bMove; };
		void	SetMoveLeftSpeed( float speed )		{ m_fBackLeftMove = speed;	};
		void	SetMoveRightSpeed( float speed )	{ m_fBackRightMove = speed;	};
		void	SetMoveTopSpeed( float speed )		{ m_fBackTopMove = speed;	};
		void	SetMoveBottomSpeed( float speed )	{ m_fBackBottomMove = speed;};

	protected:

		void SetUVOffset(const float u, const float v);
		CElementNode*	_CreateNode( URegionID ID );
		void Destroy(void) {};	

	protected:
		ITexture*				m_pSkyTexture;	
		CRenderOperation		m_RenderOP;
		RenderParam				m_Param;
		CRenderTarget*			m_pRenderTarget;
		CVector3f				m_PreDest;
		bool					first;
		float					umove, preumove, vmove, prevmove;
		void					InitSkyBox();
		
		bool				m_isReset;
		bool				m_bMove;
		float				m_fBackLeftMove;    // ����ͼ�����ƶ��ٶ�
		float				m_fBackRightMove;	// ����ͼ�����ƶ��ٶ�
		float				m_fBackTopMove;		// ����ͼ�����ƶ��ٶ�
		float				m_fBackBottomMove;  // ����ͼ�����ƶ��ٶ�
	};

	//------------------------------------------------------------------------------
	inline CElementNode*
		CSkyMgr::_CreateNode( URegionID ID )
	{
		return NULL;
	}

}