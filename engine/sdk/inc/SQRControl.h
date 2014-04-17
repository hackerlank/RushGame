#pragma once
#include "SQRWnd.h"

namespace sqr
{
	class GUI_API SQRControl : public SQRWnd
	{
		DYNAMIC_DECLARE_WND( SQRControl )
	public:
		SQRControl(void);
		virtual ~SQRControl(void);
		//�����ڴ�����Ϣ�ĺ���
		virtual LRESULT WndProc( uint32 message, WPARAM wParam, LPARAM lParam );
		//������Ϣ��������
		LRESULT	SendCtrlMsg( uint32 uMsgID, uint32 uParam1, uint32 uParam2 );
	};
}