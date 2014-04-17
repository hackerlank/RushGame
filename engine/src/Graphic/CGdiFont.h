#pragma once
#include "CFontBase.h"
/**
	@class CGdiFont

	����GDI���õ�Fontʵ�ֵ�����.
	����8λ�Ҷȼ����������
	��ΪGDI����Ĵ�С�ڴ���ʱ���Ѿ�������, ���Է���ʱ��Ҫ���´��������ֺŵ�.
	���ڿ���ֱ�Ӷ�ȡϵͳTTF����, �����Ͳ�����������������, ��������Ҳ������.
	ͬʱҲ������Ԥ������������Ҫ�ڴ������������ַ���Ϣ���������ڴ�ռ������.
	Ϊ�˼���GUIϵͳ(��֧�ַǵȿ��ַ����Ű�), �����ֺŶ���ż��(�������ֲ�����ASCII����)

	@todo ����AddFontMemResourceEx����Զ�������(�ͻ����Դ�TTF�ļ�)
 */

namespace sqr
{

class CGdiFont : public CFontBase
{
public:
	CGdiFont();

private:
	bool OnCreate();
	void OnResize();
	void OnDestroy();
	const CharInfo* GetCharInfo(wchar_t ch, uint8 idx = 0);
	void CacheCharacter(wchar_t ch, int x, int y);
	DWORD GetShaderID();
	virtual void	PreProcessStr();

	virtual float	GetLineSpacing() const;
	virtual float	GetFontHeight() const;
	virtual float	GetFontGlyphHeight(wchar_t c)	const;

private:
	struct CharInfoEx : public CharInfo
	{
		uint size;
	};
	typedef GHashMap<wchar_t, CharInfoEx> CharInfoMap;
private:
	HDC memDC;
	HFONT font;
	HFONT m_defFont;
	float m_ascender;
	float m_descender;
	//! (ascender - descender) + linegap
	float m_height;

	CharInfoMap charInfoMap;
	GSet<wchar_t> invalidChar;	///< ����������ʾ�ַ�
};

}// namespace sqr
