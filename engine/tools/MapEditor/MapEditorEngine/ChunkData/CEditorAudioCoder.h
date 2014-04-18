#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CEditorAudioCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ETAD'; }	//Coder������ Edit audio 
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CEditorAudioFormat : public CFileFormatMgr
	{
	public:
		CEditorAudioFormat(void);
		~CEditorAudioFormat(void);

		void InitAudioCode();
		bool LoadAudioFile(const string& strFile, int nRegionIndex);
		bool SaveAudioFile(const string& strFile, int nRegionIndex);
	};
}