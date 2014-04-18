#pragma once

#include "CBaseProp.h"
#include "CPropChinese.h"

namespace sqr_tools {

	ref class CSceneProp :public CBaseProp
	{
	public:
		CSceneProp(void* ctrl);
		~CSceneProp();

	public:
		#pragma region mapProp
			[CategoryAttribute("��������"), DescriptionAttribute("��������")]
			property System::String^ SceneName 
			{
				System::String^ get();
				void set(System::String^ value);
			}
			[CategoryAttribute("��������"), DescriptionAttribute("�������")]
			property int SceneWidth 
			{
				int get();
				void set(int value);
			}
			[CategoryAttribute("��������"), DescriptionAttribute("�����߶�")]
			property int SceneHeight 
			{
				int get();
				void set(int value);
			}
			[CategoryAttribute("��������"), DescriptionAttribute("��ǰ����ID")]
			property int SceneRegion 
			{
				int get();
				void set(int value);
			}
	#pragma endregion

	#pragma region camProp
			[CategoryAttribute("�������"), DescriptionAttribute("�������")]
			property int CamType 
			{
				int get();
				void set(int value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���λ��X")]
			property  float CamPosX
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���λ��Y")]
			property float CamPosY
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���λ��Z")]
			property float CamPosZ
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("�������X")]
			property float CamDirX
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("�������Y")]
			property float CamDirY
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("�������Z")]
			property float CamDirZ
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���Ŀ��X")]
			property float CamTargetX
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���Ŀ��Y")]
			property float CamTargetY
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���Ŀ��Z")]
			property float CamTargetZ
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���Զ����")]
			property float CamFarDis
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("�������"), DescriptionAttribute("���������")]
			property float CamNearDis
			{
				float get();
				void set(float value);
			}
	#pragma endregion

	#pragma region GlightProp
			[CategoryAttribute("ȫ�ֹ��"), DescriptionAttribute("��������ɫ")]
			property System::Drawing::Color AmbientColor
			{
				System::Drawing::Color get();
				void set(System::Drawing::Color value);
			}
			[CategoryAttribute("ȫ�ֹ��"), DescriptionAttribute("ȫ��̫������ɫ")]
			property System::Drawing::Color SunColor 
			{
				System::Drawing::Color get();
				void set(System::Drawing::Color value);
			}
			[CategoryAttribute("ȫ�ֹ��"), DescriptionAttribute("���ﻷ������ɫ")]
			property System::Drawing::Color PlayerAmbientColor 
			{
				System::Drawing::Color get();
				void set(System::Drawing::Color value);
			}
			[CategoryAttribute("ȫ�ֹ��"), DescriptionAttribute("����̫������ɫ")]
			property System::Drawing::Color PlayerSunColor 
			{
				System::Drawing::Color get();
				void set(System::Drawing::Color value);
			}
			[CategoryAttribute("ȫ�ֹ��"), DescriptionAttribute("̫������X")]
			property float SunDirX
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("ȫ�ֹ��"), DescriptionAttribute("̫������Y")]
			property float SunDirY
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("ȫ�ֹ��"), DescriptionAttribute("̫������Z")]
			property float SunDirZ
			{
				float get();
				void set(float value);
			}
	#pragma endregion
			
	#pragma region FogProp
	public:
			enum struct FOGMODE
			{
				FOG_LINEAR		=1,
				FOG_EXPONENT
			};

			[CategoryAttribute("������"), DescriptionAttribute("������")]
			property FOGMODE FogType 
			{
				FOGMODE get();
				void set(FOGMODE value);
			}

			[CategoryAttribute("������"), DescriptionAttribute("����ɫ")]
			property System::Drawing::Color FogColor
			{
				System::Drawing::Color get();
				void set(System::Drawing::Color value);
			}

			[CategoryAttribute("������"), DescriptionAttribute("��ʼλ��")]
			property float FogBegin
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("������"), DescriptionAttribute("����λ��")]
			property float FogEnd
			{
				float get();
				void set(float value);
			}
			[CategoryAttribute("������"), DescriptionAttribute("�ܶ�")]
			property float FogDensity
			{
				float get();
				void set(float value);
			}
	#pragma endregion

	protected:
		//����
		System::String^ m_SceneName;
		int		m_Width;
		int		m_Height;
		int		m_SceneRegion;
		int		m_LastSceneRegion;
		//Cam
		int		m_CamType;
		float	m_CamPosX;
		float	m_CamPosY;
		float	m_CamPosZ;
		float	m_CamDirX;
		float	m_CamDirY;
		float	m_CamDirZ;
		float	m_CamTargetX;
		float	m_CamTargetY;
		float	m_CamTargetZ;
		float	m_CamFarDis;
		float	m_CamNearDis;
		//glight
		System::Drawing::Color m_AmbientColor;
		System::Drawing::Color m_SunColor;
		System::Drawing::Color m_PlayerAmbientColor;
		System::Drawing::Color m_PlayerSunColor;
		float	m_SunDirX;
		float	m_SunDirY;
		float	m_SunDirZ;
		//fog
		FOGMODE					m_FogType;
		System::Drawing::Color	m_FogColor;
		float			m_Begin;
		float			m_End;
		float			m_Density;

	public:
		virtual	void UpdateProp() override;
		virtual	void UpdateTimerProp() override;
	};


//!namespace
}