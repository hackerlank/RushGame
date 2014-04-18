#include "stdafx.h"
#include "Texture.h"
#include "BaseHelper.h"
#include "../../Include/GraphicCommon.h"

#define MAX_MIPMAP 3
static int		g_nLinePerPitch[sqr::TF_COUNT]	= {	 4,  4,  1,  1,  1,  1, 1 };
static int		g_nBitPerPixel[sqr::TF_COUNT]	= {  4,  8, 24, 32, 16, 16, 8 };
static int		g_nSpace[sqr::TF_COUNT]			= {  2,  4,  3,  4,  2,  2, 1 };
static D3DFORMAT	g_eDataFormat[sqr::TF_COUNT]		= { D3DFMT_DXT1, D3DFMT_DXT5, D3DFMT_R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_R5G6B5, D3DFMT_A4R4G4B4, D3DFMT_A8 };
static D3DFORMAT	g_eSaveFormat[sqr::TF_COUNT]		= { D3DFMT_DXT1, D3DFMT_DXT5, D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_R5G6B5, D3DFMT_A4R4G4B4, D3DFMT_A8 };

//����MipMap��С����С4x4,�������ظ���
int GetMipMapSize( int nWidth, int nHeight )
{
	int n = nWidth*nHeight;
	while( nWidth > 4 && nHeight > 4 )
	{
		nWidth  = nWidth >> 1;
		nHeight = nHeight >> 1;
		n += nWidth*nHeight;
	}
	return n;
}


////////////////////////////////////////////////////////////////////////
// ���ܣ����ѹ��BUG
// ������BYTE *buf        ѹ��������,
//       D3DFORMAT format ѹ����������ʽ,
//       DWORD* ref       δѹ��������,
//       int w,int h  ����
// ���أ���
////////////////////////////////////////////////////////////////////////
inline DWORD CheckARGB8toDXT(BYTE *buf,DWORD* ref,D3DFORMAT format)
{
	int i,j,k;
	BYTE alpha[8];
	BYTE out[16];
	BYTE re[16];
	for(i=0;i<16;i++)
		re[i]=(BYTE)(ref[i]>>24);
	
	//�Ƚ�ѹ��ƫ�����ͳ��
	alpha[0]=buf[0];
	alpha[1]=buf[1];
	if (alpha[0]>alpha[1]) 
	{    
		alpha[2]=(6*alpha[0]+1*alpha[1]+3)/7;    // bit code 010
		alpha[3]=(5*alpha[0]+2*alpha[1]+3)/7;    // bit code 011
		alpha[4]=(4*alpha[0]+3*alpha[1]+3)/7;    // bit code 100
		alpha[5]=(3*alpha[0]+4*alpha[1]+3)/7;    // bit code 101
		alpha[6]=(2*alpha[0]+5*alpha[1]+3)/7;    // bit code 110
		alpha[7]=(1*alpha[0]+6*alpha[1]+3)/7;    // bit code 111  
	}    
	else 
	{  
		alpha[2]=(4*alpha[0]+1*alpha[1]+2)/5;    // Bit code 010
		alpha[3]=(3*alpha[0]+2*alpha[1]+2)/5;    // Bit code 011
		alpha[4]=(2*alpha[0]+3*alpha[1]+2)/5;    // Bit code 100
		alpha[5]=(1*alpha[0]+4*alpha[1]+2)/5;    // Bit code 101
		alpha[6]= 0;                             // Bit code 110
		alpha[7]= 255;                           // Bit code 111
	}
	for(i=0,j=0;i<16;i++)
	{
		int bt=j/8;
		int st=j%8;
		BYTE alin;
		if(st<6)
			alin=((buf[2+bt])>>st)&0x7;
		else if(st==6)				
			alin=(((buf[2+bt])>>st)&0x7)|(((buf[3+bt])<<2)&0x7);
		else if(st==7)				
			alin=(((buf[2+bt])>>st)&0x7)|(((buf[3+bt])<<1)&0x7);
		out[i]=alpha[alin];
		j+=3;
	}

	int sum=0;
	for(i=0;i<16;i++)
		sum+=abs(out[i]-re[i]);

	//���ƫ��̫������ѹ��
	if(sum>256)
	{
		alpha[0]=re[0];
		alpha[1]=re[0];
		//ѡ��ALPHA��������ֵ
		for(i=0;i<16;i++)
		{
			if(re[i]>alpha[0])
				alpha[0]=re[i];
			if(re[i]<alpha[1])
				alpha[1]=re[i];
		}
		buf[0]=alpha[0];
		buf[1]=alpha[1];
		buf[2]=0;
		buf[3]=0;
		buf[4]=0;
		buf[5]=0;
		buf[6]=0;
		buf[7]=0;
		//���Բ�ֵ�����������
		if (alpha[0]>alpha[1]) 
		{    
			alpha[2]=(6*alpha[0]+1*alpha[1]+3)/7;    // bit code 010
			alpha[3]=(5*alpha[0]+2*alpha[1]+3)/7;    // bit code 011
			alpha[4]=(4*alpha[0]+3*alpha[1]+3)/7;    // bit code 100
			alpha[5]=(3*alpha[0]+4*alpha[1]+3)/7;    // bit code 101
			alpha[6]=(2*alpha[0]+5*alpha[1]+3)/7;    // bit code 110
			alpha[7]=(1*alpha[0]+6*alpha[1]+3)/7;    // bit code 111  
		}    
		else 
		{  
			alpha[2]=(4*alpha[0]+1*alpha[1]+2)/5;    // Bit code 010
			alpha[3]=(3*alpha[0]+2*alpha[1]+2)/5;    // Bit code 011
			alpha[4]=(2*alpha[0]+3*alpha[1]+2)/5;    // Bit code 100
			alpha[5]=(1*alpha[0]+4*alpha[1]+2)/5;    // Bit code 101
			alpha[6]= 0;                             // Bit code 110
			alpha[7]= 255;                           // Bit code 111
		}
		//��16��4X4������ѡ������ʵ�ALPHAֵ��������������д�루����Ϊÿ������3��BIT��
		for(i=0,j=0;i<16;i++)
		{
			int bt=j/8;
			int st=j%8;

			int alin=0;
			int rem=255;
			for(k=0;k<8;k++)
			{
				if(abs(alpha[k]-re[i])<rem)
				{
					rem=abs(alpha[k]-re[i]);
					alin=k;
				}
			}

			buf[2+bt]|=alin<<st;
			if(st==6)
				buf[3+bt]|=alin>>2;
			else if(st==7)
				buf[3+bt]|=alin>>1;
			j+=3;
		}
	}
	
	return 1;
}


