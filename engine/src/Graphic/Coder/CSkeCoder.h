#pragma once
#include "CFileFormatMgr.h"
#include "CSkeletal.h"

namespace sqr
{
	class CSkeFormat : public CFileFormatMgr
	{
		// friend class CAnyObject;
	public:
		CSkeFormat(void);
		~CSkeFormat(void);
	};

	class CSkeCoder 
		: public CCoder
		, public CCoderHelper
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'SKEC'; }	//Coder������
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_Ske( const CDataChunk& inData , OBJ_HANDLE handle );
	};
}