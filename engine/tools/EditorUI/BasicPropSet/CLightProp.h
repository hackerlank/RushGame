#pragma once

#include "CBaseProp.h"

namespace sqr_tools {

	enum struct LIGHTTYPE
	{
		DIR_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};
	/// <summary>
	/// Summary for CLightCom
	/// </summary>
	public ref class CLightProp :public CBaseProp
	{
	public:
		CLightProp(void* ctrl);
		~CLightProp();

		public:
			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹ�ID")]
			property int LightID 
			{
				int get();
				void set(int value);
			}

			[CategoryAttribute("�ƹ�����"),DescriptionAttribute("�ƹ�����")]
			property LIGHTTYPE LightType 
			{
				LIGHTTYPE get();
				void set(LIGHTTYPE value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹ���ɫ")]
			property System::Drawing::Color LightColor
			{
				System::Drawing::Color get();
				void set(System::Drawing::Color value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹ�λ��X")]
			property float LightPosX
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹ�λ��Y")]
			property float LightPosY
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹ�λ��Z")]
			property float LightPosZ
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹⷽ��X")]
			property float LightDirX
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹⷽ��Y")]
			property float LightDirY
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹⷽ��Z")]
			property float LightDirZ
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�ƹ�����"), DescriptionAttribute("�ƹ�������")]
			property float LightMaxDis
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�۹������"), DescriptionAttribute("�۹������˥��")]
			property float LightFalloff
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�۹������"), DescriptionAttribute("����˥��ϵ��1")]
			property float LightAtten0
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�۹������"), DescriptionAttribute("����˥��ϵ��2")]
			property float LightAtten1
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�۹������"), DescriptionAttribute("����˥��ϵ��3")]
			property float LightAtten2
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�۹������"), DescriptionAttribute("�۹���ھ�")]
			property float LightInnerR
			{
				float get();
				void set(float value);
			}

			[CategoryAttribute("�۹������"), DescriptionAttribute("�۹���⾶")]
			property float LightOuterR
			{
				float get();
				void set(float value);
			}

	protected: 
			int			m_LightID;
			LIGHTTYPE				m_LightType;
			System::Drawing::Color	m_LightColor;
			float			m_LightPosX;
			float			m_LightPosY;
			float			m_LightPosZ;		
			float			m_LightDirX;
			float			m_LightDirY;
			float			m_LightDirZ;
			float			m_LightMaxDis;
			float			m_LightFalloff;
			float			m_LightAtten0;
			float			m_LightAtten1;
			float			m_LightAtten2;
			float			m_LightInnerR;
			float			m_LightOuterR;
	
	public:
		virtual	void UpdateProp() override;
		virtual	void UpdateTimerProp() override;
	};
}
