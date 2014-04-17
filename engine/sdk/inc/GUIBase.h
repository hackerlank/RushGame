#pragma once
#include "CDynamicObject.h"
#include "IGraphic.h"
#include "CallBackX.h"
#include "SQRGuiMallocObject.h"
#include "GUITextureHelper.h"

namespace sqr
{

#ifdef _USRDLL
#ifdef GUI_EXPORTS
#define GUI_API __declspec(dllexport)
#else
#define GUI_API __declspec(dllimport)
#endif
#else
#define GUI_API
#endif

#ifdef _STINT_MODE
#define TEXTURE_STINT_MODE
#endif
#define UI_HIDE_MODE
#define GUITEX_LOAD_ASYN

#define RENDERDLOG_FLUSH_MODE
#define DEF_MAX_BLOCK_NUM 32
enum
{
	AUF_VERSION = 10005,	//û�ĸ�ʽ��auf�汾��
	AUF_ADD_AUDIO,			//������Ч��
	AUF_ADD_MULTIFONT,
};

enum
{
	EDLG_FLUSH_TEX	,
	EDLG_FLUSH_FLASH,
	EDLG_FLUSH_FX	,
	EDLG_FLUSH_COUNT
};

class SQRWnd;
class SQRRichString;

	// ----------------------------------------------------------------------------------------
	// ��������
	// ----------------------------------------------------------------------------------------
	extern const float FLOAT_MAX_VALUE;
	extern const float FLOAT_MIN_VALUE;
	extern const float FLOAT_EPSILON;

	// ----------------------------------------------------------------------------------------
	// �궨��
	// ----------------------------------------------------------------------------------------
	
	/**
	\brief
		����������䵽����	
	*/
#define PixelAligned(x) ( (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) )

	// ----------------------------------------------------------------------------------------
	// ��������
	// ----------------------------------------------------------------------------------------

	//����������
#define WS_MODULE			0x80000000L		//ģ̬
#define WS_CHILD			0x40000000L		//�Ӵ���
#define WS_MSGTRANSPT		0x20000000L		//��Ϣ͸������
#define WS_AREAUSETEXRECT	0x10000000L		//��Ч����ʹ����ͼ��Χ
#define WS_MULLINETEXT		0x0f000000L		//�������
#define WS_TEXTYB			0x08000000L		//��������y����
#define WS_TEXTYC			0x04000000L		//��������y����
#define WS_TEXTXR			0x02000000L		//��������x����
#define WS_TEXTXC			0x01000000L		//��������x����
#define WS_TOPMOST			0x00800000L		//�����
#define WS_IGNOREWHEEL		0x00400000L		//���Թ�����Ϣ
#define WS_HORSCROLL		0x00200000L		//��ˮƽ������
#define WS_VERSCROLL		0x00100000L		//�д�ֱ������
#define WS_CUTBYPARENT		0x00080000L		//�������ڼ���
#define WS_NOTIFY			0x00040000L		//���Ӵ���WND_NOTIFY����Ȥ,Ҫ���Ӵ�������Ӧϵͳ��Ϣʱ����WND_NOTIFY��Ϣ
#define WS_TINYTEXTBACK		0x00020000L		//���ֱ߿�Ϊ��Ӱ��̬
#define WS_ITEM				0x00010000L     //Item�ؼ�(������Ⱦ˳��ı�) ---- add by lx
#define WS_NSITEM			0x00001000L     //�Ӵ��ڲ���ʾ ---- add by guo

	// ----------------------------------------------------------------------------------------
	// �ؼ��Զ������� 
	//

	// EDIT����
#define	ECS_MULTILINE		0x0001L			//EDIT�ؼ�����
#define ECS_PASSWORD		0x0002L			//EDIT�ؼ�Ϊ��������
#define ECS_ENABLECLIPBD	0x0004L			//EDIT�ؼ����ռ��а�����
#define ECS_DISABLEIME		0x0008L			//��ֹ���뷨����
#define ECS_YBORDER			0x00F0L			//EDIT����߽���
#define ECS_XBORDER			0x0F00L			//EDIT����߽���
#define ECS_DIGITAL			0x1000L			//EDIT�ؼ�Ϊ��������
#define ECS_STICK_BOTTOM	0x2000L			//EDIT�ؼ���ֱ��������������ճ��
#define ECS_UNSIGNEDINT		0x4000L			//EDIT�ؼ�Ϊ��������

