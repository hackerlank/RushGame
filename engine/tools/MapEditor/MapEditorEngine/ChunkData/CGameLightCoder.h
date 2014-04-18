#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CGamePointLightCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'SPLT'; }	//Coder������scene point light
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CGameSpotLightCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'SSLT'; }	//Coder������scene spot light
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CGameLightFormat : public CFileFormatMgr
	{
	public:
		CGameLightFormat(void);
		~CGameLightFormat(void);

		void InitLightCode();
		bool LoadLightFile(const string& strFile, int nRegionIndex);
		bool SaveLightFile(const string& strFile, int nRegionIndex);
	};
}