////////////////////////////////////////////////////////////////////////
// ���ܣ����ѹ��BUG
// ������BYTE* dest   Ŀ�꣨ѹ����������,
//       D3DFORMAT df Ŀ�꣨ѹ������������ʽ,
//       BYTE* src    Դ��δѹ����������,
//       D3DFORMAT sf Դ��δѹ������������ʽ,
//       int w,int h  ����
// ���أ���
////////////////////////////////////////////////////////////////////////
BOOL CheckCompress(BYTE* dest,D3DFORMAT df,BYTE* src,D3DFORMAT sf,int w,int h)
{	
	//DXT��ʽ��4X4Ϊ��Ԫ�������ÿһ����Ԫ���м��
	if(sf==D3DFMT_A8R8G8B8&&(df==D3DFMT_DXT1||df==D3DFMT_DXT5))
	{
		DWORD* ts=(DWORD*)src;
		BYTE* td=dest;
		DWORD temp[16];
		int i,j,n,m;
		int space=(df==D3DFMT_DXT1)?2:4;
		int unisize=space*4;
		for(i=0;i<h/4;i++)
		{
			for(j=0;j<w/4;j++)
			{
				for(m=0;m<4;m++)
				{
					for(n=0;n<4;n++)
					{
						temp[m*4+n]=ts[(i*4+m)*w+j*4+n];
					}
				}
				CheckARGB8toDXT(td,temp,sf);
				td+=unisize;
			}
		}
	}

	return 1;
}

