#pragma once
#include "CRefObject.h"
#include "CRenderOperation.h"
#include "CWaterRegionMesh.h"
#include "CHardwareBufferMgr.h"
#include "CSceneMallocObject.h"


/**
	@class CWaterRegionMesh

	һ���ͼ�����С��ˮ��.
	VertexBuffer�ǹ̶���С��, ����������numGrids����.
	IndexBuffer�������е�Region����һ����, ���Թ���һ����̬����ʵ��.
*/
namespace sqr
{
struct VerColorTex;
SmartPointer(CWaterRegionMesh);
class CWater;
class CWaterRegionMesh : public CSceneMallocObject
{
	friend class CWaterSurface;
public:
	CWaterRegionMesh();
	virtual ~CWaterRegionMesh();

	static void InitIndexBuffer( void );

public:
	/// ������ˮ�ĸ�����
	uint GetNumGrids() const;
	/// ������ˮ�ĸ�����
	void InitRegion(CVertexHardwareBuffer* pVertex , uint num);

	///// ��������, д�붥������.
	//size_t Lock(VerColorTex** pVer);
	///// ��������
	//void Unlock();
	

	/// ���ذ�Χ��
	const CAxisAlignedBox& GetBoundingBox() const;
	/// ���ð�Χ��
	void SetBoundingBox(const CAxisAlignedBox& box);

	/// ��������, ����ı�����ʽ������WaterEffect
	void Render();

private:
	CRenderOperation renderOperation;
	CAxisAlignedBox aabb;
	CVertexHardwareBuffer*	m_pVertexBuffer;
	uint numGrids;

private:
	
	static CIndexHardwareBuffer* st_IndexBuffer;

public://��ʱ�ṹ
	typedef	SVector<CWater*>	WaterPool;
	WaterPool		m_vecWater;
};

//------------------------------------------------------------------------------
inline uint
CWaterRegionMesh::GetNumGrids() const
{
	return this->numGrids;
}

//------------------------------------------------------------------------------
inline const CAxisAlignedBox&
CWaterRegionMesh::GetBoundingBox() const
{
	return this->aabb;
}

//------------------------------------------------------------------------------
inline void
CWaterRegionMesh::SetBoundingBox( const CAxisAlignedBox& box )
{
	this->aabb = box;
}

}// namespace sqr
