#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CEditorTerGridCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'EGRI'; }	//Coder������ edit grid
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};


	class CEditorTerWaterCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'EWTR'; }	//Coder������edit water
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );

		void LoadWaveSideInfo(CBufFile& tData);
		void SaveWaveSideInfo(WriteDataInf& wdi);
	};

	class CEditorTerrainFormat : public CFileFormatMgr
	{
	public:
		CEditorTerrainFormat(void);
		~CEditorTerrainFormat(void);

		void InitTerrainCode();
		bool LoadTerrainFile(const string& strFile, int nRegionIndex);
		bool SaveTerrainFile(const string& strFile, int nRegionIndex);
	};
}