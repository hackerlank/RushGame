#pragma once
//------------------------------------------------------------------------------
/**
	@class CFontBase

	�������.
	ÿ���ַ�����һ������, ��������ͱ������.
	���Ծ��ο��Ի�������λ��(������3D������).
	����û��һ���Դ���һ�Ű���ȫ���ַ�������(����̫TMD����),
	���Բ���LRU�㷨ֻ�ѻ������ַ���������������, ���˵Ļ���������û�����ù���.
	Ϊ�˽�ʡ�����Դ�, ���ûҶ�ͼ, �ֵ���ɫ�ɶ���ɫ������, �������������ʺ���(ÿ������һ����ɫ)��Ч��.
 */
#include "CRefObject.h"
#include "CVector2.h"
#include "CVector3.h"
#include "CColor.h"
#include "CTexture.h"
#include "RenderTextParam.h"
#include "CTextureFontMgr.h"
#include "CFontListContainer.h"
#include <hash_map>

namespace sqr
{
	class CFontRenderer;

	class CFontBase 
		: public CRefObject
		, public CGraphicMallocObject
	{
	public:
		CFontBase();
		virtual ~CFontBase();

		bool			Create( int32 id, bool bold, float fSize);
		void			Resize(float size);

		int32			GetID() const;
		float			GetSize() const;
		bool			IsBold() const;

		virtual CVector2f CalculateSize(const wstring& text);
		size_t			GetCharAtPixel(const wstring &text, size_t start_char, float pixel) const;

		void			Begin();
		void			DrawText(const RenderTextParam& param);
		void			End();

		void			FlushOneFrame();
		void			SetFrameFirst();

		void			UpdateCacheTexture( const wchar_t& ch, const float size = -1.0f, uint8 idx = 0 );
		void			UpdateCache(const wstring& text, const float size = -1.0f, uint8 idx = 0);
		bool			IsScalable() const;
		float			GetScale(float size);
		virtual	void	InitTextKey(RenderTextParam& param, bool bInKey){};

		void			SetAlignToPixel(bool bAlignToPixel);

		void			UpdateCharInfo(const wstring& text);

		virtual float	GetLineSpacing()	const;
		virtual float	GetFontHeight()		const;
		virtual float	GetFontGlyphHeight(wchar_t c)	const;

		// �ַ���һ����Ԫ������ռ������.
		struct CharInfo
		{
			int16 top;
			int16 left;
			int16 width;
			int16 height;
		};

		static const int32 INVALID_FONT_ID	= -1;
		static const int32 DISTANCE_FONT_ID = 99999;
		// ����ʹ������
		static const int32 EMT_FONT_ID_BEGIN = 0xE000;
		static const int32 EMT_FONT_ID_END = 0xF8FF;
		static const int32 st_CacheTextureSize;
	protected:
		virtual bool			OnCreate();
		virtual void			OnResize();
		virtual void			OnDestroy();
		virtual void			PreProcessStr(){};
		virtual const CharInfo* GetCharInfo(wchar_t ch, uint8 idx = 0)	 = 0;
		virtual void			CacheCharacter(wchar_t ch, int x, int y) = 0;
		virtual DWORD			GetShaderID()							 = 0;

		virtual void AddText(const RenderTextParam& param);
		virtual bool CheckAndAddTex(wchar_t ch, uint8 idx, float& x, float& y, float& scale, 
									const CFRect& clipRect, float& charWidth, const RenderTextParam& param );
		void CFontBase::FormatVerticalTex(const wstring& text, const CFRect& clipRect, const CVector3f& pos , float& scale, float& height, float& x, float& y, const RenderTextParam& param );

	protected:
		typedef TVector2<float>						CharComposeSize;
		typedef GSet<wchar_t>						WCharSet;
		typedef GHashMap<wchar_t, CFRect>			RectMap;
		typedef GHashMap<wchar_t, CharComposeSize>	ComposeSizeMap;
		typedef GMap<wchar_t, CFontListNode*>		LRUFindMap;

		int32			 id;
		float			 m_font_size;
		bool			 isBold;
		bool			 isScalable;
		bool			 isBegin;
		RectMap			 charRectMap;		// �������е��ַ���ռ����
		CFontList		 charLRU;			// �������, Least Recently Used
		LRUFindMap		 m_lruFindMap;		// ���ڼ���charLRU�Ĳ���
		uint			 curX, curY;		// ��������д��ı�־
		ComposeSizeMap	 composeSizeMap;	// �ַ��Ű��С, keyΪwchar_t
		ITexture		*cacheTex;			// �������ֻ��������
		WCharSet		 m_disChar;			// ��ǰ������û�е�wchar
		const wchar_t	*m_curChar;			// ��ǰstr��ָ��
		wstring			 m_oldChar;			// ����󻺴��wstr
		CFontRenderer	*fontRenderer;
		bool			 m_bAlignToPixel;
		int              m_PixelSize;
		float			 m_formatHeight;	// �����߶�
		bool			 m_beFrameFirst;	// ��ǰ֡��һ����
		CFontListNode*	 m_FirstCharItr;
	};

	//------------------------------------------------------------------------------
	inline int32 CFontBase::GetID() const
	{
		return this->id;
	}

	//------------------------------------------------------------------------------
	inline void CFontBase::SetFrameFirst()
	{
		m_beFrameFirst = true;
	}

	//------------------------------------------------------------------------------
	inline float
	CFontBase::GetSize() const
	{
		return m_font_size;
	}

	//------------------------------------------------------------------------------
	inline bool
	CFontBase::IsBold() const
	{
		return this->isBold;
	}

	//------------------------------------------------------------------------------
	inline bool
	CFontBase::IsScalable() const
	{
		return this->isScalable;
	}

	//------------------------------------------------------------------------------
	inline float CFontBase::GetLineSpacing() const
	{
		return 0;
	}

	//------------------------------------------------------------------------------
	inline float CFontBase::GetFontGlyphHeight(wchar_t c)	const
	{
		return 0;
	}

	//------------------------------------------------------------------------------
	inline float CFontBase::GetFontHeight() const
	{
		return 0;
	}

	inline void CFontBase::SetAlignToPixel(bool bAlignToPixel)
	{
		m_bAlignToPixel = bAlignToPixel;
	}
}
