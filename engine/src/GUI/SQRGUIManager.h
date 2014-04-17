#pragma once

#include "IGUIManager.h"
#include "SQRToolTip.h"
#include "GUIStuffParser.h"
#include "GUILayoutHandler.h"

#define		ACCEKEY_CTRL		0x0100
#define		ACCEKEY_SHIFT		0x0200
#define		ACCEKEY_ALT			0x0400
#define     ACCEKEY_TOPLEVEL	0x0600

namespace sqr
{
	extern const wstring GUI_PATH_ALIAS_GUITEX;
	extern const wstring GUI_PATH_ALIAS_GUI;
	extern const wstring GUI_PATH_ALIAS_LAN;

	typedef UIMap< uint32, UIVector<UIWString> >     MsgResMap;

	class SQRGUIManager : public IGUIManager
	{
	public:
		typedef UIMap<const UIString, ResInfo> MapResStaticInfo;
		struct TextBuf
		{
			RenderTextParam Pa;
			CRenderFont*	Rf;
		};

	private:
		struct DtMsg
		{
			DtMsg() : pWnd(NULL), wParam(0), lParam(0) {}
			SQRWnd*		pWnd;
			uint32		wParam;
			uint32		lParam;
		};
		
		typedef UIVector<TextBuf> TextBufVec;
		typedef UIMap<IHook*,UIVector<uint32> > HookMap;
		typedef UIMap<IAccelerator*,UIVector<uint32> > AcceleratorMap;
		typedef UIMap<UIString, WndResParam*> ResMap;
		typedef UIMap< WORD, DtMsg > MapAcceKeyToCommWnd;

		
		TextBufVec			m_TextBuf;
		CRenderFont*		m_curRdFont; 
		UIString			m_sFileName;
		SQRRootWnd*			m_pRootWnd;
		SQRWnd*				m_pModule;				// ��ǰģ̬����
		SQRWnd*				m_pHeldWnd;
		SQRWnd*				m_pFocusWnd;
		SQRWnd*				m_pTipHoldWnd;
		IGraphic*			m_pGraphic;
		IEffectManager*		m_pEffectMgr;
		IGUIHandler*		m_pIGUIHandler;
		SQRWnd*				m_PreCurSet;
		CFPos				m_CurCursor;
		CFPos				m_LogicCurCursor;
		uint32				m_CurMessage;
		uint32				m_nPreUpdateTime;
		SQRWnd*				m_pMoreLastMouseMsgWnd; // ��¼���һ�����������Ϣ�Ĵ���
		SQRWnd*				m_pLastMouseMsgWnd;		// ���һ�����������Ϣ�Ĵ���
		SQRWnd*				m_pLastLButtonDownWnd;	// ���һ������������������Ϣ�Ĵ���
		SQRWnd*				m_pLastRButtonDownWnd;	// ���һ����������Ҽ�������Ϣ�Ĵ���
		uint32				m_uPreMouseMsgTime;		// ��һ����Ϣ��ʱ��
		bool				m_bUserDefineDesc;		// ʹ���Զ���MouseDesc��ʾ
		bool				m_bIsEditActive;		// �Ƿ���SQREdit����
		SQRWnd*				m_UserShowWnd;			// ��ʾ�Ĵ���
		bool				m_bUserShowWnd;			// ʹ����ʾ�Ĵ���
		static HookMap		m_Hook;					// GUI����
		static AcceleratorMap	m_Accelerator;		// GUI��ݼ�
		UISet<SQRWnd*>				m_AllWnds;		// ���д����Ĵ���
		UIMap<UIString, HCURSOR>	m_CursorRes;
		UIMap<HCURSOR, UIString>	m_Cursor2Name;
		HCURSOR					m_hGlobCursor;
		MapAcceKeyToCommWnd		m_AcceKey;			// ��ݼ�ע���
		MsgResMap				m_ResContainer;		// MessageBox
		bool					m_isShowWndArea;
		MapResStaticInfo		m_ResStaticInfoContainer;// GUI��̬�ı�����
		HWND					m_hMainWnd;
		bool					m_bLockCursorMessage;		// �������λ��
		float					m_fDragResponseDis;			//��ק�ݴ����
		bool					m_bRefreshToolTip;

