
#pragma once
#include "SQRControl.h"

namespace sqr
{
	class SQRWndEx : public SQRControl
	{
	public:
		SQRWndEx();
		~SQRWndEx();

	protected:
		//��������Ϣ
		virtual void	OnLButtonDown( unsigned int nFlags, int x, int y );
	};
}