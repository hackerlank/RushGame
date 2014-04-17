#pragma once
#include "CModel.h"
#include "CEditorClassPre.h"
#include "float.h"
#include "CEditSubject.h"
#include "CRenderModel.h"

namespace sqr_tools
{
	struct PickSkeletalParam
	{
		PickSkeletalParam(){};
		PickSkeletalParam( int x, int y, int w, int h )
		{
			m_SkeletalID = 0;
			m_Z = FLT_MAX;
			m_CurPos.x = x;
			m_CurPos.y = y;
			m_Size.x = w;
			m_Size.y = h;
		};
		BYTE          m_SkeletalID;
		float         m_Z;
		POINT         m_CurPos;
		POINT         m_Size;
		string        m_SkeletalName;
	};

	struct PickSocketParam
	{
		PickSocketParam(){};
		PickSocketParam( int x, int y, int w, int h )
		{
			m_SkeletalID = 0;
			m_Dist = FLT_MAX;
			m_CurPos.x = x;
			m_CurPos.y = y;
			m_Size.x = w;
			m_Size.y = h;
		};
		BYTE          m_SkeletalID;
		BYTE		  m_SocketNum;
		BYTE		  m_Coordinate;
		float		  m_Dist;
		POINT         m_CurPos;
		POINT         m_Size;
	};

	/// ����mesh
	struct SkeletalPos
	{
		SkeletalPos():m_IsSelected(FALSE){};
		CVector3f	m_Scale;
		BOOL				m_IsSelected;  //�����Ƿ�ѡ��

		//sqr::VerColor3D			m_SklPos[6];		//���ɵĹ���MESH
	};

