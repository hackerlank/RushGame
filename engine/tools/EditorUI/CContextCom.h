#pragma once
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;

namespace sqr_tools
{
	public ref class SceneSeleteEditor : public System::Windows::Forms::Design::FileNameEditor
	{
	public:
		virtual void InitializeDialog(System::Windows::Forms::OpenFileDialog^ openFileDialog) override;
	};

	class CCtrlBase;
	public ref class CContextComponent  : public System::ComponentModel::Component
	{
	public: 
		CContextComponent(void);
		~CContextComponent(void);
	
	protected:	CCtrlBase*		m_pCtrl;
	public: 	void*			GetCoreCtrl(void);
	public:
				[CategoryAttribute("��������"), DescriptionAttribute("��������"),EditorAttribute(SceneSeleteEditor::typeid,
					System::Drawing::Design::UITypeEditor::typeid)]
				property System::String^ SceneName
				{
					System::String^ get();
					void set(System::String^ value);
				}

				[CategoryAttribute("��������"), DescriptionAttribute("������������ɫ")]
				property System::Drawing::Color AmbientColor 
				{
					System::Drawing::Color get();
					void set(System::Drawing::Color value);
				}


				[CategoryAttribute("��������"), DescriptionAttribute("����̫������ɫ")]
				property System::Drawing::Color SunColor 
				{
					System::Drawing::Color get();
					void set(System::Drawing::Color value);
				}

				[CategoryAttribute("��������"), DescriptionAttribute("��ʾ�ر����")]
				property bool ShowGrid
				{
					bool get();
					void set(bool value);
				}

				[CategoryAttribute("��������"), DescriptionAttribute("��ʾ�ر��赲")]
				property bool ShowBlock
				{
					bool get();
					void set(bool value);
				}

	protected: String^	m_strSceneName;
	protected: bool		m_bShowGrid;
	protected: bool		m_bShowBlock;
	protected: System::Drawing::Color	m_cAmbinetColor;
	protected: System::Drawing::Color	m_cSunColor;
	};
}