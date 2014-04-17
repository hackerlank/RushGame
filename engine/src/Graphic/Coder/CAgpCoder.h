#pragma once
#include "CFileFormatMgr.h"
#include "CAnimation.h"
namespace sqr
{
	class CAgpFormat : public CFileFormatMgr
	{
	public:
		CAgpFormat(void);
		~CAgpFormat(void);
	};

	class CAgpCoder 
		: public CCoder
		, public CCoderHelper
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'AGPC'; }	//Coder������
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_Agp( const CDataChunk& inData , OBJ_HANDLE handle );
	};
}