#pragma once
#include "CSceneClassDefine.h"
#include "CDataScene.h"
#include "CDataChunk.h"
#include "IPathFinding.h"

namespace sqr
{
	class CMetaGridMgr;
	class CSceneClientCoder;
	class CSceneAmpCoder;
	class CSceneArgCoder;
	class CSceneArsCoder;
	class CVertexHardwareBuffer;
	class CNormalTerrainCoder;
	class CShaderTerrainCoder;
	class CBakedColorData;
	class CMetaRegion;
	struct GridColor;


	/*
	*	�˳�����������Ϣ
	*/
	struct CWeatherInfo : public CSceneMallocObject
	{
		uint32		nWeatherParam;			// ������������������ļ��ʣ���ѩ�ļ���
		uint32		nWindParam;				// ����,��Ӱ�����ʵ�ֶ���
	};

	class CSceneManagerClient : public sqr::CDataScene
	{
		friend class CSceneClientCoder;
		friend class CSceneAmpCoder;
		friend class CSceneArgCoder;
		friend class CSceneArsCoder;
		friend class CNormalTerrainCoder;
		friend class CShaderTerrainCoder;
		friend class CWaterCoder;
	public:
		CSceneManagerClient( const SString& SceneName, CTreeScene* pTreeScene , CRenderGroup* pGroup);
		CSceneManagerClient( const SString& SceneName, CTreeScene* pTreeScene , CRenderGroup* pGroup, uint32 Wdith , uint32 Height, uint32 RecycleSize );
		~CSceneManagerClient();
	public:
		uint32 GetSubRegionNum();
		uint32 GetRegionNum();
		uint32 GetWidthInSubRegion();
		uint8  GetGridPixel();

		IMap*					GetMapPath();

		CMetaRegion*			GetRegion(uint32 uGridX, uint32 uGridY);
		CMetaRegion*			GetRegion(uint32 RegionId);
		
		const GridColor*		GetGridColor(uint32 RegionId );
		CVertexHardwareBuffer*	GetColorBuffer(void);

		CTerrainSetUnit&		GetSetUnit( uint16 uIndex )
		{
			return m_SetUnit[uIndex];
		}	

		SString&					GetAudioUnit(uint16 uIndex)
		{
			return m_AudioUnit[uIndex];
		}

		//-------------------		OffsetHeight	-------------------
		int16	GetOffsetHeight( uint32 uX, uint32 uY );

		//-------------------		TerranHeight	-------------------
		int16	GetTerranHeight( uint32 uX, uint32 uY );

		//-------------------		TerranLogicHeight	-------------------
		int16	GetTerranLogicHeight( uint32 uX, uint32 uY );

		//-------------------		TerranNomal	-------------------
		int8	GetTerranNomal( uint32 uX, uint32 uY );

		//-------------------		TerranLogicNomal	-------------------
		CVector3f GetTerranLogicNomal( uint32 uX, uint32 uY );
	protected:
		float				m_fPercent;

		CBakedColorData*	m_pBakedColorData;
		CMetaGridMgr*		m_pGridMgr;

		
		IMap*				m_pMapPath;			// Ѱ·��Ϣ //���Ӧ�ò��ȥ��

		uint32				m_nVersion;			// �汾
		uint32				m_uRegionID;
		char				m_szDesc[64];		// ��ͼ������

		uint8				m_uGridWidth;			// ÿ�����ӵĵĿ���ж��ٸ�����
		uint32				m_uRegionNum;			// ����������ĸ���
		uint32				m_uSubRegionNum;		// ������������ĸ���
		uint32				m_uWidthInSubRegion;	// �����Ŀ�ȣ���������Ϊ��λ
		char				m_szTileSet[64];		// ��ͼ��ص�ͼ�ص�����

		//�������
		SVector<CTerrainSetUnit>		m_SetUnit;

		SVector<SString>				m_AudioUnit;

		//bool						m_bEnableBaking;
	protected:
		void	Initialize( void );
		void	DoProcess( float fPercent );
		void	CalculateNomal();

		//	-----------------------------
		void	OnPreLoad(void);
		void	OnLoaded(void);
	};

	inline uint8  
		CSceneManagerClient::GetGridPixel()
	{
		return m_uGridWidth;
	}


	inline uint32 
		CSceneManagerClient::GetSubRegionNum()
	{
		return m_uSubRegionNum;
	}

	inline uint32 
		CSceneManagerClient::GetRegionNum()
	{
		return m_uRegionNum;
	}

	inline uint32 
		CSceneManagerClient::GetWidthInSubRegion()
	{
		return m_uWidthInSubRegion;
	}

	inline IMap* 
		CSceneManagerClient::GetMapPath()
	{
		return m_pMapPath;
	}
}