#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CGameAmbientCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'AMBT'; }	//Coder������ambient
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );

		void SaveAmbientFXInfo(WriteDataInf& wdi);
		void LoadAmbientFXInfo(CBufFile& tData);
	};

	///ֻ�洢�������Ϣ
	class CGameRegionLightCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'RGLT'; }	//Coder������region light�������Ϣ
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CGameMapResCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'MRES'; }	//Coder������map resource
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );

		//------------------------------------------------------------------------------
		void MakeAtlasTexture();
	};

	class CGameAmpArsFormat : public CFileFormatMgr
	{
	public:
		CGameAmpArsFormat(void);
		~CGameAmpArsFormat(void);

		void InitAmpArsCode();
		bool LoadAmpArsFile(const string& strFile);
		bool SaveAmpArsFile(const string& strFile);
	};
}