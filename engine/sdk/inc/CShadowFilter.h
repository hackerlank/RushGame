#pragma once
#include "CRenderFilter.h"
#include "FilterDefine.h"
#include "CFilterNode.h"
#include "CRenderList.h"
//��Ӱ������
namespace sqr
{

class CShadowFilter : public CRenderFilter
{
public:
	CShadowFilter(CRenderGroup* pRenderGroup);
	virtual ~CShadowFilter();
	virtual FilterNode* Filter(FilterNode* fn);
	virtual void Update(FilterNode* fn) {};
	virtual void Refresh(CRenderFilter* newFilter);
	virtual void Render(void);
	virtual void Begin(void) {};
	virtual void End(void) {};

public:
	void SetLightVector(const CVector3f& lv);
	const CVector3f& GetLightVector(void);
	virtual void ResetFilterQueue(RenderQueueFlag& fQueueFlag) {};
	virtual void SetShadowParams(STATIC_RS& RS) {}; //Ϊ�����������ò���
	virtual void UpdateShadowParams(void) {};
	virtual EShadowType GetShadowType() const = 0;
	virtual void UpdateRenderStyle(const RenderParam& RS );

protected:
	CRenderList		m_RenderList;
	CVector3f		m_LightVector;
	STATIC_RS		m_ShadowRs;
	CRenderGroup*	m_pRenderGroup;
};

//------------------------------------------------------------------------------

}