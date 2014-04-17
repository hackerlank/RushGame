#pragma once
/**
	@class CWater

	ˮ.
	��ˮ��, ��Ե�˻�, �������.
	ˮ������Ϊ�͵���������������, ��ʱ��ʵʱ�������, Ĭ��Ϊ��������
	@todo �˻�������
	@todo ʵʱ����Ч��
 */
#include "TRefPointer.h"
#include "CRenderBatch.h"
#include "CWaterEffect.h"
#include "CWaterRegion.h"
//#include "CWaterRegionMesh.h"
#include "CWaterSurface.h"
#include "CElementManager.h"

namespace sqr
{
	SmartPointer(CWaterSurface);
	SmartPointer(CWaterEffect);
	class CRenderGroup;

	class CWaterSurTex
	{
		SString		m_TextName;
		ITexture*	m_Texture;

	public:
		CWaterSurTex()
		{
			m_Texture = NULL;
		}
		~CWaterSurTex();
		void		SetTextName( const char* FileName )
		{
			m_TextName = FileName;
		}
		ITexture*	GetTexture(void);
	};

	class CWaterMgr 
		: public CRenderBatch
		, public CElementManager
	{
	public:
		enum WaterEffectType
		{
			TextureAnimation,
			EnvironmentMapping,

			WaterEffectNum
		};
	public:
		CWaterMgr(CRenderGroup* pGroup);
		virtual ~CWaterMgr();
	public:
		// ����ˮ��
		CWaterSurface* 
			CreateSurface(WaterEffectType type, SVector<ITexture*>& textures);

		// ���ˮ��
		void ClearSurfaces();
		
		// ����
		void Render();

		void SetLightIndex(uint index)
		{
			m_LightIndex = index;
		}

		// ��ǰˮ������
		EWaterType GetWaterType() const;

		// ���õ�ǰˮ������, �����ӦЧ��������, ����false
		bool SetWaterType(EWaterType type);

		// �������ˮ������ʹ�õ�������������s
		void SetLowestEffectTexNameVec( const SVector<SString>& texNameVec );

	public:
		// ˮ��Ч��IDת��
		static SHADER_HANDLE EffectTypeToID(WaterEffectType type);

	protected:
		CElementNode*	_CreateNode( URegionID ID ) { return new CWaterRegion(this,ID); };

	private:
		void	SetLowestEffect();

	private:
		typedef pair<SHADER_HANDLE, SVector<ITexture*> > WaterSurfaceKey;
		typedef SMap<WaterSurfaceKey, CWaterSurfacePtr> WaterSurfaceMap;
		typedef SMap<WaterSurfaceKey, CWaterEffectPtr> WaterEffectMap;
		typedef SVector<SString> LowWaterTexNameVec;

		EWaterType					m_WaterType;	// ˮ������
		CWaterEffectPtr				m_LowestEffect;	// �������
		CWaterEffectPtr				highestEffect;	// �������

		SVector<CWaterSurfacePtr>	m_Surfaces;
		WaterSurfaceMap				surfaceMap;
		WaterEffectMap				effectMap;
		uint						m_LightIndex;
		CRenderGroup*				m_pGroup;
		LowWaterTexNameVec			m_LowTexNameVec;
	
	public: //��ʱ�ṹ
		void	RenderWave(uint32 uCurTime);
		CWater*	GetWater(uint32 OrgId , int32 nGrid);
		const CWaterWaveInfo&		GetWaterWaveInfo(void);
		SVector<CWaterSurTex>		m_TextUnit;		
		CWaterWaveInfo				m_WaveInfo;				// ���������Ϣ
	};

	// ---------------------------------------------------------------------------
	inline			EWaterType		CWaterMgr::GetWaterType() const		{ return m_WaterType; }
	inline const	CWaterWaveInfo&	CWaterMgr::GetWaterWaveInfo(void)	{ return m_WaveInfo;  }

}
