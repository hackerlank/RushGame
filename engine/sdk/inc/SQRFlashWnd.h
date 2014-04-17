#pragma once
#include "SQRControl.h"

namespace sqr
{
	class CFlashPlayer;
	class GUI_API SQRFlashWnd : public SQRControl
	{
		DYNAMIC_DECLARE_WND( SQRFlashWnd )
	public:
		SQRFlashWnd();  //�����ھ��
		~SQRFlashWnd(void);

		virtual int32	Create(const WndCreateParam& param );
		virtual void	DrawWndBackground();
		virtual void	SetszSourceName( const char* szDesc );
		virtual void    SetStyle( uint Style );
		virtual void	ResizeRect( const CFRect& rt ,bool refreshbak = true);
		virtual void    OnProcess();
		
		void PlayFlash();
		void StopFlash();
		void UpdateFlash();

		//���˫����Ϣ
		 void	OnLButtonDblClk( uint32 nFlags, int32 x, int32 y );
		//��������Ϣ
		 void	OnLButtonDown( uint32 nFlags, int32 x, int32 y ); 
		//����ͷ���Ϣ
		 void	OnLButtonUp( uint32 nFlags, int32 x, int32 y ); 
		//һ������������̧�����
		 void	OnLButtonClick( uint32 nFlags, int32 x, int32 y ); 
		//�м�˫����Ϣ
		 void	OnMButtonDblClk( uint32 nFlags, int32 x, int32 y );
		//�м������Ϣ
		 void	OnMButtonDown( uint32 nFlags, int32 x, int32 y );
		//�м�������Ϣ
		 void	OnMButtonUp( uint32 nFlags, int32 x, int32 y );
		//����ƶ���Ϣ
		 void	OnMouseMove( uint32 nFlags, int32 x, int32 y );
		//��������Ϣ
		 void	OnMouseWheel( uint32 nFlags, short zDelta, int32 x, int32 y );
		//�Ҽ�˫����Ϣ
		 void	OnRButtonDblClk( uint32 nFlags, int32 x, int32 y );
		//�Ҽ������Ϣ
		 void	OnRButtonDown( uint32 nFlags, int32 x, int32 y );
		//�Ҽ��ͷ���Ϣ
		 void	OnRButtonUp( uint32 nFlags, int32 x, int32 y ); 
		//һ������Ҽ�����̧�����
		 void	OnRButtonClick( uint32 nFlags, int32 x, int32 y );

		//�����ڴ�����Ϣ�ĺ���
		virtual LRESULT WndProc( uint32 message, WPARAM wParam, LPARAM lParam );

	private:
		void*			m_WndHandle;
		CFlashPlayer*	m_FlashPlayer;
	};

}