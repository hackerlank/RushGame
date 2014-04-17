#pragma once

#include "PropCtrl\CBasePropCtrlImp.h"

namespace sqr
{
	class CCamera;
}

namespace sqr_tools
{
	class CEditContext;
	class CScenePropCtrlImp :public CBasePropCtrlImp
	{
	public:
		CScenePropCtrlImp(void);
		~CScenePropCtrlImp(void);

	public:
		virtual string	GetSceneName();
		virtual int		GetSceneWidth();
		virtual int		GetSceneHeight();
		virtual int		GetSceneRegion();
		//��ȡ������ȫ���������
		virtual int		GetCameraType(int& cam_type);
		virtual float	GetCameraPosX();
		virtual float	GetCameraPosY();
		virtual float	GetCameraPosZ();
		virtual float	GetCameraDirX();
		virtual float	GetCameraDirY();
		virtual float	GetCameraDirZ();
		virtual float	GetCameraTargetX();
		virtual float	GetCameraTargetY();
		virtual float	GetCameraTargetZ();
		virtual float	GetCameraFarDis();
		virtual float	GetCameraNearDis();
		//��ȡ������ȫ����Ч������
		virtual int		GetFogType();
		virtual float	GetFogBegin();
		virtual float	GetFogEnd();
		virtual float	GetFogDensity();
		virtual unsigned int	GetFogColor();
		//��ȡȫ�������(����� ������)
		virtual float	GetSunLightDirX();
		virtual float	GetSunLightDirY();
		virtual float	GetSunLightDirZ();
		virtual unsigned int	GetSunLightColor();
		virtual unsigned int	GetAmbientColor();
		virtual unsigned int	GetPlayerSunLightColor();
		virtual unsigned int	GetPlayerAmbientColor();
		//����ȫ���������
		virtual void SetCameraType(int cam_type);
		//����ȫ����Ч������
		virtual void SetFogType(int fog_type);
		virtual void SetFogBegin(float begin);
		virtual void SetFogEnd(float end);
		virtual void SetFogDensity(float destiny);
		virtual void SetFogColor(unsigned int color);
		//����ȫ�������(����� ������)
		virtual void SetSunLightDir(float dx,float dy,float dz);
		virtual void SetSunLightColor(unsigned int color);
		virtual void SetAmbientColor(unsigned int color);
		virtual void SetPlayerSunLightColor(unsigned int color);
		virtual void SetPlayerAmbientColor(unsigned int color);
	protected:
		CCamera*	Camera;
	};

//!namespace
}