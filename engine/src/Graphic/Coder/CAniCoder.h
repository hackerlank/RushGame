#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CAniFormat : public CFileFormatMgr
	{
		// friend class CAnyObject;
	public:
		CAniFormat(void);
		~CAniFormat(void);
	};

	class CAniCoder 
		: public CCoder
		, public CCoderHelper
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ANIC'; }	//Coder������
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_Ani( const CDataChunk& inData , OBJ_HANDLE handle );
	};
}