#pragma once
#include "GraphicBase.h"
#include "../../src/Graphic/Graphic_dx9/Dx9Base.h"

class CTexture 
{
public:
	enum	ESaveType { eSTY_TEX = 1, eSTY_DDS, eSTY_COUNT };

public:
	static sqr::eTextureFormat	  TransToLocalFormat( D3DFORMAT fmt );

public:
	CTexture(void);
	~CTexture(void);

	BOOL            IsValid()			const	{ return m_pTextures != NULL; }
	UINT            GetWidth()			const	{ return m_Head.uWidth;  }
	UINT			GetOriginWidth()	const	{ return m_OldHead.uWidth; };
	UINT            GetHeight()			const	{ return m_Head.uHeight; }
	UINT			GetOriginHeight()	const	{ return m_OldHead.uHeight; }
	UINT			GetMipMapCount()	const	{ return m_Head.uMipMap;  }
	UINT			GetFrameCount()		const	{ return m_Head.uFrameCount; }
	UINT			GetOriginMipMapCount() const { return m_OldHead.uMipMap; }
	const string&	GetFileName()		const	{ return m_Filename; }
	int				GetFrameTime()		const	{ return m_Head.uFrameCircle; }
	LPDIRTEX		GetD3DTexture()		const;												//��Ⱦʱ�趨��ͼ
    sqr::eTextureFormat   GetFormat()	const	{ return m_Head.eFormat;   }
	sqr::eTextureFormat   GetOriginFormat()	const	{ return m_OldHead.eFormat;   }
	void			AddExFrame( LPCTSTR szFileName );

	int  LoadPic( LPDIRDEV pD3dDev, LPCTSTR TextName );                                     //�����ļ�
	int  SaveTEX( LPDIRDEV pD3dDev, LPCTSTR TextName, sqr::eTextureFormat Format, UINT nMipMap, int nFrameTime ); //�����ļ�
	int  SaveAsTga( LPCTSTR TextName );
	int  Free();						//�ͷ���Դ										
	void SetUseOriginSize( BOOL bUseOriginSize ) { m_bUseOriginSize = bUseOriginSize; }

private:
	sqr::sTextureInfo	m_Head;						// �ļ�ͷ
	sqr::sTextureInfo	m_OldHead;					// ԭʼ�ļ���Ϣ
	string				m_Filename;
	LPDIRECT3DTEXTURE9*	m_pTextures;				// ��ͼ
	LPDIRDEV			m_pD3dDev;

	ESaveType			m_SaveType;
	BOOL				m_bSizeChanged;				// ����ߴ��Ƿ񱻸ı�
	BOOL				m_bUseOriginSize;			// �Ƿ�ʹ��ԭʼ����ߴ�

	int  LoadCOM( LPDIRDEV pD3dDev, LPCTSTR TextName );	//�����׼�ļ���bmp��tga��jpg
	int  LoadTEX( LPDIRDEV pD3dDev, LPCTSTR TextName );	//����tex���ݶ����ļ�
};