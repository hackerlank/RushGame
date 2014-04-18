#pragma once

#include "CBaseProp.h"

namespace sqr_tools 
{

	public ref class CModelProp :public CBaseProp
	{
	public:
		CModelProp(void* ctrl);
		~CModelProp();
	public:
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ������")]
		property System::String^ ModelName
		{
			System::String^ get();
			void set(System::String^ value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ��λ��X")]
		property  float ModelPosX
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ��λ��Y")]
		property float ModelPosY
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ��λ��Z")]
		property float ModelPosZ
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ����X��ת")]
		property  float ModelRotX
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ����Y��ת")]
		property float ModelRotY
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ����Z��ת")]
		property float ModelRotZ
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ������X")]
		property  float ModelScaleX
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ������Y")]
		property float ModelScaleY
		{
			float get();
			void set(float value);
		}
		[CategoryAttribute("ģ������"), DescriptionAttribute("ģ������Z")]
		property float ModelScaleZ
		{
			float get();
			void set(float value);
		}

	protected:
		System::String^ m_ModelName;
		float m_ModelPosX;
		float m_ModelPosY;
		float m_ModelPosZ;
		float m_ModelRotX;
		float m_ModelRotY;
		float m_ModelRotZ;
		float m_ModelScaleX;
		float m_ModelScaleY;
		float m_ModelScaleZ;
	
	public:
		virtual	void UpdateProp() override;
		virtual	void UpdateTimerProp() override;
	};

//!namespace
}