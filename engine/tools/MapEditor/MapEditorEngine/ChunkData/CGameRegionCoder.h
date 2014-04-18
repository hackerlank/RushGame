#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CGameRegionCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'SRGN'; }	//Coder������
		DWORD	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WirteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CGameRegionFormat : public CFileFormatMgr
	{
	public:
		CGameRegionFormat(void);
		~CGameRegionFormat(void);

		void InitRegionCode();
		bool LoadRegionFile(const string& strFile, int nRegionIndex);
		bool SaveRegionFile(const string& strFile, int nRegionIndex);
	};
}