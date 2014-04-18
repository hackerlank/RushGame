#pragma once
/**
	@class CChangeMoveMapData

	Save .local File

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
SmartPointer(CChangeMoveMapData);

class CChangeMoveMapData : public CRefObject
{
public:
	CChangeMoveMapData();

	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);

	/// nStartReginX��nStartReginZ��Ҫ�ƶ���ͼ��������ʼ���꣬ 
	void MoveAsMap(const int nStartReginX, const int nStartReginZ, const int nOffsetReginWidth, const int nOffsetReginDepth);

	void SetMoveOffset(const int xoffset, const int zoffset);

private:
	void ChangeRegionLightInfo();
	void ClearInfo();

private:
	CTerrainMesh* terrain;
	IProgressCallback* progress;

	///first �ɵĸ�������ֵ
	///second �ƶ����Ӧ���µĸ�������ֵ
	map<int, int> mapOldMapGridIndex;
	map<int, int> mapOldMapVertexIndex;
	///��չ��ɵ�ͼϸ�������������µ�ͼ�е�������ӳ�� first old region index second new region index
	map<int, int> m_OldMapRegionIndexMap;

	int m_nMapXMoveOffset;//m_nMapXMoveOffset m_nMapZMoveOffsetΪ�ƶ��ĸ�����
	int m_nMapZMoveOffset;
};

//------------------------------------------------------------------------------
inline void
CChangeMoveMapData::SetTerrainMesh( CTerrainMesh* t )
{
	Ast(NULL != t);
	this->terrain = t;
}

//------------------------------------------------------------------------------
inline void
CChangeMoveMapData::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

//------------------------------------------------------------------------------
inline void
CChangeMoveMapData::SetMoveOffset(const int xoffset, const int zoffset)
{
	m_nMapXMoveOffset = xoffset;
	m_nMapZMoveOffset = zoffset;
}// namespace sqr
}
