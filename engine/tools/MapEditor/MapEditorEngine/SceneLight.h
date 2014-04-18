#pragma once
#include "CLight.h"

namespace sqr
{
	#define SCENE_LIGHT_INFO_SIZE	28

	struct ScenePointLight_Gac
	{
		float           m_fAttenuation0;
		float           m_fAttenuation1;
		float           m_fAttenuation2;
		uint16          m_fRang;
		uint8           m_rColor;
		uint8           m_gColor;
		uint8           m_bColor;
		uint8           m_rDyn_Color;
		uint8           m_gDyn_Color;
		uint8           m_bDyn_Color;
		uint8           m_Dyn_ChangeProbability;
		uint8           m_Dyn_BrightTime;
		uint8           m_Dyn_DarkTime;
		uint8           m_Dyn_ChangeTime;
	};

	struct SceneSpotLight_Gac
	{
		float				m_fAttenuation0;
		float				m_fAttenuation1;
		float				m_fAttenuation2;
		float				m_DirectionX;	 ///�۹�Ʒ���
		float				m_DirectionY;
		float				m_DirectionZ;
		float				m_TargetPosX;	///�۹�Ʒ����յ�λ�ã�Ҳ��Ŀ��λ��
		float				m_TargetPosY;
		float				m_TargetPosZ;
		float				m_fTheta;		 ///ָ����Բ׶�Ƕȣ���λ�ǻ���
		float				m_fPhi;			 ///ָ����Բ׶�Ƕȣ���λ�ǻ���
		uint16				m_fRang;
		uint16				m_fFallOff;      ///����Բ׶����Բ׶֮���ǿ��˥��
		uint8				m_rColor;
		uint8				m_gColor;
		uint8				m_bColor;
	};

	struct SceneLight_OLD
	{
		SceneLight_OLD();
		bool				m_bIAmbScenePointLight;
		bool				m_ShowMe;
		TCHAR				m_Info[SCENE_LIGHT_INFO_SIZE];
		float				m_Attenuation0;
		float				m_Attenuation1;
		float				m_Attenuation2;
		float				m_Rang;
		uint32				m_Color;

		bool                m_bDynamic;
		uint32				m_Dyn_Color;
		uint8				m_Dyn_BrightTime;
		uint8				m_Dyn_DarkTime;
		uint8				m_Dyn_ChangeTime;
		uint8				m_Dyn_ChangeProbability;

		uint8				m_SHOW_CurState;
		DWORD               m_SHOW_LastRefreshTime;
		bool                m_SHOW_FirstTime;
	};

	// �������Դ���ݽṹ��������̬������
	struct SceneLight
	{
		SceneLight();
		bool				m_bIAmbScenePointLight;
		bool				m_ShowMe;
		TCHAR				m_Info[SCENE_LIGHT_INFO_SIZE];
		float				m_Attenuation0;
		float				m_Attenuation1;
		float				m_Attenuation2;
		float				m_Rang;
		uint32				m_Color;

		bool                m_bDynamic;
		uint32				m_Dyn_Color;
		uint8				m_Dyn_BrightTime;
		uint8				m_Dyn_DarkTime;
		uint8				m_Dyn_ChangeTime;
		uint8				m_Dyn_ChangeProbability;

		uint8				m_SHOW_CurState;
		DWORD               m_SHOW_LastRefreshTime;
		bool                m_SHOW_FirstTime;
		bool				m_bEffectTerrain;///�Ƿ�Ӱ��ر�

		SceneLight& operator= ( const ScenePointLight_Gac& gacLight );
	};

	// �����۹�����ݽṹ
	// �����˶�֮�󣬲���Ҫ��̬�ľ۹��
	struct SceneSpotLight
	{
		SceneSpotLight();
		bool				m_bIAmbSceneSpotLight;
		TCHAR				m_Info[SCENE_LIGHT_INFO_SIZE];
		float				m_fAttenuation0;
		float				m_fAttenuation1;
		float				m_fAttenuation2;
		float				m_fRang;
		float				m_fFallOff;      ///����Բ׶����Բ׶֮���ǿ��˥��
		float				m_fTheta;		 ///ָ����Բ׶�Ƕȣ���λ�ǻ���
		float				m_fPhi;			 ///ָ����Բ׶�Ƕȣ���λ�ǻ���
		uint32				m_Color;
		CVector3f			m_Direction;	 ///�۹�Ʒ���
		CVector3f			m_TargetPos;	 ///�۹�Ʒ����յ�λ�ã�Ҳ��Ŀ��λ��
		bool				m_bEffectTerrain;///�Ƿ�Ӱ��ر�

		SceneSpotLight& operator= ( const SceneSpotLight_Gac& gacLight );
	};

	uint32 RefreshDynamicSceneLight( SceneLight &DSL, DWORD nRefreshTime);
	const CLight& SceneLightToCLight(CLight& light, const SceneLight& point);
	const CLight& SceneLightToCLight(CLight& light, const SceneSpotLight& spot);
}