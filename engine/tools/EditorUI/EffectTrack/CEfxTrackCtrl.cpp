#include "StdAfx.h"
#include "CEfxTrackCtrl.h"
#include "CEfxTrackCtrlImp.h"
#include "CEfxEditProp.h"
#include "EffectKeyUint/CEfxKeyUintCtrl.h"

#define TRACK_HIGHT_NORAML		35
#define TRACK_HIGHT_EXPAND		120	

namespace sqr_tools
{
	char* CEfxTrackCtrl::GetImpTypeName( void )
	{
		return GET_IMP_NAME(CEfxTrackCtrlImp);
	}

	void CEfxTrackCtrl::UpdateTrackInfo(void)
	{
		// ������ʾ����,�������ݣ���ɫ
	}

	System::Void  CEfxTrackCtrl::InitOtherComponent()
	{
		m_KeyCtrls = gcnew System::Collections::Generic::List<CEfxKeyUintCtrl^>();
		m_uCtrlState = TRACKCTRL_STATE_NORMAL;
	}

	System::Void  CEfxTrackCtrl::DestroyOtherComponent()
	{
		
	}

	void  CEfxTrackCtrl::SetTrackContent(void* content, void* editprop)
	{
		// ��������
		CEfxTrackCtrlImp* EfxFileimp = (CEfxTrackCtrlImp*)m_pCtrl;
		EfxFileimp->SetEditTrack(content, editprop);
	}

	void  CEfxTrackCtrl::ClearKeys()
	{
		this->SuspendLayout();
		for( int32 i = 0; i < m_KeyCtrls->Count; ++i )
			delete m_KeyCtrls[i];
		m_KeyCtrls->Clear();
		this->ResumeLayout(false);
	}

	void  CEfxTrackCtrl::RefreshTrack()
	{
		ClearKeys();
		// ��ɱ�׼����£��߶ȣ���Ⱥ������ݵ�����,����ʾ��Щ����
		int iWidth = this->Parent->Width;
		this->Size = System::Drawing::Size(iWidth, TRACK_HIGHT_NORAML);

		// �������ݣ�����
		this->SuspendLayout();
		CEfxTrackCtrlImp* EfxTrackimp = (CEfxTrackCtrlImp*)m_pCtrl;
		int32 ct = EfxTrackimp->GetKeyCount();
		for (int32 i = 0; i < ct; ++i)
		{
			void*	editKey = EfxTrackimp->GetTrackKey(i);
			// ����һ��KEY�ؼ�
			CEfxKeyUintCtrl^ pKeyCtrl = gcnew CEfxKeyUintCtrl();
			this->Controls->Add(pKeyCtrl);
			pKeyCtrl->SetKeyContent(editKey);
			
			m_KeyCtrls->Add( pKeyCtrl );
		}
		this->ResumeLayout(false);
	}

	void  CEfxTrackCtrl::RefreshTrackPos()
	{
		int32 iLength = this->Width;
		CEfxTrackCtrlImp* EfxTrackimp = (CEfxTrackCtrlImp*)m_pCtrl;
		int posX, iwidth, posY = 1, iheight = TRACK_HIGHT_NORAML - 2;
		for( int32 i = 0; i < m_KeyCtrls->Count; ++i )
		{
			// ����ÿ��KEY������,��Ҫ��POS
			
			CEfxEditProp^ editProp = static_cast<CEfxEditProp^>(this->Parent->Parent);
			float scale = editProp->GetCurScale();
			EfxTrackimp->GetKeyPosXAndWidth(posX, iwidth, i,  iLength, scale);

			m_KeyCtrls[i]->Location = System::Drawing::Point(posX, posY);
			m_KeyCtrls[i]->Size = System::Drawing::Size(iwidth, iheight);
		}
	}

	void  CEfxTrackCtrl::RefreshTrackState(bool ctrl)
	{
		if (ctrl)
		{
			if (m_uCtrlState & TRACKCTRL_STATE_SEL || m_uCtrlState & TRACKCTRL_STATE_SELS)
			{
				m_uCtrlState = (m_uCtrlState & (~TRACKCTRL_STATE_SEL)) | TRACKCTRL_STATE_SELS;
			}
			else if(m_uCtrlState & TRACKCTRL_STATE_SELPRE)
			{
				m_uCtrlState = (m_uCtrlState & (~TRACKCTRL_STATE_SELPRE)) | TRACKCTRL_STATE_SEL;
			}
		}
		else
		{
			if (m_uCtrlState & TRACKCTRL_STATE_SELPRE)
			{
				m_uCtrlState = (m_uCtrlState & (~TRACKCTRL_STATE_SELPRE)) | TRACKCTRL_STATE_SEL;
			}
			else
				m_uCtrlState = m_uCtrlState & (~TRACKCTRL_STATE_SEL) & (~TRACKCTRL_STATE_SELS);
		}

		// ������ɫ
		RefreshTrackColor();
	}

