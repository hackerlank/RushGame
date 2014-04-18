#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CGameAudioCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'AUID'; }	//Coder������ Audio
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CGameAudioFormat : public CFileFormatMgr
	{
	public:
		CGameAudioFormat(void);
		~CGameAudioFormat(void);

		void InitAudioCode();
		bool LoadAudioFile(const string& strFile, int nRegionIndex);
		bool SaveAudioFile(const string& strFile, int nRegionIndex);
	};
}