#pragma once
#include "GraphicBase.h"
#include "CViewport.h"
#include "RenderSettings.h"

namespace sqr
{
	class CWindowTarget : public CRenderTarget
	{
		friend class CViewport;
	public:
		CWindowTarget();
		virtual ~CWindowTarget();
		virtual CViewport*		AddViewport( void );
		virtual bool			ReCreate(void)		= 0;
		virtual void			RestoreSize( bool useClientRect = true )	= 0;
		virtual void			Reset(void)			= 0;
		virtual void			ReSaveSize( uint w, uint h ){};
		void					SetBeProtected(bool pr);
		bool					GetBeProtected();

	protected:
		void DeleteViewport( CViewport* pTarget );
		typedef GVector<CViewport*> TargetList;
		TargetList				m_lRenderTarget;
		bool					m_beFullScreen;
		uint32					m_uHeight;
		uint32					m_uWidth;
		bool					m_BeProtected;			///< ��ǰ�����Ƿ��ڱ�����״̬
	};

	inline void CWindowTarget::SetBeProtected(bool pr)
	{
		m_BeProtected = pr;
	}

	inline bool CWindowTarget::GetBeProtected()
	{
		return m_BeProtected;
	}

	//��ʵ�ܶ�ʱ�� ��Ϸֻ��һ�������
	//ȫ����ʱ��Ҳֻ����һ������
	class CMainWindowTarget : public CWindowTarget, public Singleton<CMainWindowTarget>
	{
	public:
		const RenderSettings& GetRenderSettings() const
		{
			return m_RenderSettings;
		};
		HWND GetHwnd()
		{
			return m_RenderSettings.GetWindowHandle();
		}
		virtual HRESULT ChangeRenderSettings(const RenderSettings& settings)
		{
			m_RenderSettings = settings;
			return S_OK;
		};
		virtual HRESULT	IsRenderActive()
		{
			return S_OK;
		};
		virtual bool	IsFullScreen()
		{
			return !m_RenderSettings.IsWindowed();
		};
		virtual vector<uint> GetResolutionWidths()
		{
			return vector<uint>();
		}

		void	SetBackBufferNum( UINT Num );
		UINT	GetBackBufferNum();
		void	Render(void);

		virtual EAntiAliasQuality	GetAntiAliasQuality() { return AA_None; };
		virtual bool				Initialize(const RenderSettings& settings) { return false; };

	protected:
		RenderSettings	m_RenderSettings;		///< ��ǰ�豸����
	};
}