CTexture::CTexture(void)
{
    m_pTextures		= NULL;
	m_pD3dDev		= NULL;
	m_bSizeChanged	= FALSE;
	m_bUseOriginSize	= FALSE;
	m_SaveType		= eSTY_TEX;
}

CTexture::~CTexture(void)
{
	Free();
}

int CTexture::Free()
{
	if( m_pTextures )
	{
		for( unsigned i = 0; i < m_Head.uFrameCount; i++ )
			SafeRelease( m_pTextures[i] );
		delete[] m_pTextures;
	}

	m_Head      = sqr::sTextureInfo();
	m_Filename  = string("");
    m_pTextures	= NULL;
	m_pD3dDev   = NULL;

	return 0;
}


////////////////////////////////////////////////////////////////////////
// ���ܣ�����ͼƬ
// ������CD3d* D3d ��Ⱦ�豸, 
//       LPCTSTR TextName �ļ���
// ���أ�>=0 �ɹ���<0 ʧ��
////////////////////////////////////////////////////////////////////////
int CTexture::LoadPic( LPDIRDEV pD3dDev, LPCTSTR TextName )
{
	if( !TextName || strlen( TextName )<4 )
		return GERROR;

	LPCTSTR Ext = TextName + strlen( TextName ) - 4;

	if( !_stricmp( ".tex", Ext ) )
		return LoadTEX( pD3dDev, TextName );
	else
		return LoadCOM( pD3dDev, TextName );
}

int CTexture::LoadCOM( LPDIRDEV pD3dDev, LPCTSTR TextName )
{
	LPDIRECT3DTEXTURE9 TempText;

	if( FAILED( D3DXCreateTextureFromFile( pD3dDev, TextName, &TempText ) ) )
	{
		MessageBox( 0, TextName, "��������", 0 );
		return GERROR;
	}

	
	// ����ļ���Ϣ
	D3DSURFACE_DESC desc;
	TempText->GetLevelDesc( 0,&desc );
	SafeRelease( TempText );

	// ��ȡԭʼ�ļ���Ϣ
	D3DXIMAGE_INFO	originImageInfo;
	D3DXGetImageInfoFromFile( TextName, &originImageInfo );
	if ( originImageInfo.Width != desc.Width || originImageInfo.Height != desc.Height )
	{
		m_bSizeChanged	= true;
	}
	else
	{
		m_bSizeChanged	= false;
	}
	memset( &m_OldHead, 0, sizeof(sqr::sTextureInfo) );
	m_OldHead.eFormat	= TransToLocalFormat( originImageInfo.Format );
	m_OldHead.uWidth	= originImageInfo.Width;
	m_OldHead.uHeight	= originImageInfo.Height;
	m_OldHead.uMipMap	= originImageInfo.MipLevels;
	m_OldHead.uFrameCount	= 1;

	//���ļ���С���ó�2����
	sqr::sTextureInfo TempHead;
	memset( &TempHead, 0, sizeof(sqr::sTextureInfo) );
	TempHead.uWidth		= desc.Width;
	TempHead.uHeight	= desc.Height;
	TempHead.uMipMap	= m_OldHead.uMipMap;
	TempHead.eFormat	= sqr::TF_ARGB32;
	TempHead.uFrameCount = 1;

	//��������D3DFMT_A8R8G8B8��ʽ��ͼ
	if( FAILED( D3DXCreateTextureFromFileEx( pD3dDev, TextName, 
		TempHead.uWidth, TempHead.uHeight, TempHead.uMipMap, 0, D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE, D3DX_FILTER_TRIANGLE, 0, NULL, NULL, &TempText ) ) )
		return GERROR;

	TempText->GetLevelDesc( 0,&desc );
	if(desc.Format!=D3DFMT_A8R8G8B8||desc.Width!=TempHead.uWidth||desc.Height!=TempHead.uHeight)
	{
		SafeRelease( TempText );
		return GERROR;
	}

	Free();
	m_Filename	   = TextName;
	m_pD3dDev      = pD3dDev;
	m_Head         = TempHead;
	m_pTextures	   = new LPDIRECT3DTEXTURE9[1];
	m_pTextures[0] = TempText;

	return 0;
}

