#pragma once
#include "CWaterEffect.h"
#include "GraphicBase.h"
#include "CVector4.h"
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
	void OnRenderEnd();

private:
	ITexture*	m_pEnvironmentMap;
	ITexture*	m_pWaveNormalMap[2];
	CVector2f	m_WaveSpeed[2];
	CVector4f	m_WaveOffset;
};

}// namespace sqr