	inline float GetXBorder( uint32 n )
	{
		return float( (n&ECS_XBORDER)>>8 );
	}
	inline float GetYBorder( uint32 n )
	{
		return float( (n&ECS_YBORDER)>>4 );
	}

	// ----------------------------------------------------------------------------
	// SCROLL����
	//
#define SS_HSCROLL			0x4000L			//Scroll�ؼ�ˮƽʽ��
#define AUTODOWN			0x0000000AL     //ֱ�ӻ������ڵײ���

	// ----------------------------------------------------------------------------
	// SPIN����
	//
#define PS_HSCROLL			0x4000L			//Spin�ؼ�ˮƽʽ��

	// ----------------------------------------------------------------------------
	// CheckButton����
	//
#define BTN_CHECK			0x0001L
#define BTN_RADIO			0x0002L

#define BTN_XOF				0xF000L			// button��check button ��title��Xƫ��
#define BTN_YOF				0x0F00L			// button��check button ��title��Yƫ��
	inline float GetXOF( uint32 n )
	{
		return float((n&BTN_XOF)>>12);
	}
	inline float GetYOF( uint32 n )
	{
		return float((n&BTN_YOF)>>8);
	}
	//-----------------------------------------------------------------------------------------
	// STATIC����
	//
#define STA_VERTICAL		0x0001L
	// ----------------------------------------------------------------------------------------
	// SQRProgress����
	//
#define PRG_FILLFIRST		0x0001L

	// ----------------------------------------------------------------------------
	// ListCtrl����
	//
#define LCTL_SCLSMOOTH		0x0002L
#define LCTL_SHOWSCLAUTO	0x0004L
#define LCTL_MULTSEL		0x0008L
#define LCTL_CLICKSEL		0x0010L
#define LCTL_SELFULLLINE	0x0020L
#define LCTL_SHOWGRID		0x0040L

	// ----------------------------------------------------------------------------
	// ComboCtrl����
	//
#define COMBO_DISEDIT		0x0001L

	// ----------------------------------------------------------------------------
	// TreeCtrl����
	//
#define TREE_FULLSELECT		0x0001L
#define TREE_HASLINE		0x0002L

	// ----------------------------------------------------------------------------
	// SQRRichText����
	//
#define RCHW_AUTORESETH			0x0001L
#define RCHW_STICK_BOTTOM		0x0002L					//��ֱ��������������ճ��
#define RCHW_ENABLE_SELECT		0x0004L					//����������ѡ��
#define RCHW_AUTOSETRECT_DOWN	0x0008L					//SQRRichText ������������������չ

	// ----------------------------------------------------------------------------
	// ������Ϣ
	//

