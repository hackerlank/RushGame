#pragma once
#include "SQREdit.h"
#include "SQRButton.h"
#include "SQRListCtrl.h"

/////////////////////////////////////////////////////////
// class SQRComboBox
/////////////////////////////////////////////////////////

namespace sqr
{
	class GUI_API SQRComboBox : public SQREdit
	{
		DYNAMIC_DECLARE_WND( SQRComboBox )

	public:
		SQRComboBox();
		virtual ~SQRComboBox();

	public:
		//����
		virtual int32			Create( const WndCreateParam& param );
		//������
		virtual void			DrawWndBackground();
		//������
		virtual void			DrawWndText();
		//�����Ӵ���
		virtual void			DrawAfterChildren();
		virtual void			OnCreatedAfter();

	public:
		int32					InsertItem( const char* sText, int32 nItem = -1, float fHeight = -1.0f );
		SQRWnd*					GetItem( int32 nItem );
		void					RemoveAllItem();
		void					RemoveItem( int32 nItem );
		void					RemoveItemByText( const char* sText );

		void					SetItemHeight( float fItemHeight, int32 nItem = -1 );
		int32					GetCurrentSel();
		void					SetSel( int32 nItem );

		const char*				GetSelText();
		const char*				GetItemText( int32 nItem );

		virtual WND_IMAGE_LIST* GetWndBkImage( const IMAGE_PARAM& ImageFlag );

		virtual uint32			GetWndTextColor( const IMAGE_PARAM& ImageFlag );
		virtual void			SetWndTextColor( const IMAGE_PARAM& ImageFlag, uint32 uColor );

		virtual uint32			GetWndTextBcColor( const IMAGE_PARAM& ImageFlag );
		virtual void			SetWndTextBcColor( const IMAGE_PARAM& ImageFlag, uint32 uColor );

		virtual SQRWnd*			GetResWnd( int32 nCtrlMask );

		static bool				IsHasEventState(EventStateMask Mask);
		virtual bool			ObjHasEventState(EventStateMask Mask);
		int32					GetItemCount();
		int32					InsertColumn(int32 nCol, float fWidth);
		void					InsertDefaultColumn();
	protected:
		//��Ӧ�Ӵ���
		virtual void			OnCtrlmsg( SQRWnd* pChild, uint32 uMsgID, uint32 uParam1, uint32 uParam2 );
		//�״̬�ı�
		virtual void			OnActive( bool bActive, SQRWnd* Wnd);
		//�����ڴ�����Ϣ�ĺ���
		virtual LRESULT			WndProc( uint32 message, WPARAM wParam, LPARAM lParam );

		virtual void			OnSelChanged() {};

	protected:
		SQRButton         m_Button;
		SQRListCtrl       m_ListBox;
	};
}