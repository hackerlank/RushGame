#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CEditorAmbientCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'EAMB'; }	//Coder������editor ambient
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );

		void SaveOperationInfo(WriteDataInf& wdi);
		void SaveToolMainPlayerInfo(WriteDataInf& wdi);
		void SaveWaveInfo(WriteDataInf& wdi);
		void SaveGridModeProVersion(WriteDataInf& wdi);
		
		void LoadOperationInfo(CBufFile& tData);
		void LoadToolMainPlayerInfo(CBufFile& tData);
		void LoadWaveInfo(CBufFile& tData);
		void LoadGridModeProVersion(CBufFile& tData);
	};

	class CEditorTileResoureCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ETLR'; }	//Coder������editor tile resoure
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );

		void SaveResoureceSetInfo(WriteDataInf& wdi);
		void LoadResoureceSetInfo(CBufFile& tData);///��ͼ����ͼ�ذ�����
	};

	///ֻ�洢���������
	class CEditorRegionLightCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ERLT'; }	//Coder������editor region light�������Ϣ
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CEditorMapResCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ERES'; }	//Coder������editor resource
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );

	};

	///record info 
	class CEditorRecordCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ERRD'; }	//Coder������editor record
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );

	};

	class CEditorAmpArsFormat : public CFileFormatMgr
	{
	public:
		CEditorAmpArsFormat(void);
		~CEditorAmpArsFormat(void);

		void InitAmpArsCode();
		bool LoadAmpArsFile(const string& strFile);
		bool SaveAmpArsFile(const string& strFile);
	};
}