#pragma once
#include "SQRGuiMallocObject.h"
#include "SQRFlashLayer.h"


namespace sqr
{
	class GUIRenderTask : public SQRGuiMallocObject
	{
	public:
		GUIRenderTask(){}
		virtual ~GUIRenderTask(){}

		virtual void RenderTask() = 0;
	};

	// ��˸���ʵ���Ⱦ����
	class FLASHRenderTask : public GUIRenderTask
	{
	public:
		FLASHRenderTask();
		virtual ~FLASHRenderTask();

		virtual void RenderTask();
		virtual void AddFlashTask(UIString flashTex, const CFRect& rt, SQRWnd* pChildWnd, uint32 ct, float v, 
								  float lx, float ty ,float rx, float by);

		virtual void AddFlashTask(FlashWndInfo* flash, SQRWnd* pwnd, uint32 st);
		virtual void DelFlashTaskAll(SQRWnd* pWnd);	// ɾ�������ϵ�������˸Ч��
		virtual void DelFlashTask(SQRWnd* pWnd, UIString flash); // ɾ�������ϵ��ض���˸Ч��
		virtual bool HasCountFlash();
	
	private:
		UIVector<SQRFlashLayer*>	m_FlashWnd[2];		// 0������У�1״̬����
	};

	// ������Ⱦ����
	class TextRenderTask : public GUIRenderTask
	{
	public:
		TextRenderTask();
		virtual ~TextRenderTask();

		virtual void	RenderTask();
		void			SetFlushDlg(SQRWnd* pDlg);
	private:
		SQRWnd*			m_pDlg;
	};

	inline void TextRenderTask::SetFlushDlg(SQRWnd* pDlg)
	{
		m_pDlg = pDlg;
	}

	// ��Ч��Ⱦ����
	class FxRenderTask : public GUIRenderTask
	{
	public:
		struct _FxTask
		{
			SQRWnd* pWnd;
			uint	curTime;
			float	zoom;
			_FxTask(SQRWnd* wnd, uint ct, float zm);
		};
		FxRenderTask();
		virtual ~FxRenderTask();

		virtual void	RenderTask();
		void			PushRenderStack(SQRWnd* pWnd, uint curTime, float zoom);
	private:
		UIVector<_FxTask>	m_FxTaskVec;
	};
}
