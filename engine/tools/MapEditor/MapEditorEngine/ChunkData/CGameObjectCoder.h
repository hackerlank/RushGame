#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	enum CODER_RES_TYPE
	{
		RFT_MOD = MAKEFOURCC( 'M', 'O', 'D', 0 ),		//ģ�������ļ�
		//RFT_SKE = MAKEFOURCC( 'S', 'K', 'E', 0 ),		//���������ļ� RenderObject define
		RFT_EFX = MAKEFOURCC( 'E', 'F', 'X', 0 ),		//��Ч�ļ�
	};

	class CGameSceneObjectCoder : public CCoder
	{

	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'OBJT'; }	//Coder������ Object
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CGameLogicObjectCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'LOGI'; }	//Coder������ Object
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CGameObjectFormat : public CFileFormatMgr
	{
	public:
		CGameObjectFormat(void);
		~CGameObjectFormat(void);

		void InitObjectCode();
		bool LoadObjectFile(const string& strFile, int nRegionIndex);
		bool SaveObjectFile(const string& strFile, int nRegionIndex);
	};
}