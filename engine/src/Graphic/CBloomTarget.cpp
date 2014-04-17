//------------------------------------------------------------------------------
//  CBloomTarget.cpp
//  Ϊ�˼����ϴ���, ���������ֻ�������.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "CBloomTarget.h"
#include "CPostProcessManager.h"
#include "BaseHelper.h"
#include "CGpuProgramParameters.h"
#include "CGpuProgramMgr.h"
#include "CRenderTarget.h"
#include "CRenderPipeline.h"

namespace sqr
{
CBloomTarget::CBloomTarget(CRenderTarget* pRenderTarget)
: m_pRenderTarget(pRenderTarget)
{
	m_IsInit = false;

	//���ó�ʼֵ - ֱ�ӹ�ϵ��Ч�� - �������Ҫ
	m_fBrightPass		= 0.000f;	//��ͨ����0~1��
	m_fGaussMultiplier	= 0.35f;	//����ǿ��(0~2)
	m_fGaussMean		= 0.292f;	//����ƫ��(-1~+1)
	m_fGaussStdDev      = 0.363f;	//�ں϶� (0~1)
	m_fExposure			= 1.00f;	//��������0~2
	m_fSample           = 0.0f;
}

CBloomTarget::~CBloomTarget()
{
	if (CPostProcessManager::HasInst())
		CPostProcessManager::Destroy();
	Release();
}

bool CBloomTarget::Create(void)
{
	if (CGpuProgramMgr::GetInst()->isLoaded())
		CPostProcessManager::GetInst()->Setup();
	return true;
}

bool CBloomTarget::Release(void)
{
	if (CPostProcessManager::HasInst())
		CPostProcessManager::GetInst()->Discard();
	m_IsInit = false;
	return true;
}

bool CBloomTarget::IsUseful()
{
	return m_IsInit;
}

//������������˱仯 ������ȾǰӦ������-�����д˺�����
//�������ܿ��ǣ�û��ʹ������Ⱦ�����У�Ӧ���ֶ�����
void CBloomTarget::InitParams(void)
{
	if (m_IsInit)
		return;

	CIRect viewport;
	m_pRenderTarget->GetRect(viewport);

	float fInvWidthA = 1.0f / ( viewport.Width() * 0.25f );
	float fInvHeightA = 1.0f / ( viewport.Height() * 0.25f );

	for ( int i = 0; i < 9; i++ )
	{
		m_HBloomOffsets[i] = (float( i ) - 4.0f) * fInvWidthA;
		float x = (float( i ) - 4.0f) / 4.0f;
		m_HBloomWeights[i] = m_fGaussMultiplier * ComputeGaussianValue( x, m_fGaussMean, m_fGaussStdDev );
	}

	for ( int i = 0; i < 9; i++ )
	{
		m_VBloomOffsets[i] = (float( i ) - 4.0f) * fInvHeightA;
		float x = (float( i ) - 4.0f) / 4.0f;
		m_VBloomWeights[i] = m_fGaussMultiplier * ComputeGaussianValue( x, m_fGaussMean, m_fGaussStdDev );
	}
	
	m_IsInit = true;
}

////------------------------------------------------------------------------------
//bool CBloomTarget::BeginTarget( CRenderGroup* pRenderGroup )
//{
//	
//
//	//manager->Begin(pRenderGroup);
//	return true;
//}

//------------------------------------------------------------------------------
bool CBloomTarget::DoPostProcess( void )
{
	if (CPostProcessManager::GetInst()->IsEffectEnabled("Bloom"))
	{
		InitParams();
		CRenderPipeline* pPipeline = CRenderPipeline::GetInst();
		pPipeline->SetFragmentShaderF(PPSP_BRIGHT_PASS_THRESHOLD_PS, &m_fBrightPass, 1);
		pPipeline->SetFragmentShaderF(PPSP_HBLOOM_OFFSETS_PS, m_HBloomOffsets, 3);
		pPipeline->SetFragmentShaderF(PPSP_HBLOOM_WEIGHTS_PS, m_HBloomWeights, 3);
		pPipeline->SetFragmentShaderF(PPSP_VBLOOM_OFFSETS_PS, m_VBloomOffsets, 3);
		pPipeline->SetFragmentShaderF(PPSP_VBLOOM_WEIGHTS_PS, m_VBloomWeights, 3);
		pPipeline->SetFragmentShaderF(PPSP_SCALER_PS, &m_fSample, 1);
		pPipeline->SetFragmentShaderF(PPSP_EXPOSURE_PS, &m_fExposure, 1);
	}

	CPostProcessManager::GetInst()->DoPostProcess();
	return true;
}

//------------------------------------------------------------------------------
bool CBloomTarget::IsTurnOn( const string& effect )
{
	return CPostProcessManager::GetInst()->IsEffectEnabled(effect);
}

//------------------------------------------------------------------------------
void CBloomTarget::SetTurnOn( const string& effect, bool b )
{
	CPostProcessManager::GetInst()->SetEffectEnabled(effect, b);
}
}