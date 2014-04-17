#pragma once

#include "CCameraController.h"

namespace sqr
{

//ͨ������Target���������λ��

enum FREECAM_STATE
{
	FREECAM_MOVE_LEFT,
	FREECAM_MOVE_RIGHT,
	FREECAM_MOVE_FORWAR,
	FREECAM_MOVE_BACK,
	FREECAM_PULL,
	FREECAM_PUSH,
	FREECAM_PULL_MOUSE,
	FREECAM_PUSH_MOUSE,
	FREECAM_ROT,
	FREECAM_STOP
};

class CFreeCameraCtrller :public CCameraController
{
public:
	CFreeCameraCtrller(void);
	~CFreeCameraCtrller(void);

public:
	//����
	CtrlState	_DoCameraCtrl( uint32 uCurTime,CCamera* pCamera, CSceneQuery* pQuery );

protected:
	//����
	//��ǰֵ
	CVector3f	m_NewTargetPos;
	CVector3f	m_Dir;
	float		m_Length;
	//ʱ��
	uint32 m_LastTime;
	//�ٶ� ÿ���ƶ��׻���ת����
	float	m_MoveSpeed;
	float	m_PullSpeed;
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
	FREECAM_STATE m_State;

public:
	//������Ϊ����
	void SetNewTargetPos(float x,float y,float z);
	void SetDistance(float length);
	void SetState(FREECAM_STATE _state);
	//��ʼ���Ƕ�
	void SetXAngle(float _xAngle);
	void SetYAngle(float _yAngle);
	float GetAngleX();
	float GetAngleY();
	//ÿ���ض��ٶ� ����ת���ٶ�
	void SetMoveSpeed(float _moveSpeed);
	void SetPullSpeed(float _pullSpeed);
	void SetXRate(float _xRate);
	void SetYRate(float _yRate);
	void SetXPixel(float _xPixel);
	void SetYPixel(float _yPixel);
	float GetMoveSpeed();
	float GetPullSpeed();
	float GetXRate();
	float GetYRate();
	//
};

inline void CFreeCameraCtrller::SetNewTargetPos(float x,float y,float z)
{
	m_NewTargetPos.x=x;	m_NewTargetPos.y=y;	m_NewTargetPos.z=z;
	this->SetTargetPos(x,y,z);
}

inline void CFreeCameraCtrller::SetDistance(float length)
{
	m_Length=length;
}

inline void CFreeCameraCtrller::SetState(FREECAM_STATE _state)
{
	m_State=_state;
}

inline void CFreeCameraCtrller::SetMoveSpeed(float _moveSpeed)
{
	m_MoveSpeed=_moveSpeed;
}

inline void CFreeCameraCtrller::SetPullSpeed(float _pullSpeed)
{
	m_PullSpeed=_pullSpeed;
}

inline void CFreeCameraCtrller::SetXAngle(float _xAngle)
{
	m_XAngle=_xAngle;
}

inline void CFreeCameraCtrller::SetYAngle(float _yAngle)
{
	m_YAngle=_yAngle;
}

inline void CFreeCameraCtrller::SetXRate(float _xRate)
{
	m_XRate=_xRate;
}

inline void CFreeCameraCtrller::SetYRate(float _yRate)
{
	m_YRate=_yRate;
}

inline void CFreeCameraCtrller::SetXPixel(float _xPixel)
{
	m_XPixel=_xPixel;
}

inline void CFreeCameraCtrller::SetYPixel(float _yPixel)
{
	m_YPixel=_yPixel;
}

inline float CFreeCameraCtrller::GetAngleX()
{
	return m_XAngle;
}

inline float CFreeCameraCtrller::GetAngleY()
{
	return m_YAngle;
}

inline float CFreeCameraCtrller::GetMoveSpeed()
{
	return m_MoveSpeed;
}

inline float CFreeCameraCtrller::GetPullSpeed()
{
	return m_PullSpeed;
}

inline float CFreeCameraCtrller::GetXRate()
{
	return m_XRate;
}

inline float CFreeCameraCtrller::GetYRate()
{
	return m_YRate;
}


//!namespace
}
