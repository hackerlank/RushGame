#pragma once

#include "CCameraController.h"

namespace sqr
{

//�������λ�úͷ��� ȷ��Target

enum FPS_STATE
{
	STATE_MOVE_LEFT,
	STATE_MOVE_RIGHT,
	STATE_MOVE_FORWAR,
	STATE_MOVE_BACK,
	STATE_PULL,
	STATE_PUSH,
	STATE_ROT,
	STATE_STOP
};

class CFPSCameraCtrller :public CCameraController
{
public:
	CFPSCameraCtrller(void);
	~CFPSCameraCtrller(void);

public:
	//����
	CtrlState	_DoCameraCtrl( uint32 uCurTime,CCamera* pCamera, CSceneQuery* pQuery );

protected:
	//����
	//��Ļ���ĵ� �ӿڿ��
	int m_CenterX;
	int m_CenterY;
	float m_ViewWidth;
	float m_ViewHeight;
	//��ǰֵ
	CVector3f m_Pos;
	CVector3f m_Dir;
	//ʱ��
	uint32 m_LastTime;
	//�ٶ� ÿ���ƶ��׻���ת����
	float	m_MoveSpeed;
	float	m_XRate;//���ض�
	float	m_YRate;
	//�Ƕ�
	float	m_MaxYAngle;
	float	m_MinYAngle;
	float	m_XAngle;
	float	m_YAngle;
	float	m_XPixel;
	float	m_YPixel;
	//
	bool	m_UpdateUnit;
	FPS_STATE m_State;

public:
	//������Ϊ����
	void UpdateCenterPos(int cx,int cy);
	//��ʼ���Ƕ�
	void SetXAngle(float _xAngle);
	void SetYAngle(float _yAngle);
	float GetAngleX();
	float GetAngleY();

	//ÿ���ض��ٶ� ����ת���ٶ�
	void SetMoveSpeed(float _moveSpeed);
	void SetXRate(float _xRate);
	void SetYRate(float _yRate);
	void SetXPixel(float _xPixel);
	void SetYPixel(float _yPixel);
	float GetMoveSpeed();
	float GetXRate();
	float GetYRate();
	//
	void SetPostion(float x,float y,float z);
	void SetState(FPS_STATE _state);

};

inline void CFPSCameraCtrller::UpdateCenterPos(int cx,int cy)
{
	m_CenterX=cx;
	m_CenterY=cy;
}

inline void CFPSCameraCtrller::SetPostion(float x,float y,float z)
{
	m_Pos.x=x;
	m_Pos.y=y;
	m_Pos.z=z;
}

inline void CFPSCameraCtrller::SetMoveSpeed(float _moveSpeed)
{
	m_MoveSpeed=_moveSpeed;
}

inline void CFPSCameraCtrller::SetXAngle(float _xAngle)
{
	m_XAngle=_xAngle;
}

inline void CFPSCameraCtrller::SetYAngle(float _yAngle)
{
	m_YAngle=_yAngle;
}

inline void CFPSCameraCtrller::SetXRate(float _xRate)
{
	m_XRate=_xRate;
}

inline void CFPSCameraCtrller::SetYRate(float _yRate)
{
	m_YRate=_yRate;
}

inline void CFPSCameraCtrller::SetXPixel(float _xPixel)
{
	m_XPixel=_xPixel;
}

inline void CFPSCameraCtrller::SetYPixel(float _yPixel)
{
	m_YPixel=_yPixel;
}

inline float CFPSCameraCtrller::GetAngleX()
{
	return m_XAngle;
}

inline float CFPSCameraCtrller::GetAngleY()
{
	return m_YAngle;
}

inline float CFPSCameraCtrller::GetMoveSpeed()
{
	return m_MoveSpeed;
}

inline float CFPSCameraCtrller::GetXRate()
{
	return m_XRate;
}

inline float CFPSCameraCtrller::GetYRate()
{
	return m_YRate;
}


//!namespace
}
