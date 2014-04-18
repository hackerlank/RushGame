#pragma once
#include "TSingleton.h"
#include "CVector3.h"

namespace sqr
{
	class CLoadMapResInfo: public Singleton<CLoadMapResInfo>
	{
	public:
		struct CREATE_ITEM
		{
			uint32					dwResType;		//��Դ����:MOD,SKE,EFX		(����)
			std::string				szFileName;		//��Դ�ļ���						(����)
			std::string				szClassName;	//��Դ����							(����)
			std::string				szParam;		//���Ͳ���
		};

		struct EditorModelInfo
		{
			///��editor�õı���
			CVector3f vUserOffsetEx; //�����ƶ�
			bool	  bBakingColor;
			int		  nMirrorType;
			int		  nAffectRatio;
			///trans state 0 - auto 1 -  trans 2 - untrans
			BYTE	  bTransState;
		};

		struct EditorOffsetInfo
		{
			///��editor�õı���
			CVector3f vUserOffsetEx; //�����ƶ�
		};

		struct OBJECT_CREATE_PARAM
		{
			std::vector<CREATE_ITEM> Items;
		};

		struct CTerrainSetUnit
		{
			std::vector<OBJECT_CREATE_PARAM> m_UnitPart;
		};

		void DealGridObjectsToTileRegion();
		void MakeTerrainTexture();

	public:
		CLoadMapResInfo(void);
		~CLoadMapResInfo(void);
		void  DestroyEditorModelInfo();
		void  DestroyAllContainerInfo();

		//�������
		vector<CTerrainSetUnit>		m_SetUnit;
		vector<string>			    m_CueNameUnit;

		///���ڼ�¼��ͼ���õ���������������֣��������������Ϣ�򱣴�index����
		map<string, int16>			m_mapTerrainTextureNames;
		map<int16, string>			m_mapIndexTextureNames;///���ڼ�¼��ͼ���õ����������������
		vector<string>				m_vecWaterTextureNames;///���ڼ�¼��ͼ���õ�������ˮ���������
		set<string>					m_setTerrainTexIndexs;
		vector<EditorModelInfo *>   vecEditorSingleModelInfo;///��ȡһ��TileRegion(4*4Region)���������б����ڸ�gameobject�ã�������������Ӧ���ĸ�ģ�ͣ���ʱ����ģ�ͻ�ʧ��
		vector<EditorModelInfo *>   vecEditorLoginModelInfo;
		vector<EditorOffsetInfo *>  vecEditorPointLightOffsetInfo;///light offset info
		vector<EditorOffsetInfo *>  vecEditorSpotLightOffsetInfo;
		vector<EditorOffsetInfo *>  vecEditorAudioOffsetInfo;///light offset info
		size_t						editorSingleModelCnt;///not coclude login mode;
		size_t						editorLoginModelCnt;
	};
}