/*
 *	����engine�е�ÿ���ĳ���
 */
#pragma once

class CRegion;
class IGameObject;
class CObjectCreateInfo;

struct CWeatherInfo
{
	uint32		nAmbientColor;			// ���������ɫ
	uint32		nWeatherParam;			// ������������������ļ��ʣ���ѩ�ļ���
	uint32		nWindParam;				// ����,��Ӱ�����ʵ�ֶ���
};

struct CBkMusicInfo
{
	uint8		nCount;		// �������ֵĸ���
	uint8		nFlag;		// ���ŵı�־��ѭ���������
	char**		szName;		// nCount ���������ֵ����֣�ÿ��64���ֽ�
};

struct CCameraSetting
{
	float	fFov;		// �ӽ�
	float	fBack;		// ���˶���, ��������ǵ�����
	float	fUp;		// ̧�߶���, ��������ǵ�����
};

struct CSceneFileHeader
{
	int32				nMagicID;		// "AMAP"
	int32				nVersion;		// �汾
	char				szDesc[64];		// ��ͼ������
	uint16				nWidth;			// ��ͼ�Ŀ�ȣ�������Ϊ��λ
	uint16				nHeight;		// ��ͼ�ĸ߶ȣ�������Ϊ��λ
	uint16				dwRegionSpace;
	uint16				dwGridSpace;
	char				szTileSet[64];	// ��ͼ��ص�ͼ�ص�����
	CWeatherInfo		WeatherInfo;	// ������ص���Ϣ	
	CBkMusicInfo		BkMusicInfo;	// �������ֵ���Ϣ
	CCameraSetting		CameraInfo;		// ���������Ϣ
};

//////////////////////////////////////////////////////////////////////////

struct RegionChunkHdr
{
	uint32			nMagicID;		// ��ʶ���ݿ������
	uint32			nOffset;		// ���ݿ����ʼλ��������ļ�ͷ��ƫ��
	uint32			nSize;			// ���ݿ�Ĵ�С
};

struct RegionHeader
{
	uint32				dwMagicID      // Region���ͱ�ʶ
	int					nVersion		// Region version
	uint32				nCount;			// ���ݿ�ĸ���
	RegionChunkHdr		ChunkHdr[1];	// nCount��ChunkHdr
};

enum enumRegionChunkType
{
	Barrier		= 0,	// �ϰ���Ϣ�����ո��Ӵ洢
	Trap,				// ��������Ϣ�����ո��Ӵ洢
	Object,				// �����б����������еĶ����ڲ���������ٷ�ΪС�Ŀ�
	Vertex,				// �ر����Ϣ��������Ϣ��������Ϣ��
	Grid,				// ��ͼ��Ϣ,������Ϣ,

	ChunkTypeCount
};

struct BarrierInfo
{
	uint8	nFlag;		// �ϰ�������
	float	fHeight;	// �߶�
};

struct TrapInfo
{
	uint32	nTrapID;	//	�ű��ļ������ֱ�ʶ���ɽű��ļ�������õ���
};


*	�������ݿ�д���˳��
����ĸ���
����ÿ������
{
	������ص��������
		���Եĸ���
		����ÿ�����ԣ�
	{
		���Ե�����
			���Ե�����
			���ԵĴ�С
			���Ե�ֵ
	}
}