	//�ؼ��Զ�����Ϣ
#define WND_NOTIFY					0x0001			//�ӿؼ���׼��Ϣ
#define	EDIT_RETURN					0x0101			//EDIT�ؼ��س���Ϣ
#define	EDIT_CTRLUP					0x0102			//EDIT�ؼ�CTRL+UP��Ϣ
#define	EDIT_CTRLDOWN				0x0103			//EDIT�ؼ�CTRL+DOWN��Ϣ
#define EDIT_CTRLRETURN				0x0104			//EDIT�ؼ�CTRL+�س���Ϣ
#define EDIT_ONCHAR					0x0105			//EDIT�ؼ������ַ���Ϣ
#define EDIT_BACK					0x0106			//EDIT�ؼ����˼���Ϣ
#define EDIT_DELETE					0x0107			//EDIT�ؼ�ɾ������Ϣ
#define EDIT_LOSEFOCUS				0x0108			//EDIT�ؼ�ʧȥ�༭������Ϣ
#define	BUTTON_CLICKUP				0x0201			//BUTTON�ؼ������Ϣ
#define	BUTTON_RIGHTCLICKUP			0x0202			//BUTTON�ؼ������Ϣ
#define	BUTTON_CLICKCONT			0x0203			//BUTTON�ؼ������Ϣ
#define	BUTTON_DRAG					0x0204			//BUTTON�ؼ������Ϣ
#define	BUTTON_CLICKDOWN			0x0205			//BUTTON�ؼ������Ϣ
#define	BUTTON_RIGHTCLICKDOWN		0x0206			//BUTTON�ؼ������Ϣ
#define BUTTON_MOUSEMOVE			0x0207			//BUTTON������ƶ�
#define BUTTON_LCLICK				0x0209			//BUTTONһ���������̧��
#define BUTTON_RCLICK				0x0210			//BUTTONһ���Ҽ�����̧��
#define SCROLL_MOVE					0x0301			//SCROLL�ؼ��ƶ�
#define SCROLL_RELEASE				0x0302			//SCROLL�ؼ��ͷŰ���
#define SPIN_DELTA					0x0401			//SCROLL�ؼ��ƶ�
#define ITEM_SELECT_CHANGED			0x0501			//Item���λ���ƶ�����һ��Item��
#define ITEM_RBUTTONUP			    0x0502			//Itemѡ��ı�
#define ITEM_SHIFTDOWN				0x0503			//Itemѡ��ı�
#define ITEM_SHIFTRDOWN				0x0504			//Itemѡ��ı�
#define ITEM_LBUTTONUP				0x0505			//Item������ƶ�
#define ITEM_CTRLDOWN				0x0506			//Item��Ctrl+������
#define ITEM_LBUTTONDBLCLICK		0x0507			//Item��˫�����
#define ITEM_LBUTTONCLICK			0x0508			//Item��һ���������̧��
#define ITEM_RBUTTONCLICK			0x0509			//Item��һ���Ҽ�����̧��
#define RICH_CLICK					0x0601			//���Rich�ؼ�
#define RICH_SCROLL					0x0602			//���Rich�ؼ�
#define RICH_CHOOSE					0x0603			//���Rich�ؼ�
#define RICH_RCLICK					0x0605			//���Rich�ؼ�
#define RICHWND_LCLICKUP			0x0606			//���Rich�ؼ�
#define RICHWND_LCLICKDOWN			0x0607			//���Rich�ؼ�
#define RICHWND_RCLICKUP			0x0608			//���Rich�ؼ�
#define RICHWND_RCLICKDOWN			0x0609			//���Rich�ؼ�
#define RICHWND_MOUSEOVER			0x0610			//���Rich�ؼ�
#define RICHWND_MOUSEOVERDOWN		0x0611			//���Rich�ؼ�
#define CILCK_WNDER					0x0701			//��������ؼ�����Ϣ
#define TREECTRL_RIGHTCLICK			0x0801			//�Ҽ�����TreeCtrl��Ϣ
#define LISTCTRL_LEFTCLICKUP		0x0901			//ListCtrl����ͷ�
#define LISTCTRL_RIGHTCLICKUP		0x0902			//ListCtrl�Ҽ��ͷ�
#define LISTCTRL_LEFTCLICK			0x0903			//ListCtrlһ���������̧��
#define LISTCTRL_RIGHTCLICK			0x0904			//ListCtrlһ���Ҽ�����̧��
#define ICON_LBUTTONDBLCLICK		0x0a01			//Icon˫��

#define WEBBROWSER_FORWARD			0x0b01
#define WEBBROWSER_LOADCOMPLETE		0x0b02
#define	WEBBROWSER_DOCCOMPLETE		0x0b03

	// ����������ɫ����
#define MASKCOLOR_TITLE				0xff000000		//������ɫ����
#define MASKCOLOR_AREA				0xffffffff		//������ɫ����

	//Base: SQRWnd,SQRButton,CheckButton,
#define SM_BS_BK					0x0001		//������ͼ

	//ScrollBar
#define SM_SC_BK					0x0001		//������ͼ
#define SM_SC_P1					0x0002		//����1
#define SM_SC_P2			        0x0003		//����2
#define SM_SC_BAR					0x0004		//����

	//Spin
#define SM_SP_P1					0x0001		//����1
#define SM_SP_P2			        0x0002		//����2

	//ComboBox
#define SM_CB_BK					0x0001		//Edit������ͼ
#define SM_CB_BT					0x0002		//Button������ͼ
#define SM_CB_LB_BK					0x0003		//ListBox������ͼ
#define SM_CB_SC_BK					0x0004		//������ͼ
#define SM_CB_SC_P1					0x0005		//����1
#define SM_CB_SC_P2					0x0006		//����2
#define SM_CB_SC_BAR			    0x0007		//����

