#pragma once

namespace sqr
{
	// ���ݿ�ı�ʶ�Ķ���
	const uint32	SRGN = 'NGRS';///< Sub Region
	const uint32	RGFG = 'GFGR';///< Flag
	const uint32	BARR = 'RRAB';
	const uint32	GATR = 'RTAG';///< Block
	const uint32	EBLK = 'KLBE';///< Exact Block
	const uint32	TRAP = 'PART';///< Trap
	const uint32	OBJT = 'TJBO';
	const uint32	VHGT = 'TGHV';///< Terrain Height
	const uint32	VDIF = 'FIDV';///< Vertex Diffuse
	const uint32	VSPE = 'EPSV';///< Vertex Specular
	const uint32	VNOR = 'RONV';
	const uint32	VTEX = 'TXET';///< Texture
	const uint32	MODL = 'LDOM';///< Model
	const uint32	LHGT = 'TGHL';///< Logical Height
	const uint32	SBVT = 'TVBS';
	const uint32	GHGT = 'TGHG';
	const uint32	WATR = 'RTAW';///< Water
	const uint32	SCLT = 'TLCS';///< Scene Point Light
	const uint32	SPLT = 'TLPS';///< Scene Spot Light 
	const uint32	AUID = 'DIUA';///< Scene Audio AUID
	const uint32	LOGI = 'IGOL';///< login model
	const uint32	TGMI = 'IMGT';///< terrain grid material index

	enum ESceneUnits
	{
		eSRegionSpan			= 64,											// ������(�Ը���Ϊ��λ)
		eSGridSpan				= 64,											// ���ӿ��(��С��Ϸ��λ)��ͼ������ר��ö��
	};

	// �����е����ݿ��ͷ
	struct RegionChunkHdr
	{
		uint32			nMagicID;		// ��ʶ���ݿ������
		uint32			nOffset;		// ���ݿ����ʼλ��������ļ�ͷ��ƫ��
		uint32			nSize;			// ���ݿ�Ĵ�С
	};


	template<class T> struct TPos;
	typedef TPos<int32> CPos;
	typedef TPos<float> CFPos;

	enum EUnits
	{
		eRegionSpan			= eSRegionSpan / 4,							// ������(�Ը���Ϊ��λ)
		eGridSpan			= eSGridSpan,								// ���ӿ��(��С��Ϸ��λ)��ͼ������ר��ö��
		eGridSpanForObj		= eGridSpan,								// ���ӿ�ȣ�����ϵͳ������߼�ר��
		eGridNumPerRegion	= eRegionSpan * eRegionSpan,				// ���������
		eVertexNumPerRegion = ( eRegionSpan + 1 )*( eRegionSpan + 1 ),	// ���򶥵���
		eRegDivideNum		= 2,										//һ��������ݱ����ֳɶ��ٸ�������
		eSubRegNum			= eRegDivideNum * eRegDivideNum,			//һ����������������
		eSubRegionSpan		= eRegionSpan / eRegDivideNum,				//��������(�Ը���Ϊ��λ)
	};

	//������������õ���������
	inline int32 GetGridByPixel( float fPixelCrd )
	{
		return static_cast<int32>(fPixelCrd) >> 6;// return LogicCrd*128 + Offset;
	}

}
