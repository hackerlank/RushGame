#include "stdafx.h"
#include "CGraphic.h"
#include "CBaseMovieImp.h"
#include "BaseHelper.h"

CBaseMovieImp::CBaseMovieImp()
:m_pTexture(NULL)
{

}

CBaseMovieImp::~CBaseMovieImp()
{
	// 2) �ͷ���ͼ
	SafeRelease(m_pTexture);
}

ITexture * CBaseMovieImp::GetMovieTexture()
{
	return m_pTexture;
}