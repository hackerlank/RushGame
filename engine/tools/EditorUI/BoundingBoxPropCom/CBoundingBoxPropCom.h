#pragma once
#include "CBaseComponent.h"

namespace sqr_tools
{
	public ref class CBoundingBoxPropCom : public CBaseComponent
	{
	public: 
		CBoundingBoxPropCom();
		~CBoundingBoxPropCom();

		virtual char* GetImpTypeName(void) override;
		virtual void  Update(void) override;

		[CategoryAttribute("ƫ��"), DescriptionAttribute("Xƫ��")]
		property float xDelta
		{
			float get();
			void set(float value);
		}

		[CategoryAttribute("ƫ��"), DescriptionAttribute("Yƫ��")]
		property float yDelta 
		{
			float get();
			void set(float value);
		}

		[CategoryAttribute("ƫ��"), DescriptionAttribute("Zƫ��")]
		property float zDelta 
		{
			float get();
			void set(float value);
		}

		[CategoryAttribute("��С"), DescriptionAttribute("X��С")]
		property float xSize
		{
			float get();
			void set(float value);
		}

		[CategoryAttribute("��С"), DescriptionAttribute("Y��С")]
		property float ySize 
		{
			float get();
			void set(float value);
		}


		[CategoryAttribute("��С"), DescriptionAttribute("Z��С")]
		property float zSize 
		{
			float get();
			void set(float value);
		}

		[CategoryAttribute("��ʾ"), DescriptionAttribute("��ʾ���Χ��")]
		property bool showBigBox 
		{
			bool get();
			void set(bool value);
		}

		[CategoryAttribute("��ʾ"), DescriptionAttribute("��ʾС��Χ��")]
		property bool showSmaBox 
		{
			bool get();
			void set(bool value);
		}

		[CategoryAttribute("�ؼ�������"), DescriptionAttribute("���ݼ�ӵ����")]
		property System::Windows::Forms::ContainerControl^ ControlParent 
		{
			System::Windows::Forms::ContainerControl^ get();
			void set(System::Windows::Forms::ContainerControl^ value);
		}

	protected: virtual System::Void _InitCtrl(void) override;
	protected: float	m_fXDelta;
	protected: float	m_fYDelta;
	protected: float	m_fZDelta;
	protected: float	m_fXSize;
	protected: float	m_fYSize;
	protected: float	m_fZSize;
	protected: bool	m_bShowBigBox;
	protected: bool	m_bShowSmaBox;
	protected: System::Windows::Forms::ContainerControl^ m_pUserCtrl;
	};
}
