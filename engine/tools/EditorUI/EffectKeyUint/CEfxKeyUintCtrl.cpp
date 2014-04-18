#include "StdAfx.h"
#include "CEfxKeyUintCtrl.h"
#include "CEfxKeyUintCtrlImp.h"
#include "EffectTrack/CEfxTrackCtrl.h"

namespace sqr_tools {

	char* CEfxKeyUintCtrl::GetImpTypeName( void )
	{
		return GET_IMP_NAME(CEfxKeyUintCtrlImp);
	}

	void CEfxKeyUintCtrl::SetKeyContent(void* content)
	{
		CEfxKeyUintCtrlImp* EfxEditKeyimp = (CEfxKeyUintCtrlImp*)m_pCtrl;
		EfxEditKeyimp->SetEditKeyContent(content);

		InitKeyName();
	}

	System::Void  CEfxKeyUintCtrl::InitOtherComponent()
	{
		m_uCtrlState = KEY_STATE_NORMAL;
	}

	System::Void  CEfxKeyUintCtrl::DestroyOtherComponent()
	{

	}


	void  CEfxKeyUintCtrl::RefreshKeyState()
	{
		
		if (m_uCtrlState & KEY_STATE_SELPRE)
		{
			m_uCtrlState = (m_uCtrlState & (~KEY_STATE_SELPRE)) | KEY_STATE_SEL;
		}
		else
			m_uCtrlState = m_uCtrlState & (~KEY_STATE_SEL);
	
		CEfxTrackCtrl^ trackCtrl = static_cast<CEfxTrackCtrl^>(this->Parent);
		int trackState = trackCtrl->GetTrackState();
		// ������ɫ
		if (m_uCtrlState & KEY_STATE_SEL)
		{
			// ѡ�е�
			if ((trackState & TRACKCTRL_STATE_HIDE) && (trackState & TRACKCTRL_STATE_SEL))
			{
				// �������ص�  ѡ�е�
				this->BackColor = System::Drawing::Color::ForestGreen;
			}
			else if (trackState & TRACKCTRL_STATE_HIDE)
			{
				// ���ص� δѡ�е�
				this->BackColor = System::Drawing::Color::Chartreuse;
			}
			else if(trackState & TRACKCTRL_STATE_SEL)
			{
				// �����أ�ѡ��
				this->BackColor = System::Drawing::Color::ForestGreen;
			}
			else
			{
				// �����أ�δѡ��
				this->BackColor = System::Drawing::Color::Chartreuse;
			}
		}
		else
		{
			// δѡ�е�
			if ((trackState & TRACKCTRL_STATE_HIDE) && (trackState & TRACKCTRL_STATE_SEL))
			{
				// �������ص�  ѡ�е�
				this->BackColor = System::Drawing::Color::DarkSeaGreen;
			}
			if ((trackState & TRACKCTRL_STATE_HIDE) && (trackState & TRACKCTRL_STATE_SELS))
			{
				// �������ص�  ��ѡѡ�е�
				this->BackColor = System::Drawing::Color::DarkSeaGreen;
			}
			else if (trackState & TRACKCTRL_STATE_HIDE)
			{
				// ���ص� δѡ�е�
				this->BackColor = System::Drawing::Color::Chartreuse;
			}
			else if(trackState & TRACKCTRL_STATE_SEL)
			{
				// �����أ�ѡ��
				this->BackColor = System::Drawing::Color::DarkSeaGreen;
			}
			else if(trackState & TRACKCTRL_STATE_SELS)
			{
				// �����أ���ѡѡ��
				this->BackColor = System::Drawing::Color::DarkSeaGreen;
			}
			else
			{
				// �����أ�δѡ��
				this->BackColor = System::Drawing::Color::Chartreuse;
			}
		}
	}
	
	void CEfxKeyUintCtrl::InitKeyName()
	{
		// ��ɿؼ����ֵ�����
		CEfxKeyUintCtrlImp* EfxEditKeyimp = (CEfxKeyUintCtrlImp*)m_pCtrl;
		String^ clrKeyText = gcnew String(EfxEditKeyimp->GetEditKeyText().c_str());
		
		KeyNameLable->Text = clrKeyText;

		CKeytoolTip->SetToolTip(this, clrKeyText);
		CKeytoolTip->SetToolTip(this->KeyNameLable, clrKeyText);
	}

	void  CEfxKeyUintCtrl::OnKeySelected()
	{
		m_uCtrlState |= KEY_STATE_SELPRE;

		CEfxTrackCtrl^ trackCtrl = static_cast<CEfxTrackCtrl^>(this->Parent);
		trackCtrl->OnTrackSelected();
	}

	void  CEfxKeyUintCtrl::OnKeySelectedLots()
	{
		m_uCtrlState |= KEY_STATE_SELPRE;

		CEfxTrackCtrl^ trackCtrl = static_cast<CEfxTrackCtrl^>(this->Parent);
		trackCtrl->OnTrackSelectedLots();
	}

	void  CEfxKeyUintCtrl::OnKeyUintEdit()
	{
		CEfxTrackCtrl^ trackCtrl = static_cast<CEfxTrackCtrl^>(this->Parent);
		CEfxKeyUintCtrlImp* EfxEditKeyimp = (CEfxKeyUintCtrlImp*)m_pCtrl;
		trackCtrl->OnKeyUintEdit(EfxEditKeyimp->GetEditKeyContent());
	}

	System::Void CEfxKeyUintCtrl::CEfxKeyUintCtrl_MouseClick( System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e )
	{
		System::Windows::Forms::MouseButtons mbt = e->Button;
		switch(mbt)
		{
		case System::Windows::Forms::MouseButtons::Middle:

			break;
		case System::Windows::Forms::MouseButtons::Right:

			break;
		case System::Windows::Forms::MouseButtons::Left:
			if (System::Windows::Forms::Control::ModifierKeys == Keys::Control)
				OnKeySelectedLots();
			else
				OnKeySelected();
			break;
		}
	}

	System::Void CEfxKeyUintCtrl::CEfxKeyUintCtrl_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		OnKeyUintEdit();
	}

	System::Void CEfxKeyUintCtrl::KeyNameLable_MouseClick( System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e )
	{
		System::Windows::Forms::MouseButtons mbt = e->Button;
		switch(mbt)
		{
		case System::Windows::Forms::MouseButtons::Middle:

			break;
		case System::Windows::Forms::MouseButtons::Right:

			break;
		case System::Windows::Forms::MouseButtons::Left:
			if (System::Windows::Forms::Control::ModifierKeys == Keys::Control)
				OnKeySelectedLots();
			else
				OnKeySelected();
			break;
		}
	}

	System::Void CEfxKeyUintCtrl::KeyNameLable_MouseDoubleClick( System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e )
	{
		OnKeyUintEdit();
	}

	System::Void CEfxKeyUintCtrl::CEfxKeyUintCtrl_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
	{
		
	}


}
