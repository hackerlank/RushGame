#pragma once
//------------------------------------------------------------------------------
/**
	@class CScreenAlignedQuad

	ȫ��Ļ����, ������.
 */
#include "TSingleton.h"
#include "CRenderOperation.h"
namespace sqr
{
class CVertexDeclaration;
class ITexture;

class CScreenAlignedQuad 
	: public Singleton<CScreenAlignedQuad>
	, public CGraphicMallocObject
{
	CScreenAlignedQuad();
	~CScreenAlignedQuad();
	friend class Singleton<CScreenAlignedQuad>;
public:
	void Render();
private:
	CRenderOperation	m_Op;
};

}// namespace sqr