int CTexture::LoadTEX( LPDIRDEV pD3dDev, LPCTSTR TextName )
{
	Free();
	m_pD3dDev	   = pD3dDev;
	m_Filename	   = TextName;

	locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·���������ģ�����ȫ��localeΪ���ػ���
	ifstream File( TextName, ios::binary|ios::in );
	locale::global(loc);
	//�����ļ�ͷ
	//�����ļ�ͷ
	DWORD Res; 
	DWORD Version;
	File.read( (TCHAR*)&Res, sizeof(DWORD) );
	File.read( (TCHAR*)&Version, sizeof(DWORD) );

	if( Res != MAKEFOURCC( 'T','E','X', 0 ) || Version < TEXTURE_VERSION - 1 || Version > TEXTURE_VERSION )
	{
		MessageBox( 0, TextName, "��������", 0 );
		return GERROR;
	}

	if( Version == TEXTURE_VERSION )
        File.read( (TCHAR*)&m_Head,sizeof(m_Head) );
	else if( Version == TEXTURE_VERSION - 1 )
	{
		int uBuf[16];
		File.read( (TCHAR*)uBuf, sizeof(uBuf) );
		m_Head.eFormat = (sqr::eTextureFormat)uBuf[1];
		m_Head.uWidth = uBuf[3];
		m_Head.uHeight = uBuf[4];
		m_Head.uMipMap = 0;
		m_Head.uFrameCount = 1;
		m_Head.uFrameCircle = 0;
	}
	m_OldHead		= m_Head;

	int nSpace		= m_Head.uWidth*g_nSpace[m_Head.eFormat];
	D3DFORMAT Format	= g_eDataFormat[m_Head.eFormat];

	//��D3DFMT_A8R8G8B8��ʽ��������
	m_pTextures = new LPDIRECT3DTEXTURE9[ m_Head.uFrameCount ];

	for( unsigned i = 0; i < m_Head.uFrameCount; i++ )
	{
		if(D3DXCreateTexture( m_pD3dDev,m_Head.uWidth,m_Head.uHeight, m_Head.uMipMap, 0,
			D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTextures[i] ) != D3D_OK )
		{
			File.close();
			Free();
			return GERROR;
		}
		D3DSURFACE_DESC desc;
		m_pTextures[i]->GetLevelDesc(0,&desc);

		int Size = 0;
		if( Version == TEXTURE_VERSION - 1 )
			File.read( (TCHAR*)&Size,sizeof(Size));
		else
		{
			unsigned uSize = m_Head.uWidth*m_Head.uHeight*g_nBitPerPixel[m_Head.eFormat]/8;
			for( unsigned i = 0; i < m_Head.uMipMap; uSize>>2, i++ )
				Size += uSize;
		}

		BYTE *Buf=new BYTE[Size];
		File.read( (TCHAR*)Buf,Size);

		LPSURFACE pSur;
		m_pTextures[i]->GetSurfaceLevel(0,&pSur);
		RECT rect={0,0,m_Head.uWidth,m_Head.uHeight};

		if( FAILED( D3DXLoadSurfaceFromMemory( pSur, NULL, NULL, Buf, Format,
			nSpace, NULL, &rect, D3DX_FILTER_TRIANGLE, 0 ) ) )
		{
			SafeRelease(pSur);
			File.close();
			Free();
			return GERROR;
		}
		SafeRelease(pSur);

		// ����mipmap
		if( FAILED( D3DXFilterTexture( m_pTextures[i], NULL, 0, D3DX_FILTER_TRIANGLE ) ) )
			return GERROR;

		delete []Buf;
	}

	File.close();

	return 0;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ��������֡
// ������ LPCTSTR szFileName  �ļ���
// ���أ�>=0 �ɹ���<0 ʧ��
////////////////////////////////////////////////////////////////////////
void CTexture::AddExFrame( LPCTSTR szFileName )
{
	CTexture Temp;
	if( FAILED( Temp.LoadPic( m_pD3dDev, szFileName ) ) )
		return;
	
	if( Temp.m_Head.uWidth != m_Head.uWidth || Temp.m_Head.uHeight != m_Head.uHeight )
		return;

	LPDIRECT3DTEXTURE9*	pTextures = new LPDIRECT3DTEXTURE9[ m_Head.uFrameCount + Temp.m_Head.uFrameCount ];
	unsigned i = 0;
	for( i = 0; i < m_Head.uFrameCount; i++ )
		pTextures[i] = m_pTextures[i];
	for( ; i < m_Head.uFrameCount + Temp.m_Head.uFrameCount; i++ )
	{
		pTextures[i] = Temp.m_pTextures[ i - m_Head.uFrameCount ];
		pTextures[i]->AddRef();
	}
	m_Head.uFrameCount++;
	delete []m_pTextures;
	m_pTextures = pTextures;
}
////////////////////////////////////////////////////////////////////////
// ���ܣ�����ͼƬ
// ������LPCTSTR TextName �ļ���
// ���أ�>=0 �ɹ���<0 ʧ��
////////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct TGAINFO
{
	BYTE btImagInfoBlockSize;		//ͼ����Ϣ�ֶΣ������ӱ�ĺ��棩���ַ���
	BYTE btTableType;				//��ɫ�������
	BYTE btImageType;				//ͼ�������� 
	WORD wTableAddress;				//��ɫ����ַ      
	WORD wTableLenght;				//��ɫ��ĳ���       
	BYTE btBitCountInTable;			//��ɫ������λ��bit���� 
	WORD wStartX;					//ͼ�� x ������ʼλ��        
	WORD xStartY;					//ͼ�� y ������ʼλ��    
	WORD wWidth;					//ͼ����	 
	WORD wHeight;					//ͼ��߶�	    
	BYTE btBitCount;				//ͼ��ÿ���ش洢ռ��λ��bit���� 
	BYTE btImagScript;				//ͼ���������ֽ�
};   
#pragma pack(4)    

int CTexture::SaveAsTga( LPCTSTR TextName )
{
	TGAINFO TtagInfo = { 0, 0, 2, 0, 0, 32, 0, 0, m_Head.uWidth, m_Head.uHeight, 32, 32 };
	SetFileAttributes( TextName, FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE );

	for( unsigned i = 0; i < m_Head.uFrameCount; i++ )
	{
		CString SaveName = TextName;
		for( int n = 0; n < SaveName.GetLength(); n++ )
		{
			if( SaveName[n] == '.' )
			{
				SaveName.Delete( n, SaveName.GetLength() );
				break;
			}
		}

		if( m_Head.uFrameCount > 1 )
		{
			TCHAR Buf[256];
			sprintf( Buf, "_%d", i );
			SaveName += Buf;
		}

		SaveName += ".tga";

		locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·���������ģ�����ȫ��localeΪ���ػ���
		ofstream File( SaveName, ios::binary|ios::out );
		locale::global(loc);

		DWORD Res = MAKEFOURCC( 'T','E','X', 0 ); 
		DWORD Version = TEXTURE_VERSION;
		File.write( (TCHAR*)&TtagInfo, sizeof(TGAINFO) );

		D3DLOCKED_RECT rect;	
		m_pTextures[i]->LockRect( 0, &rect, NULL, 0 );
		File.write( (TCHAR*)rect.pBits, m_Head.uWidth*m_Head.uHeight*4 );
		m_pTextures[i]->UnlockRect(0);
	}

	return 0;
} 

////////////////////////////////////////////////////////////////////////
// ���ܣ�����ͼƬ
// ������CD3d* D3d ��Ⱦ�豸, 
//       LPCTSTR TextName �ļ���
// ���أ�>=0 �ɹ���<0 ʧ��
////////////////////////////////////////////////////////////////////////
int CTexture::SaveTEX( LPDIRDEV pD3dDev, LPCTSTR TextName, sqr::eTextureFormat eFormat, UINT nMipMap, int nFrameTime )
{
	if ( TRUE == m_bSizeChanged )
	{
		if ( m_bUseOriginSize)
		{
			m_Head.uWidth	= m_OldHead.uWidth;
			m_Head.uHeight	= m_OldHead.uHeight;
		}
	}

	// ȡ�ô洢����
	CString		strFileName		= TextName;
	if ( strFileName.Find(".tex", max(strFileName.GetLength()-4, 0) ) >= 0 )
	{
		m_SaveType	= eSTY_TEX;
	}
	else if ( strFileName.Find(".dds", max(strFileName.GetLength()-4, 0) ) >= 0 )
	{
		m_SaveType	= eSTY_DDS;
	}
	else
	{
		return GERROR;
	}
	
	SetFileAttributes( TextName, FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE );
	if( sqr::TF_UNKNOW == eFormat )
		eFormat = GetFormat();

	sqr::sTextureInfo SaveHead = m_Head;	
	SaveHead.eFormat = eFormat;
	SaveHead.uMipMap = nMipMap;
	SaveHead.uFrameCircle = (WORD)min( nFrameTime, 65535 );

	if( ( eFormat == sqr::TF_DXT1 || eFormat==sqr::TF_DXT5 ) && ( m_Head.uWidth < 4 || m_Head.uHeight < 4 ) )
		return GERROR;

	if( SaveHead.uMipMap == 0 )
	{
		for( unsigned uWidth = SaveHead.uWidth, uHeight = SaveHead.uHeight; 
			uWidth >= 4 && uHeight >= 4; uWidth = uWidth >> 1, uHeight = uHeight >> 1 )
			SaveHead.uMipMap++;
	}

	LPDIRECT3DTEXTURE9  pCompress = NULL;	

	//����ѹ����ͼ
	if( FAILED ( D3DXCreateTexture( pD3dDev, SaveHead.uWidth,SaveHead.uHeight, SaveHead.uMipMap, 0,
		g_eDataFormat[SaveHead.eFormat], D3DPOOL_MANAGED, &pCompress ) ) )
		return GERROR;

	if ( eSTY_TEX == m_SaveType )
	{
		// ---- .tex �ļ����ʹ洢

		locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·���������ģ�����ȫ��localeΪ���ػ���
		ofstream File( TextName, ios::binary|ios::out );
		locale::global(loc);

		DWORD Res = MAKEFOURCC( 'T','E','X', 0 ); 
		DWORD Version = TEXTURE_VERSION;
		File.write( (TCHAR*)&Res, sizeof(DWORD) );
		File.write( (TCHAR*)&Version, sizeof(DWORD) );
		File.write( (TCHAR*)&SaveHead,sizeof(SaveHead));

		for( unsigned i = 0; i < m_Head.uFrameCount; i++ )
		{
			D3DLOCKED_RECT rect;
			m_pTextures[i]->LockRect(0,&rect,NULL,0);
			TCHAR *buf=(TCHAR*)rect.pBits;

			LPSURFACE pSur;
			pCompress->GetSurfaceLevel(0,&pSur);
			RECT rect1={0,0,SaveHead.uWidth,SaveHead.uHeight};

			//ѹ��ͼƬ�����ALPHA BUG
			if( FAILED( D3DXLoadSurfaceFromMemory( pSur, NULL, NULL, buf, D3DFMT_A8R8G8B8,
				rect.Pitch,NULL,&rect1,D3DX_FILTER_TRIANGLE,0 ) ) )
			{
				SafeRelease(pSur);
				SafeRelease(pCompress);
				File.close();
				return GERROR;
			}
			SafeRelease(pSur);

			pCompress->LockRect(0,&rect,NULL,0);
			CheckCompress( (BYTE*)rect.pBits, g_eDataFormat[SaveHead.eFormat], (BYTE*)buf, D3DFMT_A8R8G8B8, SaveHead.uWidth, SaveHead.uHeight );
			pCompress->UnlockRect(0);
			m_pTextures[i]->UnlockRect(0);

			// ����mipmap
			if( FAILED( D3DXFilterTexture( pCompress, NULL, 0, D3DX_FILTER_TRIANGLE ) ) )
			{
				File.close();
				pCompress->Release();
				return GERROR;
			}

			for( unsigned uWidth = SaveHead.uWidth, uHeight = SaveHead.uHeight, i = 0; 
				i < SaveHead.uMipMap; uWidth = uWidth >> 1, uHeight = uHeight >> 1, i++ )
			{
				unsigned uSize = uWidth*g_nSpace[SaveHead.eFormat];
				pCompress->LockRect( i, &rect, NULL, 0 );
				if( SaveHead.eFormat == sqr::TF_RGB24 )
				{
					unsigned* pBuf = (unsigned*)rect.pBits;
					for( unsigned n = 0; n < uHeight; n++, pBuf += rect.Pitch/4 )
						for( unsigned p = 0; p < uWidth; p++ )
							File.write( (TCHAR*)&pBuf[p], 3 );
				}
				else
				{
					TCHAR* pBuf = (TCHAR*)rect.pBits;
					for( unsigned n = 0; n < uHeight; n += g_nLinePerPitch[SaveHead.eFormat], pBuf += rect.Pitch )
						File.write( pBuf, uSize );
				}
				pCompress->UnlockRect( i );
			}
		}

		File.close();
		pCompress->Release();
	}
	else if ( eSTY_DDS == m_SaveType )
	{
		// ---- .dds �ļ����ʹ洢

		D3DLOCKED_RECT rect;
		m_pTextures[0]->LockRect(0,&rect,NULL,0);
		TCHAR *buf=(TCHAR*)rect.pBits;

		LPSURFACE pSur;
		pCompress->GetSurfaceLevel(0,&pSur);
		RECT rect1={0,0,SaveHead.uWidth,SaveHead.uHeight};

		// copy����pCompress���Ըı������ʽ����ѹ��������м��

		if( FAILED( D3DXLoadSurfaceFromMemory( pSur, NULL, NULL, buf, D3DFMT_A8R8G8B8,
			rect.Pitch,NULL,&rect1,D3DX_FILTER_TRIANGLE,0 ) ) )
		{
			pCompress->UnlockRect(0);
			SafeRelease(pSur);
			SafeRelease(pCompress);
			return GERROR;
		}
		SafeRelease(pSur);
		pCompress->LockRect(0,&rect,NULL,0);
		CheckCompress( (BYTE*)rect.pBits, g_eDataFormat[SaveHead.eFormat], (BYTE*)buf, D3DFMT_A8R8G8B8, SaveHead.uWidth, SaveHead.uHeight );
		pCompress->UnlockRect(0);
		m_pTextures[0]->UnlockRect(0);

		// ����mipmap
		if( FAILED( D3DXFilterTexture( pCompress, NULL, 0, D3DX_FILTER_TRIANGLE ) ) )
		{
			pCompress->Release();
			return GERROR;
		}

		// �洢��DDS�ļ���ʽ

		if ( FAILED(D3DXSaveTextureToFile( TextName, D3DXIFF_DDS, pCompress, NULL )) )
		{
			return GERROR;
		}
		
	}

	return 0;
}

LPDIRTEX CTexture::GetD3DTexture()		const
{ 
	if(!IsValid())
		return NULL;
	if( m_Head.uFrameCount > 1 )
		return m_pTextures[ ( GetTickCount()/max( m_Head.uFrameCircle, 1 ) ) %m_Head.uFrameCount ];
	else
		return m_pTextures ? m_pTextures[0] : NULL; 
}                                  //��Ⱦʱ�趨��ͼ

sqr::eTextureFormat CTexture::TransToLocalFormat(  D3DFORMAT fmt )
{
	switch ( fmt )
	{
	case D3DFMT_DXT1:
		return TF_DXT1;
	case D3DFMT_DXT5:
		return TF_DXT5;
	case D3DFMT_X8R8G8B8:
		return TF_RGB24;
	case D3DFMT_A8R8G8B8:
		return TF_ARGB32;
	case D3DFMT_R5G6B5:
		return TF_R5G6B5;
	case D3DFMT_A4R4G4B4:
		return TF_A4R4G4B4;
	case D3DFMT_A8:
		return TF_A8;
	default:
		return TF_UNKNOW;
	}
}
