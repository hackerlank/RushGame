#pragma once
//------------------------------------------------------------------------------
/**
	@class CRefractFilter

	���������.
	�ӽ�ȥ��ģ�ͻᱻ����shader��texture.
	��Ϊ�����Ķ��󲻶�, �Ͳ�������...
 */
#include "CRenderFilter.h"
#include "CFilterNode.h"
#include "CRenderList.h"
//------------------------------------------------------------------------------
namespace sqr
{

class CRefractFilter : public CRenderFilter
{
public:
	CRefractFilter(ERenderObjStyle ros);
	virtual ~CRefractFilter();

	/// ������
	FilterNode* Filter(FilterNode* node);
	/// ��
	void Render();
	/// ���û��
	void SetLightIndex(uint index);
	/// �Ƿ���Ҫ����
	bool IsValidQueue() const;

	/// ȡ����Чר��buffer
	virtual CMatrix* GetRenderStack(ERenderStyle RenderStyle,
									ERenderObjStyle RenderObjStyle,
									ITexture* Text0,
									ITexture* Text1,
									EPrimitiveType PrimitiveType,
									uint32 VertexNum,
									uint32 PrimitiveCount,
									uint32 VerFormat,
									uint32 VerStride,
									void** ppVB,
									void** ppIB,
									void** ppRS = NULL,
									DWORD ShaderID = 0);

private:
	STATIC_RS	m_RefractRs;
	CRenderList	m_RenderList;
	CDynamicVB*	effectVB;
};

}// namespace sqr
