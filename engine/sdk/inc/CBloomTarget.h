#pragma once
#include "GraphicBase.h"

namespace sqr
{
class CRenderTarget;
class CBloomTarget : public CGraphicMallocObject
{
public:
	CBloomTarget(CRenderTarget* pRenderTarget);
	~CBloomTarget();

public:
	bool Create(void);
	bool Release(void);
	bool IsUseful();
	bool IsTurnOn(const string& effect);
	void SetTurnOn(const string& effect, bool b);

	bool DoPostProcess(void);

	//������ʹ�������¼��������趨��Ӧ������������ȽϺ�
	//�������� �����Ŀ��ܲ��Ǻ�͸��
	void setBrightPass(float bright);			//��ͨ�� 0~1
	void setGaussMultiplier(float multiplier);	//����ǿ�� 0~2
	void setMean(float mean);					//����ƫ�� -1~+1
	void setStdDev( float sd );					//�����ںͳ̶�  0~1
	void setExposure( float Exp );				//��������0~2
	void setSample(float Sam );					//ģ��ͼ����
	//void setLuminance( D3DXVECTOR4& l );		//���ɫ��

	float getBrightPass(void);			//��ͨ�� 0~1
	float getGaussMultiplier(void);		//����ǿ�� 0~2
	float getMean(void);				//����ƫ�� -1~+1
	float getStdDev(void);				//�����ںͳ̶�  0~1
	float getExposure(void);			//��������0~2
	float getSample();					//ģ��ͼ����

	void openHDR(bool bHDR);
	void InitParams(void); //���¼��㷺����� �ڸ���ֵ�趨������ ��Ҫ���¼���...
	void UpdateParmas();
private:
	bool				m_IsInit;

	//-----�����м���--------------------------
	float		m_HBloomWeights[9];
	float		m_HBloomOffsets[9];
	float		m_VBloomWeights[9];
	float		m_VBloomOffsets[9];
	CRenderTarget*	m_pRenderTarget;
	//D3DXVECTOR4 m_vDsOffsets[16];

protected:
	//������ʹ�������¼��������趨��Ӧ������������ȽϺ�
	//�������� �����Ŀ��ܲ��Ǻ�͸��
	float		m_fBrightPass; //��ͨ�� 0~1
	float		m_fGaussMultiplier; //����ǿ�� 0~2
	float		m_fGaussMean; //����ƫ�� -1~+1
	float		m_fGaussStdDev; //�����ںͳ̶�  0~1
	float		m_fExposure;	//��������0~2
	float		m_fSample;			//��϶�

	float ComputeGaussianValue( float x, float mean, float std_deviation );
};

//------------------------------------------------------------------------------
inline void
CBloomTarget::UpdateParmas()
{
	m_IsInit = false;
}
//------------------------------------------------------------------------------
// Gassian��ʽ����^^
/*
-(x - mean)^2
-------------
1.0               2*std_dev^2
f(x,mean,std_dev) = -------------------- * e^
sqrt(2*pi*std_dev^2)

*/
inline float
CBloomTarget::ComputeGaussianValue( float x, float mean, float std_deviation )
{
	return ( 1.0f / sqrt( 2.0f * CMath::pi * std_deviation * std_deviation ) )
	       * expf( (-((x-mean)*(x-mean)))/(2.0f * std_deviation * std_deviation) );
}

//----һЩû�б�Ҫ���ĺ���--------------
inline void CBloomTarget::setBrightPass(float bright)
{
	m_fBrightPass = bright;
	m_IsInit = false;
}

inline void CBloomTarget::setGaussMultiplier(float multiplier)
{
	m_fGaussMultiplier = multiplier;
	m_IsInit = false;
}

inline void CBloomTarget::setMean(float mean)
{
	m_fGaussMean =mean;
	m_IsInit = false;
}

inline void CBloomTarget::setStdDev( float sd )
{
	m_fGaussStdDev =sd;
	m_IsInit = false;
}

inline void CBloomTarget::setExposure( float Exp )
{
	m_fExposure =Exp;
	m_IsInit = false;
}

inline float CBloomTarget::getBrightPass(void)
{
	return m_fBrightPass;
}

inline float CBloomTarget::getGaussMultiplier(void)
{
	return m_fGaussMultiplier;
}

inline float CBloomTarget::getMean(void)
{
	return m_fGaussMean;
}

inline float CBloomTarget::getStdDev(void)
{
	return m_fGaussStdDev;
}

inline float CBloomTarget::getExposure(void)
{
	return m_fExposure;
}

inline void CBloomTarget::setSample(float Sam )
{
	m_fSample = Sam;
	m_IsInit = false;
}

inline float CBloomTarget::getSample()
{
	return m_fSample;
}

inline void CBloomTarget::openHDR(bool bHDR)
{
}

}