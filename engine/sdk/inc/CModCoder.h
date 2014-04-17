#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CModFormat : public CFileFormatMgr
	{
	public:
		CModFormat(void);
		~CModFormat(void);
	};

	class CPieceGroupCoder 
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CPieceGroupCoder();
		FourCC	GetChunkName(void);						// Coder������
		uint16	GetNewVer(void)		{ return 1; }		// Coder�汾��, ������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData , OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};
}