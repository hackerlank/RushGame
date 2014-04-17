#pragma once
#include "SQRScrollBar.h"
#include "CodeCvs.h"
#include "CRectangle.h"

#define CTRL_A		( 'A' - 'A' + 1 )
#define CTRL_C		( 'C' - 'A' + 1 )
#define CTRL_V		( 'V' - 'A' + 1 )
#define CTRL_X		( 'X' - 'A' + 1 )


namespace sqr
{
	class GUI_API SQREdit : public SQRControl
	{
		DYNAMIC_DECLARE_WND( SQREdit )

	public:
		// ----------------------------------------------------------------------------------------
		// Accessor functions
		// ----------------------------------------------------------------------------------------
		void			SetSelection(size_t start_pos, size_t end_pos);

		/**
		\brief
			ȡ�ù�����
		*/
		SQRScrollBar*		GetHorzScrollbar();
		SQRScrollBar*		GetVertScrollbar();

		/**
		\brief
		���ù��λ��
		*/
		void			setCaratIndex(size_t carat_index);						

		// ----------------------------------------------------------------------------------------
		// Overridden functions
		// ----------------------------------------------------------------------------------------
		virtual int32   Create( const WndCreateParam& param );
		virtual int32	DestroyWnd();
		virtual WND_IMAGE_LIST* GetWndBkImage( const IMAGE_PARAM& ImageFlag );
		virtual void	SetIsSendMsgToParent( bool IsSendMsgToParent );
		virtual void	SetWndText( const char* str );
		virtual void	SetWndRect( const CFRect& rt );
		virtual void	ResizeRect( const CFRect& rt  ,bool refreshbak = true);
		virtual void	_ResizeRect( const CFRect& rt ,bool refreshbak = true);
		virtual void	_SetWndRect( const CFRect& rt );
	public:
		virtual void	EnableWnd( bool bEnable );
		virtual void	ShowWnd( bool bShow );
		virtual void	SetMaxTextLenght( int32 nLen );
		virtual int32	GetMaxTextLenght();
		virtual void	InsertStr( const char* str );
		virtual void	DisableIme( bool bDisable );
		virtual int32	GetLineCount( int32* pMaxLineLength );
		virtual int32	GetLineCount() { return GetLineCount(NULL); }
		virtual const char*		GetWndText();

		static bool		IsHasEventState(EventStateMask Mask);
		virtual bool	ObjHasEventState(EventStateMask Mask);
		virtual SQRWnd*	GetResWnd( int32 nCtrlMask );
		void			SetIsOnCommand(bool bFlag);
		bool			GetIsOnCommand();
		virtual void	GetCutAreaRect( CFRect& rt );
		void			SetSendType(bool bType);
		bool			GetSendType();

		// ----------------------------------------------------------------------------------------
		// Construction and Destruction
		// ----------------------------------------------------------------------------------------
		SQREdit();
		virtual		   ~SQREdit();

		// ----------------------------------------------------------------------------------------
		// Implementation struct
		// ----------------------------------------------------------------------------------------
		/**
		\brief
			struct used to store information about a formatted line within the
			control text.
		*/
		struct LineInfo
		{
			size_t  startIdx;     // Starting index for this line.
			size_t  length;       // Code point length of this line.
			float   extent;       // Rendered extent of this line.
		};
		typedef UIVector<LineInfo>   LineList;   // Type for collection of LineInfos.

	protected:
		void LoseFocusByShowFalse();

	protected:
		// ----------------------------------------------------------------------------------------
		// Overridden event handlers
		// ----------------------------------------------------------------------------------------
		virtual void	OnChar( uint32 nChar, uint32 nFlags );
		virtual void	OnImeChar( uint32 nChar, uint32 nFlags );
		virtual void	OnKeyDown( uint32 nChar, uint32 nFlags );
		virtual void	OnLButtonDblClk( uint32 nFlags, int32 x, int32 y );
		virtual void	OnLButtonDown( uint32 nFlags, int32 x, int32 y );
		virtual void	OnMouseWheel( uint32 nFlags, short zDelta, int32 x, int32 y );
		virtual void	OnDrag( const CFPos& HeldPoint, const CFPos& CurPoint );
		virtual void    OnActive( bool bActive, SQRWnd* Wnd );
		virtual void	DrawWndText();

		// ----------------------------------------------------------------------------------------
		// Implementation methods
		// ----------------------------------------------------------------------------------------
		void			CtrlC();
		void			CtrlV();
		void			SetCurShow();
		UIWString&		GetEditString();
		const UIWString& GetEditString()const;
		bool			GetIsSendMsgToParent();
		void			OnCtrlmsg( SQRWnd* pChild, uint32 uMsgID, uint32 uParam1, uint32 uParam2 );

		/**
		\brief
			���¹��ʱ�䣬����ʵ�ֹ����˸
		*/
		void			UpdateShowTime();

		/**
		\brief
			���������С��������ֵ	
		*/ 
		float			GetFontZoomSize();

		size_t			getNextTokenLength(const wstring &text, size_t start_idx) const;
		float			getTextPixelLen(const wstring &text, size_t start_idx, size_t len);
		float			getTextPixelLen(const wstring &text);
		void			formatText();
		
		void			clearSelection();

