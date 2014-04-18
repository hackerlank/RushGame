#pragma once
#include "TRefPointer.h"
#include "TSingleton.h"
#include "CColor4.h"
#include "CVector3.h"

#ifdef _DEBUG
#define VERTEX_COLOR_EDIT_WHEN_BAKED	0
#else
#define VERTEX_COLOR_EDIT_WHEN_BAKED	1
#endif
namespace sqr
{
	class SGrid;
	class CMapEditObject;
	SmartPointer(CTObject);

	class OfflineRender : public TSingleton<OfflineRender>
	{
	public:
		OfflineRender();
		virtual ~OfflineRender();

		///����ÿ���������ڵĻ�����������
		void CalGridColorParam();

		void UseBakingResult(bool use, bool bShowTerrainBaking, bool bCleanRS = true);
		void PreviewTerrainBakingResult(bool use);

		///add new spot or point light
		void UpdateGridLights( int nGirdIndex );

		///ȡ������Ч����ʱ�򣬽�vertex.bColor����vertex.oColor
		void RevertVertexBakingToVertexColor(bool use);
	private:
		///��̬��ԴӰ��ر�ļ���
		void UpdateTerrainColor(bool shadow);

		void AddLightsToIlluminator(bool terrainOnly);
	private:
		friend class CObjectVertexProcessor;
		friend class CLightMapReceiver;
		friend class CObjectVertexBaker;
		friend class CTerrainBaker;

		struct LightColor
		{
			LightColor()
			{
				ZeroMemory(this, sizeof(LightColor));
			}
			CColor4 Ambient;
			CColor4 Diffuse;
			CColor4 Shadow;
		};
		vector<LightColor>			m_GridColor;	///��������⣬����ÿ�����Ӷ�Ӧ��̫����ͻ������
		map< int32, vector< CTObjectPtr> >  mPointLight;	//��¼���Դ
		map< int32, vector< CTObjectPtr> >  mSpotLight;		//��¼�۹��
		CVector3f							mSunlightDir;
	};
}