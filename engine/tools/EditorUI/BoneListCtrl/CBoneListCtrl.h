#pragma once

#include "CUIBase.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace sqr_tools {

	/// <summary>
	/// Summary for CBoneListCtrl
	/// </summary>
	public ref class CBoneListCtrl : public CUIBase
	{
	public:
		CBoneListCtrl(void)
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
		~CBoneListCtrl()
		{
			if (components)
			{
				delete components;
			}
		}

	public:
		virtual char*	GetImpTypeName(void)	override;
		virtual void*	GetImpParam(void)		override;
		virtual void	Update(void) override;
		virtual void    InsertNode(TreeNode^ parent, String^ name);

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TreeView^  BoneListTreeView;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  ���˻������ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ӽڵ�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ӵ�ǰ�ڵ�ToolStripMenuItem;
	private: System::ComponentModel::IContainer^  components;


	public: 

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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->BoneListTreeView = (gcnew System::Windows::Forms::TreeView());
			this->contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->��ӽڵ�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ӵ�ǰ�ڵ�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���˻������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBox1->SuspendLayout();
			this->contextMenuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->BoneListTreeView);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(235, 259);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"�����б�";
			// 
			// BoneListTreeView
			// 
			this->BoneListTreeView->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->BoneListTreeView->ContextMenuStrip = this->contextMenuStrip;
			this->BoneListTreeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BoneListTreeView->Location = System::Drawing::Point(3, 17);
			this->BoneListTreeView->Name = L"BoneListTreeView";
			this->BoneListTreeView->Size = System::Drawing::Size(229, 239);
			this->BoneListTreeView->TabIndex = 0;
			this->BoneListTreeView->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &CBoneListCtrl::BoneListTreeView_MouseDown);
			// 
			// contextMenuStrip
			// 
			this->contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->��ӽڵ�ToolStripMenuItem, 
				this->��ӵ�ǰ�ڵ�ToolStripMenuItem, this->���˻������ToolStripMenuItem});
			this->contextMenuStrip->Name = L"contextMenuStrip";
			this->contextMenuStrip->Size = System::Drawing::Size(191, 70);
			// 
			// ��ӽڵ�ToolStripMenuItem
			// 
			this->��ӽڵ�ToolStripMenuItem->Name = L"��ӽڵ�ToolStripMenuItem";
			this->��ӽڵ�ToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->��ӽڵ�ToolStripMenuItem->Text = L"��ӽڵ�";
			this->��ӽڵ�ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CBoneListCtrl::��ӽڵ�ToolStripMenuItem_Click);
			// 
			// ��ӵ�ǰ�ڵ�ToolStripMenuItem
			// 
			this->��ӵ�ǰ�ڵ�ToolStripMenuItem->Name = L"��ӵ�ǰ�ڵ�ToolStripMenuItem";
			this->��ӵ�ǰ�ڵ�ToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->��ӵ�ǰ�ڵ�ToolStripMenuItem->Text = L"��ӽڵ�(�����ӽڵ�)";
			this->��ӵ�ǰ�ڵ�ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CBoneListCtrl::��ӵ�ǰ�ڵ�ToolStripMenuItem_Click);
			// 
			// ���˻������ToolStripMenuItem
			// 
			this->���˻������ToolStripMenuItem->Name = L"���˻������ToolStripMenuItem";
			this->���˻������ToolStripMenuItem->Size = System::Drawing::Size(190, 22);
			this->���˻������ToolStripMenuItem->Text = L"���˻������";
			this->���˻������ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CBoneListCtrl::���˻������ToolStripMenuItem_Click);
			// 
			// CBoneListCtrl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->groupBox1);
			this->Name = L"CBoneListCtrl";
			this->Size = System::Drawing::Size(235, 259);
			this->groupBox1->ResumeLayout(false);
			this->contextMenuStrip->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void ���˻������ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ��ӽڵ�ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ��ӵ�ǰ�ڵ�ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void BoneListTreeView_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void TravelNodes(TreeNode^ node);
};
}
