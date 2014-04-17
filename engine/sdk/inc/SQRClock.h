
#pragma once
#include "SQRControl.h"
#include "IRenderTime.h"
namespace sqr
{

	class SQRClock;
	class GUI_API SQRClockPart : public SQRControl
	{
		friend class SQRClock;
		DYNAMIC_DECLARE_WND( SQRClockPart )
	public:
		SQRClockPart(void);
		virtual ~SQRClockPart(void);
		virtual void OnProcess();
		void	SetBeFrontColor(bool b);
		void	InitClockPart();
	private:
		//���
		INT8	mflag;

		//��ǰ�ƶ��ĳ���
		FLOAT	flength;

		//��ת�ķ���
		bool    bclock;
		//�Ƿ�Ϊǰɫ
		bool	bfrontcl;		
	};

	class GUI_API SQRClock : public SQRControl
	{
		DYNAMIC_DECLARE_WND( SQRClock )

	public:
		SQRClock(void);
		virtual ~SQRClock(void);

		//����ʱ��
		void setTime(int32 dtime);
		void setTimeEx(int32 dtime, IRenderTime* pTime);
		//�����Ѿ����е�ʱ��
		void setRunedTime(int32 runedtime);
		//����Clock�ؼ�͸��ɫ
		void SetBackImageColor(uint32 uColor);
		//����Clock�ؼ�����ɫ����������ǻ����
		void SetBeFrontImageColor(bool b);

		//������ʾ��λ��
		void SetPos(int32 nPos);
		void SetPosEx(int32 nPos, IRenderTime* pTime);
		//�������λ��
		void SetRange(int32 nRange);

		//������ת����
		void SetDirection(bool Direction);

		virtual SQRWnd*				GetResWnd( int32 nCtrlMask );
		virtual WND_IMAGE_LIST*		GetWndBkImage( const IMAGE_PARAM& ImageFlag );

		//�߼��ļ���
		virtual void OnProcess();

		virtual int32	    	Create( const WndCreateParam& param );
		virtual void			OnDestroy();
		static bool				IsHasEventState(EventStateMask Mask);
		virtual bool			ObjHasEventState(EventStateMask Mask);

		virtual void			ReleaseWndTex();		// ��ʡ��Դģʽ�£����ڹر�ʹ��
		virtual void			CreatWndTex();			// ��ʡ��Դģʽ�£����ڴ�ʹ��

		virtual void			_ResizeRect( const CFRect& rt ,bool refreshbak = true);
		virtual void			_SetWndRect( const CFRect& rt );
	private:
		//��һ����̬����תButton
		//��ʼʱ��
		DWORD			 mBegintime;
		//�Ѿ����е�ʱ��
		DWORD			 mRunedtime;
		DWORD			 mRRunedtime;
		//����ʱ��
		DWORD			 mEndtiem;

		//���Clock ���� progress
		int32            Funcflag;

		//ÿ���ƶ��Ĳ���
		FLOAT			 fStep;

		//��ʾ�ߵ��ڼ���������
		int32            flagRect;

		//ÿ�����εĳ���
		FLOAT			 flength[4];

		//��¼ʱ��
		DWORD			 lastRunedTime;

		//һ���ĳ���
		FLOAT			 falllength;

		//��ǰ�ĳ���
		FLOAT			 fNowlength;

		//��������λ�ú�ͼ��
		SQRClockPart*	 pProgress[4];
		bool			 bShowState[5];

		IRenderTime*	 m_pTime;
		//ÿ��С�ؼ���CFRect
		CFRect			 m_Rect;
		WND_IMAGE_LIST	 m_BackImage;

		//�Ƿ�����������
		bool			 m_bFrontColor;
	};
}