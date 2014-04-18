#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Timers;

namespace MapEditor {

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_bLoadFinish = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm();
	private: sqr_tools::CContextComponent^  MapEditorDescrip;
	private: System::Timers::Timer^ m_Timer;
	protected: 
	private: sqr_tools::CViewCtrl^  MainView;
	private: sqr_tools::CRTSCameraCom^  RTSCameraCom;

	private: System::Windows::Forms::ToolStripContainer^  FrameContainer;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  menuFileNew;
	private: System::Windows::Forms::ToolStripMenuItem^  MenuFile;
	private: System::Windows::Forms::ToolStripMenuItem^  menuFileOpen;
	private: System::Windows::Forms::ToolStripMenuItem^  menuFileSave;
	private: System::Windows::Forms::ToolStripMenuItem^  menuFileSaveOther;
	private: System::Windows::Forms::TabControl^  TabPanel;
	private: System::Windows::Forms::TabPage^	TabGrid;
	private: System::Windows::Forms::TabPage^	TabRecord;
	private: System::Windows::Forms::TabPage^	TabAduio;
	private: System::Windows::Forms::TabPage^	TabTex;
	private: System::Windows::Forms::TabPage^	TabLight;
	private: System::Windows::Forms::TabPage^	TabEffect;	
	private: System::Windows::Forms::TabPage^	TabBasic;
	private: System::Windows::Forms::TabPage^	TabPacket;
	private: System::Windows::Forms::TabPage^	TabRegion;
	private: System::Windows::Forms::TabPage^	TabBlock;
	private: System::Windows::Forms::TabPage^	TabObject;
	private: System::Windows::Forms::TabPage^  TabProp;	
	private: sqr_tools::CGridPanel^					m_GridView;
	private: sqr_tools::CRecordPanel^				m_RecordView;
	private: sqr_tools::CTextureViewCltr^			m_TexView;
	private: sqr_tools::CMapEffectCtrl^				m_EffectView;
	private: sqr_tools::CBasicPanel^				m_BasicView;
	private: sqr_tools::CSetViewCtrl^				m_SetView;
	private: sqr_tools::CTileViewCtrl^				m_TileView;
	private: sqr_tools::CBlockViewCtrl^				m_BlockViewCtrl;
	private: sqr_tools::AudioViewCtrl^  audioViewCtrl;
	private: System::Windows::Forms::ToolStripMenuItem^  menuFileMapEx;
	private: System::Windows::Forms::ToolStripMenuItem^  MenuEdit;
	protected: System::Windows::Forms::Timer^  m_FormTimer;
	private: System::ComponentModel::IContainer^  components;
	private: sqr_tools::CPropCtrl^  m_PropShow;
	private: sqr_tools::CLightView^  cLightView1;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��1��ȫ��CtrlFToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ���ص�1��F1ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ���ص�2��F2ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ɾ������DeleteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ���õر������ЧToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ǿ��F5ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ǿ�в�����F6ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ������ͼƬCtrl1ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �赲ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��������赲ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: sqr_tools::CFREECameraCom^  FREECameraCom;
	private: System::Windows::Forms::ToolStripMenuItem^  ���ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ԭ6ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �Ŵ�7ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��С8ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��תToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x������RToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  x�Ḻ��FToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  y������IToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  y�Ḻ��KToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  z������YToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  z�Ḻ��HToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ת90��TToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �ƶ�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����LeftToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����RightToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����PageUpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����PageDownToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��������HomeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��������EndToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ������ӰToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �л���ӰF6ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �ص���ѡ�����ӰToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ������ѡ�����ӰToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ������ӰToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �л�����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ȫ��������ӰToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ȫ����������ӰToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��һ����Ⱦ״̬F9ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ����MToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ɾ��DeleteToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ����CtrlCToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ճ��CtrlVToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ��ѡCtrlRToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ѡ��������ͬ����CtrlAToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ����ʹ���������ײCtrlF6ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;








	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