		//----------------��Ⱦ------------------
		ITexture*				m_pTextue;			// ForTexturBuf
		ITexture*				m_pAlpha;
		ERenderStyle			m_RS;
		UIVector<RectGroup>		m_TextureBuf;
		uint32					m_nCurTextureBufPos;

		uint32					m_nTotalRect;
		uint32					m_nFlush;
		float					m_fZoomSize;
		bool					m_bHidenMode;	///< ����������ģʽ
		UISet<CRenderFont*>		m_FontVec;

		//----------------��Դ����--------------------
		XMLParser*				m_pXMLParser;		// XML������
		GUILayoutHandler*		m_pLayoutHandler;	// layout handler
		bool					m_beAucRes;			// �Ƿ�ΪAUC��Դ
		bool					m_beGUITool;		// �Ƿ�Ϊguitoolʹ��
		bool					m_beInitedRes;		// GUI��Դ�Ƿ�Ԥ��ʼ��

	public:
		static UIString			m_strPkgName;		// Ϊʹpkg��֪��GUI��Դ���ڰ�������¼��GUI pkg����
		ResMap					m_ResTemplate;		// ��Դģ��
		inline const char*	GetGuiPkgPath() { return m_strPkgName.c_str(); }

	private:
		void				DragWnd(SQRWnd& Wnd);
	
	public:
		SQRGUIManager( IGUIHandler* pIGUIHandler, CFRect& rt, SQRRootWnd& Root, IGraphic* pGraphic, HWND hMainWnd,
			IEffectManager* pEffectMgr, const char* szResFileName,  const char* szPkgName = NULL, bool beUITool = FALSE, bool beInit = true );
		~SQRGUIManager();

		void				LoadStaticText();
		HRESULT				WndProc( uint32 message, WPARAM wParam, LPARAM lParam );
		void				Render();


		void				SetRefreshToolTip();
		SQRRootWnd*			GetRootWnd() const;
		SQRWnd*				GetModuleWnd() const;
		IGraphic*			GetGraphic() const;
		IEffectManager*		GetEffectManager() const;
		IGUIHandler*		GetGUIHandler() const;
		void				SetXMLParser();
		void				SetLayoutHandler();
		XMLParser*			GetXMLParser() const;	
		void				SetBeGUITool( bool be_tool );
		bool				GetBeGUITool();
		GUILayoutHandler*	GetLayoutHandler() const;
		float				GetSreenWidth() const;
		float				GetSreenHeight() const;
		const CFPos&		GetCursorPos() const;
		CFPos&				GetLogicCursorPos();
		uint32				GetCurMessage() const;
		void				SetGlobalCursor( HCURSOR hCursor );
		HCURSOR				GetGlobalCursor() const;
		BOOL				FindWnd( SQRWnd* pWnd );
		bool				IsFocus( SQRWnd* pWnd );
		bool				GetIsEditActive();
		void				SetIsEditActive(bool bIsEditActive);
		SQRWnd*				GetLastMouseMsgWnd();
		SQRWnd*				GetLastLButtonDownWnd();
		void				SetLastLButtonDownWnd( SQRWnd* pWnd );
		SQRWnd*				GetLastRButtonDownWnd();
		void				SetLastRButtonDownWnd( SQRWnd* pWnd );
		MsgResMap&			GetMsgResMap();
		void				SetZoomSize( float fSize );
		float				GetZoomSize();
		//GUIRenderEvn		SetRenderEvn();
		//void				SetRenderEvn(const GUIRenderEvn &evn);
		//GUIRenderEvn		GetRenderEvn();
		static void			RegisterAccelerator( IAccelerator* pAccelerator, uint32 uWndMsg );
		static void			UnRegisterAccelerator( IAccelerator* pAccelerator );
		static void			RegisterHook( IHook* pHook, uint32 uWndMsg );
		static void			UnRegisterHook( IHook* pHook );
		void				AddHookMsg( IHook* pHook, uint32 uWndMsg );
		void				CreateTipHoldWnd();
		void				OnTipHandlerDel(SQRWnd* wnd);

		bool				CheckHook( SQRWnd& Wnd, uint32 message, WPARAM wParam, LPARAM lParam );
		bool				CheckAccelerator( SQRWnd& Wnd, uint32 message, WPARAM wParam, LPARAM lParam );

