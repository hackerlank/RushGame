#pragma once
#include "BaseHelper.h"
#include "GUIBase.h"
#include "SQRWnd.h"

namespace sqr
{
	class GUI_API SQRFlashLayer : public virtual CDynamicObject
	{
	public:
		SQRFlashLayer(void);
		SQRFlashLayer(UIString flashTex, const CFRect& rt, SQRWnd* pChildWnd, 
					uint32 ct, float v, float lx, float ty ,float rx, float by);

		SQRFlashLayer(FlashWndInfo* flash, SQRWnd* pChildWnd);
		
		virtual ~SQRFlashLayer(void);

		// ������˸��������˸��������ʵ��
		virtual void DrawBackground();
		bool GetFlashWillDel();
		bool CheckParendWnd(SQRWnd* pwnd);
		bool CheckFlushWnd(SQRWnd* pwnd, UIString tex);
		bool CheckFlushWnd(FlashWndInfo* flash, SQRWnd* pwnd);
		void ReInitFlash(FlashWndInfo* flash);
		bool CheckFlashContShow();
		void SetDrawed();
	
	private:
		void InitTexture(UIString stex);
		bool RefreshFlash();
		void SQRRefreshTrans();
		bool SQRRefreshPos();
		bool CheckWillShow();
	private:
		bool		m_bDel;					//  �Ƿ�ɾ��
		CColor		m_color;				//	��ɫ

		FlashWndInfo* m_flash;				//  ��������
		
		UIString	m_texStr;
		ITexture*	m_pTexture;
		SQRWnd*		m_pParentWnd;			//	���ص��Ĵ���
		CFRect		m_rtTex;				//  ��ͼ��С
		float		m_v;					//  ��˸�ٶȣ�����ֻ������
		uint32		m_count;				//  ��˸��������Ϊ������������һֱ��˸

		float		m_Amin;
		float		m_Amax;

		// left top right bottom��Ը�����У��λ��
		float		m_lx;
		float		m_ty;
		float		m_rx;
		float		m_by;

		float		m_fTransParent;			//  ͸����
		DWORD		m_curSysTime;			//  ��ǰϵͳʱ��
		uint32		m_curCount;				//  ��ǰ���Ŵ���
		bool		m_bAdd;					//  ͸�����Ƿ�������״̫
		CFRect		m_curPos;				//  ��ǰλ��
	};

	inline bool SQRFlashLayer::CheckParendWnd(SQRWnd* pwnd)
	{
		return pwnd == m_pParentWnd;
	}

	inline bool SQRFlashLayer::CheckFlushWnd(SQRWnd* pwnd, UIString tex)
	{
		return pwnd == m_pParentWnd && m_texStr == tex;
	}

	inline bool SQRFlashLayer::CheckFlushWnd(FlashWndInfo* flash, SQRWnd* pwnd)
	{
		return pwnd == m_pParentWnd && m_flash == flash;
	}

	inline bool SQRFlashLayer::GetFlashWillDel()
	{
		return m_bDel;
	}
}