#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CEditorRegionCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'EDRG'; }	//Coder������ edit rengion
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CEditorRegionFormat : public CFileFormatMgr
	{
	public:
		CEditorRegionFormat(void);
		~CEditorRegionFormat(void);

		void InitRegionCode();
		bool LoadRegionFile(const string& strFile);
		bool SaveRegionFile(const string& strFile);
	};
}