#pragma once
#include "SQRWnd.h"

namespace sqr
{

class CIme : public virtual CDynamicObject
{
	HWND					m_hWnd;
	bool					m_bEnableIme;   //���á��ر����뷨
	HIMC					m_hImc;			//���뷨������
	HKL						m_hCurKL;		//��ǰ���뷨
	uint32					m_uCharBuf;		//�Ǳ�׼���뷨����
	WND_IMAGE_LIST			m_ImeImage;		//���뷨��ͼ

protected:
	void					SetImeIcon();
	virtual void			OnImeChanged() {};
	CIme( HWND hWnd );
	~CIme(void);

public:
	static void				InitIme( HWND hWnd );
	static void				UnitIme();
	static CIme*&			GetIme();

	LRESULT					ImeProc( uint32 message, WPARAM wParam, LPARAM lParam );
	bool					CorrectCharMsg( UINT& uMsg, WPARAM& wParam );
	void					EnableIme( bool bEnable );
	const WND_IMAGE_LIST*	GetGuiImeImage();
};

}