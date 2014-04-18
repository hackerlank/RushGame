#include "stdafx.h"
#include "EMapTexCoder.h"
#include "Render.h"

BEGIN_VERSION_MAP(EMapTexCoder)
	ON_VERSION(1,(CCoder::PFN_DECODE)&EMapTexCoder::DeCode_Ver_1)
END_VERSION_MAP()

const DWORD EMapTexCoder::New_Version = 1;
EMapTexCoder::EMapTexCoder(CTerrainMesh* pTerrainMesh):m_pTerrain(pTerrainMesh){}
EMapTexCoder::~EMapTexCoder(){}

bool EMapTexCoder::Code()
{
	CDataChunk outData;
	outData.setName(getChunkName());
	
	//ƴ�ӵ�һЩ����
	PALETTEENTRY pale;
	pale.peBlue = 8;
	pale.peFlags = 8;
	pale.peGreen = 8;
	pale.peRed = 8;
	const UINT MAX_TEXTURE = 2048;
	const UINT TEXTURE_SIZE = 128;
	const UINT EXPAND_SIZE = 8;
	const UINT ROW_SIZE = MAX_TEXTURE/TEXTURE_SIZE;
	const UINT TEXTURE_CNT = ROW_SIZE*ROW_SIZE;
	const UINT PixelSize = 4;
	const UINT EXPAND_SIZE_TWO = EXPAND_SIZE*2;
	const UINT EXPAND_TEXTURE_SIZE = TEXTURE_SIZE + EXPAND_SIZE_TWO;
	const UINT EXPAND_MAX_SIZE = MAX_TEXTURE + ROW_SIZE*EXPAND_SIZE_TWO;
	CRenderDevice * pRender = CRenderDevice::GetInstance();
	typedef std::map<std::string, BYTE> TEXT_MAP;

	//ע������汾���ֻ��ƴ��256���������˻������ʱû��д�ݴ����

	//�����������	
	TEXT_MAP mapTerrainTextures; //���ֻ������ͨ��ͼ��������ͼҪ���
	TEXT_MAP::iterator iter;
	for ( int k = 0, l = 0; k < m_pTerrain->GetGridCount(); k++ )
	{
		SGrid & grid = m_pTerrain->GetGrid(k);
		//������Ҫע��bug
		for(int j = 0;j<4;++j)
		{
			if ( grid.nodeInfo.strTextureNames[j] != "" )
			{
				iter = mapTerrainTextures.find(grid.nodeInfo.strTextureNames[j].c_str());

				if ( iter == mapTerrainTextures.end() )
				{
					mapTerrainTextures[grid.nodeInfo.strTextureNames[j].c_str()] = l;
					grid.nodeInfo.dwTexIndex[j] = l;
					++l;
				}
				else
					grid.nodeInfo.dwTexIndex[j] = iter->second;
			}
			else
				grid.nodeInfo.dwTexIndex[j] = 0xff;
		}
	}

	//ƴ����ͼ	

	TEXT_MAP::iterator it = mapTerrainTextures.begin();
	TEXT_MAP::iterator ite = mapTerrainTextures.end();
	LPDIRECT3DTEXTURE9 pTempTexture;
	LPDIRECT3DTEXTURE9 pMainTexture;
	ITexture* pTexture;


	//��ʽ��ʼƴ��
	if(!SUCCEEDED(pRender->GetRawDevice()->CreateTexture( 
		EXPAND_MAX_SIZE,EXPAND_MAX_SIZE, 
		1, 0, D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED,&pMainTexture, NULL )))
	{
		string log = "ƴ�ӹ����д���������ʧ��";
		MessageBox(NULL,log.c_str(),"����",MB_OK);
	}

	for(;it!=ite;++it)
	{
		pTexture = pRender->GetTexture(it->first);
		if(NULL==pTexture)
		{
			string log = "ƴ�ӹ���:"+it->first+"�����޷��ҵ�";
			MessageBox(NULL,log.c_str(),"����",MB_OK);
			continue;
		}
		pTempTexture = (LPDIRECT3DTEXTURE9)pTexture->GetBuffer();	
		if(NULL==pTempTexture)
		{
			string log = "ƴ�ӹ���:"+it->first+"�����޷��ҵ�";
			MessageBox(NULL,log.c_str(),"����",MB_OK);
			continue;
		}
		UINT HotIndex = it->second;
		UINT H = (HotIndex/ROW_SIZE)*EXPAND_TEXTURE_SIZE;
		UINT W = (HotIndex%ROW_SIZE)*EXPAND_TEXTURE_SIZE;
		D3DSURFACE_DESC desc;
		pTempTexture->GetLevelDesc(0,&desc);
		if(desc.Width!=TEXTURE_SIZE||desc.Height!=TEXTURE_SIZE)
		{
			string log = "ƴ�ӹ���:"+it->first+"�ߴ粻�Ϸ�,������ͼ����ӦΪ128X128";
			MessageBox(NULL,log.c_str(),"����",MB_OK);
		}

		RECT rectT,rectO;
		D3DLOCKED_RECT lockT,lockO;
		rectO.top = rectO.left = 0;
		rectO.bottom = rectO.right = TEXTURE_SIZE;
		rectT.top = H; rectT.bottom = H + EXPAND_TEXTURE_SIZE;
		rectT.left = W; rectT.right = W + EXPAND_TEXTURE_SIZE;
		pMainTexture->LockRect(0,&lockT,&rectT,D3DLOCK_DISCARD);
		pTempTexture->LockRect(0,&lockO,&rectO,D3DLOCK_READONLY);
		char* pTar = (char*)lockT.pBits;
		char* pOrc = (char*)lockO.pBits;

		for(UINT i=0;i<EXPAND_SIZE;++i,pTar+=lockT.Pitch)
		{
			for( UINT j=0;j<EXPAND_SIZE;++j)
			{
				memcpy(pTar+PixelSize*j,pOrc,PixelSize);
				memcpy(pTar+lockO.Pitch+PixelSize*(j+EXPAND_SIZE),pOrc+lockO.Pitch-PixelSize,PixelSize);
			}					
			memcpy(pTar+PixelSize*EXPAND_SIZE,pOrc,lockO.Pitch);					
		}

		for(UINT i = 0;i<TEXTURE_SIZE;++i,pTar+=lockT.Pitch,pOrc+=lockO.Pitch)
		{

			for( UINT j=0;j<EXPAND_SIZE;++j)
			{
				memcpy(pTar+PixelSize*j,pOrc,PixelSize);
				memcpy(pTar+lockO.Pitch+PixelSize*(j+EXPAND_SIZE),pOrc+lockO.Pitch-PixelSize,PixelSize);
			}		
			memcpy(pTar+PixelSize*EXPAND_SIZE,pOrc,lockO.Pitch);	
		}
		pOrc-=lockO.Pitch;
		for(UINT i=0;i<EXPAND_SIZE;++i,pTar+=lockT.Pitch)
		{
			for( UINT j=0;j<EXPAND_SIZE;++j)
			{
				memcpy(pTar+PixelSize*j,pOrc,PixelSize);
				memcpy(pTar+lockO.Pitch+PixelSize*(j+EXPAND_SIZE),pOrc+lockO.Pitch-PixelSize,PixelSize);
			}	
			memcpy(pTar+PixelSize*EXPAND_SIZE,pOrc,lockO.Pitch);
		}

		pTempTexture->UnlockRect(0);
		pMainTexture->UnlockRect(0);					
	}

	//������Ҫ����ѹ���㷨
	LPD3DXBUFFER lpBuf;
	D3DXSaveTextureToFileInMemory(&lpBuf,D3DXIFF_BMP,pMainTexture,&pale);	
	outData.allocate(lpBuf->GetBufferSize(),(const char*)lpBuf->GetBufferPointer());
	lpBuf->Release();
	m_pMgr->Write(outData);	
	outData.free();	
	return true;
}

bool EMapTexCoder::DeCode_Ver_1(const CDataChunk& inData)
{
	//��ģ�鲻�ܴ��ļ����������
	return false;
}