	//ListCtrl
#define SM_LB_BK					0x0001		//ListBox������ͼ
#define SM_LB_SCH_BK				0x0002		//ˮƽ������������ͼ
#define SM_LB_SCH_P1				0x0003		//ˮƽ����������1
#define SM_LB_SCH_P2			    0x0004		//ˮƽ����������2
#define SM_LB_SCH_BAR				0x0005		//ˮƽ����������
#define SM_LB_SCV_BK				0x0006		//��ֱ������������ͼ
#define SM_LB_SCV_P1				0x0007		//��ֱ����������1
#define SM_LB_SCV_P2			    0x0008		//��ֱ����������2
#define SM_LB_SCV_BAR				0x0009		//��ֱ����������

	//RichWnd
#define SM_RW_BK					0x0001		//ListBox������ͼ
#define SM_RW_SCV_BK				0x0002		//��ֱ������������ͼ
#define SM_RW_SCV_P1				0x0003		//��ֱ����������1
#define SM_RW_SCV_P2			    0x0004		//��ֱ����������2
#define SM_RW_SCV_BAR				0x0005		//��ֱ����������


	//TreeCtrl
#define SM_TC_BK					0x0001		//TreeCtrl������ͼ
#define SM_TC_SCV_BK				0x0002		//��ֱ������������ͼ
#define SM_TC_SCV_P1				0x0003		//��ֱ����������1
#define SM_TC_SCV_P2			    0x0004		//��ֱ����������2
#define SM_TC_SCV_BAR				0x0005		//��ֱ����������

	//SQRClock
#define SM_CK_BK					0x0001      //SQRClock������ͼ
#define SM_CK_PG0_Bk				0x0002		//Progress[0]������ͼ
#define SM_CK_PG1_Bk				0x0003		//Progress[1]������ͼ
#define SM_CK_PG2_Bk				0x0004		//Progress[2]������ͼ
#define SM_CK_PG3_Bk				0x0005		//Progress[3]������ͼ

	//SQREdit
#define SM_CE_BK					0x0001		//SQREdit������ͼ
#define SM_CE_SCV_BK				0x0002		//��ֱ������������ͼ
#define SM_CE_SCV_P1				0x0003		//��ֱ����������1
#define SM_CE_SCV_P2			    0x0004		//��ֱ����������2
#define SM_CE_SCV_BAR				0x0005		//��ֱ����������

	//SQRProgress
#define SM_PG_BK					0x0001		//SQRProgress������ͼ
#define SM_PG_PD1					0x0002		//��ʾ��1������ͼ
#define SM_PG_BD2					0x0003		//��ʾ��2������ͼ

	//SQRVideo
#define SM_VO_BK					0x0001      //SQRClock������ͼ
#define SM_VO_B1					0x0002		//��������
#define SM_VO_B2					0x0003		//������ͣ
#define SM_VO_B3					0x0004		//�����ز�

	//////////////////////////////////////////////////////////////////////////
	// CheckButton״̬
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// MessageBox
	//////////////////////////////////////////////////////////////////////////
	//ʽ��
#define     MB_BtnNone              0x0000
#define     MB_BtnOK                0x0001
#define     MB_BtnCancel            0x0002

	//////////////////////////////////////////////////////////////////////////
	// �ı����뷽ʽ
	//////////////////////////////////////////////////////////////////////////
#define _TEXT_FMT_LEFT				0x00003001L
#define _TEXT_FMT_RIGHT				0x00003002L
#define _TEXT_FMT_CENTER			0x00003003L

	//////////////////////////////////////////////////////////////////////////
	// ���״̬
	//////////////////////////////////////////////////////////////////////////
#define _CURSOR_NORMAL				0x00000000L
#define _CURSOR_GLOBAL				0x00000001L
#define _CURSOR_EXCLUSIVE			0x00000002L


	// ������־
#define		LOCK_LEFT				0x0001L		//����
#define		LOCK_RIGHT				0x0002L		//����
#define		LOCK_TOP				0x0004L		//����
#define		LOCK_BOTTOM				0x0008L		//����
#define		LOCK_ALL				0x000FL		//�ı�����
#define		LOCK_UNZOOM				0x0010L		//������

