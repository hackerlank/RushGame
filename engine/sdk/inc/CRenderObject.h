#pragma once
//#include "TPart.h"
#include "CDir.h"
#include "CRenderSpace.h"
#include "CLapseDeleteQueue.h"
#include "CModel.h"
#include "CRenderDialog.h"
#include "IRenderTime.h"
#include "CEffectPlayer.h"

namespace sqr
{
class CCamera;
class  IEffectGroup;
class  IEffectUnitHandler;

enum RES_TYPE
{
	RFT_ARP = MAKEFOURCC( 'A', 'R', 'P', 0 ),		//ģ�������ļ�
	RFT_ARA = MAKEFOURCC( 'A', 'R', 'A', 0 ),		//���������ļ�
	RFT_ARE = MAKEFOURCC( 'A', 'R', 'E', 0 ),		//��Ч�ļ�
	RFT_AGP = MAKEFOURCC( 'A', 'G', 'P', 0 ),		//�������ļ�
	RFT_SKE = MAKEFOURCC( 'S', 'K', 'E', 0 ),		//�Ǽ��ļ�
};

enum EffectCheckResult
{
	eECR_NoExist  = 0,
	eECR_Normal   = 1,
	eECR_BoneFollow = 2,
	eECR_FrameSynch = 4,
	eECR_NormalSynch = 5,
	eECR_BoneSynch = 6,
};

struct ResCreateInfo : public CSceneMallocObject
{
	ResCreateInfo()
	{
		pRes = NULL;
		eResType = 0;
	}
	GraphicRes*		pRes;
	uint32					eResType;		//��Դ����:ARA,ARP,ARS,ARE,ARAS		(����)
	SString					szFileName;
	SString					szClassName;	//��Դ����							(����)
	SString					szParam;		//���Ͳ���
};

class CRenderSystem;
class CRenderScene;
class CEffectPlayer;
struct WaterFx : public CSceneMallocObject
{
	CRenderScene*	pScene;
	uint32			uPreTime;
	uint16			uPrePosX;
	uint16			uPrePosY;
};

struct AniParam : public CSceneMallocObject
{
	//���ŵĶ���
	SString							m_PreAction;
	//���ŵĶ�����ʼ֡
	int16							m_nFrameStart;
	//���ŵĶ�������֡
	int16							m_nFrameEnd;
};

class CRenderObject
			:public CRenderSpace
			,public CModel
			,public CEffectPlayer
{
private:
	friend class CRenderScene;
	friend class CRenderSystem;
	friend class CMiniScene;
	friend class CTestSuiteCoreObjectClient;
	friend class CObjectDialog;
protected:

protected:
	bool			m_IsVailedShow;
	uint32			m_uCurTime;
	float			m_EffectVelocity;

	//����Ⱦ����
	IRenderTime*	m_pTime;

	ERenderObjStyle m_RenderStyle;
	PortraitInfo*	m_pPortraitInfo;	///< ͷ����Ϣ
	SString			m_PortraitClassName;

	//����
	uint8			m_uCurDirection;

	//ģ�ͳ�ʼ����
	CMatrix			m_FixMat;
	//float			m_fSize;
	
	//��ǰ֡������ʱ��
	uint64			m_uFrameTime;
	//��һ֡��ʱ�̣��������㵱ǰ֡������ʱ��
	uint64			m_uLastFrameTime;
	//�������շŴ���
	float			m_fFinalScale;
	//��������ʱ��
	uint64			m_uFinalTime;
	//������ɱ�־
	bool			m_bScaleIsFinal;
	bool			m_bCanBeSelect;

	//͸���仯
	uint8			m_uAlphaDes;
	uint8			m_uAlpha;
	uint8			m_rDiffuse;
	uint8			m_gDiffuse;
	uint8			m_bDiffuse;

	bool			m_IsAsynLoadPiece;
	
	//�����������
	typedef SMap< int32, AniParam >		PartAniMap;
	PartAniMap							m_PartAni;	
	int32								m_ActivePart;

	//����
	uint8								m_uDesDirection;

	SString								m_AniFileName;
	

	uint8								m_rAmbient;
	uint8								m_gAmbient;
	uint8								m_bAmbient;



	///�ر��������
	int8								m_nMaterialIndex;

	//����ˮ��Ч
	WaterFx*							m_pWaterFx;

	//������������
	SList<ResCreateInfo>				m_ListResInCreate;
	SMap<WORD, bool>					m_FootStepFrameList;
	WORD								m_uLastKeyFrame;
	//�Ƿ��ܵ��θ߶�Ӱ��
	bool								m_bTerrainheight;

	void				IntAddPiece( ResCreateInfo& Info );
	void				CreateBaseEffect( ResCreateInfo& Info );
	void				_Render( void );

	bool				_RegisterEffect( EffectNode* pEfxNode );
	bool				_LoadedEffect( EffectNode* pEfxNode );
	void				_ClearEffect(int32 nPart);

	void				_DestoryRenderChild(CRenderSpaceNode* pObj);
	void				_UpdateAttachChild(CRenderSpaceNode* pChild);
	CRenderObject(IRenderTime* pTime = NULL,ILogicHandler* pHandler = NULL);
	virtual ~CRenderObject(void);
		
	uint32				m_LapseTime;
public:
	static const DWORD  st_RenderObjectType;
	DWORD				GetNodeType()	{ return CRenderObject::st_RenderObjectType; };
	//��ʱ����
	void				SetLapseTime( uint32 tLapse )	{ m_LapseTime = tLapse; }
	uint32				GetLapseTime(void)				{ return m_LapseTime;	}

	void				SetCanBeSelect(bool isSel)		{ m_bCanBeSelect = isSel; }
	bool				GetCanBeSelect(void)			{ return m_bCanBeSelect; }

	void				Release();

	//������Ⱦ����, ������ͬ��ResOnCreate�ṹ
	bool				AddPiece( RES_TYPE eResType, const char* szFileName, const char* szClassName, const char* szParam );
	void				SetAsynLoadPieceSwitch(bool bEnable);
	void				SetPortraitPiece(const char* className);
	PortraitInfo*		GetPortraitInfo() const;


	//void				AbsolutecalculateUpBody( int32 boneid );
	
	//�Ƿ���Ч
	bool				IsValid();

	//�õ�ĳ�������ĳ���
	int32				GetAniMaxFrame( const char* Action );
	//�õ�ĳ�������ĶԵ��ٶ�
	float				GetGroundSpeed( const char* Action );

	// �õ�ĳ������ĳ���ؼ�֡��λ��
	int32				GetFrameCountToFrameStr( const char* Action, const char* szFrameStr, int nFrameStart, int nFrameEnd, int nStr );

	//ָ����ʼ֡����
	bool				DoAni( const char* szAniFileName,int32 nPartId,bool bLoop,uint32 uDelayTime,int16 nStartFrame,int16 nEndFrame , float AniSpeed = -1.0f  );
	bool				DoAni( const char* szAniFileName, bool bLoop, int16 nStartFrame , float AniSpeed = -1.0f  );
	bool				DoAni( const char* szAniFileName,int32 nPartId,bool bLoop , float AniSpeed = -1.0f );
	bool				DoAni( const char* szAniFileName,bool bLoop , float AniSpeed = -1.0f );
	
	bool				DoDefaultAni(int32 nPartId, uint32 uDelayTime, float AniSpeed = -1.0f );
	bool				DoAniFromPart( int32 nPartId, uint32 uDelayTime = 200 );
	void				StopAni( int32 nParId );
	void				StopAni( int32 nParId, int32 DelayTime );

	//
	void				SetTerrainInfluence( bool bValue );
	//ɾ����ǰ�����йǼ�
	void				Delframework();
	//ɾ��ģ����������Piece
	void				RemoveAllPiece(void);

	//�Ƴ���Ⱦ��������岿��
	bool				RemovePiece( const char* szClassName );
	//��ʾ�����ز����е�ĳ��piece���뻺��
	void				ShowPiece(const char* szPieceName,bool bShow);

	//������Ⱦ����ķ���
	void	SetDirection( const CDir& Dir );
	
	//������Ⱦ����������ڸ��ӵķ���
	void	SetNormal( const CVector3f& Normal );
	//�õ���Ⱦ�����ģ�������������������
	const CVector3f		GetCenterPosition();
	//������Ⱦ����������õ�������������
	const CVector3f		GetSkeletalPositionByName(const char* szSkeletalName);
	//�õ���Ⱦ����Ķ���ԭ�����������
	void	GetRootPosition(CVector3f* pVec);
	//�õ���Ⱦ����Ķ���ԭ���ƽ������
	CFPos	Get2DPosition();


	//�õ�����
	const CDir&	GetDirection();
	void GetDirection(CDir& dir);

	//������Ⱦ�Գ�ʼģ�ͳߴ�
	void		SetSize( float fSize );
	//�õ���Ⱦ����ǰ�Ŵ���
	float		GetScale();
	//�õ��������շŴ���
	float		GetFinalScale();
	//������Ⱦ����Ŵ���
	void		SetScale( float fScale );
	//���ý������շŴ���
	void		SetFinalScale( float fScale, uint64 uFinalTime );
	//������µ�ǰ�Ŵ���
	void		UpdateScale( uint64 uCurTime );

	////�õ���ǰ����
	//const CMatrix& GetMatrix();

	//�õ���Ⱦ����
	//CRenderable*	GetRenderObj();

	float GetCurFrameNum();

	virtual void	GetAttachMatrix( CMatrix& matLink, uint32 uChildLinkID );
	//virtual CVector3f GetObjectPosition();

	//�õ���Ⱦ����߶�
	float				GetObjHeight();
	////�õ���Ⱦ��������
	IntersectFlag		IntersectRay( const CVector3f& rayPos, const CVector3f& rayDir );

	//�õ�͸����
	uint8				GetAlphaValue();
	//����͸����
	void				SetAlphaValue( uint8 uAlpha );
	void				SetAlphaValueEx( uint8 uAlpha );
	//���ö���ɫ
	void				SetDiffuse( CColor uColor );
	//���û���ɫ
	void				SetAmbient( CColor uColor );
	//��ȡ����ɫ
	CColor				GetDiffuse(void);
	//��ȡ����ɫ
	CColor				GetAmbient(void);
	//����ˮ��Ч
	void				MakeWaterFx( CRenderScene* pRenderScene );

	//��Ⱦǰ����
	void				OnPreRender( uint32 uCurTime,CCamera* pCamera );
	//��ȡ�Ƿ���Ⱦ
	bool				GetWillRender();
	
	bool				UpdateTime(uint32 uCurTime);
	//��Ⱦ����
	void				Render(void);
	//��Ⱦ��ѡ״̬
	void				RenderSel( uint32 uCurTime, RenderObjStyle* pRS );

	void Activate();

	void				AddChild( CRenderSpaceNode* pChild, eLinkType linkType, const char* szAttachName );
	void				DelChild( CRenderSpaceNode* pChild );
	EffectCheckResult	CheckEffect(const char* szFileName,const char* szParam);
	
	void				SetRenderStyle( ERenderObjStyle Style );
	ERenderObjStyle		GetRenderStyle() const;
	void				HoldAniStill(int32 Partid);

	CRenderDialog*		CreateRenderDialog(eLinkType linkType, const char* szAttachName);
	CRenderObject*		CreateRenderObject(eLinkType linkType, const char* szAttachName);
	void				DestroyRenderDialog(CRenderDialog* pWnd);
	void				DestroyRenderObject(CRenderObject* pRenderObj);

	//������������Ӧ���� CRenderObject���� ��취����
	void				SetTerrainMaterialIndex(const int8 index);
	int8				GetTerrainMaterialIndex() const;

	void				ReCallFootStep(const WORD uCurKeyFrame);
	IRenderTime*		GetRenderTime(){return m_pTime;}

	int32				AddSkeletal( const char* name );

	void				SetVisible(bool isVisible);
public:
	bool				IsValidEx(CAnimate* pAni);
	void				_UpdateAniGrp(void);
	CAxisAlignedBox*	GetOrgAABB(void);
	CMatrix&			GetCenterMatrix(void);
	void				Update(void);
};


}

