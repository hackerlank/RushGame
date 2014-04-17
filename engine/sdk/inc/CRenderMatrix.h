#pragma once
#include "CMatrixUnit.h"
#include "GraphicBase.h"
#include "CAnimationGroup.h"
#include "CSkeletalMask.h"


namespace sqr
{
	typedef GVector<CMatrixUnit>	CMatrixUnitVector;

	enum EAniPlayMode
	{
		APM_PLAY_ONCE = 0,
		APM_LOOP_PLAY = 1,
		APM_PLAY_BACK = 2,
		APM_KEEP_PLAY = 3,
	};

	enum EAniPlayState
	{
		APS_FADEIN	= 0,
		APS_NORMAL  = 1,
		APS_FADEOUT	= 2,
	};

	class CAnimateSeparation : public CGraphicMallocObject	// �������� �ṹ
	{
	public:
		// BYTE	m_RootSkeletalId;	// ��ʼ����ID
		EAniPlayState	m_State;			// ����״̬	 0 - FadeIn �ں�   1 - ��������   2 - FadeOut �ں�
		BYTE	m_SkeletalID; 
		IDNAME	m_AnimationName;
		WORD	m_CurAnimate;		// ��ǰ������ID
		float	m_CurFrame;			// ��ǰ֡
		float	m_AniEndFrame;		// ��ǰ�������һ֡(end = num )
		float	m_BlendFrame;		// �ں�֡��
		float	m_Weight;
		bool	m_nStopAni;
		float	m_AniSpeed;

		EAniPlayMode	m_PlayMode;			// ����ģʽ  0 - ѭ������  1 - ����һ��   2 - �������־ñ���
		CAnimateSeparation(BYTE ID)
			: m_nStopAni(false)
			, m_Weight(0.0f)
			, m_CurAnimate(0xFFFF)
			, m_CurFrame(0)
			, m_State(APS_FADEIN)
			, m_AniEndFrame(0)
			, m_PlayMode(APM_PLAY_ONCE)
			, m_SkeletalID(ID)
			, m_AniSpeed(1.0f)
		{}
			
		void Update( float DeltaFrame, CRenderMatrix & RenderMatrix );
	};



	//��Ⱦʱ��Ҫ�Ĺ��������б���
	//�����ʱ�䣬���ǰ���������������ָ�뻻������
	struct CRenderMatrix : public CGraphicMallocObject
	{
		CRenderMatrix();
		~CRenderMatrix();

		BYTE				m_MaxUseSkeletalID;					//������Ŀ

		bool				m_IsBlendStart;						//�Ƿ����ǰ��֡���״̬
		bool				m_IsAniBlend;						//�Ƿ���Ҫ����ǰ��֡���
		bool				m_IsRended;
		WORD				m_CurAnimate;						//��ǰ�ڼ���Ķ�����CAnimateGroup�е�λ��

		WORD				m_AniStartFrame;					//��һ��Ҫ���ŵĶ�������ʼ֡
		WORD				m_AniEndFrame;						//��һ��Ҫ���ŵĶ����Ľ���֡(end = num - 1)
		WORD				m_BlendFrame;						//����һ����������ǰ��Ҫ�����ںϵ�֡��
		bool				m_nStopAni;

		//CMatrixUnitPtrVector	mResBoenMatrixlist;				//�õ��Ĺ�������
		CMatrixUnitVector		m_pBoneMatrixlist;
		CMatrixUnitPtrVector	m_pMatrixUnit;					//������������
		CMatrixUnitPtrVector	m_pMatrixUnitOrg;				//�������
		CAnimateSeparationPtrVector		m_pSkeletalSepList;			
		CAnimationControllerPtrVector	m_pAniCtrlList;			//�ⲿIK
		IModelHandler*				m_pHandler;

		CMatrix				m_MatWorld;
		SKELETAL_MASK		m_SkeletalMask;	
		float				m_CurFrame;							//��ǰ֡
		//float				m_Anivelocity;						//���ŵĶ�������
		float				m_Weight;
		uint32				m_CurrentTime;
		GVector<IDNAME>		m_vecFrameString;
		GVector<IDNAME>		m_vecSepFrameString;
		AniResult			m_Result;

		void				Initialization( CSkeletalFrame* pSkeFrm );
		void				SetSkeletalMask( const SKELETAL_MASK& Mask );
		uint32				GetSkeletalCount();

		void				ClearSkeletaInfo(void);
		void				ClearAniSeparation(void);
		void				ClearAniController(void);
		void				Release();
		void				CalculateMatrix(void);//������Ⱦ����
	};
}