	// ----------------------------------------------------------------------------------------
	// GUI�����ṹ
	// ----------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	// StateMask
	//
	enum EventStateMask
	{
		IP_MASK					= 0,	//
		IP_MOUSEDESC			= 1,	//0x0001	//�����������
		IP_ENABLE				= 2,	//0x0002	//������ͼ
		IP_DISABLE				= 3,    //0x0003	//������ͼ
		IP_MOUSEOVER			= 4,    //0x0004	//BUTTON�ؼ�MOUSEOVER��ͼ
		IP_CLICKDOWN			= 5,    //0x0005	//BUTTON�ؼ�CLICKDOWN��ͼ
		IP_UNCHECK_ENABLE		= 6,    //0x0006	//BUTTON�ؼ�Uncheck Normal��ͼ
		IP_UNCHECK_DISABLE		= 7,    //0x0007	//BUTTON�ؼ�Uncheck Disable��ͼ
		IP_UNCHECK_MOUSEOVER	= 8,    //0x0008	//BUTTON�ؼ�Uncheck MOUSEOVER��ͼ
		IP_UNCHECK_CLICKDOWN	= 9,    //0x0009    //BUTTON�ؼ�Uncheck CLICKDOWN��ͼ
		IP_FILL					= 10,   //0x000A    //Progress�ؼ�����ͼ
		IP_TR_ENABLE			= 11,
		IP_TR_DISABLE			= 12,
		IP_LS_SEL				= 13,
		IP_LS_UNSEL				= 14,
		IP_FILL_FULL			= 15,
		IP_LBTNCLICK_ACTION		= 16,
		IP_RBTNCLICK_ACTION		= 17,
		IP_COUNT
	};

	enum GuiBlockMask
	{
		GBM_TOP		= 0,
		GBM_LEFT	= 1,
		GBM_RIGHT	= 2,
		GBM_BOTTOM	= 3,
		GBM_CENTER	= 4,
		GBM_TOP_LEFT	= 5,
		GBM_TOP_RIGHT	= 6,
		GBM_BOTTOM_LEFT	= 7,
		GBM_BOTTOM_RIGHT= 8,
		GBM_COUNT		= 9,
	};

	enum GUIDrawState
	{
		eDraw_No			= 0,
		eDraw_Start			= 1,
		eDraw_End			= 2,
		EDraw_StateCount	= 3,
	};
	// ----------------------------------------------------------------------------
	// CtrlMask
	//
	struct IMAGE_PARAM : public SQRGuiMallocObject
	{
		uint16	CtrlMask;
		uint16	StateMask;
		IMAGE_PARAM():CtrlMask(0),StateMask(0) {}
		IMAGE_PARAM( uint16 CtrlMask, uint16 StateMask ):CtrlMask(CtrlMask),StateMask(StateMask) {}
	};

	///effect message
	enum EMsgFx
	{
		eMF_Click	= 0,			//���
		eMF_Open	= 1,			//��
		eMF_Close	= 2,			//�ر�

		eMF_Count,
	};

	///sound message
	enum ESoundMsg
	{
		eMS_Click		= 0,			//���
		eMS_Open		= 1,			//��
		eMS_Close		= 2,			//�ر�
		eMS_MouseOver	= 3,			//Mouse Over

		eMS_Count,
	};

	// effect run kind
	enum EEffectRun
	{
		eEffRun_Normal		= 0,		// ��������
		eEffRun_Repeat		= 1,		// ѭ������
		eEffRun_Last		= 2,		// ���һ֡����
		eEffRun_Count,
	};

	// һ��GUIЧ����Ӧһ��Ч����
	// ͬ���Ч��������ͬʱ����
	enum EGUIEffect
	{
		eEff_Shape			= 0,		// ��������
		eEff_Transparent	= 1,		// �α�
		
		eEff_Count,
	};

	//struct GUIRenderEvn
	//{
	//	uint32			uAmbient;
	//	float			fFovy;
	//	CVector3f		vecCameraPos;
	//	CVector3f		vecCameraDir;
	//};

	class GUI_EVENT_FONT : public SQRGuiMallocObject
	{
	public: 
		GUI_EVENT_FONT(){ m_FontColor = 0; m_uFontBkColor = 0;}
		uint32			m_FontColor;
		uint32			m_uFontBkColor;
	};
	
	class GUI_FONT : public SQRGuiMallocObject
	{
	public:
		GUI_FONT(){ m_FontSize = 14; m_uFontGradualColor = 0;}
		
