#pragma once

#include "CUIBase.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Text;
using namespace System::Runtime::InteropServices;


namespace sqr_tools {

	/// <summary>
	/// Summary for CAnimationGroupEditCtrl
	/// </summary>
	public ref class CAnimationGroupEditCtrl : public CUIBase
	{
	public:
		CAnimationGroupEditCtrl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CAnimationGroupEditCtrl()
		{
			if (components)
			{
				delete components;
			}
		}
	public:
		virtual char*	GetImpTypeName(void)	override;
		virtual void*	GetImpParam(void)		override;
		virtual void	Update(void)			override;

	private: System::Windows::Forms::TreeView^  AnimationGroupList;
	protected: 

	private: System::Windows::Forms::GroupBox^  groupBox1;


























	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ���agpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����agpToolStripMenuItem;




	private: System::ComponentModel::IContainer^  components;


	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->AnimationGroupList = (gcnew System::Windows::Forms::TreeView());
			this->contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���agpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����agpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->contextMenuStrip->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// AnimationGroupList
			// 
			this->AnimationGroupList->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->AnimationGroupList->ContextMenuStrip = this->contextMenuStrip;
			this->AnimationGroupList->Dock = System::Windows::Forms::DockStyle::Fill;
			this->AnimationGroupList->Location = System::Drawing::Point(3, 17);
			this->AnimationGroupList->Name = L"AnimationGroupList";
			this->AnimationGroupList->Size = System::Drawing::Size(357, 223);
			this->AnimationGroupList->TabIndex = 0;
			this->AnimationGroupList->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CAnimationGroupEditCtrl::AnimationGroupList_MouseClick);
			this->AnimationGroupList->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &CAnimationGroupEditCtrl::AnimationGroupList_AfterSelect);
			this->AnimationGroupList->Click += gcnew System::EventHandler(this, &CAnimationGroupEditCtrl::AnimationGroupList_Click);
			// 
			// contextMenuStrip
			// 
			this->contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->����ToolStripMenuItem, 
				this->���agpToolStripMenuItem, this->����agpToolStripMenuItem});
			this->contextMenuStrip->Name = L"contextMenuStrip";
			this->contextMenuStrip->Size = System::Drawing::Size(117, 70);
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->����ToolStripMenuItem->Text = L"����agp";
			this->����ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CAnimationGroupEditCtrl::����ToolStripMenuItem_Click);
			// 
			// ���agpToolStripMenuItem
			// 
			this->���agpToolStripMenuItem->Name = L"���agpToolStripMenuItem";
			this->���agpToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->���agpToolStripMenuItem->Text = L"���agp";
			this->���agpToolStripMenuItem->Click += gcnew System::EventHandler(this, &CAnimationGroupEditCtrl::���agpToolStripMenuItem_Click);
			// 
			// ����agpToolStripMenuItem
			// 
			this->����agpToolStripMenuItem->Name = L"����agpToolStripMenuItem";
			this->����agpToolStripMenuItem->Size = System::Drawing::Size(116, 22);
			this->����agpToolStripMenuItem->Text = L"����agp";
			this->����agpToolStripMenuItem->Click += gcnew System::EventHandler(this, &CAnimationGroupEditCtrl::����agpToolStripMenuItem_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->AnimationGroupList);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Margin = System::Windows::Forms::Padding(0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(363, 243);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"�����б�";
			// 
			// CAnimationGroupEditCtrl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->groupBox1);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"CAnimationGroupEditCtrl";
			this->Size = System::Drawing::Size(363, 243);
			this->contextMenuStrip->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void AnimationGroupList_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AnimationGroupList_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);
	private: System::Void ����ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ���agpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ����agpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void AnimationGroupList_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Int32 m_iLastValue; 
};
}
