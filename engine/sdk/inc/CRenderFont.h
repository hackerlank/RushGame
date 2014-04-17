#pragma once
//------------------------------------------------------------------------------
/**
	@class CRenderFont

	��Ⱦ����.
	Ϊ�����ϵĽӿڼ��ݶ�����, ��Ϊ������Դ����İ�װ.
	�������������ֺŵ�(��"����-12"), ����GDI
	û���ֺŵ�(��"��Բ"), ���û���distance-field������

	(C) 2008 ThreeOGCMan
*/
#include "RenderTextParam.h"
#include "GraphicRes.h"
#include "CResourceContainer.h"
#include "TRefPointer.h"
#include "CVector2.h"
//------------------------------------------------------------------------------
namespace sqr
{
SmartPointer(CFontBase);
SmartPointer(CRenderFont);

class CRenderFont 
	: public GraphicRes
	, public CResUnit
{
	friend class CDataSources;
public:
	int				Create(int32 id, bool bBold, float fSize);
	int32			GetFontID();
	bool			IsBold();
public:
	void			Begin();
	void			DrawTextBatch(const RenderTextParam& param);
	void			End();
	void			DrawText(const RenderTextParam& param);
	void			DrawText(RenderTextParam& param, bool bInKey);
	void			Resize(float new_size);

	void			BeginFlush(float size);
	void			AddDrawContent(const RenderTextParam& param);
	void			EndFlush();
	void			SetFrameFirst();
	
	float			GetLineSpacing()	const;
	float			GetFontHeight()		const;
	float			GetFontGlyphHeight(wchar_t c)	const;
	float			GetScale(float size);

	CVector2f		CalculateSize(const wchar_t* text);
	size_t			GetCharAtPixel(const wstring &text, size_t start_char, float pixel) const;
	size_t			GetCharAtPixel(const wstring &text, float pixel)					const;

	void			SetAlignToPixel(bool bAlignToPixel);
	void			UpdateCharInfo(const wstring& text);

	static const int32 INVALID_FONT_ID	= -1;
	static const int32 DISTANCE_FONT_ID	= 99999;
	
	int32			AddRef() { return GraphicRes::AddRef(); }
	~CRenderFont(void);

	inline void		AddRenderRef();
	void			SubRenderRef();
protected:
	CRenderFont( const string& szName, const wstring& szAlias );

	bool			IsValid() { return true; }
	void			OnRead( CRefBufFile& GraphicFile ) {};
	void			Free();
private:
	CFontBasePtr realFont;
	bool		 m_bAlignToPixel;
	int32		 m_urefCount;
	bool		 m_bFreed;
};

inline void CRenderFont::SetAlignToPixel(bool bAlignToPixel)
{
	m_bAlignToPixel = bAlignToPixel;
}

inline void CRenderFont::AddRenderRef()
{
	++m_urefCount;
}

}
