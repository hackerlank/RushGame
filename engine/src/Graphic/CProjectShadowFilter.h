#pragma once
#include "CShadowFilter.h"
#include "CShadowClipper.h"

/**
	@class CProjectShadowFilter

	ͶӰ��Ӱ.
	��ΪHardware Shadow MappingӲ����֧��ʱ�����Ʒ.
 */
namespace sqr
{
class CCamera;
SmartPointer(CRenderTexture);
SmartPointer(CShadowMatrixBuilder);

class CProjectShadowFilter : public CShadowFilter
{
public:
	CProjectShadowFilter(CRenderGroup* pRenderGroup);
	~CProjectShadowFilter();
	void Begin(void);
	void End(void);
	EShadowType GetShadowType() const;
	void ResetFilterQueue(RenderQueueFlag& fQueueFlag);
	void SetShadowParams(STATIC_RS& RS);
	void UpdateShadowParams(void);

protected:
	CRenderTexturePtr shadowTexture;
	CShadowMatrixBuilderPtr matrixBuilder;
protected:
	CShadowClipper	m_BoxClipper;
	static const DWORD st_ProjectShadowVSID;
	static const DWORD st_ProjectShadowPSID;
};
//------------------------------------------------------------------------------
inline EShadowType CProjectShadowFilter::GetShadowType() const
{
	return ST_SHADOWMAP;
}

}// namespace sqr
