#pragma once
#include "CRendererBase.h"
#include "SQRWnd.h"
#include "TSingleton.h"

namespace sqr
{
	// wnd area
	class CWndAreaRendere
		: public CRendererBase
		, public Singleton<CWndAreaRendere>
	{
	public:
		CWndAreaRendere() ;
		virtual ~CWndAreaRendere();
		
		virtual void Render();
		virtual void RenderRootRect();
		virtual void RenderSelWndList();
		void	SetCurRect(const CRect &rt);
		void	SetIsRender(const bool b);

		void	SetRootRect(const CRect &rt);
		void	SetIsRootRender(const bool b);
		void	SetWndRenderList(vector<SQRWnd*>* plist);

	private:
		CRect	m_curRect;		// ��ǰѡ�о���
		bool	m_bRender;		// �Ƿ񻭾��ο�
		CRect	m_rootRect;		// ������
		bool	m_bRootRender;	// �Ƿ񻭸�����
		vector<SQRWnd*>*	m_renderWndList;
	};
}