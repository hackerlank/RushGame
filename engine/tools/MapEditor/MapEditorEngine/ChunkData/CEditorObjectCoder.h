#pragma once
#include "CFileFormatMgr.h"

namespace sqr
{
	class CEditorObjectCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ETOB'; }	//Coder������ Edit object 
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CEditorCompositeObjectCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ECOB'; }	//Coder������ Edit object ����ͼ����
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CEditorLogicObjectCoder : public CCoder
	{
	public:
		DECLARE_VERSION_MAP();
		FourCC	GetChunkName(void)	{ return 'ELOG'; }	//Coder������ editor login
		uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

		bool Code(WriteDataInf& wdi , OBJ_HANDLE handle );
		bool DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle );
	};

	class CEditorObjectFormat : public CFileFormatMgr
	{
	public:
		CEditorObjectFormat(void);
		~CEditorObjectFormat(void);

		void InitObjectCode();
		bool LoadObjectFile(const string& strFile, int nRegionIndex);
		bool SaveObjectFile(const string& strFile, int nRegionIndex);
	};
}