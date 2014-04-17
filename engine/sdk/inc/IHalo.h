// -----------------------------------------------------
// ģ�������⻷�ӿڣ�ѡ��Ŀ��ŵ׿ɲȹ⻷�����ҳ��ĵ�����Ч
// ���ߣ����
// ����ʱ�䣺2005-11-14
// -----------------------------------------------------
#pragma once
#include "CVector3.h"

class IHalo
{
public:
	virtual void	Release( void )		= 0;
	virtual void	OnRender( void )	= 0;

	virtual void	SetPos( CVector3f & pos )	= 0;
	virtual void	SetSize( float fSize )		= 0;
	virtual void	SetColor( uint32 color )	= 0;
};