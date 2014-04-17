#pragma once
#include "CGraphicMallocObject.h"
#include "IGraphic.h"
#include "CRectangle.h"
#include "BaseHelper.h"
#include "TSqrAllocator.inl"
#include "CDynamicObject.h"

namespace sqr
{
	struct TextureFontImg : public CGraphicMallocObject
	{
		TextureFontImg();
		ITexture*			pTexture;		// ��ͼ
		CFRect				texRt;			// ��ͼʹ������
		uint32				dwColor;		// ��ͼ������ɫ
		uint32				dwLockFlg;		// ��ͼ������ʽ

		void SetColor(uint32 uc);
		void SetTexRect( CFRect& rt );
		CFRect GetTexRect();
	};

	class TextureFontInfo 
		: public virtual CDynamicObject
		, public CGraphicMallocObject
	{
	public:
		TextureFontInfo();
		~TextureFontInfo();

		uint32				EmtIdx;			// ��ͼ����
		GString				EmtTexName;		// ����
		TextureFontImg		EmtImg;			// ��ͼͼƬ��Ϣ
		GWString			EmtDes;			// ��������

		bool Init(IGraphic* pGraphic, uint32 idx, GString texname, CFRect pos, GWString des);
		void Destroy();
		void SetTextureFontIdx(uint32 idx);
		uint32 GetTextureFontIdx();
		const char* GetTexName();
	};

	typedef pair<uint32, uint32> IdxKey;

	class TextureFontInfoII : public virtual TextureFontInfo
	{
	public:
		TextureFontInfoII();
		~TextureFontInfoII();

		IdxKey TexIdx;

		bool Init(IGraphic* pGraphic, IdxKey idx, GString texname, CFRect pos, GWString des);
		void SetTextureFontIdxII(IdxKey idx);
		IdxKey GetTextureFontIdxII();
	};

	struct TextureFontDraw : public CGraphicMallocObject
	{
		TextureFontDraw();
		TextureFontImg*	pEmtImg;
		CFRect			posRect;
		CFRect			texRect;
	};

}
