#include "stdafx.h"
#include "SQRWndOffSet.h"
#include "SQRGUIManager.h"

DYNAMIC_CREATE_WND( SQRWndOffSet, SQRWnd )
SQRWndOffSet::SQRWndOffSet()
		: mSteptimer(0)
		, mBeginTimer(0)
		, mStepResult(0.0f)
		, mStep(0.0f)
		, mStepOver(0.0f)
{

}

SQRWndOffSet::~SQRWndOffSet()
{

}

void SQRWndOffSet::ShowWnd( bool bShow )
{
	if ( false == bShow )
	{
		mStepResult = 0.0f;
	}
	SQRWnd::ShowWnd( bShow );
}

void SQRWndOffSet::OnProcess()
{
	if ( 0 != mBeginTimer )
	{
		DWORD time = static_cast<DWORD>(GetProcessTime());
		if ( time - mBeginTimer > mSteptimer)
		{
			mBeginTimer =  time;
			mStepResult += mStep;

			//��ʾ�������Ѿ������
			if ( mStepResult > mStepOver )
			{
				//��ʱ��ʵ�֣���Ϊ�������ַ�����ʱ����ҪһЩ�����޸�
				//mBeginTimer = 0;
				//ShowWnd(false);
				SetWndText("");
			}
		}
	}
}

void SQRWndOffSet::DrawWndBackground()
{
	OnProcess();
	//��Ҫ�ж�������޸���
	SQRWnd::DrawWndBackground();
}

void SQRWndOffSet::SetWndText(const char* str )
{
	SQRWnd::SetWndText(str);
	resstring = str;
}

const char* SQRWndOffSet::GetWndText()
{
	return resstring.c_str();
}

void SQRWndOffSet::SetStep(INT32 time, FLOAT mStep)
{
	this->mStep = mStep;
	mSteptimer  = time;
	mStepResult = static_cast<FLOAT>(-this->GetWndWidth());
	mBeginTimer = static_cast<DWORD>(GetProcessTime());
	mStepOver = strlen(this->GetWndText()) * GetFontSize()/2 * GetWndZoom(); 

}

void SQRWndOffSet::DrawText( const wstring& text, float x, float y, CColor color, CColor bkColor, bool multiline, CFRect* rect )
{
	const WndHandle* handle = GetWndHandle();
	if ( !handle->m_WndFont || text.empty())
		return;

	// �����ؼ�����x�������ֵĶ��뷽ʽ
	x = 0;

	CFRect rtWnd = rect ? *rect : CFRect( 0, 0, GetWndWidth(), GetWndHeight() );
	if ( GetStyle()&WS_CUTBYPARENT )
	{
		WndToScreen( rtWnd );
		x += rtWnd.left;
		y += rtWnd.top;
		CFRect rtParent;
		GetParent()->GetWndRect( rtParent );
		rtWnd = rtWnd.Intersection(rtParent);
		x -= rtWnd.left;
		y -= rtWnd.top;
		rect = &rtWnd;
		ScreenToWnd( rtWnd );
	}
	if ( m_pCutWnd )
	{
		CFRect rtCutWnd;
		m_pCutWnd->GetCutAreaRect(rtCutWnd);
		ScreenToWnd( rtCutWnd );
		rtWnd = rtWnd.Intersection(rtCutWnd);
	}

	CColor gradual = GetFontGradualColor();
	color.A = uint8(color.A * m_hWnd->m_fTransparent);
	bkColor.A = uint8(bkColor.A * m_hWnd->m_fTransparent);
	gradual.A = uint8(gradual.A * m_hWnd->m_fTransparent);

	FontEffect::Mask mask = GetFontEffectMask();
	if (multiline)
		mask |= FontEffect::Multiline;

	WndToScreen( rtWnd );

	if ( (WS_MULLINETEXT&GetStyle()) != WS_MULLINETEXT )
	{
		float Height = GetFontSize()*m_hWnd->m_pGUI->GetZoomSize();
		if ( GetStyle() & WS_TEXTYC )
			y = (m_hWnd->m_Height - Height)/2;
	}

#ifdef UI_HIDE_MODE
	if (handle->m_pGUI->GetHidenMode() && !IsModleState() )
	{
		return;
	}
#endif

	handle->m_pGUI->DrawText(handle->m_WndFont, handle->m_FontSize,
	                         text, color, bkColor, gradual,
	                         rtWnd, x, y, handle->m_fZ,
	                         mask, mStepResult);
}

bool SQRWndOffSet::IsHasEventState(EventStateMask Mask)
{
	return SQRWnd::IsHasEventState(Mask);
}

bool SQRWndOffSet::ObjHasEventState(EventStateMask Mask)
{
	return SQRWndOffSet::IsHasEventState(Mask);
}