	class CEditModel 
		: public sqr::CRenderModel
		, public CEditSubject 
	{
	public:
		static const DWORD  st_EditModelType;
		CEditModel(ILogicHandler* pHandler = NULL);
		~CEditModel();

		bool BinSave( const string& str );
		void SetSelectedPiece( const string& str );
		void AddMeshPieceEx( CEditPiece* pEpc, uint32 RSNum = 0 );

		void GetEditPieceGroup(CEditPieceGroup* pPieceGroup);
		void DeletePiece(CEditPiece* pEpc);

		IEffect*						GetFirstEffect();
		int								GetCurMaxFrame(void);
		const GMap<WORD,sqr::IDNAME>*	GetCurFrameString();
		int								GetCurSelSkelatal();	/// �õ���ǰ��ѡ����ID
		int								GetSkeletalIdBySocketName( sqr::IDNAME SocketName, int* SocketNum = NULL ); /// ͨ��SOCKET���ֵõ�����ID
		
		/// �����ؼ�֡�ַ���
		bool ExportFrameKey( const string& szFileName );
		/// ����ؼ�֡�ַ���
		void ImportFrameKey( const string& szFileName );
		/// ����ؼ����ַ���
		void InsertFrameString( const string& str, int FrameNum );

		/// ��ѡ����
		void PickVertex( PickSkeletalParam& Pick );
		/// ��ѡ����
		int PickSkeletal( PickSkeletalParam& Pick );
		/// ��ѡSOCKET
		int PickSocket( PickSocketParam& Pick );
		///// �༭SOCKET
		//int EditSocket( PickSocketParam& Pick, VIEWFLAG ViewFlag );

		int SaveJointVerNormal( const string& szFileName );
		int LoadJointVerNormal( const string& szFileName );
		float	GetCurFrameRatio(void);

		//bool	SetNextAnimation( IDNAME AniName, FramePair FmPair, bool bLoop = true, int32 DelayTime = 200 /*����*/, float AniSpeed = -1.0f );
		const	AniResult RenderFrame( const CMatrix& matWorld, float uCurFrame, RenderObjStyle* pRORS = NULL ) ;
		float	GetCurFrameNum();
		PortraitInfo*	 GetPortraitInfo(void) { return &m_PortraitInfo; }

		void			RenderBone(void);
		void			RenderBox(CBaseAABB* pCurBox = NULL,bool bStaticBox = false);
		void			RefreshBox(void);
		void			DelBox(CBaseAABB* pBox);
		void			AddBox(uint16 ID,CBaseAABB* pBox);
		void		    SetSkesMask(const SKELETAL_MASK& mask);

		int				CalSkeletalSize(int nDelta);
		int				CalAniSize(const CEditSkeletal& Skeletal, CEditAnimate& Animate,float Size);
		
		RenderPieceMap& GetRenderPieces(void) { return m_MeshsPiece; }
		void			SetForceRenderStyle(CEditRenderStyle* pStyle);
		bool			IsFindKeyAniName(string& aniname);

		CEditPiece*		GetSelectPiece(){return m_pSelectPiece;}

		CEditPieceGroup* GeneratePieceGroupForSave();
		void			RemoveFrmParent(void);
		virtual void	ClearModelRender(void);

		void			ClearMeshPiece();

		CVector3f		GetModelScale() const;
		CVector3f		GetModelRotate() const;
		//add by guo
		virtual void	SetFrame(float uCurFrame);
		virtual float   GetCurFrame(void);
		virtual std::string GetAnimResName(void);
		virtual void SetAnimResName(std::string name);

		bool			AddPieceGroup( const char* szFileName );
		bool			AddAnimationGroup( const char* szFileName );
		size_t			GetChildCount();
		string			GetModelName();
		string			GetModelName(const size_t index);
		string			GetAnimationGroupName();
		string			GetAnimationGroupName(const size_t index);
		string			GetEffectName(string& effectFileName);
		string			GetEffectName(const size_t index, string& effectFileName);

		void			SetEditAnimateGroup( CAnimationGroup* AnimateGroup );

		bool			IsAnimate(void);

	protected:
		virtual void	SetPortraitInfo( const PortraitInfo& pi );
		//virtual void	pieceSelected( const CEditPiece* piece );
		
		virtual int		RenderMesh(  RenderObjStyle* pRORS );

	protected:
		bool DecideAllIsSet();
		PortraitInfo		m_PortraitInfo;
		CEditPiece*			m_pSelectPiece;
		float				m_Size;
		SkeletalPos*		m_SkeletalPos;
		CEditRenderStyle*	m_pForceStyle;
		GVector<string>		m_KeyFilter;

		//////////////////////////////////////////////////////////////////////////
		//���������ǵر����õ��ģ�����ʱ����������к��ʵĵط��ٴ���
	protected:
		string				m_strModelName;
		string				m_strAnimationName;
		bool				m_bPlayAnimation;		///on / off animation		
		bool				m_bShadow;				///�Ƿ���Ӱ��	
		bool				m_bReceiveShadow;		///д�Ƿ������Ӱ��Ϣ		
		bool				m_bVisibleByConfigure;	//ģ�Ϳɼ��Ƿ���������ֵ
		bool				m_bUseCameraCollision;	///��������������ײ��ǣ���Щ���岻��ʹ��������������ʹ���������ײ��

	public:
		BYTE GetRenderStyleIndex();

		void SetIsPlayAnimation(const bool b)
		{
			m_bPlayAnimation = b;
		}
		bool GetIsPlayAnimation() const
		{
			return m_bPlayAnimation;
		}

		void SetIsCastShadow(const bool b)
		{
			m_bShadow = b;
		}
		bool GetIsCastShadow() const
		{
			return m_bShadow;
		}

		void SetIsReceiveShadow(const bool b)
		{
			m_bReceiveShadow = b;
		}
		bool GetIsReceiveShadow() const
		{
			return m_bReceiveShadow;
		}

		void SetIsVisibleByConfigure(const bool b)
		{
			m_bVisibleByConfigure = b;
		}
		bool GetIsVisibleByConfigure() const
		{
			return m_bVisibleByConfigure;
		}

		void SetIsUseCameraCollision(const bool b)
		{
			m_bUseCameraCollision = b;
		}
		bool GetIsUseCameraCollision() const
		{
			return m_bUseCameraCollision;
		}
		//////////////////////////////////////////////////////////////////////////
	};
}