		//���º���������Ϣ��ɸѡ�ͷ��䣬������GUIϵͳ�ĺ��Ĵ���

		//�õ�ĳ�����λ�������Ǹ�������
		SQRWnd* DetectWnd( SQRWnd& Wnd, CFPos posCursor );
		//���û����
		void SetActive( SQRWnd& Wnd, bool bFoucus );
		//��Ϣɸѡ
		LRESULT WndProc( SQRWnd& Wnd, SQRWnd& WndOnDrag, uint32 message, WPARAM wParam, LPARAM lParam );
		//�������ƶ����Ӵ�����Ӧ����
		void OnParentMoved( SQRWnd& Wnd, float dx, float dy );
		//����x,yλ�ô��ڵĵ����ק״̬
		void SetHeldWindow( SQRWnd* pWnd );
		//������
		/*void Draw( SQRWnd& Wnd );*/
		//������
		void DrawModulate( SQRWnd& Wnd );

		//����������
		void DrawArea( SQRWnd& Wnd );

		//�õ����ڹ�����
		static SQRGUIManager* GetGUI( SQRWnd* pWnd );
		//���õ�ǰģ̬����
		void SetModuleWnd( SQRWnd* pWnd );
		//ɾ����ǰģ̬����
		void DeleteModuleWnd( SQRWnd* pWnd );
		//�ǼǴ���
		void RegistWnd( SQRWnd* pWnd );
		//ע������
		void UnregistWnd( SQRWnd* pWnd );
		//�õ������
		SQRWnd* GetTopWnd( SQRWnd* pWnd = NULL );
		//�ǼǼ��ټ�
		void RegisterAccelKeyCommand( SQRWnd* pWnd, bool bCtrl, bool bShift, bool bAlt, uint8 VirKey, uint32 wParam, uint32 lParam );
		//������ټ�
		SQRWnd* TranslateAccelKey( uint32& message, WPARAM& wParam, LPARAM& lParam );
		//ȡ�����ټ�
		void UnRegisterAccelKeyCommand( SQRWnd* pWnd );
		//ȡ�����ټ�
		void UnRegisterAccelKeyCommand( bool bCtrl, bool bShift, bool bAlt, uint8 VirKey );
		//��ʼ����Դ����
		void LoadResMap();
		//��AUC����Դ
		bool LoadResFromAuc();
		//��XML����Դ
		bool LoadResFromXml();
		//��coder����Դ
		bool LoadResFromCoder();
		//�õ�������Դ
		WndResParam* GetWndRes( const char* szWindowNameInResFile );
		//����Դ�������ڱ���
		WND_IMAGE_LIST LoadBKRes( UIVector<WindowImage>& Res );
		//���������Դ
		HCURSOR LoadCursorRes( const char* sCurFile );
		//�õ�����ַ���
		const char* GetCursorFileName( HCURSOR hCursor );
		//���¹��
		void UpdateCursor(SQRWnd &Wnd);
		//������Ч
		void PlayFx( SQRWnd* pWnd, const char* szFx, EEffectRun eEffrun = eEffRun_Normal );
		//����������Ч
		void* PlayFxIme( SQRWnd* pWnd, const char* szFx, EEffectRun eEffrun = eEffRun_Normal );
		//������Ч
		void PlayFxEx( SQRWnd* pWnd, const char* szFxFile, const char* szFx, EEffectRun eEffrun = eEffRun_Normal );
		//����������Ч
		void* PlayFxImeEx( SQRWnd* pWnd, const char* szFxFile, const char* szFx, EEffectRun eEffrun = eEffRun_Normal );
		//����GUI��Ч
		void ClearFx( SQRWnd* pWnd, const char* szFx );
		//����GUI��Ч
		void ClearFx( SQRWnd* pWnd );
		//����GUI������Ч
		void ClearImeFx( SQRWnd* pWnd, const char* szFx );
		//ˢ��������Ч
		bool DrawImeFx( SQRWnd* pWnd, const char* szFx, uint curTime, float zoom );
		//�رա��������뷨
		void EnableIme( bool bEnable );
		
