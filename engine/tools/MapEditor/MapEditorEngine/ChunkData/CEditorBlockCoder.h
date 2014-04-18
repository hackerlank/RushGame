#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CEditorBlockCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'LBTI'; }	//Coder������ Low Block Texture Info
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CEditorBlcokFormat : public CFileFormatMgr
	{
	public:
		CEditorBlcokFormat(void);
		~CEditorBlcokFormat(void);

		void InitBlockCode();
		bool LoadBlockFile(const string& strFile);
		bool SaveBlockFile(const string& strFile);
	};
}