	bool										m_bLoadFinish;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->FrameContainer = (gcnew System::Windows::Forms::ToolStripContainer());
			this->MainView = (gcnew sqr_tools::CViewCtrl());
			this->MapEditorDescrip = (gcnew sqr_tools::CContextComponent());
			this->TabPanel = (gcnew System::Windows::Forms::TabControl());
			this->TabGrid = (gcnew System::Windows::Forms::TabPage());
			this->m_GridView = (gcnew sqr_tools::CGridPanel());
			this->TabRecord = (gcnew System::Windows::Forms::TabPage());
			this->m_RecordView = (gcnew sqr_tools::CRecordPanel());
			this->TabAduio = (gcnew System::Windows::Forms::TabPage());
			this->audioViewCtrl = (gcnew sqr_tools::AudioViewCtrl());
			this->TabTex = (gcnew System::Windows::Forms::TabPage());
			this->m_TexView = (gcnew sqr_tools::CTextureViewCltr());
			this->TabLight = (gcnew System::Windows::Forms::TabPage());
			this->cLightView1 = (gcnew sqr_tools::CLightView());
			this->TabEffect = (gcnew System::Windows::Forms::TabPage());
			this->m_EffectView = (gcnew sqr_tools::CMapEffectCtrl());
			this->TabBasic = (gcnew System::Windows::Forms::TabPage());
			this->m_BasicView = (gcnew sqr_tools::CBasicPanel());
			this->TabPacket = (gcnew System::Windows::Forms::TabPage());
			this->m_TileView = (gcnew sqr_tools::CTileViewCtrl());
			this->TabObject = (gcnew System::Windows::Forms::TabPage());
			this->m_SetView = (gcnew sqr_tools::CSetViewCtrl());
			this->TabRegion = (gcnew System::Windows::Forms::TabPage());
			this->TabBlock = (gcnew System::Windows::Forms::TabPage());
			this->m_BlockViewCtrl = (gcnew sqr_tools::CBlockViewCtrl());
			this->TabProp = (gcnew System::Windows::Forms::TabPage());
			this->m_PropShow = (gcnew sqr_tools::CPropCtrl());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->MenuFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuFileNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuFileOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuFileSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuFileSaveOther = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuFileMapEx = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MenuEdit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��1��ȫ��CtrlFToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���ص�1��F1ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���ص�2��F2ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ɾ������DeleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���õر������ЧToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ǿ��F5ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ǿ�в�����F6ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->������ͼƬCtrl1ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�ƶ�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����LeftToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����RightToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����PageUpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����PageDownToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��������HomeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��������EndToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ԭ6ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�Ŵ�7ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��С8ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��תToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x������RToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->x�Ḻ��FToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->y������IToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->y�Ḻ��KToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->z������YToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->z�Ḻ��HToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ת90��TToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->������ӰToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�л���ӰF6ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�ص���ѡ�����ӰToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->������ѡ�����ӰToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->������ӰToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�л�����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ȫ��������ӰToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ȫ����������ӰToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��һ����Ⱦ״̬F9ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����MToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ɾ��DeleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����CtrlCToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ճ��CtrlVToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ѡCtrlRToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ѡ��������ͬ����CtrlAToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ʹ���������ײCtrlF6ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�赲ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��������赲ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->RTSCameraCom = (gcnew sqr_tools::CRTSCameraCom());
			this->m_FormTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->FREECameraCom = (gcnew sqr_tools::CFREECameraCom());
			this->FrameContainer->ContentPanel->SuspendLayout();
			this->FrameContainer->TopToolStripPanel->SuspendLayout();
			this->FrameContainer->SuspendLayout();
			this->TabPanel->SuspendLayout();
			this->TabGrid->SuspendLayout();
			this->TabRecord->SuspendLayout();
			this->TabAduio->SuspendLayout();
			this->TabTex->SuspendLayout();
			this->TabLight->SuspendLayout();
			this->TabEffect->SuspendLayout();
			this->TabBasic->SuspendLayout();
			this->TabPacket->SuspendLayout();
			this->TabObject->SuspendLayout();
			this->TabBlock->SuspendLayout();
			this->TabProp->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// FrameContainer
			// 
			// 
			// FrameContainer.ContentPanel
			// 
			this->FrameContainer->ContentPanel->Controls->Add(this->MainView);
			this->FrameContainer->ContentPanel->Controls->Add(this->TabPanel);
			this->FrameContainer->ContentPanel->Size = System::Drawing::Size(841, 578);
			this->FrameContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->FrameContainer->Location = System::Drawing::Point(0, 0);
			this->FrameContainer->Name = L"FrameContainer";
			this->FrameContainer->Size = System::Drawing::Size(841, 603);
			this->FrameContainer->TabIndex = 2;
			this->FrameContainer->Text = L"FrameIn";
			// 
			// FrameContainer.TopToolStripPanel
			// 
			this->FrameContainer->TopToolStripPanel->Controls->Add(this->menuStrip1);
			// 
			// MainView
			// 
			this->MainView->BackColor = System::Drawing::Color::Gray;
			this->MainView->ContextComponent = this->MapEditorDescrip;
			this->MainView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->MainView->Location = System::Drawing::Point(0, 0);
			this->MainView->Name = L"MainView";
			this->MainView->Size = System::Drawing::Size(499, 578);
			this->MainView->TabIndex = 0;
			// 
			// MapEditorDescrip
			// 
			this->MapEditorDescrip->AmbientColor = System::Drawing::Color::Empty;
			this->MapEditorDescrip->SceneName = nullptr;
			this->MapEditorDescrip->ShowBlock = false;
			this->MapEditorDescrip->ShowGrid = false;
			this->MapEditorDescrip->SunColor = System::Drawing::Color::Empty;
			// 
			// TabPanel
			// 
			this->TabPanel->Controls->Add(this->TabGrid);
			this->TabPanel->Controls->Add(this->TabRecord);
			this->TabPanel->Controls->Add(this->TabAduio);
			this->TabPanel->Controls->Add(this->TabTex);
			this->TabPanel->Controls->Add(this->TabLight);
			this->TabPanel->Controls->Add(this->TabEffect);
			this->TabPanel->Controls->Add(this->TabBasic);
			this->TabPanel->Controls->Add(this->TabPacket);
			this->TabPanel->Controls->Add(this->TabObject);
			this->TabPanel->Controls->Add(this->TabRegion);
			this->TabPanel->Controls->Add(this->TabBlock);
			this->TabPanel->Controls->Add(this->TabProp);
			this->TabPanel->Dock = System::Windows::Forms::DockStyle::Right;
			this->TabPanel->Location = System::Drawing::Point(499, 0);
			this->TabPanel->Multiline = true;
			this->TabPanel->Name = L"TabPanel";
			this->TabPanel->SelectedIndex = 0;
			this->TabPanel->Size = System::Drawing::Size(342, 578);
			this->TabPanel->TabIndex = 2;
			this->TabPanel->Selected += gcnew System::Windows::Forms::TabControlEventHandler(this, &MainForm::TabPanel_Selected);
			// 
			// TabGrid
			// 
			this->TabGrid->Controls->Add(this->m_GridView);
			this->TabGrid->Location = System::Drawing::Point(4, 40);
			this->TabGrid->Name = L"TabGrid";
			this->TabGrid->Padding = System::Windows::Forms::Padding(3);
			this->TabGrid->Size = System::Drawing::Size(334, 534);
			this->TabGrid->TabIndex = 0;
			this->TabGrid->Text = L"����";
			this->TabGrid->UseVisualStyleBackColor = true;
			// 
			// m_GridView
			// 
			this->m_GridView->AutoScroll = true;
			this->m_GridView->ContextComponent = this->MapEditorDescrip;
			this->m_GridView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_GridView->Location = System::Drawing::Point(3, 3);
			this->m_GridView->Name = L"m_GridView";
			this->m_GridView->Size = System::Drawing::Size(328, 528);
			this->m_GridView->TabIndex = 0;
			// 
			// TabRecord
			// 
			this->TabRecord->Controls->Add(this->m_RecordView);
			this->TabRecord->Location = System::Drawing::Point(4, 40);
			this->TabRecord->Name = L"TabRecord";
			this->TabRecord->Padding = System::Windows::Forms::Padding(3);
			this->TabRecord->Size = System::Drawing::Size(334, 534);
			this->TabRecord->TabIndex = 1;
			this->TabRecord->Text = L"¼��";
			this->TabRecord->UseVisualStyleBackColor = true;
			// 
			// m_RecordView
			// 
			this->m_RecordView->AutoScroll = true;
			this->m_RecordView->ContextComponent = this->MapEditorDescrip;
			this->m_RecordView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_RecordView->Location = System::Drawing::Point(3, 3);
			this->m_RecordView->Name = L"m_RecordView";
			this->m_RecordView->Size = System::Drawing::Size(328, 528);
			this->m_RecordView->TabIndex = 1;
			// 
			// TabAduio
			// 
			this->TabAduio->Controls->Add(this->audioViewCtrl);
			this->TabAduio->Location = System::Drawing::Point(4, 40);
			this->TabAduio->Name = L"TabAduio";
			this->TabAduio->Padding = System::Windows::Forms::Padding(3);
			this->TabAduio->Size = System::Drawing::Size(334, 534);
			this->TabAduio->TabIndex = 2;
			this->TabAduio->Text = L"��Ч";
			this->TabAduio->UseVisualStyleBackColor = true;
			// 
			// audioViewCtrl
			// 
			this->audioViewCtrl->AutoScroll = true;
			this->audioViewCtrl->ContextComponent = nullptr;
			this->audioViewCtrl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->audioViewCtrl->Location = System::Drawing::Point(3, 3);
			this->audioViewCtrl->Name = L"audioViewCtrl";
			this->audioViewCtrl->Size = System::Drawing::Size(328, 528);
			this->audioViewCtrl->TabIndex = 0;
			// 
			// TabTex
			// 
			this->TabTex->Controls->Add(this->m_TexView);
			this->TabTex->Location = System::Drawing::Point(4, 40);
			this->TabTex->Name = L"TabTex";
			this->TabTex->Padding = System::Windows::Forms::Padding(3);
			this->TabTex->Size = System::Drawing::Size(334, 534);
			this->TabTex->TabIndex = 3;
			this->TabTex->Text = L"����";
			this->TabTex->UseVisualStyleBackColor = true;
			// 
			// m_TexView
			// 
			this->m_TexView->AutoScroll = true;
			this->m_TexView->ContextComponent = this->MapEditorDescrip;
			this->m_TexView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_TexView->Location = System::Drawing::Point(3, 3);
			this->m_TexView->Name = L"m_TexView";
			this->m_TexView->Size = System::Drawing::Size(328, 528);
			this->m_TexView->TabIndex = 0;
			// 
			// TabLight
			// 
			this->TabLight->Controls->Add(this->cLightView1);
			this->TabLight->Location = System::Drawing::Point(4, 40);
			this->TabLight->Name = L"TabLight";
			this->TabLight->Padding = System::Windows::Forms::Padding(3);
			this->TabLight->Size = System::Drawing::Size(334, 534);
			this->TabLight->TabIndex = 4;
			this->TabLight->Text = L"��Ч";
			this->TabLight->UseVisualStyleBackColor = true;
			// 
			// cLightView1
			// 
			this->cLightView1->ContextComponent = this->MapEditorDescrip;
			this->cLightView1->Location = System::Drawing::Point(29, 22);
			this->cLightView1->Name = L"cLightView1";
			this->cLightView1->Size = System::Drawing::Size(278, 364);
			this->cLightView1->TabIndex = 0;
			// 
			// TabEffect
			// 
			this->TabEffect->Controls->Add(this->m_EffectView);
			this->TabEffect->Location = System::Drawing::Point(4, 40);
			this->TabEffect->Name = L"TabEffect";
			this->TabEffect->Padding = System::Windows::Forms::Padding(3);
			this->TabEffect->Size = System::Drawing::Size(334, 534);
			this->TabEffect->TabIndex = 5;
			this->TabEffect->Text = L"��Ч";
			this->TabEffect->UseVisualStyleBackColor = true;
			// 
			// m_EffectView
			// 
			this->m_EffectView->ContextComponent = nullptr;
			this->m_EffectView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_EffectView->Location = System::Drawing::Point(3, 3);
			this->m_EffectView->Name = L"m_EffectView";
			this->m_EffectView->Size = System::Drawing::Size(328, 528);
			this->m_EffectView->TabIndex = 0;
			// 
			// TabBasic
			// 
			this->TabBasic->Controls->Add(this->m_BasicView);
			this->TabBasic->Location = System::Drawing::Point(4, 40);
			this->TabBasic->Name = L"TabBasic";
			this->TabBasic->Padding = System::Windows::Forms::Padding(3);
			this->TabBasic->Size = System::Drawing::Size(334, 534);
			this->TabBasic->TabIndex = 6;
			this->TabBasic->Text = L"����";
			this->TabBasic->UseVisualStyleBackColor = true;
			// 
			// m_BasicView
			// 
			this->m_BasicView->AutoScroll = true;
			this->m_BasicView->ContextComponent = nullptr;
			this->m_BasicView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_BasicView->Location = System::Drawing::Point(3, 3);
			this->m_BasicView->Name = L"m_BasicView";
			this->m_BasicView->Size = System::Drawing::Size(328, 528);
			this->m_BasicView->TabIndex = 0;
			// 
			// TabPacket
			// 
			this->TabPacket->Controls->Add(this->m_TileView);
			this->TabPacket->Location = System::Drawing::Point(4, 40);
			this->TabPacket->Name = L"TabPacket";
			this->TabPacket->Padding = System::Windows::Forms::Padding(3);
			this->TabPacket->Size = System::Drawing::Size(334, 534);
			this->TabPacket->TabIndex = 7;
			this->TabPacket->Text = L"ͼ��";
			this->TabPacket->UseVisualStyleBackColor = true;
			// 
			// m_TileView
			// 
			this->m_TileView->AutoScroll = true;
			this->m_TileView->ContextComponent = this->MapEditorDescrip;
			this->m_TileView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_TileView->Location = System::Drawing::Point(3, 3);
			this->m_TileView->Name = L"m_TileView";
			this->m_TileView->Size = System::Drawing::Size(328, 528);
			this->m_TileView->TabIndex = 0;
			// 
			// TabObject
			// 
			this->TabObject->Controls->Add(this->m_SetView);
			this->TabObject->Location = System::Drawing::Point(4, 40);
			this->TabObject->Name = L"TabObject";
			this->TabObject->Padding = System::Windows::Forms::Padding(3);
			this->TabObject->Size = System::Drawing::Size(334, 534);
			this->TabObject->TabIndex = 8;
			this->TabObject->Text = L"���";
			this->TabObject->UseVisualStyleBackColor = true;
			// 
			// m_SetView
			// 
			this->m_SetView->AutoScroll = true;
			this->m_SetView->ContextComponent = this->MapEditorDescrip;
			this->m_SetView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_SetView->Location = System::Drawing::Point(3, 3);
			this->m_SetView->Name = L"m_SetView";
			this->m_SetView->Size = System::Drawing::Size(328, 528);
			this->m_SetView->TabIndex = 0;
			// 
			// TabRegion
			// 
			this->TabRegion->Location = System::Drawing::Point(4, 40);
			this->TabRegion->Name = L"TabRegion";
			this->TabRegion->Padding = System::Windows::Forms::Padding(3);
			this->TabRegion->Size = System::Drawing::Size(334, 534);
			this->TabRegion->TabIndex = 9;
			this->TabRegion->Text = L"����";
			this->TabRegion->UseVisualStyleBackColor = true;
			// 
			// TabBlock
			// 
			this->TabBlock->Controls->Add(this->m_BlockViewCtrl);
			this->TabBlock->Location = System::Drawing::Point(4, 40);
			this->TabBlock->Name = L"TabBlock";
			this->TabBlock->Padding = System::Windows::Forms::Padding(3);
			this->TabBlock->Size = System::Drawing::Size(334, 534);
			this->TabBlock->TabIndex = 10;
			this->TabBlock->Text = L"�赲";
			this->TabBlock->UseVisualStyleBackColor = true;
			// 
			// m_BlockViewCtrl
			// 
			this->m_BlockViewCtrl->ContextComponent = this->MapEditorDescrip;
			this->m_BlockViewCtrl->Location = System::Drawing::Point(29, 0);
			this->m_BlockViewCtrl->Name = L"m_BlockViewCtrl";
			this->m_BlockViewCtrl->Size = System::Drawing::Size(272, 554);
			this->m_BlockViewCtrl->TabIndex = 0;
			// 
			// TabProp
			// 
			this->TabProp->AutoScroll = true;
			this->TabProp->Controls->Add(this->m_PropShow);
			this->TabProp->Location = System::Drawing::Point(4, 40);
			this->TabProp->Name = L"TabProp";
			this->TabProp->Padding = System::Windows::Forms::Padding(3);
			this->TabProp->Size = System::Drawing::Size(334, 534);
			this->TabProp->TabIndex = 11;
			this->TabProp->Text = L"����";
			this->TabProp->UseVisualStyleBackColor = true;
			// 
			// m_PropShow
			// 
			this->m_PropShow->ContextComponent = this->MapEditorDescrip;
			this->m_PropShow->Dock = System::Windows::Forms::DockStyle::Top;
			this->m_PropShow->Location = System::Drawing::Point(3, 3);
			this->m_PropShow->Name = L"m_PropShow";
			this->m_PropShow->Size = System::Drawing::Size(328, 469);
			this->m_PropShow->TabIndex = 1;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->MenuFile, this->MenuEdit, 
				this->����ToolStripMenuItem, this->���ToolStripMenuItem, this->�赲ToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(841, 25);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// MenuFile
			// 
			this->MenuFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->menuFileNew, 
				this->menuFileOpen, this->menuFileSave, this->menuFileSaveOther, this->menuFileMapEx});
			this->MenuFile->Name = L"MenuFile";
			this->MenuFile->Size = System::Drawing::Size(44, 21);
			this->MenuFile->Text = L"�ļ�";
			// 
			// menuFileNew
			// 
			this->menuFileNew->Name = L"menuFileNew";
			this->menuFileNew->Size = System::Drawing::Size(124, 22);
			this->menuFileNew->Text = L"�½�";
			this->menuFileNew->Click += gcnew System::EventHandler(this, &MainForm::menuFileNew_Click);
			// 
			// menuFileOpen
			// 
			this->menuFileOpen->Name = L"menuFileOpen";
			this->menuFileOpen->Size = System::Drawing::Size(124, 22);
			this->menuFileOpen->Text = L"��";
			this->menuFileOpen->Click += gcnew System::EventHandler(this, &MainForm::menuFileOpen_Click);
			// 
			// menuFileSave
			// 
			this->menuFileSave->Name = L"menuFileSave";
			this->menuFileSave->Size = System::Drawing::Size(124, 22);
			this->menuFileSave->Text = L"����";
			this->menuFileSave->Click += gcnew System::EventHandler(this, &MainForm::menuFileSave_Click);
			// 
			// menuFileSaveOther
			// 
			this->menuFileSaveOther->Name = L"menuFileSaveOther";
			this->menuFileSaveOther->Size = System::Drawing::Size(124, 22);
			this->menuFileSaveOther->Text = L"���Ϊ";
			this->menuFileSaveOther->Click += gcnew System::EventHandler(this, &MainForm::menuFileSaveOther_Click);
			// 
			// menuFileMapEx
			// 
			this->menuFileMapEx->Name = L"menuFileMapEx";
			this->menuFileMapEx->Size = System::Drawing::Size(124, 22);
			this->menuFileMapEx->Text = L"��ͼ��չ";
			this->menuFileMapEx->Click += gcnew System::EventHandler(this, &MainForm::menuFileMapEx_Click);
			// 
			// MenuEdit
			// 
			this->MenuEdit->Name = L"MenuEdit";
			this->MenuEdit->Size = System::Drawing::Size(44, 21);
			this->MenuEdit->Text = L"�༭";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->��1��ȫ��CtrlFToolStripMenuItem, 
				this->���ص�1��F1ToolStripMenuItem, this->���ص�2��F2ToolStripMenuItem, this->ɾ������DeleteToolStripMenuItem, this->���õر������ЧToolStripMenuItem, 
				this->������ͼƬCtrl1ToolStripMenuItem});
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(44, 21);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// ��1��ȫ��CtrlFToolStripMenuItem
			// 
			this->��1��ȫ��CtrlFToolStripMenuItem->Name = L"��1��ȫ��CtrlFToolStripMenuItem";
			this->��1��ȫ��CtrlFToolStripMenuItem->Size = System::Drawing::Size(192, 22);
			this->��1��ȫ��CtrlFToolStripMenuItem->Text = L"��1��ȫ��(&Ctrl+F)";
			// 
			// ���ص�1��F1ToolStripMenuItem
			// 
			this->���ص�1��F1ToolStripMenuItem->Name = L"���ص�1��F1ToolStripMenuItem";
			this->���ص�1��F1ToolStripMenuItem->Size = System::Drawing::Size(192, 22);
			this->���ص�1��F1ToolStripMenuItem->Text = L"���ص�1��(&F1)";
			// 
			// ���ص�2��F2ToolStripMenuItem
			// 
			this->���ص�2��F2ToolStripMenuItem->Name = L"���ص�2��F2ToolStripMenuItem";
			this->���ص�2��F2ToolStripMenuItem->Size = System::Drawing::Size(192, 22);
			this->���ص�2��F2ToolStripMenuItem->Text = L"���ص�2��(&F2)";
			// 
			// ɾ������DeleteToolStripMenuItem
			// 
			this->ɾ������DeleteToolStripMenuItem->Name = L"ɾ������DeleteToolStripMenuItem";
			this->ɾ������DeleteToolStripMenuItem->Size = System::Drawing::Size(192, 22);
			this->ɾ������DeleteToolStripMenuItem->Text = L"ɾ������(&Delete)";
			// 
			// ���õر������ЧToolStripMenuItem
			// 
			this->���õر������ЧToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->ǿ��F5ToolStripMenuItem, 
				this->ǿ�в�����F6ToolStripMenuItem});
			this->���õر������ЧToolStripMenuItem->Name = L"���õر������ЧToolStripMenuItem";
			this->���õر������ЧToolStripMenuItem->Size = System::Drawing::Size(192, 22);
			this->���õر������ЧToolStripMenuItem->Text = L"�ر������Ч";
			// 
			// ǿ��F5ToolStripMenuItem
			// 
			this->ǿ��F5ToolStripMenuItem->Name = L"ǿ��F5ToolStripMenuItem";
			this->ǿ��F5ToolStripMenuItem->Size = System::Drawing::Size(157, 22);
			this->ǿ��F5ToolStripMenuItem->Text = L"ǿ������(&F5)";
			// 
			// ǿ�в�����F6ToolStripMenuItem
			// 
			this->ǿ�в�����F6ToolStripMenuItem->Name = L"ǿ�в�����F6ToolStripMenuItem";
			this->ǿ�в�����F6ToolStripMenuItem->Size = System::Drawing::Size(157, 22);
			this->ǿ�в�����F6ToolStripMenuItem->Text = L"ǿ�в�����(&F6)";
			// 
			// ������ͼƬCtrl1ToolStripMenuItem
			// 
			this->������ͼƬCtrl1ToolStripMenuItem->Name = L"������ͼƬCtrl1ToolStripMenuItem";
			this->������ͼƬCtrl1ToolStripMenuItem->Size = System::Drawing::Size(192, 22);
			this->������ͼƬCtrl1ToolStripMenuItem->Text = L"������ͼƬ(&Ctrl+1)";
			// 
			// ���ToolStripMenuItem
			// 
			this->���ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(13) {this->�ƶ�ToolStripMenuItem, 
				this->����ToolStripMenuItem, this->��תToolStripMenuItem, this->������ӰToolStripMenuItem, this->������ӰToolStripMenuItem, this->��һ����Ⱦ״̬F9ToolStripMenuItem, 
				this->����MToolStripMenuItem, this->ɾ��DeleteToolStripMenuItem, this->����CtrlCToolStripMenuItem, this->ճ��CtrlVToolStripMenuItem, 
				this->��ѡCtrlRToolStripMenuItem, this->ѡ��������ͬ����CtrlAToolStripMenuItem, this->����ʹ���������ײCtrlF6ToolStripMenuItem});
			this->���ToolStripMenuItem->Name = L"���ToolStripMenuItem";
			this->���ToolStripMenuItem->Size = System::Drawing::Size(44, 21);
			this->���ToolStripMenuItem->Text = L"���";
			// 
			// �ƶ�ToolStripMenuItem
			// 
			this->�ƶ�ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->����LeftToolStripMenuItem, 
				this->����RightToolStripMenuItem, this->����ToolStripMenuItem, this->����ToolStripMenuItem, this->����PageUpToolStripMenuItem, this->����PageDownToolStripMenuItem, 
				this->��������HomeToolStripMenuItem, this->��������EndToolStripMenuItem});
			this->�ƶ�ToolStripMenuItem->Name = L"�ƶ�ToolStripMenuItem";
			this->�ƶ�ToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->�ƶ�ToolStripMenuItem->Text = L"�ƶ�";
			// 
			// ����LeftToolStripMenuItem
			// 
			this->����LeftToolStripMenuItem->Name = L"����LeftToolStripMenuItem";
			this->����LeftToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->����LeftToolStripMenuItem->Text = L"����(&Left)";
			// 
			// ����RightToolStripMenuItem
			// 
			this->����RightToolStripMenuItem->Name = L"����RightToolStripMenuItem";
			this->����RightToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->����RightToolStripMenuItem->Text = L"����(&Right)";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->����ToolStripMenuItem->Text = L"����(&��)";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->����ToolStripMenuItem->Text = L"����(&��)";
			// 
			// ����PageUpToolStripMenuItem
			// 
			this->����PageUpToolStripMenuItem->Name = L"����PageUpToolStripMenuItem";
			this->����PageUpToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->����PageUpToolStripMenuItem->Text = L"����(&PageUp)";
			// 
			// ����PageDownToolStripMenuItem
			// 
			this->����PageDownToolStripMenuItem->Name = L"����PageDownToolStripMenuItem";
			this->����PageDownToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->����PageDownToolStripMenuItem->Text = L"����(&PageDown)";
			// 
			// ��������HomeToolStripMenuItem
			// 
			this->��������HomeToolStripMenuItem->Name = L"��������HomeToolStripMenuItem";
			this->��������HomeToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->��������HomeToolStripMenuItem->Text = L"��������(&Home)";
			// 
			// ��������EndToolStripMenuItem
			// 
			this->��������EndToolStripMenuItem->Name = L"��������EndToolStripMenuItem";
			this->��������EndToolStripMenuItem->Size = System::Drawing::Size(170, 22);
			this->��������EndToolStripMenuItem->Text = L"��������(&End)";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->��ԭ6ToolStripMenuItem, 
				this->�Ŵ�7ToolStripMenuItem, this->��С8ToolStripMenuItem});
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// ��ԭ6ToolStripMenuItem
			// 
			this->��ԭ6ToolStripMenuItem->Name = L"��ԭ6ToolStripMenuItem";
			this->��ԭ6ToolStripMenuItem->Size = System::Drawing::Size(115, 22);
			this->��ԭ6ToolStripMenuItem->Text = L"��ԭ(&6)";
			// 
			// �Ŵ�7ToolStripMenuItem
			// 
			this->�Ŵ�7ToolStripMenuItem->Name = L"�Ŵ�7ToolStripMenuItem";
			this->�Ŵ�7ToolStripMenuItem->Size = System::Drawing::Size(115, 22);
			this->�Ŵ�7ToolStripMenuItem->Text = L"�Ŵ�(&7)";
			// 
			// ��С8ToolStripMenuItem
			// 
			this->��С8ToolStripMenuItem->Name = L"��С8ToolStripMenuItem";
			this->��С8ToolStripMenuItem->Size = System::Drawing::Size(115, 22);
			this->��С8ToolStripMenuItem->Text = L"��С(&8)";
			// 
			// ��תToolStripMenuItem
			// 
			this->��תToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->x������RToolStripMenuItem, 
				this->x�Ḻ��FToolStripMenuItem, this->y������IToolStripMenuItem, this->y�Ḻ��KToolStripMenuItem, this->z������YToolStripMenuItem, this->z�Ḻ��HToolStripMenuItem, 
				this->��ת90��TToolStripMenuItem});
			this->��תToolStripMenuItem->Name = L"��תToolStripMenuItem";
			this->��תToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->��תToolStripMenuItem->Text = L"��ת";
			// 
			// x������RToolStripMenuItem
			// 
			this->x������RToolStripMenuItem->Name = L"x������RToolStripMenuItem";
			this->x������RToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->x������RToolStripMenuItem->Text = L"X������(&R)";
			// 
			// x�Ḻ��FToolStripMenuItem
			// 
			this->x�Ḻ��FToolStripMenuItem->Name = L"x�Ḻ��FToolStripMenuItem";
			this->x�Ḻ��FToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->x�Ḻ��FToolStripMenuItem->Text = L"X�Ḻ��(&F)";
			// 
			// y������IToolStripMenuItem
			// 
			this->y������IToolStripMenuItem->Name = L"y������IToolStripMenuItem";
			this->y������IToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->y������IToolStripMenuItem->Text = L"Y������(&I)";
			// 
			// y�Ḻ��KToolStripMenuItem
			// 
			this->y�Ḻ��KToolStripMenuItem->Name = L"y�Ḻ��KToolStripMenuItem";
			this->y�Ḻ��KToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->y�Ḻ��KToolStripMenuItem->Text = L"Y�Ḻ��(&K)";
			// 
			// z������YToolStripMenuItem
			// 
			this->z������YToolStripMenuItem->Name = L"z������YToolStripMenuItem";
			this->z������YToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->z������YToolStripMenuItem->Text = L"Z������(&Y)";
			// 
			// z�Ḻ��HToolStripMenuItem
			// 
			this->z�Ḻ��HToolStripMenuItem->Name = L"z�Ḻ��HToolStripMenuItem";
			this->z�Ḻ��HToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->z�Ḻ��HToolStripMenuItem->Text = L"Z�Ḻ��(&H)";
			// 
			// ��ת90��TToolStripMenuItem
			// 
			this->��ת90��TToolStripMenuItem->Name = L"��ת90��TToolStripMenuItem";
			this->��ת90��TToolStripMenuItem->Size = System::Drawing::Size(141, 22);
			this->��ת90��TToolStripMenuItem->Text = L"��ת90��(&T)";
			// 
			// ������ӰToolStripMenuItem
			// 
			this->������ӰToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->�л���ӰF6ToolStripMenuItem, 
				this->�ص���ѡ�����ӰToolStripMenuItem, this->������ѡ�����ӰToolStripMenuItem});
			this->������ӰToolStripMenuItem->Name = L"������ӰToolStripMenuItem";
			this->������ӰToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->������ӰToolStripMenuItem->Text = L"������Ӱ";
			// 
			// �л���ӰF6ToolStripMenuItem
			// 
			this->�л���ӰF6ToolStripMenuItem->Name = L"�л���ӰF6ToolStripMenuItem";
			this->�л���ӰF6ToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->�л���ӰF6ToolStripMenuItem->Text = L"�л���Ӱ(&F6)";
			// 
			// �ص���ѡ�����ӰToolStripMenuItem
			// 
			this->�ص���ѡ�����ӰToolStripMenuItem->Name = L"�ص���ѡ�����ӰToolStripMenuItem";
			this->�ص���ѡ�����ӰToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->�ص���ѡ�����ӰToolStripMenuItem->Text = L"�ص���ѡ�����Ӱ";
			// 
			// ������ѡ�����ӰToolStripMenuItem
			// 
			this->������ѡ�����ӰToolStripMenuItem->Name = L"������ѡ�����ӰToolStripMenuItem";
			this->������ѡ�����ӰToolStripMenuItem->Size = System::Drawing::Size(172, 22);
			this->������ѡ�����ӰToolStripMenuItem->Text = L"������ѡ�����Ӱ";
			// 
			// ������ӰToolStripMenuItem
			// 
			this->������ӰToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->�л�����ToolStripMenuItem, 
				this->ȫ��������ӰToolStripMenuItem, this->ȫ����������ӰToolStripMenuItem});
			this->������ӰToolStripMenuItem->Name = L"������ӰToolStripMenuItem";
			this->������ӰToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->������ӰToolStripMenuItem->Text = L"������Ӱ";
			// 
			// �л�����ToolStripMenuItem
			// 
			this->�л�����ToolStripMenuItem->Name = L"�л�����ToolStripMenuItem";
			this->�л�����ToolStripMenuItem->Size = System::Drawing::Size(193, 22);
			this->�л�����ToolStripMenuItem->Text = L"�л�������Ӱ״̬(&F5)";
			// 
			// ȫ��������ӰToolStripMenuItem
			// 
			this->ȫ��������ӰToolStripMenuItem->Name = L"ȫ��������ӰToolStripMenuItem";
			this->ȫ��������ӰToolStripMenuItem->Size = System::Drawing::Size(193, 22);
			this->ȫ��������ӰToolStripMenuItem->Text = L"ȫ��������Ӱ";
			// 
			// ȫ����������ӰToolStripMenuItem
			// 
			this->ȫ����������ӰToolStripMenuItem->Name = L"ȫ����������ӰToolStripMenuItem";
			this->ȫ����������ӰToolStripMenuItem->Size = System::Drawing::Size(193, 22);
			this->ȫ����������ӰToolStripMenuItem->Text = L"ȫ����������Ӱ";
			// 
			// ��һ����Ⱦ״̬F9ToolStripMenuItem
			// 
			this->��һ����Ⱦ״̬F9ToolStripMenuItem->Name = L"��һ����Ⱦ״̬F9ToolStripMenuItem";
			this->��һ����Ⱦ״̬F9ToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->��һ����Ⱦ״̬F9ToolStripMenuItem->Text = L"��һ����Ⱦ״̬(&F9)";
			// 
			// ����MToolStripMenuItem
			// 
			this->����MToolStripMenuItem->Name = L"����MToolStripMenuItem";
			this->����MToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->����MToolStripMenuItem->Text = L"����(M)";
			// 
			// ɾ��DeleteToolStripMenuItem
			// 
			this->ɾ��DeleteToolStripMenuItem->Name = L"ɾ��DeleteToolStripMenuItem";
			this->ɾ��DeleteToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->ɾ��DeleteToolStripMenuItem->Text = L"ɾ��(&Delete)";
			// 
			// ����CtrlCToolStripMenuItem
			// 
			this->����CtrlCToolStripMenuItem->Name = L"����CtrlCToolStripMenuItem";
			this->����CtrlCToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->����CtrlCToolStripMenuItem->Text = L"����(&Ctrl+C)";
			// 
			// ճ��CtrlVToolStripMenuItem
			// 
			this->ճ��CtrlVToolStripMenuItem->Name = L"ճ��CtrlVToolStripMenuItem";
			this->ճ��CtrlVToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->ճ��CtrlVToolStripMenuItem->Text = L"ճ��(&Ctrl+V)";
			// 
			// ��ѡCtrlRToolStripMenuItem
			// 
			this->��ѡCtrlRToolStripMenuItem->Name = L"��ѡCtrlRToolStripMenuItem";
			this->��ѡCtrlRToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->��ѡCtrlRToolStripMenuItem->Text = L"��ѡ(&Ctrl+R)";
			// 
			// ѡ��������ͬ����CtrlAToolStripMenuItem
			// 
			this->ѡ��������ͬ����CtrlAToolStripMenuItem->Name = L"ѡ��������ͬ����CtrlAToolStripMenuItem";
			this->ѡ��������ͬ����CtrlAToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->ѡ��������ͬ����CtrlAToolStripMenuItem->Text = L"ѡ��������ͬ����(&Ctrl+A)";
			// 
			// ����ʹ���������ײCtrlF6ToolStripMenuItem
			// 
			this->����ʹ���������ײCtrlF6ToolStripMenuItem->Name = L"����ʹ���������ײCtrlF6ToolStripMenuItem";
			this->����ʹ���������ײCtrlF6ToolStripMenuItem->Size = System::Drawing::Size(234, 22);
			this->����ʹ���������ײCtrlF6ToolStripMenuItem->Text = L"����ʹ���������ײ(&Ctrl+F6)";
			// 
			// �赲ToolStripMenuItem
			// 
			this->�赲ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->��������赲ToolStripMenuItem});
			this->�赲ToolStripMenuItem->Name = L"�赲ToolStripMenuItem";
			this->�赲ToolStripMenuItem->Size = System::Drawing::Size(44, 21);
			this->�赲ToolStripMenuItem->Text = L"�赲";
			// 
			// ��������赲ToolStripMenuItem
			// 
			this->��������赲ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->����ToolStripMenuItem, 
				this->����ToolStripMenuItem, this->����ToolStripMenuItem});
			this->��������赲ToolStripMenuItem->Name = L"��������赲ToolStripMenuItem";
			this->��������赲ToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->��������赲ToolStripMenuItem->Text = L"��������赲";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// RTSCameraCom
			// 
			this->RTSCameraCom->ContextComponent = this->MapEditorDescrip;
			// 
			// m_FormTimer
			// 
			this->m_FormTimer->Enabled = true;
			this->m_FormTimer->Tick += gcnew System::EventHandler(this, &MainForm::m_FormTimer_Tick);
			// 
			// FREECameraCom
			// 
			this->FREECameraCom->ContextComponent = this->MapEditorDescrip;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(841, 603);
			this->Controls->Add(this->FrameContainer);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::MainForm_FormClosed);
			this->FrameContainer->ContentPanel->ResumeLayout(false);
			this->FrameContainer->TopToolStripPanel->ResumeLayout(false);
			this->FrameContainer->TopToolStripPanel->PerformLayout();
			this->FrameContainer->ResumeLayout(false);
			this->FrameContainer->PerformLayout();
			this->TabPanel->ResumeLayout(false);
			this->TabGrid->ResumeLayout(false);
			this->TabRecord->ResumeLayout(false);
			this->TabAduio->ResumeLayout(false);
			this->TabTex->ResumeLayout(false);
			this->TabLight->ResumeLayout(false);
			this->TabEffect->ResumeLayout(false);
			this->TabBasic->ResumeLayout(false);
			this->TabPacket->ResumeLayout(false);
			this->TabObject->ResumeLayout(false);
			this->TabBlock->ResumeLayout(false);
			this->TabProp->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void InitExp();
	//��ʱ��������
	private: System::Void m_FormTimer_Tick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void UpdateTimer(System::Object^ sender,System::Timers::ElapsedEventArgs^ e);
	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MainForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) ;
	private: System::Void TabPanel_Selected(System::Object^  sender, System::Windows::Forms::TabControlEventArgs^  e);
	private: System::Void menuFileNew_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void menuFileOpen_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void menuFileSave_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void menuFileSaveOther_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void menuFileMapEx_Click(System::Object^  sender, System::EventArgs^  e);
	
};
}
