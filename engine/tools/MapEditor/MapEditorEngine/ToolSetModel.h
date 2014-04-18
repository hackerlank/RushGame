#pragma once
#include "TRefPointer.h"
#include "CEventOwner.h"
#include "ExsertStructDB.h"
#include "TSingleton.h"

namespace sqr
{
	class ITexture;
	struct RenderState;
	struct MoldeSaveProperty;
	class CTileEditorModel;
	SmartPointer(CTObject);

	class CToolSetModel: public Singleton<CToolSetModel>
	{
	public:

		///EMT_XWAYǰ���� EMT_ZWAY���Ҿ���
		enum EMirrorType		{ EMT_NONE = 0,   EMT_XWAY,    EMT_ZWAY };
		enum ECopyType			{ ECT_OBJECT = 0, ECT_TERRAIN, ECT_ALL,	   ECT_GENERATE };///ECT_ALL: copy terrain and object
		enum EGenType			{ EGT_SINGLE = 0, EGT_DOUBLE,  EGT_VEGETATION };
		enum ESelectType		{ SCL_OBJECT = 0, SCL_EFFECT,  SCL_AUDIO, SCL_LIGHT };///ѡ�����������: ���壬��Ч����Ч,light

	private:

		HWND m_hResSetView;
		HWND m_hLoginSetView;

		vector< CTObjectPtr> m_vecLockedObjects;
		vector< CTObjectPtr> m_vecShowHideObjects;
		vector< CTObjectPtr> m_vecActiveObjects;

		void Place();

		EMirrorType m_eMirrorType;

		ECopyType			m_eCopyType;
		EGenType			m_eGenType;
		ESelectType			m_eSelectTpye;
		EEdit_Scale_Type	m_eScaleType;

		vector<int> m_vecGridIndicesToCopy;

		CVector3f m_vMouseStart,m_vMouseEnd;

		CVector3f m_vGroupCenter;

		bool  m_bSelecting;
		bool  m_bPreSelecting;

		DWORD m_dwWidth;
		DWORD m_dwDepth;

		DWORD m_dwWidthToCopy;
		DWORD m_dwDepthToCopy;


		DWORD m_SetLogicLong;
		DWORD m_SetLogicShort;
		DWORD m_SetLogicType;
		bool  m_SetShowPath;

		int m_nAnchorPoint; //%%%%% ճ���ر�ʱ�Ķ�λ�㣨1��9���ֱ����9����ͬ�Ķ�λ�㣩

		ITexture *m_pAreaLockTexture;
		RenderState *m_pRS;
		
	protected:
		void ShiftKeyDownEvent(const MSG_KEY_INF& key);
		void ControlKeyDownEvent(const MSG_KEY_INF& key);
		void NormalKeyDownEvent(const MSG_KEY_INF& key);

	public:
		bool m_bAlwaysSelectthisObject;

		CToolSetModel();
		~CToolSetModel();

		void OnDelete();
		void OnDeleteSelectModels();
		void OnDeleteModelByIndex(const size_t index);

		void SetGenType(const int nGenType);
		void SetSceleType(const EEdit_Scale_Type eScaleType);

		//���ڵ���
		void CreateActiveObject(BYTE bResGroupMark, bool bVisibleByConfigure, bool bShadow, bool bTranslucent, string strItemName, vector<string> & veCEditModelFileNames, bool bSubUnit, sqr::ECenter_Type eCenterType, float fLift, float fShiftX, float fShiftZ, float fLiftEx, float fShiftXEx, float fShiftZEx, float fRotation = 0.0f, int nRotationCnt = 0, float fPitch = 0.0f, int nPitchCnt = 0, float fYaw = 0.0f, int nYawCnt = 0, float xScale = 1.0f, float yScale = 1.0f, float zScale = 1.0f,
								CVector3f vGroupOffset = CVector3f(0.0f,0.0f,0.0f), bool bCameraOriented = false, bool bEffectBase = false, bool bAudio = false, bool bLogin = false);
		//����ģ��
		void CreateTitleActiveObject(vector<string> & veCTileEditorModelFileNames, vector< vector<sqr::SBlockInfo> > & vvBlock, vector< vector<sqr::SLogicHeightInfo> > & vvHeight, bool bSubUnit, sqr::ECenter_Type eBlockType, float fLift, float fShiftX, float fShiftZ);
		CTileEditorModel *m_pTitleModel;
		void RenderPreViewModel();

		void SetActiveObjects(vector< CTObjectPtr> & v);

		vector< CTObjectPtr> & GetActiveObjects();
		vector< CTObjectPtr> & GetLockObjects();
		CTObject * GetLockObjectHot();

		CTObject * GetLastActiveObject();

		CTObject * GetLastLockedObject();

		void SetLastLockedObject(CTObject * p);

