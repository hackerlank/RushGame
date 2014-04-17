#include "stdafx.h"
#include "CTextureFontDef.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////
// TextureFontImg

TextureFontImg::TextureFontImg()
: pTexture(NULL)
{
}

void TextureFontImg::SetColor(uint32 uc)
{
	dwColor = uc;
}

void TextureFontImg::SetTexRect( CFRect& rt )
{
	texRt = rt;
}

CFRect TextureFontImg::GetTexRect()
{
	return texRt;
}

//////////////////////////////////////////////////////////////////////////
// TextureFontInfo

TextureFontInfo::TextureFontInfo()
: EmtIdx(0)
{
}

TextureFontInfo::~TextureFontInfo()
{
	Destroy();
}

// ��ʼ����������
bool TextureFontInfo::Init(IGraphic* pGraphic, uint32 idx, GString texname, CFRect pos, GWString des)
{
	ITexture* pTexture = NULL;

	SafeRelease(EmtImg.pTexture);
	if ( texname.c_str() && texname[0] )
	{
		HRESULT hr = pGraphic->CreateTexture( PATH_ALIAS_GUITEX.c_str(), texname.c_str(), &pTexture ); // szTextName
		if(FAILED(hr))
			OutputDebugString(texname.c_str());
		if ( !pTexture )
			return false;
	}

	EmtImg.pTexture	= pTexture;
	EmtImg.texRt	= pos;
	EmtIdx			= idx;
	EmtTexName		= texname;
	EmtDes			= des;
	return true;
}

// �ͷ���ͼ
void TextureFontInfo::Destroy()
{
	SafeRelease( EmtImg.pTexture );
}

// ���ñ�������ֵ
void TextureFontInfo::SetTextureFontIdx(uint32 idx)
{
	EmtIdx = idx;
}

// ��ȡ��������ֵ
uint32 TextureFontInfo::GetTextureFontIdx()
{
	return EmtIdx;
}

// ��ȡͼƬ����
const char* TextureFontInfo::GetTexName()
{
	return EmtTexName.c_str();
}

//////////////////////////////////////////////////////////////////////////
// TextureFontInfoII

TextureFontInfoII::TextureFontInfoII()
{
}

TextureFontInfoII::~TextureFontInfoII()
{
	Destroy();
}

// ��ʼ����������
bool TextureFontInfoII::Init(IGraphic* pGraphic, IdxKey idx, GString texname, CFRect pos, GWString des)
{
	ITexture* pTexture = NULL;

	SafeRelease(EmtImg.pTexture);
	if ( texname.c_str() && texname[0] )
	{
		HRESULT hr = pGraphic->CreateTexture( PATH_ALIAS_RES.c_str(), texname.c_str(), &pTexture ); // szTextName
		if(FAILED(hr))
			OutputDebugString(texname.c_str());
		if ( !pTexture )
			return false;
	}

	EmtImg.pTexture	= pTexture;
	EmtImg.texRt	= pos;
	TexIdx			= idx;
	EmtTexName		= texname;
	EmtDes			= des;
	return true;
}

// ���ñ�������ֵ
void TextureFontInfoII::SetTextureFontIdxII(IdxKey idx)
{
	TexIdx = idx;
}

// ��ȡ��������ֵ
IdxKey TextureFontInfoII::GetTextureFontIdxII()
{
	return TexIdx;
}

TextureFontDraw::TextureFontDraw()
: pEmtImg(NULL)
{
}