#pragma once
/**
	@class CEnlargetMapData

	Save .local File

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
SmartPointer(CEnlargetMapData);

class CEnlargetMapData : public CRefObject
{
public:
	CEnlargetMapData();

	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);
	void SaveAsEnlargeMapCoder(string strTitle, DWORD dwWidth, DWORD dwDepth);

	void NewSaveAsEnlargeMapCoder(DWORD dwWidth, DWORD dwDepth,int offW,int offL);
	CTerrainMesh* ResizeTerrainMesh(DWORD dwWidth, DWORD dwDepth,int offW,int offL);

private:	
	
	//resize .local
	void CreateTransformTerrainMesh(string strTitle, DWORD dwWidth, DWORD dwDepth);
	vector<int> CreateRiverInfo(CTerrainMesh *pTransformWater);
	void ChangeRegionLightInfo();
	void ClearInfo();

private:
	CTerrainMesh      *terrain;
	IProgressCallback *progress;
	string			  m_strDefaultTerrainTexName;
	string			  m_strDefaultWaterTexName;

	///��չ��ɵ�ͼϸ�������������µ�ͼ�е�������ӳ�� first old region index second new region index
	map<int, int> m_OldMapRegionIndexMap;
	///��չ��ɵ�ͼ�����������µ�ͼ�е�������ӳ�� first old grid   index second new grid index
	map<int, int> mapRevertSplitMapGridIndex;
};

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
inline void
CEnlargetMapData::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

}
