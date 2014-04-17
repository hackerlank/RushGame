#pragma once
#include "CRenderNode.h"
#include "CVertexHardwareBuffer.h"
#include "CSkeletalFrame.h"
#include "CVector3.h"
#include "CAxisAlignedBox.h"
#include "CRenderPiece.h"
#include "CAnimationGroup.h"
#include "CRenderMatrix.h"

namespace sqr
{
	/// �ɻ�������
	class CBaseModel : public CRenderNode
	{
	public:
		CBaseModel(ILogicHandler* pHandler = NULL);
		virtual ~CBaseModel();

		typedef GMap< GString, CRenderPiece > RenderPieceMap;
	protected:
		CVector3f					m_Scale;
		CVector3f					m_Off;
				
		CAnimationGroup*			m_pAnimationGroup;
		CSkeletalFrame*				m_pSkeFrame;
		CRenderMatrix*				m_pRenderAniInst;

		RenderPieceMap				m_MeshsPiece;
		bool						m_Amalgamation;						//��ǰ���ŵĶ����Ƿ������ںϡ�һ���ǻ��Ǽܺ󲥷ŵĵ�һ�������������ں�
		
		static const DWORD			st_ModelType;
		int32						m_CenterLinkId;
		virtual void				_UpdateMesh(SKELETAL_MASK& SkeletalMask);
		virtual void				_AddAABB(CPiece* pPiece){};
		virtual void				_UpdateSkeFrm(void){};
		virtual void				_UpdateAniGrp(void){};
	
		//�󶨲������
		float						m_ParentFrameRatio;
		
		//uint16					m_ParentFrame;
		//bool						m_IsParentAniBlend;
		//bool						m_bFrameSynch;
		//bool						m_bFrameSynchSepAni;
		float						m_Velocity;
		bool						m_bLoop;
	public:
		
		virtual	void ClearModelRender(void){};
		virtual bool SetNextAnimation( IDNAME AniName, FramePair FmPair, bool bLoop = true, int32 DelayTime = 200 /*����*/, float AniSpeed = -1.0f ) = 0;
		virtual void AddMeshPiece( CPieceGroup* pPieceGroup, const TCHAR* szClassName, uint32 RSNum );

		virtual void ClearMeshPiece();
		
		void				GetSkeletaChildNum( BYTE SkeletalID, int32& Num );				//�õ���ǰ������һ���ж����ӹ���
		int					GetSkeletalIdByName( const char * szName );
	
		CSkeletalFrame*		GetSketetalFrame();
		CAnimationGroup*	GetAnimateGroup();
		void				SetSkeletalFrame(CSkeletalFrame* SkeletalFrame);
		void				SetAnimateGroup( CAnimationGroup* AnimateGroup );
		void				DeleteMeshPiece( const TCHAR* szPieceName );
		
		bool  ModifyMesh(const TCHAR* szClassName, CVertexHardwareBuffer* pBuffer, size_t Offset, ModifyOperator fModifyOp 
			, uint PieceIndex = 0 ,size_t VertexNum = 0,CVertexElementSemantic Ves = VES_DIFFUSE, 
			CVertexElementType Vet = VET_COLOUR , uint Index = 0);

		uint32	GetID(CPieceGroup* pPieceGroup, const TCHAR* szClassName,const TCHAR* szRsIndx);//�õ�rs����
		void	SetPieceRSIndex( const TCHAR* szClassName, uint32 nRSIndex );
		bool	GetMatrix( CMatrix& Matrix, uint32 SkeletalID, uint32 SocketID );
		bool	GetLocalMatrix( CMatrix& Matrix, uint32 SkeletalID, uint32 SocketID );
		//void	OnPreRender( const AniResult& ParentAniRes, float FrameRatio );
		bool	UpdateTime(uint32 uCurTime);
		virtual void RenderImme(void){};	
	public: //		
		void				SetRenderVelocity( float Velocity , int32 id = -1 );
		bool				IsMeshsPieceEmpty();
		void				SetModelHandler( IModelHandler * pHandler );
		IModelHandler*		GetModelHandler(); 
		ERenderObjType		GetType();
		DWORD				GetNodeType();

		const CVector3f&	GetBoxScale();
		const CVector3f&	GetBoxOff();
					bool	IsValid();
					void	SetVisible( bool isVisible );

		inline		void	SetVisible( CCamera* pCamera ) { CRenderNode::SetVisible(pCamera); }
					bool	IsVaildSkeId(BYTE SkeletalID);
	};

	inline ERenderObjType		
		CBaseModel::GetType()
	{
		return ROT_MODEL;
	}

	inline DWORD CBaseModel::GetNodeType()	
	{ 
		return CBaseModel::st_ModelType; 
	};

	inline const CVector3f& CBaseModel::GetBoxScale()
	{
		return m_Scale;
	}

	inline const CVector3f& CBaseModel::GetBoxOff()
	{
		return m_Off;
	}

	inline bool  CBaseModel::IsValid()
	{
		return true;
	}

	inline CAnimationGroup*	CBaseModel::GetAnimateGroup()
	{
		return m_pAnimationGroup;
	}

	inline CSkeletalFrame* CBaseModel::GetSketetalFrame()
	{
		return m_pSkeFrame;
	}

	inline IModelHandler*	CBaseModel::GetModelHandler()
	{
		return m_pRenderAniInst->m_pHandler;
	}
}
