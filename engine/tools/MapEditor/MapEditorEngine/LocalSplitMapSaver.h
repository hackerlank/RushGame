#pragma once
/**
	@class CLocalSplitMapSaver

	Save .local File

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
SmartPointer(CLocalSplitMapSaver);

///�ָ��ͼ
class CLocalSplitMapSaver : public CRefObject
{
public:
	CLocalSplitMapSaver();
	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);
	
	void SetSplitMapPathName(const string& name);
	void SetSplitMapSizeInfo(const DWORD dwStartX, const DWORD dwStartZ, const DWORD dwSpiltMapWidth, const DWORD dwSplitMapDepth);

	/// Save merge .project File
	void SaveAsSplitProject();
	void LoadAsMapProject(const string& path);

	/// Save merge .operaterinfo File
	void SaveAsSplitRgnLightInfo();

	/// Save merge .Local File
	void SaveAsSplitLocal();

	/// Save .audio File
	void SaveAsAudio();

private:
	// �����ͼ�赲��Ϣ
	void SaveSplitMapBlockInfo(const int nSaveMapGridCnt);
	// �����ͼ�ĵ��Դ��Ϣ
	void SavePointLightInfo(FILE * fp, const int SceneLightCount);
	// �����ͼ�ľ۹����Ϣ
	void SaveSpotLightInfo(FILE * fp, const int nSpotLightCnt);
	/// ����ÿ��������û���˻�
	void SaveWaveSideInfo(FILE *pRead, FILE *pSave, const int nSaveMapGridCnt, const int nReadMapGridCnt);
	/// ֻ������������ǲ����Ź�������Ϣ
	void SaveOffAnimateModelIndex(FILE * fp);	
	/// ������Ӳ�������ֵ
	void SaveSplitMapGridMatIndexInfo(FILE * fp, DWORD newGridCnt);
	/// ���澫ȷ���赲��Ϣ
	void SaveSplitMapExactBlockInfo(FILE * fp, DWORD newGridCnt);
	/// ����ÿ��������û���˻�
	void SaveWaveSideInfo(FILE* file, DWORD newGridCnt);

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

	///�ָ��ĵ�ͼ���������뱻�ָ��ͼ�ĸ����������ӳ�� first split map grid index; second splited map grid index
	map<int, int> mapSplitMapGridIndex;
	map<int, int> mapRevertSplitMapGridIndex;
	///�ָ��ĵ�ͼ���������뱻�ָ��ͼ�Ķ����������ӳ�� first split map vertex index; second splited map vertex index
	map<int, int> mapSplitMapVertexIndex;

	DWORD m_dwSplitStartX;
	DWORD m_dwSplitStartZ;
	DWORD m_dwSplitMapWidth;
	DWORD m_dwSplitMapDepth;

	string m_strDefaultTerrainTexName;
};

//------------------------------------------------------------------------------
inline void
CLocalSplitMapSaver::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

//------------------------------------------------------------------------------
inline void
CLocalSplitMapSaver::SetSplitMapPathName(const string& name)
{
	m_strMapPathName = name;
}

//------------------------------------------------------------------------------
inline void
CLocalSplitMapSaver::SetSplitMapSizeInfo(const DWORD dwStartX, const DWORD dwStartZ, 
										 const DWORD dwSpiltMapWidth, const DWORD dwSplitMapDepth)
{
	m_dwSplitStartX		= dwStartX;
	m_dwSplitStartZ		= dwStartZ;
	m_dwSplitMapWidth   = dwSpiltMapWidth;
	m_dwSplitMapDepth   = dwSplitMapDepth;
}


}// namespace sqr