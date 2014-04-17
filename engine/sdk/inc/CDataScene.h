#pragma once 
#include "RegionLightParam.h"
#include "CBaseScene.h"
#include "CCoder.h"

namespace sqr
{
	class CShadowedGrids;
	class CDataScene;
	class CCameraPath;
	class CTerrainMgr;
	class CWaterMgr;
	class CLogicDataMgr;
	class CSkyMgr;
	class CRenderGroup;
	class IMap;

	// ��������
	struct CSceneInfo : public CSceneMallocObject
	{
		uint32		dwAmbientColor;			// ��������ɫ
		uint32		dwShadowColor;			// ��Ӱ��ɫ
		uint32		dwSunColor;				// ̫������ɫ
		CVector3f	vSunDir;				// ̫���ⷽ��
		uint32		dwPlayerSunColor;		// ����̫����
		uint32		dwPlayerAmbientColor;	// ���ǻ�������ɫ
		uint32		dwPlayerLightColor;		// ���ǵ���ɫ
		float		fAttenuation;			// ���ǵ�ǿ��
		float		fRange;					// ���ǵƷ�Χ

		// fog
		uint32		dwFogColor;				// �����ɫ
		float		fFogStart;				// ��Ľ���
		float		fFogEnd;				// ���Զ��

		// sky
		char		szSkyTexture[64];		// �����ͼ
	};

	enum  ESceneTime
	{
		EST_MORNING		= 0,
		EST_NOON		= 1,
		EST_AFTERNOON	= 2,
		EST_NIGHT		= 3,
		EST_COUNT		= 4,
	};

	/*
	*	�˳�������Ч��Ϣ
	*/
	struct CAmbientFxInfo : public CSceneMallocObject
	{
		struct FX : public CSceneMallocObject
		{
			int32	nProbability;	// ����
			SString	szFxName;		// ��Ч��
			SString	szFxFileName;	// ��Ч�ļ���
		};
		SVector<FX>	Fx;
	};

	/*
	*	�˳������򻷾���Ч��Ϣ
	*/
	struct CRgnAmbientFxInfo : public CSceneMallocObject
	{
		int32	nPlayMax;		// ����ʱ������
		int32	nPlayMin;		// ����ʱ������
		int32	nIntervalMax;	// ���ż������
		int32	nIntervalMin;	// ���ż������
		SString	szFxName;		// ��Ч��
	};

	///������Ϣ���ݽṹ
	struct RegionsInfo : public CSceneMallocObject
	{
		RegionLightParam			lightParam;
		uint8						uFogMode;			 ///����������
		SString						strMusicName;
		SVector<CRgnAmbientFxInfo*> rgnAmbientFxList;

		~RegionsInfo();
	};
	
	class CSceneCoder
		: public CCoder
		, public CCoderHelper
		, public CSceneMallocObject
	{
	public:
		CSceneCoder(void);
		virtual bool SceneDeCode( CDataScene* pSceneManager ) = 0;
	protected:
		bool Lock(CDataScene* pScene);
		void UnLock(void);
	protected:
		CDataScene* m_pLockScene;
	};

	class CDataScene : public CBaseScene
	{
	public:
		CDataScene( const SString& SceneName, CTreeScene* pTreeScene, CRenderGroup* pRenderGroup  );
		CDataScene( const SString& SceneName, CTreeScene* pTreeScene, CRenderGroup* pRenderGroup, uint32 Wdith , uint32 Height, uint32 RecycleSize );
		~CDataScene();
	public:
		virtual	void	Create( void );
		virtual void	Destroy( void );

		void			SetCoder(CSceneCoder* pCoder);
		
		CTerrainMgr*	GetTerrainMgr(void);
		CSkyMgr*		GetSkyMgr(void);
		CWaterMgr*		GetWaterMgr(void);
		CLogicDataMgr*	GetLogicDataMgr(void);
		CRenderGroup*	GetRenderGroup(void);
	public:	//��ȡȫ�ֳ�����Ϣ
			uint32	GetWidthInRegion();
			uint32	GetHeightInRegion();
			uint32	GetWidthInGrid();
			uint32	GetHeightInGrid();
			uint32	GetWidthInPixel();
			uint32	GetHeightInPixel();