		int32			m_nFontID;					// ������
		uint32			m_FontSize;					// �����С

		typedef	UIMap	<EventStateMask,GUI_EVENT_FONT> EvtFontVector;
		GUI_EVENT_FONT* GetEventFont(EventStateMask esm);
		EvtFontVector	m_EventFont;
		uint32			m_uFontGradualColor;
		uint32			m_uFontEffectMask;
	};

	class GUI_BLOCK : public SQRGuiMallocObject
	{
	public:
		typedef UIVector<CFRect> BLOCK_RECT;
		BLOCK_RECT	m_BlockRect;
		GUI_BLOCK() { m_BlockRect.resize(GBM_COUNT); };
		inline CFRect& operator [] (UINT i) { return m_BlockRect[i]; }
	};

	class BLOCK_POOL
	{
	public:
		typedef UIVector<GUI_BLOCK*> RectVector;
		RectVector	m_vecRect;
		static const uint32 MAX_BLOCK_NUM;
		BLOCK_POOL();
		~BLOCK_POOL();
		inline GUI_BLOCK* operator[] (UINT i) { return m_vecRect[i-1]; }//1~MAX_BLOCK_NUM
		GUI_BLOCK& InitBlock(UINT i);
	};

	class StyleStruct : public SQRGuiMallocObject
	{
	public:
		StyleStruct();
		~StyleStruct() { Release(); }

		void Release(void);

		typedef	UIMap<EventStateMask,BLOCK_POOL> PoolVector;
		typedef UIMap<int32,GUI_FONT*>	FontVector;
		ITexture*	pTexture;
		UIString	m_TexString;
		DWORD		m_dwColor;
		PoolVector	m_vecPool;
		FontVector	m_vecFont;
	};

	struct IMAGE : public SQRGuiMallocObject
	{
		IMAGE()
			: pTexture(0)
			, dwColor(0)
			, dwLockFlag(0)
			, bWidthNegtvDir(false)
			, bHeightNegtvDir(false)
		{
		}
		UIString		texName;		// ��ͼ����
		ITexture*		pTexture;		// ��ͼ
		CFRect			rtTexture;		// ��ͼʹ������
		CFPos			posWnd;			// ��ͼʹ���������Ͻ��ڴ����е�λ��
		uint32			dwColor;		// ��ͼ������ɫ
		uint32			dwLockFlag;		// ��ͼͣ�������ķ���
		bool			bWidthNegtvDir;
		bool			bHeightNegtvDir;

		void SetColor(uint32 uColor)
		{
			dwColor = uColor;
		}
		void	SetPosWndX(float nX)
		{
			posWnd.x=nX;
		}
		float	GetPosWndX()const
		{
			return posWnd.x;
		}
		void	SetPosWndY(float nY)
		{
			posWnd.y=nY;
		}
		float	GetPosWndY()const
		{
			return posWnd.y;
		}
		const char* GetTexName()
		{
			/*if(pTexture)
				return pTexture->GetName();
			else
				return "";*/
			return texName.c_str();
		}
	};
	struct VEC_IMAGE
	{
		UIVector<IMAGE> Image;
	};

	class IGUIHandler : public SQRGuiMallocObject
	{
	public:
		typedef pair<SQRWnd*, UIString> EffectKey;
		virtual void	OnEnableIme( bool bEnable ) = 0;
		virtual void	OnPlayFx( SQRWnd* pWnd, const char* szFxName, EEffectRun eEffrun = eEffRun_Normal ) = 0;
		virtual void*	OnPlayFxIme( SQRWnd* pWnd, const char* fxName, EEffectRun eEffrun = eEffRun_Normal ) = 0;
		virtual void	OnPlayFxEx( SQRWnd* pWnd, const char* fxFileName, const char* fxName, EEffectRun eEffrun = eEffRun_Normal ) = 0;
		virtual void*	OnPlayFxImeEx( SQRWnd* pWnd, const char* fxFileName, const char* fxName, EEffectRun eEffrun = eEffRun_Normal ) = 0;
		virtual void	OnClearFx( SQRWnd* pWnd, const char* fxName ) = 0;
		virtual void	OnClearFx( SQRWnd* pWnd ) = 0;
		virtual void	OnClearImeFx( SQRWnd* pWnd, const char* fxName ) = 0;
		virtual void	OnUpdateFx(uint curTime, float zoom) = 0;
		virtual void	OnWndSized(const CFRect& rect) = 0;
		virtual bool	OnRenderFx( EffectKey& key, uint curTime, float zoom ) = 0;
	};

