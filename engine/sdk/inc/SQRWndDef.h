#pragma once
#include "SQRGuiMallocObject.h"
#include "CPos.h"
#include "CRectangle.h"
#include "GUIBase.h"
#include "SQRRichToolTip.h"

namespace sqr
{
	class SQRGUIManager;
	class SQRToolTip;
	class SQRWnd;
	class SQRRichToolTip;

	struct WindowImage : public SQRGuiMallocObject
	{
		WindowImage();// : dwLockFlage(0), dwColor(0) {}
		UIString	szTexture;
		RECT		rtScale;		// ��ͼʹ������
		CFPos		posWnd;			// ��ͼʹ���������Ͻ��ڴ����е�λ��
		uint32		dwColor;		// ��ͼ������ɫ
		uint32		dwLockFlage;	// ��ͼ������־
	};

	struct ChildImageList : public SQRGuiMallocObject
	{
		ChildImageList();
		int32						nStateMask;
		uint32						uTextColor;
		uint32                      uRimColor;			// ����߿���ɫ
		UIVector<WindowImage>	ImageList;
	};

	struct ChildWndInfo : public SQRGuiMallocObject
	{
		ChildWndInfo();// : m_uStyleIndex(0),m_uLockFlag(LOCK_LEFT|LOCK_TOP ){};
		CFRect						rtWnd;
		uint32						m_uStyleIndex;
		uint32						m_uLockFlag;
		UIVector<ChildImageList>	ChildBkList;
	};

	// �ýṹ�����ڳ����д���������Ҫ������Դ�м���
	struct FlashWndInfo : public SQRGuiMallocObject
	{
		FlashWndInfo();
		~FlashWndInfo();
		bool Init(IGraphic* pGraphic, UIString flashname, UIString texname, CFRect Texpos, uint32 time,
				  uint32 ct, float amin, float amax, CFRect repos);
		void Destroy();
		void CreateTexture();
		void ReleaseTexture();

		UIString				m_FlashName;		// ����
		IMAGE					m_image;			// ��ͼ��Ϣ
		uint32					m_count;			// ��˸������0Ϊһֱ��˸
		uint32					m_time;				// ����ʱ��, ����

		// ���Ϊ����˸
		float					m_Amin;				// ���͸���� ��СΪ0F
		float					m_Amax;				// ��С͸���� ���Ϊ1F
		CFRect					m_repos;			// У��λ��
		bool					m_bAdd;				// ͸�����Ƿ�����

		int32					m_iRef;				// ʹ�ô���
	};

	struct WndCreateParam : public SQRGuiMallocObject
	{
		const char*		szTitle;
		uint32			uStyle;
		float			x;
		float			y;
		float			width;
		float			height;
		SQRWnd*			pParentWnd;
		int32			nFontID;
		bool			bBold;
		float			font_size;
		uint32			uGroupID;
		const char*		szDesc;
		const char*		szSource;//add by guo

		WndCreateParam()
			: szTitle("")
			, uStyle(0)
			, x(0.0f)
			, y(0)
			, width(0.0f)
			, height(0.0f)
			, pParentWnd(NULL)
			, nFontID(-1)
			, bBold(false)
			, font_size(0.0f)
			, uGroupID(-1)
			, szDesc("")
			, szSource("")
		{}

		void SetParentWnd(SQRWnd* pWnd){ pParentWnd = pWnd; }
		SQRWnd* GetParentWnd() const { return pParentWnd; }
	};



	struct WndResParam : public SQRGuiMallocObject
	{
		char*									m_ClassName;
		char*									m_WindowTitle;              //���ڱ���
		uint32									m_WndStyle;                 //��������
		int32									m_x;						//�����ڸ����ڵ�ˮƽ����
		int32									m_y;						//�����ڸ����ڵĴ�ֱ����
		int32									m_Width;                    //���ڵĿ��
		int32									m_Height;                   //���ڵĸ߶�
		int32									m_nFontID;					//������
		uint32									m_FontSize;					//�����С
		bool									m_bBold;					//�����Ƿ�Ӵ�
		uint32									m_uFontGradualColor;
		uint32									m_uFontEffectMask;
		bool									m_isShowBackground;
		uint32									m_uStyleIndex;
		char*									m_TransTexture;
		uint32									m_uLockFlage;				//����������־
		uint32									m_GroupID;					//��ID
		char*									m_szDesc;					//����ƹ�����ʱ����Ϣ��ʾ
		char*									m_MsgFx[eMF_Count];			//��Ϣ��Ч�ַ���
		UIString								m_MsgSound[eMS_Count];		//��Ϣ��Ч�ַ���
		UIMap<EventStateMask, UIString>			m_FlashWndStr;				//��˸������Դ��ͬ״̬
		char*									m_UseClassName;				//�Զ�����
		char*									m_UseInfo;					//�Զ�����Ϣ
		char*									m_WindowName;
		char*									m_SourceName;				//add by Guo
		UIVector<ChildWndInfo>					m_ImageList;				//ͼƬ�б�
		UIMap<UIString, WndResParam*>			m_Child;
		bool									m_beAucRes;					//��AUC��Դ����

		WndResParam() 
			: m_ClassName(NULL)
			, m_isShowBackground(true)
			, m_uStyleIndex(0)
			, m_FontSize(14)
			, m_beAucRes(true)
			, m_uFontGradualColor(0)
		{
			InitTChar();
		}
		~WndResParam();

		void InitTChar();
		void UninitTChar();

	private:
		const WndResParam& operator= ( const WndResParam& )
		{
			return *this;
		}
		WndResParam( const WndResParam& ) {}
		void clearTChar( char* str);
	};

