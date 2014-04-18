#pragma once
/**
	@class CLocalMergeMapSaver

	Save .local File

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
SmartPointer(CLocalMergeMapSaver);

class CLocalMergeMapSaver : public CRefObject
{
public:
	CLocalMergeMapSaver();
	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);
	void SetMapPathName(const string& name);

	/// Save merge .project File
	void SaveAsMergeProject();
	void LoadAsMapProject(const string& path);

	/// Save merge .operaterinfo File
	void SaveAsMergeRgnLightInfo();
	void LoadAsRgnLightInfo(const string& path);

	void CreateReadMapLightInfo(const string& path);

	/// Save merge .Local File
	void SaveAsMergeLocal(const string& path, const int xSort, const int zSort);

	/// Save .audio File
	void SaveAsAudio(const string& path);
	
	/// Save .audio File
	void SaveAsLogin(const string& path, set<int> nGridIndexs);

	/// Save .rgn File
	void SaveAsRgn(const string& path);

private:
	// �����ͼ�赲��Ϣ
	void SaveMergeMapBlockInfo(const string& path, const int nSaveMapGridCnt, const int nReadMapGridCnt);
	// ����ˮ��Ϣ
	void SaveWaterInfo(FILE *pRead, FILE *pSave, const int nSaveMapGridCnt, const int nReadMapGridCnt);
	// �����ͼ�ĵ��Դ��Ϣ
	void SavePointLightInfo(FILE *pRead, FILE *pSave);
	// �����ͼ�ľ۹����Ϣ
	void SaveSpotLightInfo(FILE *pRead, FILE *pSave);
	/// ���澫ȷ���赲��Ϣ
	void SaveExactBlockInfo(FILE *pRead, FILE *pSave);
	/// ����ÿ��������û���˻�
	void SaveWaveSideInfo(FILE *pRead, FILE *pSave, const int nSaveMapGridCnt, const int nReadMapGridCnt);
	/// ֻ������������ǲ����Ź�������Ϣ
	void SaveOffAnimateModelIndex(FILE *pRead, FILE *pSave);	
	/// ������Ӳ�������ֵ
	void SaveTerGridMatIndexInfo(FILE *pRead, FILE *pSave, const int nSaveMapGridCnt, const int nReadMapGridCnt);
	void SaveRecordInfo(FILE *pRead, FILE *pSave);
	void MakeTerrainTexture(set<string> terrainTexIndexsSet);

private:
	CTerrainMesh* terrain;
	IProgressCallback* progress;
	set<string>	  setTitleResourceName;
	set<string>	  setEffecteResourceName;
	string		  m_strMapPathName;
	///�ϲ���ɵ�ͼ�����������µ�ͼ�е�������ӳ�� first old vertex index second new vertex index
	map<int, int> mapOldMapVertexIndex;
	map<int, int> mapReadMapVertexIndex;
	///�ϲ���ɵ�ͼ�����������µ�ͼ�е�������ӳ�� first old grid index second new grid index
	map<int, int> mapOldMapGridIndex;
	map<int, int> mapReadMapGridIndex; //first read grid index second new save index
	///�ϲ���ɵ�ͼϸ�������������µ�ͼ�е�������ӳ�� first read region divide index second save region divide index
	map<int, int> mapReadMapDivideRegionIndex;
	set<string> setHasRegNames;

	uint32 uModelVersion;
	uint32 uGridProVersion;	

	string m_strDefaultTerrainTexName;
};

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
inline void
CLocalMergeMapSaver::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

//------------------------------------------------------------------------------
inline void
CLocalMergeMapSaver::SetMapPathName(const string& name)
{
	m_strMapPathName = name;
}

}// namespace sqr