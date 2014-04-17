
#pragma once
#include "SQRControl.h"

namespace sqr
{
	//�����������Լ��ƶ��Ĵ���
	class GUI_API SQRWndOffSet : public SQRControl
	{
		DYNAMIC_DECLARE_WND( SQRWndOffSet )
	public:
		SQRWndOffSet();
		~SQRWndOffSet();

		//���ô�������
		virtual void            SetWndText( const char* str );

		//�õ���������
		virtual const char*	GetWndText();

		//���ر�������
		virtual void            DrawWndBackground();

		//�����Ƿ���ʾ
		virtual void			ShowWnd( bool bShow );

		virtual void            DrawText( const wstring& text, float x, float y, CColor color, CColor bkColor, bool multiline, CFRect* rect );

		//�����ƶ����ٶ�
		void		            SetStep(INT32 time, FLOAT mStep);

		static bool				IsHasEventState(EventStateMask Mask);
		virtual bool			ObjHasEventState(EventStateMask Mask);
	private:
		//�ؼ����ڲ��Լ�����
		virtual void            OnProcess();

	private:
		//����ԭ�����ַ���
		UIString  resstring;
		DWORD   mSteptimer;
		DWORD   mBeginTimer;
		FLOAT   mStepResult;
		FLOAT   mStepOver;
		FLOAT   mStep;
	};
}