		/**
		\brief
			���ַ����������ַ����ڵ���
		*/
		size_t			getLineNumberFromIndex(size_t index)	const;

		/**
		\brief
			������������λ�õ��ַ�������(��������ϵ��)
		*/
		size_t			getTextIndexFromPosition(CFPos pt)		const;

		/**
		\brief
			��ȡ�ַ���Ⱦ����(��������ϵ��)
		*/
		CFRect			getTextDrawArea()						const;

		/**
		\brief
			ȡ������ѡȡ�����Ϣ(ѡȡ��ʼλ�á�����λ�úͿ��)
		*/
		size_t			getSelectionStartIndex()				const;
		size_t			getSelectionEndIndex()					const;
		size_t			getSelectionLength()					const; 

		/**
		\brief
			ɾ����ѡ�������
		*/
		void			eraseSelectedText();

		/**
		\brief
			ȡ�ù��λ��
		*/
		size_t			getCaratIndex()							const;

		/**
		\brief
			ȷ������ƶ���ɼ�
		*/
		void			ensureCaratIsVisible();

		/**
		\brief
			�������ֻ���
		*/
		void			drawTextLines(const CFRect &dest_area);
		void			drawCarat(const CFRect &textArea);

		/**
		\brief
			���ı����ı����Ҫ���ô˺������˺���ȷ���ı���'\n'���������ҽ����ı�
			��ʽ��������������ȷ�����ɼ�
		*/
		void			onTextChanged();

		/**
		\brief
			ȡ�ø�ʽ���õ��ı���
		*/
		const LineList& getFormattedLines(void)				const;

		/**
		\brief
			���������
		*/
		void			configureScrollbars();

		/**
		\brief
			����������
		*/
		void			handleNewLine();
		void			handleBackspace();
		void			handleDelete();
		void			handleCharLeft();
		void			handleCharRight();
		void			handleWordLeft();
		void			handleWordRight();
		void			handleLineUp();
		void			handleLineDown();
		void			handleLineHome();
		void			handleLineEnd();
		void			handleDocHome();		
		void			handleDocEnd();

		// ----------------------------------------------------------------------------------------
		// Implementation data
		// ----------------------------------------------------------------------------------------
		SQRScrollBar		m_sbHorizon;		//ˮƽ������
		SQRScrollBar		m_sbVertical;		//��ֱ������

		uint32			m_uMaxTextLen;
		uint64			m_ShowTime;			// ����ʵ�ֹ����˸��
		bool			m_IsSendMsgToParent;

		LineList		m_lines;			// �����ı�������Ϣ
		size_t			m_caratIndex;		// ������ַ����е�λ��
		size_t			m_selectStart;		// ѡ��������ʼλ��
		size_t			m_selectEnd;		// ѡ���������λ��
		size_t			m_dragAnchorIdx;	// �����קê��
		float			m_widestExtent;		// ����п��
		UIWString		m_szPassWord;

		const static UIWString	m_lineBreakChars;
		const static float		m_fCaratWidth;		// �����
		
	private:
		bool			m_bOnCommand;		//�Ƿ���������Ͽ�ݼ�
		bool			m_bEnterSend;		//�Ƿ�ʹ��Enter������(Ĭ����Ctrl+Enter)
	};

	// ----------------------------------------------------------------------------------------
	// Inline methods
	// ----------------------------------------------------------------------------------------
	inline SQRScrollBar* SQREdit::GetHorzScrollbar()
	{
		return &m_sbHorizon;
	}

	inline SQRScrollBar* SQREdit::GetVertScrollbar()
	{
		return &m_sbVertical;
	}

	inline void SQREdit::SetMaxTextLenght( int32 nLen )
	{
		m_uMaxTextLen = uint32( nLen );
	}

	inline int32 SQREdit::GetMaxTextLenght()
	{
		return int32( m_uMaxTextLen );
	}

	inline bool SQREdit::GetIsSendMsgToParent()
	{
		return m_IsSendMsgToParent;
	}

	inline void SQREdit::UpdateShowTime()
	{
		m_ShowTime = GetProcessTime();
	}

	inline UIWString& SQREdit::GetEditString()
	{
		return m_hWnd->m_WindowTitleW;
	}

	inline const UIWString& SQREdit::GetEditString()const
	{
		return m_hWnd->m_WindowTitleW;
	}

	inline void SQREdit::SetIsSendMsgToParent( bool IsSendMsgToParent )
	{
		m_IsSendMsgToParent = IsSendMsgToParent;
	}

	inline size_t SQREdit::getSelectionLength() const
	{
		return m_selectEnd - m_selectStart;
	}

	inline size_t SQREdit::getCaratIndex() const 
	{ 
		return m_caratIndex; 
	}

	inline const SQREdit::LineList& SQREdit::getFormattedLines(void) const   
	{
		return m_lines;
	}

	inline void SQREdit::SetIsOnCommand(bool bFlag)
	{
		m_bOnCommand = bFlag;
	}

	inline bool SQREdit::GetIsOnCommand()
	{
		return m_bOnCommand;
	}

	inline void SQREdit::SetSendType(bool bType)
	{
		m_bEnterSend = bType;
	}

	inline bool SQREdit::GetSendType()
	{
		return m_bEnterSend;
	}

}
