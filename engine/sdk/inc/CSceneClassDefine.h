#pragma once
#include "CSceneBase.h"

namespace sqr
{
	struct CWaterWaveInfo : public CSceneMallocObject
	{
		struct WaterPhysic
		{
			int32	nWavePerGrid;			// ÿ�����ӵĲ��˸���
			int32	nWaveSize;				// ���˴�С
			int32	nWaveOff;				// ����ƫ��
			float	fWaveSpeed;				// �����ٶ�
			int32	nWaveLife;				// ������������
		};
		WaterPhysic	WavePhysicInfo;
		SString		szWaveTexture;			// ������ͼ
		SString		szMoveFxFile;			// �ƶ�������Ч�ļ�
		SString		szMoveFxName;			// �ƶ�������Ч��
		SString		szStandFxFile;			// �ƶ�������Ч�ļ�
		SString		szStandFxName;			// �ƶ�������Ч��
	};

	class CBufFile;
	class CTreeScene;
	class CElementCoder;
	class CElementManager;
	class CBaseScene;
	class CRegion;
}