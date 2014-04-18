#pragma once

#include "BluelyG.h"

#define TEXTURE_VERSION 10003

class CTexture 
{
	TEXTINFO	m_Head;                 //�ļ�ͷ
	string		m_Filename;
	LPDIRTEX	m_Texture;				//��ͼ

	LPDIRDEV	m_pD3dDev;

	int  LoadCOM( LPDIRDEV pD3dDev, LPCTSTR TextName );                         //�����׼�ļ���bmp��tga��jpg
	int  LoadTEX( LPDIRDEV pD3dDev, LPCTSTR TextName );                         //����tex���ݶ����ļ�

	int  SaveCompress( LPDIRDEV pD3dDev, LPCTSTR TextName, TEXTINFO& SaveHead );//����Ϊѹ����ʽ
	int  SaveHighColor( LPCTSTR TextName, TEXTINFO& SaveHead );          //����Ϊ����ɫ��ʽ

public:
	CTexture(void);
	~CTexture(void);

	BOOL            IsValid()		const	{ return m_Texture != NULL; }
	UINT            GetWidth()		const	{ return m_Head.dwWidth;  }
	UINT            GetHeight()		const	{ return m_Head.dwHeight; }
	TEXTUREFORMAT   GetFormat()		const	{ return m_Head.Format;   }
	const string&	GetFileName()	const	{ return m_Filename; }
	LPDIRTEX		GetD3DTexture() const	{ return m_Texture; }                                  //��Ⱦʱ�趨��ͼ

	int  LoadPic( LPDIRDEV pD3dDev, LPCTSTR TextName );                                    //�����ļ�
	int  SaveTEX( LPDIRDEV pD3dDev, LPCTSTR TextName, TEXTUREFORMAT Format = TF_UNKNOW );  //�����ļ�
	int  Free();                                                                    //�ͷ���Դ
	int  AlphaImport( CTexture& AlphaCannel );										//��ָ��ͼƬ��ΪALPHAͨ��
};
