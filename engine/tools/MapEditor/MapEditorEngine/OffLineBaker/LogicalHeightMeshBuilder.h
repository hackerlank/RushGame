#pragma once
//------------------------------------------------------------------------------
/**
    @class CLogicalHeightMeshBuilder

	��TerrainMesh�ｨ���߼���mesh.

    (C) 2009 ThreeOGCMan
*/
#include "CRenderOperation.h"

namespace sqr
{
	SmartPointer(LogicalMesh);
	class LogicalMesh : public CRefObject
	{
	public:
		virtual ~LogicalMesh();
		CRenderOperation rp;
		CAxisAlignedBox aabb;
	};
	class CLogicalHeightMeshBuilder
	{
	public:
		static void Build(vector<LogicalMeshPtr>& outMeshes);
	};
}