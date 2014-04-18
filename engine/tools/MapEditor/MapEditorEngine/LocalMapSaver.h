#pragma once
/**
	@class CLocalMapSaver

	Save .local File

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"
#include "CVector3.h"

namespace sqr
{
class CTerrainMesh;
class COperator;
class SGrid;
class IProgressCallback;
SmartPointer(CLocalMapSaver);

class CLocalMapSaver : public CRefObject
{
public:
	CLocalMapSaver();
	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);

	/// Save .Local File
	void SaveAsLocal(const string& path);

	///resize .local
	bool ResizeAndSaveAsLocal(string strTitle, DWORD dwWidth, DWORD dwDepth);

	/// Save .audio File
	void SaveAsAudio(const string& path, bool bResize);
	
	/// Save .audio File
	void SaveAsLogin(const string& path, set<int> nGridIndexs);

	/// Save .rgn File
	void SaveAsRgn(const string& path);
private:
	// �����ͼ�ĵ��Դ����Ϣ
	void SavePointLightInfo(FILE * fp, const int SceneLightCount, bool& bSceneLightCntEqual);
	// �����ͼ�ľ۹����Ϣ
	void SaveSpotLightInfo(FILE * fp, const int nSpotLightCnt, bool& bSaveLocalFile);
	/// ���澫ȷ���赲��Ϣ
	void SaveExactBlockInfo(FILE * fp);
	/// ���澫ȷ���赲��Ϣ
	void SaveReSizeExactBlockInfo(FILE * fp, DWORD dwWidth, DWORD dwDepth);
	/// ����ÿ��������û���˻�
	void SaveWaveSideInfo(FILE* file);
	/// ֻ������������ǲ����Ź�������Ϣ
	void SaveOffAnimateModelIndex(FILE* fp);	
	/// ������Ӳ�������ֵ
	void SaveTerGridMatIndexInfo(FILE* fp);
	/// ���澫ȷ���赲��Ϣ
	void SaveReSizeTerGridMatIndexInfo(FILE * fp, DWORD dwWidth, DWORD dwDepth);
	///����ɵ�ͼ������������µ�ͼ�е�����ֵ
	void SaveOldMapStartCoordinateInNewMap(const DWORD dwNewWidth, const DWORD dwNewDepth, const int nOldSatrtGridInNewIndex);
	//// ����������Ч��Ϣ
	//void SaveRgnAmbientFxInfo(FILE *fp, bool bResize, bool bSplit);
private:
	CTerrainMesh*	   terrain;
	COperator*		   pOperator;
	IProgressCallback* progress;
	string			   m_strDefaultTerrainTexName;
};

//------------------------------------------------------------------------------
inline void
CLocalMapSaver::SetTerrainMesh( CTerrainMesh* t )
{
	Ast(NULL != t);
	this->terrain = t;
}

//------------------------------------------------------------------------------
inline void
CLocalMapSaver::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

}// namespace sqr