		//����GUI��̬�ı�
		void LoadResStaticText();
		void LoadResStaticTxt();
		bool ProcessStaticText(UIVector<UIString>& vec, string& txt);		// true title; false desc
		void LoadResTxt(UIMap<UIString,UIString>& txtmap, const wchar_t* pkg, const char* file);
		UIMap<const UIString, ResInfo>* GetResStaticInfoContainer();
		ResInfo*	GetResInfo(const UIString& str);

		//������
		void DrawRect( const CFRect& rtr, uint32 uColor, ITexture* pTexture, const CFRect* rtText, ERenderStyle rs, float z, FLOAT *pOffset = NULL);
		//������
		void DrawRect( const CFRect& rtr, uint32 uColor, ITexture* pTexture, const CPosRect& rtText, ERenderStyle rs, float z );
		//������
		void DrawRect( const CVector2f* rt, uint32 uColor, ITexture* pTexture, const CVector2f* rtText, ERenderStyle rs, float z );
		//������
		void DrawRect( const CVector2f* rt, uint32 uColor, ITexture* pTexture0, ITexture* pTexture1, const CFRect* rtText0, const CVector2f* rtText1, ERenderStyle rs, float z );
		//������
		void DrawRect( const CVector2f* rt, uint32 uColor, ITexture* pTexture0, ITexture* pTexture1, ITexture* pTexture2, ERenderStyle rs, float z );
		//������
		void FlushRect();
		//������
		void FlushText();
		//�����κ�����
		void FlushDraw();
		//д��
		void DrawText( CRenderFont* font, float size, const wstring& text,
			uint32 color, uint32 bkColor, uint32 gradualColor,
			CFRect rect, float x, float y, float z,
			FontEffect::Mask mask, float offset );
		//����������
		void ClearFontCatch(CRenderFont* font);
		//��ǰ֡���ֻ��ƽ���
		void EndFontFrame();

		virtual void SwitchHidenMode(){ m_bHidenMode = !m_bHidenMode; }
		bool GetHidenMode();

		//��Ҫ��ʾ�Ĵ���
		void LoadWnd(SQRWnd *pWnd);

		//ж����Ҫ��ʾ�Ĵ���
		void UnloadWnd();

		//��ʾ��������
		void ShowWndArea(bool isShow)				{ m_isShowWndArea = isShow;	};
		bool isShowWndArea()						{ return m_isShowWndArea;	};
		void SetLockCursorMessage(bool bLock)		{ m_bLockCursorMessage = bLock; };
		bool GetLockCursorMessage()					{ return m_bLockCursorMessage; };
		void RenderArea();
		void ShowCursor(bool bShow);
		void GetSysCursorPos(CFPos& pos);
		void SetSysCursorPos(int x, int y);
		void GetClientRect(CIRect& rect);
		void ClipCursor(CIRect& rect);
		void SetCapture();
		void ReleaseCapture();
	};

	inline int16 GetWndZOrder( SQRWnd* pWnd )
	{
		SQRWnd* pParent = pWnd->GetParent();
		int16 nOrder = 1;
		if ( !pParent )
			return nOrder;
		SQRWnd* pChild = pParent->FindLastChild();
		while ( pChild )
		{
			if ( pChild == pWnd )
				return nOrder;
			nOrder++;
			pChild = pChild->FindPreWnd();
		}

		return nOrder;
	}

	//-----------------------------------------------------------------------------
	inline void	
		SQRGUIManager::SetRefreshToolTip()
	{
		m_bRefreshToolTip = true;
	}

	//------------------------------------------------------------------------------
	inline SQRRootWnd*
		SQRGUIManager::GetRootWnd() const
	{
		return m_pRootWnd;
	}

	//------------------------------------------------------------------------------
	inline SQRWnd*
		SQRGUIManager::GetModuleWnd() const
	{
		return m_pModule;
	}

	//------------------------------------------------------------------------------
	inline bool 
		SQRGUIManager::GetHidenMode()
	{
		return m_bHidenMode;
	}

	//------------------------------------------------------------------------------
	inline IGraphic*
		SQRGUIManager::GetGraphic() const
	{
		return m_pGraphic;
	}

	//------------------------------------------------------------------------------
	inline IEffectManager*
		SQRGUIManager::GetEffectManager() const
	{
		return m_pEffectMgr;
	}

