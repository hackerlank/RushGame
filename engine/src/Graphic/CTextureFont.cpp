//------------------------------------------------------------------------------
//  CTextureFont.cpp
//  (C) 2010 ob_one
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "CTextureFont.h"
#include "CGraphic.h"
#include "CPkgFile.h"
#include "CGpuProgramParameters.h"
#include "TSqrAllocator.inl"
#include "CFontMgr.h"
#include "BaseHelper.h"
#include "CFontRenderer.h"
#include "CGpuProgramMgr.h"
#include "CHardwareCapacity.h"
#include <math.h>

#define COLOR_DEF_BOTTOM (uint8)40
#define COLOR_DEF_TOP	(uint8)215
#define COLOR_DEF_DIS	(uint8)10


bool is_spacetest(wchar_t ch)
{
	return (' ' == ch || '��' == ch);
}

namespace sqr
{

	CTextureFont::CTextureFont()
	{
	}

	const CFontBase::CharInfo* CTextureFont::GetCharInfo( wchar_t ch, uint8 idx )
	{
		if ( CTextureFontMgr::GetInst() )
		{
			TextureFontInfo* EmtInfo = NULL;
			if ( NULL != (EmtInfo = CTextureFontMgr::GetInst()->GetTexFontInfo(ch, idx)) )
			{
				float cellX, cellY;
				cellX = abs(EmtInfo->EmtImg.texRt.right - EmtInfo->EmtImg.texRt.left) * CTextureFontMgr::GetInst()->GetImgScale();
				cellY = abs(EmtInfo->EmtImg.texRt.bottom - EmtInfo->EmtImg.texRt.top) * CTextureFontMgr::GetInst()->GetImgScale();
				if (this->composeSizeMap.find(ch) != this->composeSizeMap.end())
					this->composeSizeMap[ch] = CharComposeSize(cellX, cellY);
				else
					this->composeSizeMap.insert(make_pair(ch, CharComposeSize(cellX, cellY)));
				return NULL;
			}
		}
		if ( CGpuProgramMgr::GetInst()->GetShaderVer() < Shader_V_2 )
			return NULL;
		else
			return CDistanceFont::GetCharInfo(ch, idx);
	}

	DWORD CTextureFont::GetShaderID()
	{
		if ( CGpuProgramMgr::GetInst()->GetShaderVer() < Shader_V_2 )
			return 0;
		else
			return CDistanceFont::GetShaderID();
	}

	void CTextureFont::InitTextKey(RenderTextParam& param, bool bInKey)
	{
		if (bInKey)
		{
			// ����KEY
			uint8 fKey;
			fKey = GetSimilarColor( param.GetColor() );

			if (fKey > TEX_FONT_COLOR_DEF)
				fKey = 0;
			param.SetIdx(fKey);
		}
	}

	// ����ֵ 0������ 1���� 2���� 3���� 4���� 5���� 6���� 7���� 8���� 9����
	uint8 CTextureFont::GetSimilarColor(const CColor& color)
	{
		uint8 r = color.R;
		uint8 g = color.G;
		uint8 b = color.B;

		if (  (r > g && r > b && g == b)	// ��
		   || (g > r && g > b && r == b)	// ��
		   || (b > r && b > g && r == g)	// ��
		   )
			return (uint8)((((uint8)( r > g )) << 2) + (((uint8)( g > r )) << 1) +((uint8)( b > r )));
		else if ( (r == g && r > b)		// ��
				||(g == b && g > r)		// ��
				||(r == b && b > g)		// ��
			)
			return (uint8)(
						  (((uint8)( r == g )) << 2) + (((uint8)( b == r )) << 2)
						 +(((uint8)( r == g )) << 1) + (((uint8)( b == g )) << 1) 
						 +((uint8)( b == g )) + ((uint8)( b == r ))
						  );
		else if (r == g && r == b && b == g)
		{
			if (r < COLOR_DEF_BOTTOM)
				return TEX_FONT_COLOR_DEF;		// ��ɫ
			if (r > COLOR_DEF_TOP)
				return TEX_FONT_COLOR_DEF - 1;	// ��ɫ
			return TEX_FONT_COLOR_DEF - 2;	// ��ɫ
		}
		else if (r < COLOR_DEF_BOTTOM && g < COLOR_DEF_BOTTOM && b < COLOR_DEF_BOTTOM)
			return TEX_FONT_COLOR_DEF;		// ��ɫ
		else if (r > COLOR_DEF_TOP && g > COLOR_DEF_TOP && b > COLOR_DEF_TOP)
			return TEX_FONT_COLOR_DEF - 1;	// ��ɫ
		else
		{
			bool tp = 0;
			uint8 rt = 0;
			tp = ( r + COLOR_DEF_DIS > g );
			rt += (uint8)(tp);	rt = rt << 1;

			tp = ( g + COLOR_DEF_DIS > b );
			rt += (uint8)(tp);	rt = rt << 1;

			tp = ( b + COLOR_DEF_DIS > r );
			rt +=(uint8)(tp);
			return tp;
		}
		return 0;
	}


	CVector2f CTextureFont::CalculateSize( const wstring& text )
	{
		// compute text rect area
		return CDistanceFont::CalculateSize(text);
	}

}
