#pragma once
#include "../IO/VersionMapChunk.h"

namespace bly
{
class COperator;
}

namespace Map
{

/// ������Ϣ.
class MapSceneInfo : public IO::DataChunk
{
	DeclareChunk(MapSceneInfo);
public:
	MapSceneInfo();
	virtual ~MapSceneInfo();

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ����
	void LoadFromOperator(COperator* o);
public:
	struct SceneInfo
	{
		uint		ambientColor;			///< ��������ɫ
		uint		playerColor;			///< �����������ɫ
		uint		sunColor;				///< ̫������ɫ
		D3DXVECTOR3	sunDir;					///< ̫���ⷽ��
		uint		playerSunColor;			///< ����̫����
		uint		playerAmbientColor;		///< ���ǻ�������ɫ
		uint		playerLightColor;		///< ���ǵ���ɫ
		float		attenuation;			///< ���ǵ�ǿ��
		float		rangeOfLight;			///< ���ǵƷ�Χ
		uint		fogColor;				///< �����ɫ
		float		fogStart;				///< ��Ľ���
		float		fogEnd;					///< ���Զ��
		char		skyTexture[64];			///< �����ͼ(����һ����д, ��Ū���̶���С-_-)
	};
	enum
	{
		DAYTIME_NUM = 4
	};
private:
	SceneInfo sceneInfo[DAYTIME_NUM];
};

}// namespace Map