	void  CEfxTrackCtrl::RefreshTrackColor()
	{
		if ((m_uCtrlState & TRACKCTRL_STATE_HIDE) && (m_uCtrlState & TRACKCTRL_STATE_SEL) && (m_uCtrlState & TRACKCTRL_STATE_SELS))
		{
			// ���صģ�ѡ�еģ���ѡѡ�е�
			this->BackColor = System::Drawing::Color::OliveDrab;
		}
		else if ((m_uCtrlState & TRACKCTRL_STATE_HIDE) && (m_uCtrlState & TRACKCTRL_STATE_SEL))
		{
			// ���صģ�ѡ�е�
			this->BackColor = System::Drawing::Color::OliveDrab;
		}
		else if ((m_uCtrlState & TRACKCTRL_STATE_HIDE) && (m_uCtrlState & TRACKCTRL_STATE_SELS))
		{
			// ���صģ���ѡѡ�е�
			this->BackColor = System::Drawing::Color::OliveDrab;
		}
		else if((m_uCtrlState & TRACKCTRL_STATE_SELS) && (m_uCtrlState & TRACKCTRL_STATE_SEL))
		{
			// ��ѡѡ�еģ�ѡ�е�
			this->BackColor = System::Drawing::Color::RoyalBlue;
		} 
		else if(m_uCtrlState & TRACKCTRL_STATE_SELS)
		{
			// ��ѡѡ�е�
			this->BackColor = System::Drawing::Color::CornflowerBlue;
		}
		else if(m_uCtrlState & TRACKCTRL_STATE_SEL)
		{
			// ѡ�е�
			this->BackColor = System::Drawing::Color::RoyalBlue;
		}
		else if(m_uCtrlState & TRACKCTRL_STATE_HIDE)
		{
			// ���ص�
			this->BackColor = System::Drawing::Color::YellowGreen;
		}
		else
		{
			// ƽ����
			this->BackColor = System::Drawing::Color::LightSteelBlue;
		}

		// ˢ��KEY
		for( int32 i = 0; i < m_KeyCtrls->Count; ++i )
			m_KeyCtrls[i]->RefreshKeyState();
	}

	void  CEfxTrackCtrl::OnTrackSelected()
	{
		m_uCtrlState |= TRACKCTRL_STATE_SELPRE;
		
		CEfxEditProp^ editProp = static_cast<CEfxEditProp^>(this->Parent->Parent);
		editProp->UpdateTracksBG(false);
	}

	void  CEfxTrackCtrl::OnTrackSelectedLots()
	{
		int bHide = m_uCtrlState & TRACKCTRL_STATE_HIDE;
		int bSels = m_uCtrlState & TRACKCTRL_STATE_SELS;
		if (bSels != 0)
			m_uCtrlState = bHide;
		else
			m_uCtrlState = bHide | TRACKCTRL_STATE_SELPRE;

		CEfxEditProp^ editProp = static_cast<CEfxEditProp^>(this->Parent->Parent);
		editProp->UpdateTracksBG(true);
	}

	void  CEfxTrackCtrl::OnKeyUintEdit(void* content)
	{
		CEfxEditProp^ editProp = static_cast<CEfxEditProp^>(this->Parent->Parent);
		editProp->OnKeyUintEdit(content);
	}

	System::Void CEfxTrackCtrl::CEfxTrackCtrl_MouseClick( System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e )
	{
		System::Windows::Forms::MouseButtons mbt = e->Button;
		switch(mbt)
		{
		case System::Windows::Forms::MouseButtons::Middle:
				
			break;
		case System::Windows::Forms::MouseButtons::Right:
		case System::Windows::Forms::MouseButtons::Left:
			if (System::Windows::Forms::Control::ModifierKeys == Keys::Control)
				OnTrackSelectedLots();
			else
				OnTrackSelected();
			break;
		}
	}

	System::Void CEfxTrackCtrl::CEfxTrackCtrl_MouseDoubleClick( System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e )
	{

	}
}