	class IHook	
		: public virtual CDynamicObject
		, public SQRGuiMallocObject
	{
	public:
		IHook();
		~IHook();
		virtual bool OnHook( SQRWnd* pWnd, uint32 message, WPARAM wParam, LPARAM lParam );
	};

	class IAccelerator 
		: public virtual CDynamicObject
		, public SQRGuiMallocObject
	{
	public:
		IAccelerator();
		~IAccelerator();
		virtual bool OnAccelerator( uint32 message, WPARAM wParam, LPARAM lParam )
		{
			CALL_CLASS_CALLBACK_3_RET(bool, message, wParam, lParam)
			return false;
		};
		virtual void OnAcceleratorInEdit() { CALL_CLASS_CALLBACK() };
		virtual void OnAcceleratorIsModule(uint32 message, WPARAM wParam, LPARAM lParam) { CALL_CLASS_CALLBACK_3(message,wParam,lParam) };
	};
	class GUI_API WND_IMAGE_LIST
		: public virtual CDynamicObject
		, public SQRGuiMallocObject
	{
		VEC_IMAGE*			m_pImageList;
		bool				m_IsAsynEnable;

	public:
		WND_IMAGE_LIST( const WND_IMAGE_LIST& ImageList );
		WND_IMAGE_LIST();
		~WND_IMAGE_LIST();

		void			Release();
		void			SetGUIAsynEnable(bool enable);
		bool			IsImageLoadValid();
		int32			AddImage( IGraphic* pGraphic, int32 nImageIndex, const char* szTextName,
			const CFRect* prt, const CFPos& posWnd, uint32 uColor, uint32 uLockFlag = 0 );
		int32			AddImageWin( IGraphic* pGraphic, int32 nImageIndex, const char* szTextName,
			const CFRect* prt, const CFPos& posWnd, uint32 uColor, uint32 uLockFlag = 0 );

		int32			AddImageFromImageList( const WND_IMAGE_LIST & Imagelist, int32 nIndexBegin, int32 nIndexEnd );

		int32			GetImageCount() const;
		IMAGE&			GetImage( int32 nImageIndex )const;
		void			GetImage(int32 nImageIndex, IMAGE& image);
		void			DeleteImage( int32 nImageIndex );
		int32			DetectImage( const CFPos& CursorInWnd );
		void			ClearImage();
		void			Scale( float xDel, float yDel );

		void			ReleaseTex();
		void			CreatTex( IGraphic* pGraphic, GUITextureHelper* texhelp = NULL );
	
		const WND_IMAGE_LIST& operator = ( const WND_IMAGE_LIST& ImageList );
	};

	//class GUI_API WND_RENDER_OBJ 
	//	: public virtual CDynamicObject
	//	, public SQRGuiMallocObject
	//{
	//	HANDLE			m_hRenderObj;

	//public:
	//	WND_RENDER_OBJ( const WND_RENDER_OBJ& RenderObj );
	//	WND_RENDER_OBJ();
	//	~WND_RENDER_OBJ();

	//	void			Release();
	//	bool			IsEmpty() const;
	//	const CVector2f&	GetDir()const;
	//	void			GetDir(CVector2f& vecDir);
	//	void			SetDir( const CVector2f& vecDir );
	//	float			GetScale() const;
	//	void			SetScale( float fScale );
	//	CVector3f		GetOff() const;
	//	void			GetOff(CVector3f& vecOff);
	//	void			SetOff( const CVector3f& vecOff );
	//	void			SetNextAnimation( const char* AniName, const FramePair& FmPair, bool bLoop = true, int32 DelayTime = 200 /*����*/ );
	//	void			AddRenderObj( SQRWnd* pAnyWnd, const char* sArpFile, const char* sClass,
	//		uint32 uRS, const char* sAraFile, const char* sFxFile, const char* sFxName );
	//	AniResult		Render( const CMatrix& matWorld, uint32 uCurTime ) const;

	//	const WND_RENDER_OBJ& operator = ( const WND_RENDER_OBJ& RenderObj );
	//};
}
