#pragma once

#include "IGraphic.h"
#include "CRenderFont.h"
#include "IEffectManager.h"
#include "GUIBase.h"
#include "SQRWndClass.h"
#include "CRectangle.h"
#include "CDynamicObject.h"
#include "TimeHelper.h"
#include "CColor.h"
#include "SQRWndDef.h"
#include "CallBackX.h"
#include "CodeCvs.h"
#include "SQRGUITransparentEffect.h"
#include "SQRGUIShapeEffect.h"

namespace sqr
{
	class ITexture;
	class SQRDialog;
	class SQRRootWnd;
	class SQRGUIEffectHandler;

	class GUI_API SQRWnd 
		: public virtual CDynamicObject
		, public SQRGuiMallocObject
	{
	protected:
		WndHandle*				m_hWnd;
		SQRGUIEffectHandler*	m_effHld;		// Ч��
		friend		class   SQRDialog;
		DYNAMIC_DECLARE_WND( SQRWnd )
	private:
		void			Relock(CFRect& rect, WndHandle* pWndH, SQRWnd* pChildWnd, const CFRect& rt, float Zoom,bool bTestIsGUITool);

	public:
		WndHandle*		GetWndHandle(void) const;
		static bool		IsHasEventState(EventStateMask Mask);

		virtual void	OnShapeEffectEnded();
	protected:

		//////////////////////////////////////////////////////////////////////////
		//��Ϣ����

		//�����ڴ�����Ϣ�ĺ���
		virtual LRESULT WndProc( uint32 message, WPARAM wParam, LPARAM lParam );

		//���ڱ���ס����
		virtual void	OnHeld( float /*xScreen*/, float /*yScreenCToolTip*/ ) {}
		//�����ס����
		virtual void	OnDrag( const CFPos& HeldPoint, const CFPos& CurPoint )
		{
			OnDrag(HeldPoint.x, HeldPoint.y, CurPoint.x, CurPoint.y);
		}
		virtual void	OnDrag(float iPosX1, float iPosY1, float iPosX2, float iPosY2) {
			CALL_CLASS_CALLBACK_4(iPosX1, iPosY1, iPosX2, iPosY2)
		}
		//�״̬�ı�
		virtual void    OnActive( bool bActive, SQRWnd* Wnd ) ;
		//���ڴ���
		virtual void    OnCreated();
		//���ڴ���
		virtual void    OnDestroy();
		//���ڴ���
		virtual void    OnResCreated( const char* szResInfo );
		//�Ӵ��ڴ���
		virtual void    OnChildCreated();
		//�������ƶ�
		virtual void	OnParentWndMoved( float dx, float dy );
		//�����ƶ�ǰ
		virtual bool	OnWndPreMoved( float& /*dx*/, float& /*dy*/ )
		{
			return true;
		}
		//�����ƶ�
		virtual void	OnWndMoved( float dx, float dy );
		//������ʾ
		virtual void	OnWndShow( bool bShow );
		//���Ŵ�����Ч
		virtual void	OnPlayWndFx( EMsgFx eFx );
		//���Ŵ�����Ч
		virtual void	OnPlayWndSound( ESoundMsg eSound );
		//��ʾ�����Ϣ֮ǰ��������ʱʱ��
		virtual uint32	OnPreShowMouseDesc();
		//���Tooltip��ʾ��RichText��Ч������falseʹ��Ĭ�ϱ���
		virtual bool	OnMouseDescBegin()
		{
			CALL_CLASS_CALLBACK_RET(bool) 
			return false;
		}
		//���Tooltip��ʾ��RichText��Ч
		virtual void	OnMouseDescEnd();

		//--------------------��׼Windows��Ϣ--------------------
		//�ַ���Ϣ
		virtual void	OnChar( uint32 nChar, uint32 nFlags ) ;
		//Ime�ַ���Ϣ
		virtual void	OnImeChar( uint32 nChar, uint32 nFlags );
		//���̰�����Ϣ
		virtual void	OnKeyDown( uint32 nChar, uint32 nFlags );
		//���̵�����Ϣ
		virtual void	OnKeyUp( uint32 nChar, uint32 nFlags );
		//���˫����Ϣ
		virtual void	OnLButtonDblClk( uint32 nFlags, int32 x, int32 y );
		//��������Ϣ
		virtual void	OnLButtonDown( uint32 nFlags, int32 x, int32 y ); 
		//����ͷ���Ϣ
		virtual void	OnLButtonUp( uint32 nFlags, int32 x, int32 y ); 
		//һ������������̧�����
		virtual void	OnLButtonClick( uint32 nFlags, int32 x, int32 y ); 
		//��סshiftʱ������ͷ���Ϣ
		virtual void	OnShiftLButtonUp( uint32 nFlags, int32 x, int32 y );
		//��סshiftʱ������ͷ���Ϣ
		virtual void	OnShiftLButtonDown( uint32 nFlags, int32 x, int32 y );
		//�м�˫����Ϣ
		virtual void	OnMButtonDblClk( uint32 nFlags, int32 x, int32 y );
		//�м������Ϣ
		virtual void	OnMButtonDown( uint32 nFlags, int32 x, int32 y );
		//�м�������Ϣ
		virtual void	OnMButtonUp( uint32 nFlags, int32 x, int32 y );
		//����ƶ���Ϣ
		virtual void	OnMouseMove( uint32 nFlags, int32 x, int32 y );
		//��������Ϣ
		virtual void	OnMouseWheel( uint32 nFlags, short zDelta, int32 x, int32 y );
		//�Ҽ�˫����Ϣ
		virtual void	OnRButtonDblClk( uint32 nFlags, int32 x, int32 y );
		//�Ҽ������Ϣ
		virtual void	OnRButtonDown( uint32 nFlags, int32 x, int32 y );
		//�Ҽ��ͷ���Ϣ
		virtual void	OnRButtonUp( uint32 nFlags, int32 x, int32 y ); 
		//һ������Ҽ�����̧�����
		virtual void	OnRButtonClick( uint32 nFlags, int32 x, int32 y );
		//���ù����Ϣ
		virtual bool	OnSetCursor()
		{
			return true;
		}
		//���뽹�㼤����Ϣ
		virtual void	OnEditActive() {}
		//�߼�����
		virtual	void	OnProcess();

		friend class SQRGUIManager;

	public:

		SQRWnd(void);
		virtual ~SQRWnd(void);

		void SetTransparentObj(int32 runtime, bool bvalue);
		void EndTransparent( bool bvalue);

		//��Ӧ�Ӵ���
		virtual void			OnCtrlmsg( SQRWnd* pChild, uint32 uMsgID, uint32 uParam1, uint32 uParam2 );
		//���ڴ���������
		void					RegisterAccelKeyCommand( bool bCtrl, bool bShift, bool bAlt, uint8 VirKey, uint32 wParam, uint32 lParam );
		void					UnRegisterAccelKeyCommand( bool bCtrl, bool bShift, bool bAlt, uint8 VirKey );///////////add by lily
		virtual int32			DestroyWnd();		// ��������
		virtual int32			Create( const WndCreateParam& param );
		int32					ReCreateRes( const char* szChildName );
		GUI_FONT*				GetFontInfo(uint32 idx);
		//��Ļ����λ�øı�
		virtual void			OnScreenPosChanged();

		//�ƶ���ת���ٻ���
		void					SetRotation( FLOAT angle );

		//�ƶ����Ǹ�����ת
		void					SetRotationOrigin( FLOAT offsetx, FLOAT offsety);
		//////////////////////////////////////////////////////////////////////////
		//�����Ժ��������Ա�����ʵ��ָ������

		//������Ϣ
		virtual LRESULT         OnCommand( WPARAM wParam, LPARAM lParam );
		//������Ϣ��������
		LRESULT					SendMsg( uint32 message, WPARAM wParam, LPARAM lParam );
		//�ػ������Ӵ���
		void					ReDrawChildren();
		//����Ч����Ч�ֲ㴦���о����ã�ʹUI��ĸ��ӣ�������Ч���ǿ���ͨ��ֱ��������Чʵ�ֵģ��ȼ�����ӿڰɣ�
		virtual void			DrawImeFx( uint curTime, float zoom );
		virtual void			FlashImeFx( uint curTime, float zoom );

		//������
		virtual void			DrawRect( ITexture* pTexture, const CFRect& rt, uint32 Color, const CFRect* pTextRect = NULL, bool bClip = false );
		virtual void			DrawRect( ITexture* pTexture, const CFRect& rt, uint32 Color, const CPosRect& TextRect );
		virtual void			DrawRect( const CVector2f* rt, uint32 uColor, ITexture* pTexture,const CVector2f* rtText, bool bClip = false );
		virtual void			DrawRect( const CVector2f* rt, uint32 uColor, ITexture* pTexture0, ITexture* pTexture1, const CFRect* rtText0,const CVector2f* rtText1, ERenderStyle rs, float z );
		virtual void			DrawRect( const CVector2f* rt, uint32 uColor, ITexture* pTexture0, ITexture* pTexture1, ITexture* pTexture2,ERenderStyle rs, float z );
		virtual void			DrawRect( WND_IMAGE_LIST& ImageList, const CFRect* rt = NULL );
		virtual void			DrawRect( WND_IMAGE_LIST& ImageList, const CFRect* rt, float offsetX, float offsetY);
		virtual void			DrawRect( const CFRect& rt, uint32 color, bool screenPos = false );
		/// @brief ����SQRWnd�ı߿�
		virtual void			DrawRect(CColor color = CColor::Red);

		//����
		virtual void			DrawLine( const PointGroup* pPointGroup, int32 nNum, bool bScreenPos = false );
		//д��
		virtual void			DrawText( const wstring& text, uint32 textColor, uint32 textBcColor );
		virtual void			DrawText(const wstring& text, float x, float y, CColor color, CColor bkColor, bool multiline, CFRect* rect, bool vertical);
		virtual void			DrawText(const wstring& text, float x, float y, CColor color, CColor bkColor, bool multiline, CFRect* rect);

		//ǿ�ƻ�
		void                    FlushRect();
		void					FlushDraw();
		//���ӿ�
		virtual void			Draw();

		//���Ժ���
		IGraphic*				GetGraphic();
		IEffectManager*			GetEffectManager();
		CRenderFont*			GetFont()							const;
		SQRRootWnd*				GetRootWnd();
		float					GetRootZoomSize();
		SQRWnd*					GetModuleWnd();
		uint32					GetStyle() const;
		virtual void			SetStyle( uint32 Style );
		SQRWnd*					GetParent();
		SQRToolTip*				GetToolTipRichText();
		// rich tooltip
		SQRRichToolTip*			GetRichToolTip();
		SQRRichToolTip*			SetNewRichToolTip();
		void					DelRichToolTip();
		// end of rich tooltip
		float					GetFontSize()const;
		void					SetFontSize(float size);
		bool					IsFontBold();
		int32					GetFontID();
		int32					GetGroupID();
		void					SetGroupID( int32 nGroupID );
		const char*				GetszClassName();
		void					SetszClassName( const char* szDesc );
		const char*				GetszCreateInfo();
		void					SetszCreateInfo( const char* szDesc );
		const char*				GetszWindowName();
		void					SetszWindowName( const char* szDesc );
		const char*				GetMouseOverDesc();
		void					SetMouseOverDesc( const char* szDesc );
		void					SetMouseOverDescAfter( const char* szDesc );
		void					SetMouseOverRichTextImageForAllWnd( int32 nIndex, WND_IMAGE_LIST& ImageList );
		const char*				GetMsgFx( EMsgFx eFxType );
		void					SetMsgFx( EMsgFx eFxType, const char* szDesc );
		const char*				GetMsgSound( ESoundMsg eSoundType );
		void					SetMsgSound( ESoundMsg eSoundType, const UIString& strSoundName );
		uint32					GetLockFlag();
		void					SetLockFlag( uint32 nLockFlag );
		ERenderStyle			GetRenderStyle();
		void					SetRenderStyle( ERenderStyle eRS );
		float					GetDepth();
		virtual void			SetToAutoFormat(bool b){};
		bool					GetBGIsDrawed();
		void					SetBGIsDrawed(bool bd);
		// ������Ч������ѡ����Ч���Ľӿ�
		void					PlayScreenPathFx(const char* fxFile, const char* fxName, EEffectRun eEffrun = eEffRun_Normal);
		void					PlayWndPathFx(const char* fxFile, const char* fxName, EEffectRun eEffrun = eEffRun_Normal);
		void					PlayWndImePathFx(const char* fxFile, const char* fxName, EEffectRun eEffrun = eEffRun_Normal);
		//������Ч�ĺ���
		void					PlayWndFx(const char* fxName);
		void					PlayScreenFx(const char* fxName);
		void					PlayWndFxEx(const char* fxName, EEffectRun eEffrun = eEffRun_Normal);
		void					PlayScreenFxEx( const char* fxName, EEffectRun eEffrun = eEffRun_Normal);
		//ɾ����Ч����д�����Զ�ɾ����Ч����Ҫ�ֶ�ɾ��
		void					DelWndFx(const char* fxName);
		void					DelWndFxEx();
		void					DelScreenFx(const char* fxName);
		// ���ż�ʱ��Ч
		void					PlayWndImeFx(const char* fxName, EEffectRun eEffrun);
		void					DelWndImeFx(const char* fxName);

		/// @brief ���ô��ڵ�ZOrder��0�����1��Զ
		void					SetDepth( float fDepth );
		const CFRect&			WndToScreen( CFRect& rt );
		const CFRect&			ScreenToWnd( CFRect& rt );
		void					WndToScreen(CFRect& rt, CFRect& out);
		void					ScreenToWnd(CFRect& rt, CFRect& out);
		void					SetWndZoom( float zoom ); // ��������������ϲ����仯����Ҫ�ֶ�����ResizeRect
		float					GetWndZoom( void );
		const CFPos&			GetCursorPos();
		void					GetCursorPos(CFPos& pos);
		const CFPos&			GetLogicCursorPos();
		void					GetLogicCursorPos(CFPos& pos);
		uint32					GetCurMessage();
		bool					FindParent( SQRWnd* pParent );
		SQRWnd*					FindNextWnd();
		SQRWnd*					FindPreWnd();
		SQRWnd*					FindFirstChild();
		SQRWnd*					FindLastChild();
		SQRWnd*					InserBefore( SQRWnd* pWnd );
		SQRWnd*					GetTopWnd();
		const char*				GetZOrder();
		SQRWnd*					GetDlgChild( const char* szChildName );
		bool					AssignDlgChildToWnd( SQRWnd& Wnd, const wchar_t* szChildName );
		void					SetWndData( uint32 dwData );
		uint32					GetWndData();
		static void				RegisterAccelerator( IAccelerator* pAccelerator, uint32 uWndMsg );
		static void				UnRegisterAccelerator( IAccelerator* pAccelerator );
		static void				RegisterHook( IHook* pHook, uint32 uWndMsg );
		static void				UnRegisterHook( IHook* pHook );
		void					AddHookMsg( IHook* pHook, uint32 uWndMsg );
		virtual bool			TestHook( IHook* pHook );
		virtual void			PrepareDefaultDesc();
		void					PushTipData();
		void					CopyToClipboard( const char* str );

		virtual void			EnableWnd( bool bEnable );
		virtual void			ShowWnd( bool bShow );
		virtual void			ShowWndIgnTex( bool bShow );	// ���Բ��ʴ���
		virtual void			SetLogicRect( const CFRect& rt );
		virtual void			GetLogicRect( CFRect& rt );
		virtual float			RefreshWndDis( float val );
		virtual void			SetWndRect( const CFRect& rt );
		virtual void			ResizeRect( const CFRect& rt ,bool refreshbak = true);
		virtual void			ResetWndPos(float x, float y);
		virtual void			_ResetWndPos(float x, float y);
		virtual void			OnSized();
		virtual void			ResizeRectOneChild( const CFRect& rt, SQRWnd* pChild );

		void					SetShapeEffect(int32 runtime, CFRect& src, CFRect& des);
		void					DelShapeEffect( SQRWnd* Wnd );
		void					SetInShape();

		void					GetWndRect( CFRect& rt ); //�õ�ȫ������
		void					GetWndRectBak( CFRect& rt );	// ��ȡ�α䴰��ʵ�ʴ�С
		virtual void			_ResizeRect( const CFRect& rt ,bool refreshbak = true);
		virtual void			_SetWndRect( const CFRect& rt );
		virtual	float			GetWndLeft();
		virtual float			GetWndTop();
		float					GetWndRight();
		float					GetWndBottom();
		float					GetWndWidth()						const;
		float					GetWndHeight()						const;
		float					GetWndOrgWidth();
		float					GetWndOrgHeight();
		void					SetWndWidth(float width);
		void					SetWndHeight(float height);
		void					SetActive();
		void					SetFocus();
		void					SetTransparent( float fTrans ); //���ô���͸����
		float					GetTransparent();
		bool					SetFont( int32 nFontID, bool bBold, float nSize );
		SQRWnd*					SetParent( SQRWnd* pParent );
		void					AddChildWnd( SQRWnd *pWnd );		//Ϊ��ǰ���ڼ���һ���Ӵ���
		virtual	CFRect			GetParentCutRect();//�õ������д���
		virtual void			GetCutAreaRect( CFRect& rt );

		virtual void			OnCreatedAfter(){};
		virtual void			ReleaseWndTex();		// ��ʡ��Դģʽ�£����ڹر�ʹ��
		virtual void			CreatWndTex();			// ��ʡ��Դģʽ�£����ڴ�ʹ��
		bool					IsModleState();

		const char*				GetWndName();
		virtual const char*		GetWndText();
		const wchar_t*			GetWndNameW();
		virtual const wchar_t*	GetWndTextW();
		virtual const bool		GetIsBottom();
		const bool				GetHaveEditActive();
		virtual void			SetWndText( const char* str );
		/// @brief �����ַ���׷��
		/// @remark �Ȳ���SQRWnd::SetWndText((SQRWnd::GetWndText() + str).c_str());�кܴ���ٶ�����
		virtual const char*		AppendWndText(const string& str);
		virtual void			SetIsBottom(bool bIsBottom);

		void					SetWndBkImage( const IMAGE_PARAM& ImageFlag, const WND_IMAGE_LIST& Image );
		virtual WND_IMAGE_LIST*	GetWndBkImage( const IMAGE_PARAM& ImageFlag );

		//----------------------------------------------------------------------------
		virtual void			SetWndTextColor( const IMAGE_PARAM& ImageFlag, uint32 uColor );
		virtual uint32			GetWndTextColor( const IMAGE_PARAM& ImageFlag );

		virtual void			SetWndTextBcColor( const IMAGE_PARAM& ImageFlag, uint32 uColor );
		virtual uint32			GetWndTextBcColor( const IMAGE_PARAM& ImageFlag );

		virtual void			SetFontGradualColor( uint color );
		virtual void			SetFontEffectMask( uint mask );

		virtual uint			GetFontGradualColor(void);
		virtual uint			GetFontEffectMask(void);

		virtual uint32			GetStyleIndex();
		virtual void			SetStyleIndex( uint32 index );

		virtual const char*		GetszSourceName();
		virtual void			SetszSourceName( const char* szDesc );
		//----------------------------------------------------------------------------

		virtual SQRWnd*			GetResWnd( int32 nCtrlMask );
		// ��꺯��
		void					SetWndCursor( const char* szCursorFile );
		const char*				GetWndCursor();
		void					SetGlobCursor( const char* szCursorFile );
		const char*				GetGlobCursor();
		void					UpdateGlobCursor(SQRWnd &Wnd);			//���¹��

		void                    SetDetectMouseOverWnd(bool bFlag);
		bool                    IsDetectMouseOverWnd();

		bool					IsChild( SQRWnd* pWnd );
		bool					IsHeld();					// �����Ƿ񱻵�ס����
		bool					IsActive();
		bool					IsFocus();
		bool					IsEnable();
		bool					IsEnableTest();
		bool					IsShow();;
		bool					IsCreated();
		bool					IsSysCreated();
		bool					IsLastMouseMsgWnd();
		bool					IsDownTooltips();
		void					SetDownTooltips( bool bIsDown );
		void					RefreshToolTip();
		SQRWnd*					GetLastMouseMsgWnd();
		virtual bool			IsInWndArea( float x, float y  );
		virtual bool			IsInTitleArea( float x, float y );
		virtual bool			IsInFrameArea( float x, float y );
		bool					GetMouseWheel();
		void					SetMouseWheel(bool bGetMouseWheel);
		void					ClearImeFx();
		void					SetTipFontSize(float size);
		float					GetTipFontSize();
		
		//////////////////////////////////////////////////////////////////////////
		//��ϵͳ�����������ڵĺ�����ͨ����Ҫ������

		//������
		virtual void			DrawWndBackground();
		//������
		virtual void			DrawWndArea();

		//�������ı�
		virtual void			DrawWndText();
		//���Ӵ��ڱ���������
		virtual void			DrawAfterChildren();
		//��ʾ�����Ϣ
		virtual void			DrawMouseOver();

		//
		virtual void			DrawAfterAllEyeCandy();

		bool					IsDrawBackground();
		void					SetIsDrawBackground(bool bShaow);
		void					SetAlignToPixel(bool bAlignToPixel);

		void					DestroyFlashAtt();
		virtual SQRDialog*		GetFlushDialog();
		virtual bool			IsFlushDialog();

		void					AddFlashInfoShow();		// ��ӵ���ʾ����
		void					AddFlashInfoShow(FlashWndInfo* info, uint32 fs);		// ��ӵ���ʾ����
		void					AddFlashAttentionTex( char* flashTex, const CFRect& rt, SQRWnd* pChildWnd, uint32 ct, float v, 
												   float lx, float ty ,float rx, float by);
		void					AddFlashInfoByName(const char* info);
		void					AddFlashInfoByNameImp(const char* info);
		void					AddFlashInfoByNameEx(const char* info, bool cut);
		void					DelFlashAttention();
		void					DelFlashAttentionSpe( SQRWnd* pChildWnd, const char* sTex );

		void					SetFlashInfo(EventStateMask fs, const char* name);
		const char*				GetFlashInfo(EventStateMask fs);
		void					DelFlashInfo(EventStateMask fs);
		UIMap<EventStateMask, FlashWndInfo*>&	GetFlashInfoMap();

		virtual void			SetEventStateMask(EventStateMask es);
		EventStateMask			GetEventStateMask();
		virtual bool			ObjHasEventState(EventStateMask Mask);
		
		void					SetCutWnd(SQRWnd* wnd);
		SQRWnd*					GetCutWnd();

		bool					GetCutFlash();
		void					SetCutFlash(bool cut);
		void					RejustRect(CFRect& rt);

		void					SetWndDebugInfo(const char* info);
		void					GetWndDebugInfo(string& str);

		UIVector<SQRDialog*>	m_pDlgVec;			// ����ĸ�Dlg,flash���
		SQRDialog*				m_FlushDlg;			// flush��ص�DLG
		bool					m_bInShape;
		bool					m_bCutFlash;

		bool					bIsBottom; //�Ƿ�����ײ㴰��,ture�����ʾ����������֮��
		bool					bDetectMouseOverWnd; //�Ƿ����������λ�õĴ���
		bool					bNotModule;

		float					m_WndZoom;
		float					m_itX;			//���ֵ�ƫ��
		float					m_itY;			//���ֵ�ƫ��
		FLOAT					offset[7];		//�ĸ������ƫ���� 0 ƫ����  1 ƫ���� ...   4 �ƶ������ĸ���    5
		UIString				debugWndInfo;	//����ʹ��

		bool					m_bAlignToPixel;
		typedef UIVector<UIString> EffectVec;
		EffectVec				m_Effect;		// GUI��Ч
		bool					m_beBGDrawed;	// ������֡�Ƿ���

		SQRWnd*					m_pCutWnd;		//�����趨��ĳ���������
	private:
		EventStateMask			m_ESMask;
		bool					m_bIgnTex;
		
	};

