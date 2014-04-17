#pragma once
#include "CRefObject.h"
#include "CWaterSurface.h"
#include "CSceneMallocObject.h"
/**
	@class CWaterSurface

	ˮ��.
	��������пɼ��Բü�.
	���Ըı�WaterEffect���л�ˮ��Ч��(����ģʽ).

	@note ͬһЧ����ˮ��ֻ��һ��ʵ��.
 */
namespace sqr
{
SmartPointer(CWaterEffect);
class CWaterRegionMesh;
class CCamera;
class CWater;
class ITexture;

class CWaterSurface 
	: public CRefObject
	, public CSceneMallocObject
{
public:
	CWaterSurface();
	virtual ~CWaterSurface();

	/// ����ָ�������ˮ������, �����ڷ��ؿ�ָ��
	//CWaterRegionMesh* GetRegionMesh(uint32 regionId) const;
	/// ����һ���µ�ˮ������
	void AddRegionMesh(CWaterRegionMesh* pRegion);
	/// ɾ��һ���µ�ˮ������
	void RemoveRegionMesh(CWaterRegionMesh* pRegion);
	/// ȡ��ˮ��������
	size_t GetRegionMeshNum();
	/// �Ƿ����ָ������
	bool HasRegionMesh(CWaterRegionMesh* pRegion);

	/// �Ƿ�����
	bool IsHide() const;
	/// �����Ƿ�����
	void SetHide(bool b);

	/// ����ˮ��Ч��
	const CWaterEffectPtr& GetWaterEffect() const;
	/// ����ˮ��Ч��
	void SetWaterEffect(const CWaterEffectPtr& effect);

	/// ����ˮ��
	void Render(CCamera* pCamera);
	void RenderWave(CCamera* pCamera ,ITexture* pTexture, uint32 uCurTime);
	CWater* GetWater(uint32 OrgId , int32 nGrid);
private:
	typedef SSet<CWaterRegionMesh*> RegionMeshSet;

	bool isHide;
	RegionMeshSet regionMeshSet;
	CWaterEffectPtr waterEffect;
};

//------------------------------------------------------------------------------
inline const CWaterEffectPtr&
CWaterSurface::GetWaterEffect() const
{
	return this->waterEffect;
}

//------------------------------------------------------------------------------
inline void
CWaterSurface::SetWaterEffect( const CWaterEffectPtr& effect )
{
	this->waterEffect = effect;
}

//------------------------------------------------------------------------------
inline size_t
CWaterSurface::GetRegionMeshNum()
{
	return this->regionMeshSet.size();
}

//------------------------------------------------------------------------------
inline bool
CWaterSurface::HasRegionMesh( CWaterRegionMesh* pRegion )
{
	return (this->regionMeshSet.end() != this->regionMeshSet.find(pRegion));
}

//------------------------------------------------------------------------------
inline bool
CWaterSurface::IsHide() const
{
	return this->isHide;
}

//------------------------------------------------------------------------------
inline void
CWaterSurface::SetHide( bool b )
{
	this->isHide = b;
}

}// namespace sqr
