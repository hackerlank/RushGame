#pragma once

#include <vector>

namespace sqr
{
	class CTerrainMesh;
	
}

namespace MapEditor
{
	class CMapDataTrans;

	class CMapMgr
	{
	public:
		CMapMgr(void);
		CMapMgr::~CMapMgr(void);
		void Init(char* opFileName);					//�ⲿ�ļ������˲������ĳ���
		//����
		bool CreateMap(int w,int h,bool useHMap=false);
		bool ResizeMap(int dwWidth, int dwDepth,int offW,int offL);
		bool LoadOldMap(void* fname);
		void SaveOldMap(void* filename);
	};
}