			///�õ���ǰ���صĸ߶� = TerrainHeight + LogicHeight
			float	GetOffsetHeightInPixel( float fPixelX, float fPixelY );
			bool	IsGridValid( uint32 uX, uint32 uY );

			CShadowedGrids*		GetShadowedGrids(void);
			RegionsInfo*		GetRegionInfo( const int index );
			SubRegionData*		GetSubRegion( const int index );
			  CCameraPath*		GetCameraPath( uint32 Index );
		const CSceneInfo&		GetSceneInfo( ESceneTime eStime );
		const CAmbientFxInfo&	GetAmbientFxInfo(void);
	protected:
		virtual void	OnPreLoad(void) {};
		virtual void	OnLoaded(void) {};
		virtual void	Initialize( void );;
	protected:
		virtual IMap*		CreateIMap(uint32 uWidthInRegion, uint32 uHeightInRegion) { return NULL; }; //����ӿ�Ҫ����ж��
		CTerrainMgr*		m_pTerrainMgr;
		CSkyMgr*			m_pSkyMgr;
		CWaterMgr*			m_pWaterMgr;
		CLogicDataMgr*		m_pLogicDataMgr;

		CSceneCoder*		m_pClientCoder;
		CShadowedGrids*		m_pShadowedGrids;
		CRenderGroup*		m_pRenderGroup;
	
	protected: //Data
		SVector<RegionsInfo*>			m_RgnsList;			//�������Ϣ�б�
		SMap<int,SubRegionData*>		m_SubRegionsMap;	//һ�������������������������Ϣ�б�
		CSceneInfo						m_SceneInfo[EST_COUNT];	// ȫ�����������Ϣ
		SVector<CCameraPath*>			m_vecCamPath;			// ������켣����
		CAmbientFxInfo					m_AmbientFxInfo;		// ������Ч��Ϣ

	protected:
		uint32				m_uWidthInRegion;		// �����Ŀ�ȣ���1/4����Ϊ��λ
		uint32				m_uHeightInRegion;		// �����ĸ߶ȣ���1/4����Ϊ��λ
		uint32				m_uWidthInGrid;			// �����Ŀ�ȣ��Ը���Ϊ��λ
		uint32				m_uHeightInGrid;		// �����ĸ߶ȣ��Ը���Ϊ��λ
		uint32				m_uWidthInPixel;		// �����Ŀ�ȣ�������Ϊ��λ
		uint32				m_uHeightInPixel;		// �����ĸ߶ȣ�������Ϊ��λ
	};


	inline CTerrainMgr*		CDataScene::GetTerrainMgr(void)		{ return m_pTerrainMgr;		}
	inline CSkyMgr*			CDataScene::GetSkyMgr(void)			{ return m_pSkyMgr;			}
	inline CWaterMgr*		CDataScene::GetWaterMgr(void)		{ return m_pWaterMgr;		}
	inline CLogicDataMgr*	CDataScene::GetLogicDataMgr(void)	{ return m_pLogicDataMgr;	}
	inline CRenderGroup*	CDataScene::GetRenderGroup(void)	{ return m_pRenderGroup;	}

	inline uint32 CDataScene::GetWidthInRegion()	{ return m_uWidthInRegion;	}
	inline uint32 CDataScene::GetHeightInRegion()	{ return m_uHeightInRegion;	}
	inline uint32 CDataScene::GetWidthInGrid()		{ return m_uWidthInGrid;	}
	inline uint32 CDataScene::GetHeightInGrid()		{ return m_uHeightInGrid;	}
	inline uint32 CDataScene::GetWidthInPixel()		{ return m_uWidthInPixel;	}
	inline uint32 CDataScene::GetHeightInPixel()	{ return m_uHeightInPixel;	}

	inline			CShadowedGrids*	CDataScene::GetShadowedGrids(void)				{ return m_pShadowedGrids;		}
	inline const	CSceneInfo&		CDataScene::GetSceneInfo( ESceneTime eStime )	{ return m_SceneInfo[eStime];	}
	inline const	CAmbientFxInfo&	CDataScene::GetAmbientFxInfo(void)				{ return m_AmbientFxInfo;		}
	inline			CCameraPath*	CDataScene::GetCameraPath( uint32 Index )		
	{
		if(Index<m_vecCamPath.size())
			return m_vecCamPath[Index];
		return NULL;
	}



}