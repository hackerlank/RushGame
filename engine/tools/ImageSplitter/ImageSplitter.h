#pragma once
#include "BaseDefs.h"

	class IIndexGenerator;
	
	class CImageSplitter
	{
		static const uint32		DELTA_X		= 256;
		static const uint32		DELTA_Y		= 256;
		static const D3DFORMAT	SAVE_FORMAT = D3DFMT_DXT1;

		uint32			m_uWidth;
		uint32			m_uHeight;
		string			m_szTgtPath;
		string			m_szSrcPath;
		string			m_szSrcFile;
		IIndexGenerator	*m_pIndexGenerator;

		LPDIRECT3DTEXTURE9	m_pTexOld;

		CImageSplitter(const CImageSplitter&);
		CImageSplitter& operator = (const CImageSplitter&);

		// ---- ��ȡ����ԭͼƬ���ŵ�256��n�η��ߴ�
		bool	_ReadOldPicture();

		// ---- ����Ŀ��ͼƬ
		bool	_GenerateTargetPicture( void *pSrcBuf, uint32 uPitch, 
										RECT &rectSrc, const string &szTarget);

		// ---- ִ���з�
		bool	_DoCut();

	public:
		CImageSplitter(IIndexGenerator *pIndexGenerator);
		~CImageSplitter();

		// ---- ����ԭͼƬ·��
		void	SetSourceFile(const string &szSrcFile);

		// ---- �����зֺ��ͼƬ�Ĵ洢Ŀ¼
		void	SetTargetPath(const string &szTargetPath);
		
		// ---- ����������Ϣ
		void	SetRgnInfo(uint32 uWidth, uint32 uHeight);

		// ---- ִ���з�
		bool	Run();
	};