#pragma once

#include "CCameraController.h"
#include "CEventDelegate.h"
#include "CRecordStruct.h"

namespace sqr_tools
{
class CEditContext;
class CRecordCamDelegate: public CEventDelegate, public CCameraController
{
public:
	CRecordCamDelegate(void);
	~CRecordCamDelegate(void);
	
	void SetAnimate(RecordAnimate _animate);
	void SetPlay(bool _play);

	bool MsgProc(const CMsg& msg);
	CtrlState _DoCameraCtrl( uint32 uCurTime,CCamera* pCamera, CSceneQuery* pQuery );

protected:
	CEditContext* m_pContext;
	void		_Init(CEventOwner* pOwner);
	void		_InitCameraInfo(void);
	void		_LineLerpCam();
	//ʱ��
	int			m_TotalTime;	//��ʱ�� ����
	int			m_PassTime;		//����ʱ�� ����
	
	//�����������ڴ��һ���������ݰ�
	RecordAnimate	m_CurAnimate;
	
	//��ǰ����
	CVector3f	curPos;
	CVector3f	curDir;
	CVector3f	curTarget;

	bool		m_Play;

};

//!namespace
}
