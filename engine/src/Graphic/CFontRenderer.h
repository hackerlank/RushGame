#pragma once
//------------------------------------------------------------------------------
/**
    @class sqr::CFontRenderer

	���������.
	GUI�Ǳ�DrawText̫Ƶ����, Ϊ�˼���DP��, ����һ��Begin/End��������ƽ��д��.
	��̭ԭ��static����Ļ��淽ʽ, ���ö�̬��С������, ����������������.

	@todo �������干��ͬһ������

    (C) 2009 ThreeOGCMan
*/
#include "TSingleton.h"
#include "GraphicBase.h"
#include "CGraphic.h"
#include "CTextureFontMgr.h"

//------------------------------------------------------------------------------
namespace sqr
{
class ITexture;

class CFontRenderer 
	: public TSingleton<CFontRenderer>
	, public CGraphicMallocObject
{
public:
	/// ��ʼ���
	void Begin(ITexture* texture, uint shader);
	/// ����ַ�
	void AddChar(const CFRect& charRect,
				 const CFRect& uvRect,
				 float depth,
				 const CColor& top,
				 const CColor& bottom,
				 float italicOffset);
	void AddVerticalChar(const CFRect& charRect,
				const CFRect& uvRect,
				float depth,
				const CColor& top,
				const CColor& bottom,
				float italicOffset);
	/// �������, �ύ��Graphic
	void End();

	void FlushOneFrame();

	void SetAlignToPixel(bool bAlignToPixel);
	
	inline float PixelAligned(float x);

private:
	friend class TSingleton<CFontRenderer>;
	CFontRenderer();
	~CFontRenderer();

	void GrowTextBuffer(size_t size);
private:
	size_t numChars;
	ITexture* texture;
	DWORD shaderId;

	size_t bufferSize;
	GVector<Vertex2D> textVB;
	GVector<uint16> textIB;
	bool	m_bAlignToPixel;
};

inline float CFontRenderer::PixelAligned(float x)
{
	if ( m_bAlignToPixel )
		return (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f));
	else
		return x;
}

}// namespace sqr
