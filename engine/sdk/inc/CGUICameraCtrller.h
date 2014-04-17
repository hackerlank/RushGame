#pragma once
#include "CCameraController.h"

namespace sqr
{
#define GUI_CAM_UNRESET_CTRLTIME	2000
#define GUI_CAM_XSCALE_MAX			1.5f
#define GUI_CAM_YSCALE_MAX			2.0f
#define GUI_CAM_ZSCALE_MAX			1.5f
#define GUI_CAM_YDIS_DFT			100	
#define GUI_CAM_ZDIS_DFT			-600	
#define Z_GUI_FAR					50000.0f
#define Z_GUI_NEAR					10.0f

	class CGUICameraCtrller : public CCameraController
	{	
	public:
		CGUICameraCtrller();
		void			SwitchCamState();
		void			SetWearTime( float t );
		void			SetCamNormalPos( float x, float y, float z );
		void			InitCamCtrller();
		void			InitCamCtrller( const CVector3f& pos, const CVector3f& look );

	protected:
		CtrlState		_DoCameraCtrl( uint32 uCurTime,CCamera* pCamera, CSceneQuery* pQuery );
		void			_InitBind( void );


		bool			m_NeedReQuery;
		float			m_fWearTime;			// ����ʱ��
		float			m_fWearAlready;			// �Ѿ�����ʱ��
		float			m_uCurTime;				// ��ǰˢ�µ�ʱ��

		float			m_fCurXDis;				// ��ǰ�����Y
		float			m_fCurYDis;				// ��ǰ�����Y
		float			m_fCurZDis;				// ��ǰ�����Z
		CVector3f		m_LookAt;				// ���������
		bool			m_bNormal;				// ��ǰ�ڳ���״̬ = TRUE Զ��״̬ = FALSE
		bool			m_bInMove;				// ������ڵ���������
	};

} 