#pragma once
#include "CDistanceFont.h"
#define TEX_FONT_COLOR_DEF 9
/*
	��ͼ���壬֧���Զ���Ĳ�������
	���������û�ж�������֣���distance font��ΪĬ������
	�����������ı༭����GUI������
*/

namespace sqr
{
	class CTextureFont : public CDistanceFont
	{
	public:
		CTextureFont();
		virtual	void		InitTextKey(RenderTextParam& param, bool bInKey);
		virtual CVector2f	CalculateSize(const wstring& text);

	private:
		const CharInfo*		GetCharInfo(wchar_t ch, uint8 idx = 0);
		uint8				GetSimilarColor(const CColor& color);
		DWORD				GetShaderID();
	};
}