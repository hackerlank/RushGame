#pragma once
#include "CFileFormatMgr.h"
#include "SQRWnd.h"

namespace sqr
{
	class CGuiFormat : public CFileFormatMgr
	{
	public:
		CGuiFormat(void);
		~CGuiFormat(void);
	};
	
	struct CoderSoundInfo
	{
		int		tp;
		string	sf;
	};

	struct CoderFlashInfo
	{
		int		tp;
		string	fi;
	};

	struct CoderImagePro
	{
		int		cm;
		SQRWnd* wnd;
	};

	struct CoderImageState
	{
		IMAGE_PARAM		ip;
		WND_IMAGE_LIST* wil;
		SQRWnd*			wnd;
	};

	class CGUIPerSoundProCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIPerSoundProCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};
	// һЩ���Ե�CODER
	// GUI ����
	class CGUISoundProCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUISoundProCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};


	class CGUIPerFlashInfoCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIPerFlashInfoCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// flash info ָʾ��Ϣ
	class CGUIFlashInfoCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIFlashInfoCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};


	class CGUIPerImageProCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIPerImageProCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// image pro coder
	class CGUIImageProCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIImageProCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// image pro coder
	class CGUIImgProMskOneCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIImgProMskOneCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// image pro coder
	class CGUIImgProMskOtherCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIImgProMskOtherCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// image pro coder
	class CGUIImgProStateCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIImgProStateCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// image pro coder
	class CGUIImgTextureCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIImgTextureCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};


	// ����CHILD WND��CODER
	class CGUIChildWndCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIChildWndCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// ��WND MAP��CODER
	class CGUIWndsMapCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIWndsMapCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};


	class CGUIWndsDefProCoder
		: public CCoder
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIWndsDefProCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// ����WND��CODER
	class CGUIWndItemCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIWndItemCoder();
		FourCC	GetChunkName(void);							// coder chunk name
		uint16	GetNewVer(void)		{ return 1; }			// coder version
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData, OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};

	// ����GUI��ԴCODER
	class CGUIResCoder
		: public CCoder
		, public CCoderHelper
	{
		DECLARE_VERSION_MAP();
	public:
		CGUIResCoder();
		FourCC	GetChunkName(void);						// Coder������
		uint16	GetNewVer(void)		{ return 1; }		// Coder�汾��, ������Ϣʹ�ú�ӷ����������汾�ţ�Chunk��ʽ��Ѫ��ʱ�������汾��
		uint16	GetChunkType()		{ return CDataChunk::DCT_AUTO; }

		bool	Encode( WriteDataInf& wdi, OBJ_HANDLE handle );
		bool	Decode1( const CDataChunk& inData , OBJ_HANDLE handle );

	private:
		bool	_EncodeImpl( WriteDataInf& wdi, PRIVATE_HANDLE handle );
	};
}
