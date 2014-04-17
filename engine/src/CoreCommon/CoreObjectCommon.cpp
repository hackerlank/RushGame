#include "stdafx.h"
#include "IPathFinding.h"
#include "CPos.h"
#include "CoreObjectCommon.h"
#include "CoreCommon.h"


namespace sqr
{
	//******************************************************
	//���ض����step����
	//******************************************************

	//�ͻ��˶���ÿ���ٴ�step֪ͨһ��Զ�̶������step
	const uint32 g_uClientTellStepInterval = 6;

	//�ͻ��˺ͷ��������Ҳ�Aoi�ľ���(��λ����)
	const uint32 g_uPlayerAoiPrecision = 1;

	//Dir����֪ͨMed����step�Ĳ������
	const float g_fDirStepMaxSentMovedDist = (float)g_uPlayerAoiPrecision*eGridSpan / 2;
	//Dir����֪ͨMed����step�����ʱ����(����)
	const uint32 g_uDirTellMedStepMaxInterval = 1000;

	//�����ٶȺ��ƶ����ڣ������ƶ�һ���߶��پ���
	float CalMoveStep(float fSpeed,uint32 uMilliseconds)
	{
		float fStep = fSpeed * eGridSpanForObj;
		fStep = fStep * uMilliseconds;
		fStep = fStep / 1000;
		return fStep;
	}
	float CalMoveStepInMicroSeconds(float fSpeed,uint32 uMicroseconds)
	{
		float fStep = fSpeed * eGridSpanForObj;
		fStep = fStep * uMicroseconds;
		fStep = fStep / 1000000;
		return fStep;
	}

	void CalcBarrierArea(vector<CPos>& vecBarrierArea, const CFPos& pixelPosCenter, uint8 uRadius, uint32 uSceneWidth, uint32 uSceneHeight)
	{
		vecBarrierArea.clear();
		if(uRadius > 0)
		{
			uint32 uDist;
			uint32 uMaxX;
			uint32 uMinX;
			uint32 uMaxY;
			uint32 uMinY;
			CPos gridPosCenter;
			if(uRadius % 2 == 0)
			{
				gridPosCenter.x = int32(pixelPosCenter.x / eGridSpanForObj + 0.5f);
				gridPosCenter.y = int32(pixelPosCenter.y / eGridSpanForObj + 0.5f);
				uDist = uRadius / 2;
				uMaxX = gridPosCenter.x + uDist - 1;
				uMinX = uint32(gridPosCenter.x) > uDist ? gridPosCenter.x - uDist : 0;
				uMaxY = gridPosCenter.y + uDist - 1;
				uMinY = uint32(gridPosCenter.y) > uDist ? gridPosCenter.y - uDist : 0;
			}
			else
			{
				gridPosCenter.x = int32(pixelPosCenter.x / eGridSpanForObj);
				gridPosCenter.y = int32(pixelPosCenter.y / eGridSpanForObj);
				uDist = (uRadius - 1) / 2;
				uMaxX = gridPosCenter.x + uDist;
				uMinX = uint32(gridPosCenter.x) > uDist ? gridPosCenter.x - uDist : 0;
				uMaxY = gridPosCenter.y + uDist;
				uMinY = uint32(gridPosCenter.y) > uDist ? gridPosCenter.y - uDist : 0;
			}

			uMaxX = min(uMaxX, uSceneWidth);
			uMaxY = min(uMaxY, uSceneHeight);
			for(uint32 x = uMinX; x <= uMaxX; ++x)
			{
				for(uint32 y = uMinY; y <= uMaxY; ++y)
				{
					vecBarrierArea.push_back(CPos(x, y));
				}
			}
		}
	}
}