		int GetActiveObjectsCount();

		void OnEvent(const CMsg& msg);
		void Render();
		void Initialize(HWND hResSetView, HWND hLoginSetView);

		void SetMirrorType(const int nMirrorType);
		void SetCopyType(const int nCopyType);
		void SetSelectSceneType(const int nType);

		CTObject* PlaceRebirthModel(const int nGridIndexHot, const int nRenderStyle, float fRotation, int nRotationCnt, float fPitch, int nPitchCnt, float fYaw, int nYawCnt, float scaleX, float scaleY, float scaleZ, const CVector3f& pos);
		bool Rebirth(int nGridIndexHot, float fRotation, int nRotationCnt, float fPitch, int nPitchCnt, float fYaw, int nYawCnt, float scaleX, float scaleY, float scaleZ, const CVector3f& pos, int nAniStart, float fPlus, sqr::EAUTO_GENERATION_TYPE eAutoGenerationType, int nRenderStyle = 0, int nAtten = 0, bool bVisible = false,bool bShadow=true,bool bTrans=true,bool bZTest=false,bool bSelfZTest=true,bool bExport=true);
		bool Rebirth(int nGridIndexHot, BYTE bTransState, int nMirrorType, int nAffectRatio, float fRotation, int nRotationCnt, float fPitch, int nPitchCnt, float fYaw, int nYawCnt, float scaleX, float scaleY, float scaleZ, int nAniStart, float fPlus, sqr::EAUTO_GENERATION_TYPE eAutoGenerationType, int nRenderStyle = 0, int nAtten = 0, bool bVisibleByConfigure = false,bool bShadow=true,bool bTrans=true, string strCurName = "");
		bool Rebirth(const MoldeSaveProperty* pModelSavePro, int nAffectRatio, const string strCurName = "");

		vector<int> & GetSelectedGridIndices();

		void AddActiveObject(CTObject * p);
		void RemoveActiveObject(CTObject * p);
		void ReMoveLastObject();

		void OnUseNextRenderStyle();

		void OnSwitchShadow();
		void OnObjectsShadow(const bool bOn);

		void CalculateGroupCenter();//%%%%%
		void PlaceActiveObject();
		void MoveHotObject();

		void ReMoveObjects(int nWay);
		void SwitchReceiveShadow();

		void SetAllObjectReceiveShadow(const bool bReceiveShadow);

		void CopyTerrainAndObject();
		void CopyTerrain();
		void CopySceneObject();
		void PastateTerrain();
		void SelectSceneAllTheSameObjects();
		void RevertSelectObject();
		void PlayAniOnOff();
		void SelectGridNextObject();
		void ChangeObjectSize(const int index);
		void MoveSelectObject(const int index);
		void RotateSelectObject(const int index);
		void ApeakRotateObject();
		void PickObject(bool bRotate);
		void ChangeObjectAttention(const int index);
		void MirrorSelectObject();
		void HideAndShowObject(bool bHide);
		void HideAndShowAllObjects(bool bHide);
		void HideModelVertexColor();
		void UpdateSpotTargetPos(const int index);

		void SetLightEffectterrain();
		void SetLightUnEffectterrain();

		void StopModelMusic();

		void SetNextAnimation( const TCHAR* szAniFileName, bool bLoop, int32 DelayTime, float AniSpeed );
		void SetAnimationFrame(const string& str, const int8 nFrame);
		int16 GetAnimationFrame(const string& str) const;
		vector<int16> GetAnimationFrames(const string& str) const;
		void CleanAniFrames(const string& str);
		void DeleteAniFrame(const string& str, const int nCurrKey);

		void SetIsVisibleByConfigure(const bool b);
		//ģ�ͺ�����ɫ������Ӱɫ����,����ǿϣ���決��Ĳݸ��ر��lightmap��ɫһ��, ����color = max(bakedColor, globalShadowColor);
		void SetBakingColorIsNotGreaterShadow();
		//��������������ײ��ǣ���Щ���岻��ʹ��������������ʹ���������ײ��
		void SwitchModelIsUseCameraCollison();

		void SelectAllSceneObject();
		void SetWhichLoginObjectIsLock(const string& str);
		///frist str modelname or effectname
		vector<int> GetSelectSceneObject(const string& str);
		void LockSceneObject( const string& str, const int& nGridIndex );
		void LockObjectLinkEffect(const string& effectname);
		void LockObjectLinkAnimation();

		///ɾ������������ģ��
		void DeleteSceneObjects();

		void  LockSelectObjects();

		void GetObjectIntersectedPosHeight();
		void GetHotModelInfo(string& name);

		void SwitchLinkModelEffect();

		HWND GetLoginSetView()
		{
			return m_hLoginSetView;
		}
	};
}
