#pragma once
#include "TSingleton.h"
#include "CSceneMallocObject.h"
#include "CVector3.h"

namespace sqr
{
	class CRenderable;
	struct CRgnAmbientFxInfo;

	class CRegionAmbientEffec : public CSceneMallocObject
	{
 		uint32			m_uPlayTimeLenMin;		///����ʱ�䳤����Сֵ
 		uint32			m_uPlayTimeLenMax;		///����ʱ�䳤�����ֵ
 		uint32			m_uPlayIntervalMin;		///2�β���ʱ����������Сֵ
 		uint32			m_uPlayIntervalMax;		///2�β���ʱ�����������ֵ
 		uint32			m_uCurRandomPlayTime;	///��ǰ�������ʱ�䳤��
 		uint32			m_uCurRandomInterval;	///��ǰ���2�β���ʱ��������
 		uint32			m_uPlayTime;			///��ǰ�Ѿ����Ŷ��ʱ��
 		uint32			m_uIntervalTime;		///��ǰ������ʱ��
 		bool			m_bPlayNotRepeat;
 		bool			m_bNeedPlayBegin;
 		bool			m_bNeedIntervalBegin;
		CRenderable*	m_pEffectRenderObj;


		bool	IsNeedPlay(const uint32 dwTime);
		uint32	GetRandomValue(uint32 nLowBound, uint32 nHighBound);

	public:
		CRegionAmbientEffec();
		~CRegionAmbientEffec();

		bool CreateAmbientEffect( const CRgnAmbientFxInfo* pAmbFxInfo );
		void Render( const uint32 uCurTime, const CVector3f& efxpos );
	};

	class CRegionAmbientEffectMgr : public CSceneMallocObject
	{
	private:
		SVector<CRegionAmbientEffec*>	m_pRegAmbientEffectList;
		void DestroyRegionAmbientEffect();

	public:
		CRegionAmbientEffectMgr();
		~CRegionAmbientEffectMgr();

		void CreateRegionAmbientEffect(const SVector<CRgnAmbientFxInfo*>& rgnAmbFxInfoList);
		void Render(const uint32 uCurTime, const CVector3f& efxpos);
	};

}