	inline bool
		SQRWnd::GetCutFlash()
	{
		return m_bCutFlash;
	}

	inline void
		SQRWnd::SetCutFlash(bool cut)
	{
		m_bCutFlash = cut;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRWnd::SetDetectMouseOverWnd( bool bFlag )
	{
		bDetectMouseOverWnd = bFlag;    //�趨�Ƿ����������λ�õĴ���
	}

	//------------------------------------------------------------------------------
	inline bool
		SQRWnd::IsDetectMouseOverWnd()
	{
		return bDetectMouseOverWnd;
	}
	//------------------------------------------------------------------------------
	inline bool
		SQRWnd::IsShow()
	{
		return !GetParent()|| (m_hWnd->m_bShow && GetParent()->IsShow());
	}

	inline bool
		SQRWnd::GetBGIsDrawed()
	{
		return m_beBGDrawed;
	}

	inline void
		SQRWnd::SetBGIsDrawed(bool bd)
	{
		m_beBGDrawed = bd;
	}


	//------------------------------------------------------------------------------
	inline bool
		SQRWnd::IsDrawBackground()
	{
		return m_hWnd->m_bIsShowBackground;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRWnd::SetIsDrawBackground( bool bShaow )
	{
		m_hWnd->m_bIsShowBackground = bShaow;
	}

	//------------------------------------------------------------------------------
	inline SQRWnd*
		SQRWnd::FindFirstChild()
	{
		return m_hWnd->m_FirstChild;
	}

	//------------------------------------------------------------------------------
	inline WndHandle*
		SQRWnd::GetWndHandle( void ) const
	{
		return m_hWnd;
	}

	//------------------------------------------------------------------------------
	inline uint32 SQRWnd::GetStyle() const
	{
		return m_hWnd->m_WndStyle;
	}

	inline SQRWnd* SQRWnd::GetResWnd( int32 nCtrlMask )
	{
		return nCtrlMask == SM_BS_BK ? this : NULL;
	}

	inline float SQRWnd::GetFontSize() const
	{
		return m_hWnd->m_WndFont ? m_hWnd->m_FontSize : 0.0f;
	}

	inline void SQRWnd::SetFontSize(float size)
	{
		WndHandle* wnd = GetWndHandle();
		this->SetFont(wnd->m_WndFont->GetFontID(), wnd->m_WndFont->IsBold(), size);
	}
	
	inline void SQRWnd::SetTipFontSize(float size)
	{
		m_hWnd->m_TipFontSize = size;
	}

	inline float SQRWnd::GetTipFontSize()
	{
		return m_hWnd->m_TipFontSize; 
	}
	inline CRenderFont* SQRWnd::GetFont() const
	{
		return m_hWnd->m_WndFont;
	}

	inline bool SQRWnd::IsHeld()
	{
		return m_hWnd->m_bIsHeld ;
	}

	inline bool SQRWnd::IsDownTooltips()
	{
		return m_hWnd->m_bIsDownTooltips;
	}

	inline void SQRWnd::SetDownTooltips( bool bIsDown )
	{
		m_hWnd->m_bIsDownTooltips = bIsDown;	
	}
	
	inline bool SQRWnd::IsActive()
	{
		return m_hWnd->m_IsActive && IsEnable();
	}

	inline bool SQRWnd::IsCreated()
	{
		return ( m_hWnd->m_pGUI != NULL );
	}

	inline bool SQRWnd::IsSysCreated()
	{
		return IsCreated() && m_hWnd->m_bNewByGUISys;
	}

	inline bool SQRWnd::GetMouseWheel()
	{
		return m_hWnd->m_bGetMouseWheel;
	}

	inline void SQRWnd::SetMouseWheel(bool bGetMouseWheel)
	{
		m_hWnd->m_bGetMouseWheel = bGetMouseWheel;
	}

	inline SQRWnd* SQRWnd::GetParent()
	{
		return m_hWnd->m_ParentWnd;
	}

	inline SQRToolTip*
		SQRWnd::GetToolTipRichText()
	{
		return m_hWnd->m_DescRichText; 
	}

	inline SQRRichToolTip*
		SQRWnd::GetRichToolTip()
	{
		return m_hWnd->m_pRichToolTip;
	}

	inline bool SQRWnd::IsEnable()
	{
		if ( GetParent() && GetParent()->IsEnable() == false )
			return false;
		return m_hWnd->m_bEnable && IsShow();
	}

	//ר�Ų����Լ��Ƿ�ΪEnable,������Ҫ���Ǹ����ڵ�
	inline bool SQRWnd::IsEnableTest()
	{
		return m_hWnd->m_bEnable;
	}

	inline void  SQRWnd::SetWndData( uint32 dwData )
	{
		m_hWnd->m_dwOtherData = dwData;
	}

	inline uint32 SQRWnd::GetWndData()
	{
		return m_hWnd->m_dwOtherData;
	}

	inline SQRWnd* SQRWnd::FindNextWnd()
	{
		return m_hWnd->m_NextBrother;
	}

	inline SQRWnd* SQRWnd::FindPreWnd()
	{
		return m_hWnd->m_PreBrother;
	}

	inline SQRWnd* SQRWnd::FindLastChild()
	{
		return m_hWnd->m_LastChild;
	}

	inline const char* SQRWnd::GetMsgFx( EMsgFx eFxType )
	{
		return m_hWnd->m_MsgFx[eFxType].c_str();
	}

	inline void SQRWnd::SetMsgFx( EMsgFx eFxType, const char* szDesc )
	{
		m_hWnd->m_MsgFx[eFxType] = szDesc ? szDesc : "";
	}

	inline const char* SQRWnd::GetMsgSound( ESoundMsg eSoundType )
	{
		return m_hWnd->m_MsgSound[eSoundType].c_str();
	}

	inline void SQRWnd::SetMsgSound( ESoundMsg eSoundType, const UIString& strSoundName )
	{
		m_hWnd->m_MsgSound[eSoundType] = strSoundName;
	}

	inline uint32 SQRWnd::GetLockFlag()
	{
		return m_hWnd->m_uLockFlage;
	}

	inline void SQRWnd::SetLockFlag( uint32 nLockFlag )
	{
		m_hWnd->m_uLockFlage = nLockFlag;
	}

	inline ERenderStyle SQRWnd::GetRenderStyle()
	{
		return m_hWnd->m_eRS;
	}

	inline void SQRWnd::SetRenderStyle( ERenderStyle eRS )
	{
		m_hWnd->m_eRS = eRS;
	}

	inline float SQRWnd::GetDepth()
	{
		return m_hWnd->m_fZ;
	}

	inline void SQRWnd::SetDepth( float fDepth )
	{
		m_hWnd->m_fZ = fDepth;
	}

	inline int32 SQRWnd::GetGroupID()
	{
		return m_hWnd->m_GroupID;
	}

	inline void SQRWnd::SetGroupID( int32 nGroupID )
	{
		m_hWnd->m_GroupID = nGroupID;
	}

	inline void SQRWnd::GetCursorPos(CFPos& pos)
	{
		pos = GetCursorPos();
	}

	inline void SQRWnd::GetLogicCursorPos(CFPos& pos)
	{
		pos = GetLogicCursorPos();
	}

	inline void SQRWnd::WndToScreen(CFRect& rt, CFRect& out)
	{
		out = WndToScreen(rt);
	}

	inline void SQRWnd::ScreenToWnd(CFRect& rt, CFRect& out)
	{
		out = ScreenToWnd(rt);
	}

	inline bool SQRWnd::IsFontBold()
	{
		return m_hWnd->m_WndFont ? m_hWnd->m_WndFont->IsBold() : false;
	}

	inline float SQRWnd::GetWndRight()
	{
		return GetWndLeft() + m_hWnd->m_Width;
	}

	inline float SQRWnd::GetWndBottom()
	{
		return GetWndTop() + m_hWnd->m_Height;
	}

	inline float SQRWnd::GetWndWidth() const
	{
		return m_hWnd->m_Width;
	}

	inline float SQRWnd::GetWndHeight() const
	{
		return m_hWnd->m_Height;
	}

	inline float SQRWnd::GetWndOrgWidth()
	{
		return m_hWnd->m_WidthInitial;
	}

	inline float SQRWnd::GetWndOrgHeight()
	{
		return m_hWnd->m_HeightInitial;
	}

	inline void SQRWnd::SetWndWidth(float width)
	{
		m_hWnd->m_Width = width;
	}

	inline void SQRWnd::SetWndHeight(float height)
	{
		m_hWnd->m_Height = height;
	}

	inline float SQRWnd::GetTransparent()
	{
		return m_hWnd->m_fTransparent;
	}

	inline const char* SQRWnd::GetWndName()
	{
		return m_hWnd->m_WindowName.c_str();
	}

	inline const wchar_t* SQRWnd::GetWndNameW()
	{
		return m_hWnd->m_WindowNameW.c_str();
	}

	inline const wchar_t* SQRWnd::GetWndTextW()
	{
		return m_hWnd->m_WindowTitleW.c_str();
	}

	inline const bool SQRWnd::GetIsBottom()
	{
		return this->bIsBottom;
	}

	inline void SQRWnd::EnableWnd( bool bEnable )
	{
		m_hWnd->m_bEnable = bEnable;
	}

	inline void SQRWnd::SetRotation( FLOAT angle )
	{
		offset[5] = angle;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRWnd::SetFontGradualColor( uint color )
	{
		m_hWnd->m_FontGradualColor = color;
	}

	//------------------------------------------------------------------------------
	inline void
		SQRWnd::SetFontEffectMask( uint mask )
	{
		m_hWnd->m_FontEffectMask = mask;
	}

	//------------------------------------------------------------------------------
	inline uint
		SQRWnd::GetFontGradualColor( void )
	{
		return m_hWnd->m_FontGradualColor;	
	}

	//------------------------------------------------------------------------------
	inline uint
		SQRWnd::GetFontEffectMask( void )
	{
		return m_hWnd->m_FontEffectMask;
	}

	//------------------------------------------------------------------------------
	inline EventStateMask			
		SQRWnd::GetEventStateMask()
	{
		return m_ESMask;
	}

	//------------------------------------------------------------------------------
	inline void		
		SQRWnd::SetCutWnd(SQRWnd* wnd)
	{
		m_pCutWnd = wnd;
	}

	//------------------------------------------------------------------------------
	inline SQRWnd*			
		SQRWnd::GetCutWnd()
	{
		return m_pCutWnd;
	}

}// namespace sqr