	struct WndHandle : public SQRGuiMallocObject
	{
	public:
		WndHandle();
	public:
		SQRGUIManager*				m_pGUI;						//���ڹ�����
		uint32						m_WndStyle;                 //��������
		UIString					m_WindowTitle;              //���ڱ���
		UIString					m_WindowName;				//�������֣���Դ����Դ���ߴ��ڱ���
		UIWString					m_WindowTitleW;
		UIWString					m_WindowNameW;
		float						m_xScreen;                  //�����ڸ����ڵ�ˮƽ����
		float						m_yScreen;                  //�����ڸ����ڵĴ�ֱ����
		float						m_Width;                    //���ڵĿ��
		float						m_Height;                   //���ڵĸ߶�

		float						m_xScreenbak;               //�����ڸ����ڵ�ˮƽ����
		float						m_yScreenbak;               //�����ڸ����ڵĴ�ֱ����
		float						m_Widthbak;                 //���ڵĿ��
		float						m_Heightbak;                //���ڵĸ߶�


		float						m_xInitial;                 //���ڳ�ʼ��ˮƽ����
		float						m_yInitial;                 //���ڳ�ʼ�ĵĴ�ֱ����
		float						m_WidthInitial;             //���ڳ�ʼ�ĵĿ��
		float						m_HeightInitial;            //���ڳ�ʼ�ĵĸ߶�
		int32						m_GroupID;					//
		uint32						m_uLockFlage;				//����������־
		float                       m_fTransparent;             //����͸����
		uint32						m_StyleIndex;				//�������
		UIString					m_szSource;					//��Դ���ƣ��ֶ���Ƶ�ؼ����ã�//add by guo

		bool						m_bIsShowBackground;		//���ڱ����Ƿ���ʾ
		bool						m_bEnable;					//�����Ƿ���Ч
		bool						m_bShow;					//�����Ƿ���ʾ
		bool						m_bIsHeld;					//�����Ƿ񱻵�ס
		bool						m_IsActive;					//�����Ƿ�״̬
		bool						m_bNewByGUISys;				//��GUIϵͳ��̬������Ҳ��������Դ�����Ĵ���
		bool						m_bGetMouseWheel;			//���ڵõ������֣���ݼ����Թ�����Ϣ
		bool						m_bIsDownTooltips;			//tooltips�Ƿ�������·���ʾ

		float						m_FontSize;					//���������С
		CRenderFont*				m_WndFont;                  //��������

		float						m_TipFontSize;				//����ToolTips�����С

		ITexture*					m_pTransparenTex;			//������Ϣ͸������
		UIString					m_strTransName;				//������Ϣ͸������·��

		float						m_fZ;						//��Ⱦ�����
		ERenderStyle				m_eRS;						//��Ⱦ����

		uint32						m_dwHeldTime;
		CFPos						m_CurHeldPos;				//���ڱ���ס��λ��

		WND_IMAGE_LIST				m_Title;					//����ͷλ��
		WND_IMAGE_LIST				m_Enable;					//������ͼ
		WND_IMAGE_LIST				m_Disable;					//������ͼ

		uint32						m_EnableColor;              //���ڻʱ������ɫ
		uint32						m_DisableColor;				//���ڷǻʱ������ɫ

		uint32						m_EnableBcColor;            //���ڻʱ����߿���ɫ
		uint32						m_DisableBcColor;           //���ڻʱ����߿���ɫ

		uint32						m_FontGradualColor;			///< ���彥��ɫ
		uint32						m_FontEffectMask;			///< ����Ч��

		CFPos						m_posMouseOver;
		WND_IMAGE_LIST				m_MouseDesc;				//Desc��ʾ������Դ
		SQRToolTip*					m_DescRichText;				//����ƹ�����ʱ����Ϣ��ʾ
		SQRRichToolTip*				m_pRichToolTip;				//��������tooltip
		UIWString					m_szDescW;
		UIString					m_MsgFx[eMF_Count];			//��Ϣ��Ч
		UIString					m_MsgSound[eMS_Count];		//��Ϣ��Ч
		UIMap<EventStateMask, FlashWndInfo*>	m_FlashInfo;	//��˸��Ϣ

		SQRWnd*						m_ParentWnd;                //������
		SQRWnd*						m_FirstChild;				//��Ӵ���
		SQRWnd*						m_LastChild;                //����Ӵ���
		SQRWnd*						m_PreBrother;               //ͬ��ǰ����
		SQRWnd*						m_NextBrother;              //ͬ���󴰿�

		SQRWnd*						m_pNextModule;				//��һ��ģ̬����
		HCURSOR						m_hCursor;                  //���

		uint32						m_dwOtherData;              //��������������
		WndResParam*				m_pResParam;				//��Դ��������
		bool						m_bTexInit;					//������Դ�Ƿ��״γ�ʼ��

		UIString					m_szClassName;
		UIString					m_szCreateInfo;
		UIString					m_szWindowName;
	};

	struct ChildResInfo : public SQRGuiMallocObject
	{
		ChildResInfo(const UIString& strTitle, const UIString& strDesc)
			: szTitle(strTitle), szDesc(strDesc)
		{}
		UIString	szTitle;
		UIString	szDesc;
	};

	struct ResInfo : public SQRGuiMallocObject
	{
		typedef UIMap<const UIString, ChildResInfo> ChildInfoMap;
		
		ResInfo(const UIString& strTitle, const UIString& strDesc, const ChildInfoMap& Map)
			: szTitle(strTitle), szDesc(strDesc), mapChildInfo(Map)
		{}
		UIString		szTitle;
		UIString		szDesc;
		ChildInfoMap	mapChildInfo;
	};

}// namespace sqr
