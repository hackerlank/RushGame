#pragma once
/**
	@class CLocalMapLoader

	Load .local file

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class COperator;
class SVertex;
class SGrid;
class IProgressCallback;
SmartPointer(CLocalMapLoader);

class CLocalMapLoader : public CRefObject
{
public:
	CLocalMapLoader();

	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);

	/// Load .Local File
	void LoadAsLocal(const string& path);

	void NewLoadAsLocal(const string& path);
	
private:
	void LoadAsOOldLocal(FILE * fp, DWORD& version, string strFileName);
	void LoadAsOldLocal(FILE * fp, DWORD& version, string strFileName);
	void LoadAsNewLocal(FILE * fp, DWORD& version, string strFileName);

	// ��ȡ������������
	void LoadTerrainGridTexIndex(FILE * fp, map<int16, string>& terrainTextureNames);
	// ��ȡÿ��������Ϣ
	void LoadEachGridInfo(FILE * fp, CTerrainMesh*pTerrain, SGrid* pGrid);

	/// ��ȡ��ͼ�ĵ��Դ��Ϣ
	void LoadPointLightInfo(FILE * fp);
	/// ��ȡ��ͼ�ľ۹����Ϣ
	void LoadSpotLightInfo(FILE * fp);
	/// ��ȡ��ȷ���赲��Ϣ
	void LoadExactBlockInfo(FILE * fp);
	/// ��ȡÿ��������û���˻�
	void LoadWaveSideInfo(FILE* file);
	/// ��ȡ���������ǲ����Ź�������Ϣ
	void LoadOffAnimateModelIndex(FILE* fp);	
	/// ��ȡˮ����Ϣ
	void LoadWaterInfo(FILE* fp);	
	/// ƴ�ӵر�����
	void MakeTerrainTexture(set<string> terrainTexIndexsSet);

	/// ������Ӳ�������ֵ
	void LoadTerGridMatIndex(FILE* fp);

// 	/// ��ȡ������Ч��Ϣ
// 	void LoadRgnAmbientFxInfoInfo(FILE * fp);
private:
	COperator*		      pOperator;
	IProgressCallback*    progress;
	map<int, vector<int>> LoadFaildModelMap;
	vector<string>		  ErrObject;
	locale				  loc;
	uint32				  uModelProVersion;
	uint32				  uGridProVersion;
	string				  m_strDefaultTerrainTexName;
	string				  m_strDefaultWaterTexName;
};

//------------------------------------------------------------------------------
inline void
CLocalMapLoader::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

}// namespace sqr