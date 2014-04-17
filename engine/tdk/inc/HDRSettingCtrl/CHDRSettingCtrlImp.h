#pragma once

#include "CCtrlBase.h"

namespace sqr{
	class CBloomTarget;
}

namespace sqr_tools
{
	#define	MGT_UPDATE_PARA	0x01
	class CHDRSettingCtrlImp :public CCtrlBase
	{
		REG_UI_IMP(CHDRSettingCtrlImp);
	public:
		CHDRSettingCtrlImp(void* param);
		~CHDRSettingCtrlImp(void);

	private:
		CBloomTarget    *m_pTarget;
		// ȫ�������һЩ����
		bool			m_bTurnOnGassTarget;// ����ȫ������
		float			m_fGaussBrightPass;	// ��ͨ�� 0~1
		float			m_fGaussMultiplier; // ����ǿ�� 0~2
		float			m_fGaussMean;		// ����ƫ�� -1~+1
		float			m_fGaussStdDev;		// �����ںͳ̶�  0~1
		float			m_fGaussExposure;	// ��������0~2
		float			m_fGaussSam;		// ��϶�

	private:
		virtual void  InitGaussParams();

	public:

		virtual void  CreateBloomTarget();
		virtual bool  HadBloomTarget();

		virtual void  SetIsTurnOnGaussTarget( const bool bTurn );
		virtual bool  GetIsTurnOnGaussTarget() const;

		virtual void  SetIsTurnOnRefract(const bool bTurn);

		virtual void  SetGaussBrightPass( const float fBrightPass );
		virtual float GetGaussBrightPass() const;

		virtual void  SetGaussMultiplier( const float fGaussMultiplier );
		virtual float GetGaussMultiplier() const;

		virtual void  SetGaussMean( const float fGaussMean );
		virtual float GetGaussMean() const;

		virtual void  SetGaussStdDev( const float fGaussStdDev );
		virtual float GetGaussStdDev() const;

		virtual void  SetGaussExposure( const float fExposure );
		virtual float GetGaussExposure() const;

		virtual void  SetGaussSample( const float fSam );
		virtual float GetGaussSample() const;

		virtual void  UpdateGaussParams();

		virtual void  SaveGaussParamsInfo( FILE * fp );
		virtual void  LoadGaussParamsInfo( FILE * fp );

		virtual void  Update();

		virtual bool _ProcMsg( const CMsg& msg );
	};

}