	//------------------------------------------------------------------------------
	inline IGUIHandler*
	SQRGUIManager::GetGUIHandler() const
	{
		return m_pIGUIHandler;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::SetXMLParser()
	{
		if (!m_pXMLParser)
		{
			// Ŀǰֻ����һ��������
			m_pXMLParser = new StuffParser();
		}
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::SetLayoutHandler()
	{
		if (!m_pLayoutHandler)
		{
			m_pLayoutHandler = new GUILayoutHandler();
		}
	}

	//------------------------------------------------------------------------------
	inline XMLParser*			
		SQRGUIManager::GetXMLParser() const
	{
		return m_pXMLParser;
	}

	inline void
		SQRGUIManager::SetBeGUITool(bool be_tool)
	{
		m_beGUITool = be_tool;
	}

	inline bool
		SQRGUIManager::GetBeGUITool()
	{
		return m_beGUITool;
	}

	//------------------------------------------------------------------------------
	inline GUILayoutHandler*	
		SQRGUIManager::GetLayoutHandler() const
	{
		return m_pLayoutHandler;
	}

	//------------------------------------------------------------------------------
	inline float
		SQRGUIManager::GetSreenWidth() const
	{
		return m_pRootWnd->m_hWnd->m_Width;
	}

	//------------------------------------------------------------------------------
	inline float
		SQRGUIManager::GetSreenHeight() const
	{
		return m_pRootWnd->m_hWnd->m_Height;
	}

	//------------------------------------------------------------------------------
	inline const CFPos&
		SQRGUIManager::GetCursorPos() const
	{
		return m_CurCursor;
	}

	//------------------------------------------------------------------------------
	inline uint32
		SQRGUIManager::GetCurMessage() const
	{
		return m_CurMessage;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::SetGlobalCursor( HCURSOR hCursor )
	{
		m_hGlobCursor = hCursor;
	}

	//------------------------------------------------------------------------------
	inline HCURSOR
		SQRGUIManager::GetGlobalCursor() const
	{
		return m_hGlobCursor;
	}

	//------------------------------------------------------------------------------
	inline BOOL
		SQRGUIManager::FindWnd( SQRWnd* pWnd )
	{
		return m_AllWnds.find( pWnd ) != m_AllWnds.end();
	}

	//------------------------------------------------------------------------------
	inline bool
		SQRGUIManager::IsFocus( SQRWnd* pWnd )
	{
		return m_pFocusWnd == pWnd;
	}

	//------------------------------------------------------------------------------
	inline bool
		SQRGUIManager::GetIsEditActive()
	{
		return m_bIsEditActive;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::SetIsEditActive( bool bIsEditActive )
	{
		m_bIsEditActive = bIsEditActive;
	}

	//------------------------------------------------------------------------------
	inline SQRWnd*
		SQRGUIManager::GetLastMouseMsgWnd()
	{
		return m_pLastMouseMsgWnd;
	}

	//------------------------------------------------------------------------------
	inline SQRWnd*
		SQRGUIManager::GetLastLButtonDownWnd()
	{
		return m_pLastLButtonDownWnd;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::SetLastLButtonDownWnd(SQRWnd* pWnd)
	{
		m_pLastLButtonDownWnd = pWnd;
	}

	//------------------------------------------------------------------------------
	inline SQRWnd*
		SQRGUIManager::GetLastRButtonDownWnd()
	{
		return m_pLastRButtonDownWnd;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::SetLastRButtonDownWnd(SQRWnd* pWnd)
	{
		m_pLastRButtonDownWnd = pWnd;
	}

	//------------------------------------------------------------------------------
	inline MsgResMap&
		SQRGUIManager::GetMsgResMap()
	{
		return m_ResContainer;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::SetZoomSize( float fSize )
	{
		m_fZoomSize = fSize;
	}

	//------------------------------------------------------------------------------
	inline float
		SQRGUIManager::GetZoomSize()
	{
		return m_fZoomSize;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::LoadWnd( SQRWnd *pWnd )
	{
		m_UserShowWnd = pWnd;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRGUIManager::UnloadWnd()
	{
		m_UserShowWnd = NULL;
	}

	//------------------------------------------------------------------------------
	inline UIMap<const UIString, ResInfo>*
		SQRGUIManager::GetResStaticInfoContainer()
	{
		return &m_ResStaticInfoContainer;
	}
	
}// namespace sqr
