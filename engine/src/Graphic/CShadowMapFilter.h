#pragma once
#include "CShadowFilter.h"
#include "CShadowClipper.h"

/**
	@class CShadowMapFilter

	Hardware Shadow Mapping.
	����nVidia DSTʵ����ȵ��Զ�д��ͱȽ�. (A�����ں���Ҳ֧����)
 */
namespace sqr
{
class CCamera;
SmartPointer(CRenderTexture);
SmartPointer(CShadowMatrixBuilder);

class CShadowMapFilter : public CShadowFilter
{
public:
	CShadowMapFilter(CRenderGroup* pRenderGroup, bool isReceiveShadow);
	~CShadowMapFilter();
	void Begin();
	void End();
	EShadowType GetShadowType() const;
	void ResetFilterQueue(RenderQueueFlag& fQueueFlag);
	void SetShadowParams(STATIC_RS& RS);
	void UpdateShadowParams(void);
protected:
	CRenderTexturePtr		shadowTexture;
	CRenderTexture*			depthTexture;
	CShadowMatrixBuilderPtr matrixBuilder;
	CShadowClipper	m_BoxClipper;
	bool isReceiveShadow;
protected:
	static const DWORD st_ShadowMapVSID;
	static const DWORD st_ShadowMapPSID;
};

//------------------------------------------------------------------------------
inline EShadowType CShadowMapFilter::GetShadowType() const
{
	return this->isReceiveShadow ? ST_SHADOW_EX : ST_SHADOWMAP;
}
}// namespace sqr
