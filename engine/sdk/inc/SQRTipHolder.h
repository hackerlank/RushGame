#pragma once
#include "SQRWnd.h"
// ��������richtooltip���������壬�޹������� 
namespace sqr
{
	class SQRWnd;
	class SQRTipHolder 
		: public Singleton<SQRTipHolder>
	{
	public:
		SQRTipHolder();
		~SQRTipHolder();

		void  CreatHolderWnd(SQRWnd* wnd);
		SQRWnd* GetHolder();

	private:
		SQRWnd* m_pTipHolder;
	};

	//-----------------------------------------------------------------
	inline SQRWnd*
	SQRTipHolder::GetHolder()
	{
		return m_pTipHolder;
	}
}