using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;

namespace sqr_tools {

ref class TestProp //:public Object
{
public:
	TestProp(void);
public:
	[CategoryAttribute("������"), DescriptionAttribute("��ID")]
		property System::Int32^ Region 
		{
			System::Int32^ get();
			void set(System::Int32^ value);
		}

		[CategoryAttribute("������"), DescriptionAttribute("����ɫ")]
		property System::Drawing::Color FogColor
		{
			System::Drawing::Color get();
			void set(System::Drawing::Color value);
		}

		[CategoryAttribute("������"), DescriptionAttribute("��ʼλ��")]
		property System::Single^ Begin
		{
			System::Single^ get();
			void set(System::Single^ value);
		}

		[CategoryAttribute("������"), DescriptionAttribute("����λ��")]
		property System::Single^ End
		{
			System::Single^ get();
			void set(System::Single^ value);
		}

		[CategoryAttribute("������"), DescriptionAttribute("�ܶ�")]
		property System::Single^ Density
		{
			System::Single^ get();
			void set(System::Single^ value);
		}

	protected: 
		System::Int32^	m_Region;
		System::Drawing::Color	m_FogColor;
		System::Single^ m_Begin;
		System::Single^ m_End;
		System::Single^ m_Density;
};

}
