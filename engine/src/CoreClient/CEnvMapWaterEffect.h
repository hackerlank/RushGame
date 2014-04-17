#pragma once
#include "CWaterEffect.h"
#include "GraphicBase.h"
/**
	@class CEnvMapWaterEffect

	����ӳ��ˮЧ��.
	������Ϊ�Ͷ˻���������ˮ���������.
	�ᷴ�������廷��ӳ����ͼ.
 */
namespace sqr
{

class CEnvMapWaterEffect : public CWaterEffect
{
public:
	CEnvMapWaterEffect();
	virtual ~CEnvMapWaterEffect();

	/// ����
	void OnFrameUpdate();
	/// ��Ⱦ��ʼ
	void OnRenderBegin();

private:
	ITexture*	m_pEnvironmentMap;
	ITexture*	m_pWaveNormalMap[2];
	CVector2f	m_WaveSpeed[2];
	CVector2f	m_WaveOffset[2];
};

}// namespace sqr
