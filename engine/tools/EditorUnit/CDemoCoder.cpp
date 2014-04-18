#include "stdafx.h"
#include "CDemoCoder.h"
#include <string>
//----------------------------------------------------------
class ChunkCoder1 : public CCoder
{
public:
	DECLARE_VERSION_MAP();
	FourCC	GetChunkName(void)	{ return 'CKC1'; }	//Coder������
	uint16	GetNewVer(void)		{ return 1; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

	bool Code(WriteDataInf& wdi , OBJ_HANDLE handle )
	{
		string str = "Hello World!";
		size_t len = str.size();
		wdi.Write(&len,sizeof(size_t));
		wdi.Write(str.c_str(),str.size());
		return true;
	}

	bool	DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle )
	{
		CBufFile tData( inData.GetBuffer(), inData.GetSize() );
		size_t	len;
		string  str;
		tData.SafeRead( &len, sizeof(size_t) );
		str.resize(len);
		tData.SafeRead( &str[0], len );// �����ȡʧ��, SafeRead����ı�ԭ����ֵ
		cout<<str;
		return true;
	}
};

BEGIN_VERSION_MAP(ChunkCoder1)
	ON_VERSION( 1 , DeCode_1 )
END_VERSION_MAP()

//-----------------------------------------------------------

class ChunkCoder2 
	: public CCoder
	, public CCoderHelper
{
public:
	DECLARE_VERSION_MAP();
	FourCC	GetChunkName(void)	{ return 'CKC2'; }	//Coder������
	uint16	GetNewVer(void)		{ return 2; }		//Coder�汾��,������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��

	bool Code(WriteDataInf& wdi , OBJ_HANDLE handle )
	{
		string str = "�Һ������һ��Chunk";
		size_t len = str.size();
		wdi.Write(&len,sizeof(size_t));
		str.resize(len);
		wdi.Write(str.c_str(),str.size());
		_AutoEncode(wdi,handle); //��������ط�˳����ֵ�Chunk
		str = "��ǰ����һ��Chunk";
		len = str.size();
		wdi.Write(&len,sizeof(size_t));
		str.resize(len);
		wdi.Write(str.c_str(),str.size());
		return true;
	}

	bool	DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle )
	{
		_AutoDecode(inData,handle); //�����Chunk�е���������Ҳ����Chunk��ʹ������������Զ����� 
		return true;
	}

	bool	DeCode_2( const CDataChunk& inData , OBJ_HANDLE handle )
	{
		CBufFile tData( inData.GetBuffer(), inData.GetSize() );
		size_t	len;
		string  str;
		tData.SafeRead( &len, sizeof(size_t) );
		tData.SafeRead( &str[0], len );
		cout<<str;

		//��ȡ�����Chunk
		CDataChunk tChunk;
		tData.SafeRead(&tChunk.GetHdr(),sizeof(CDataChunk::ChunkHdr));
		ReadChunk(tData,tChunk);
		_DoDecode(tChunk,handle);//��ȡ����ط�˳����ֵ�Chunk


		tData.SafeRead( &len, sizeof(size_t) );
		tData.SafeRead( &str[0], len );
		cout<<str;
		return true;
	}
};

BEGIN_VERSION_MAP(ChunkCoder2)
	ON_VERSION( 1 , DeCode_1 )
	ON_VERSION( 2 , DeCode_2 )
END_VERSION_MAP()

//-----------------------------------------------------------

class ChuckCoder2inner
	: public CCoder
{
public:
	DECLARE_VERSION_MAP();
	FourCC	GetChunkName(void)	{ return 'CK2i'; }	
	uint16	GetNewVer(void)		{ return 1; }

	bool Code(WriteDataInf& wdi , OBJ_HANDLE handle )
	{
		string str = "��������ָ���ַ��:";
		size_t len = str.size();
		wdi.Write(&len,sizeof(size_t));
		wdi.Write(str.c_str(),str.size());

		wdi.Write(&handle,sizeof(OBJ_HANDLE));
		return true;
	}

	bool	DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle )
	{
		CBufFile tData( inData.GetBuffer(), inData.GetSize() );
		size_t	len;
		string  str;
		tData.SafeRead( &len, sizeof(size_t) );
		tData.SafeRead( &str[0], len );// �����ȡʧ��, SafeRead����ı�ԭ����ֵ
		cout<<str;
		void* pObj;
		tData.SafeRead( &pObj, sizeof(void*) );
		if( pObj == (void*)(0xeefefeff) )
			cout<< "����ָ����ȷ";
		return true;
	}
};

BEGIN_VERSION_MAP(ChuckCoder2inner)
	ON_VERSION( 1 , DeCode_1 )
END_VERSION_MAP()

//-----------------------------------------------------------

CDemoFormat::CDemoFormat(void)
: CFileFormatMgr( 'Demo'/*�ļ���ʽ��ʾ*/, 0 /*�ļ����°汾��*/ )
{
}

CDemoFormat::~CDemoFormat(void)
{ /*do nothing~~~*/ }

class CAnyObject;

void CDemoFormat::TestMe(void)
{
	CDemoFormat TestFormat;
	CAnyObject* pObj = (CAnyObject*)(0xeefefeff); //ָ��Ҫ��ȡ/�洢�Ķ���
	
	TestFormat.RegisterCoder(new ChunkCoder1);

	ChunkCoder2* pCoder = new ChunkCoder2;
	pCoder->RegisterCoder(new ChuckCoder2inner);
	TestFormat.RegisterCoder(pCoder);

	TestFormat.Save( "C:/Demo.data", pObj ); //������ UE �򿪿���
	TestFormat.Load( "C:/Demo.data", pObj );
}