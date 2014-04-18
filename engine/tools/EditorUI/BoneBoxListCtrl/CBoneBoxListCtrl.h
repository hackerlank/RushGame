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
	/// Summary for CBoneBoxListCtrl
	/// </summary>
	public ref class CBoneBoxListCtrl : public CUIBase
	{
	public:
		CBoneBoxListCtrl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		virtual char*	GetImpTypeName(void)	override;
		virtual void*	GetImpParam(void)		override;
		virtual void	Update(void) override;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CBoneBoxListCtrl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBox1;

	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  ɾ��ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ���ӵ�ѡ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����Ĭ�Ϲ���ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ͷ�ǰ�Χ��ToolStripMenuItem;
	private: System::Windows::Forms::ListView^  BoneBoxTreeView;
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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->BoneBoxTreeView = (gcnew System::Windows::Forms::ListView());
			this->contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->ɾ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���ӵ�ѡ����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����Ĭ�Ϲ���ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ͷ�ǰ�Χ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBox1->SuspendLayout();
			this->contextMenuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->BoneBoxTreeView);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(235, 259);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"���а�Χ�еĹ���";
			// 
			// BoneBoxTreeView
			// 
			this->BoneBoxTreeView->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->BoneBoxTreeView->ContextMenuStrip = this->contextMenuStrip;
			this->BoneBoxTreeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->BoneBoxTreeView->Location = System::Drawing::Point(3, 17);
			this->BoneBoxTreeView->Name = L"BoneBoxTreeView";
			this->BoneBoxTreeView->Size = System::Drawing::Size(229, 239);
			this->BoneBoxTreeView->TabIndex = 0;
			this->BoneBoxTreeView->UseCompatibleStateImageBehavior = false;
			this->BoneBoxTreeView->View = System::Windows::Forms::View::List;
			this->BoneBoxTreeView->SelectedIndexChanged += gcnew System::EventHandler(this, &CBoneBoxListCtrl::BoneBoxTreeView_SelectedIndexChanged);
			// 
			// contextMenuStrip
			// 
			this->contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->ɾ��ToolStripMenuItem, 
				this->���ӵ�ѡ����ToolStripMenuItem, this->����Ĭ�Ϲ���ToolStripMenuItem, this->ͷ�ǰ�Χ��ToolStripMenuItem});
			this->contextMenuStrip->Name = L"contextMenuStrip";
			this->contextMenuStrip->Size = System::Drawing::Size(147, 92);
			// 
			// ɾ��ToolStripMenuItem
			// 
			this->ɾ��ToolStripMenuItem->Name = L"ɾ��ToolStripMenuItem";
			this->ɾ��ToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->ɾ��ToolStripMenuItem->Text = L"ɾ��";
			this->ɾ��ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CBoneBoxListCtrl::ɾ��ToolStripMenuItem_Click);
			// 
			// ���ӵ�ѡ����ToolStripMenuItem
			// 
			this->���ӵ�ѡ����ToolStripMenuItem->Name = L"���ӵ�ѡ����ToolStripMenuItem";
			this->���ӵ�ѡ����ToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->���ӵ�ѡ����ToolStripMenuItem->Text = L"���ӵ�ѡ����";
			this->���ӵ�ѡ����ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CBoneBoxListCtrl::���ӵ�ѡ����ToolStripMenuItem_Click);
			// 
			// ����Ĭ�Ϲ���ToolStripMenuItem
			// 
			this->����Ĭ�Ϲ���ToolStripMenuItem->Name = L"����Ĭ�Ϲ���ToolStripMenuItem";
			this->����Ĭ�Ϲ���ToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->����Ĭ�Ϲ���ToolStripMenuItem->Text = L"����Ĭ�Ϲ���";
			this->����Ĭ�Ϲ���ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CBoneBoxListCtrl::����Ĭ�Ϲ���ToolStripMenuItem_Click);
			// 
			// ͷ�ǰ�Χ��ToolStripMenuItem
			// 
			this->ͷ�ǰ�Χ��ToolStripMenuItem->Name = L"ͷ�ǰ�Χ��ToolStripMenuItem";
			this->ͷ�ǰ�Χ��ToolStripMenuItem->Size = System::Drawing::Size(146, 22);
			this->ͷ�ǰ�Χ��ToolStripMenuItem->Text = L"ͷ�ǰ�Χ��";
			this->ͷ�ǰ�Χ��ToolStripMenuItem->Click += gcnew System::EventHandler(this, &CBoneBoxListCtrl::ͷ�ǰ�Χ��ToolStripMenuItem_Click);
			// 
			// CBoneBoxListCtrl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->groupBox1);
			this->Name = L"CBoneBoxListCtrl";
			this->Size = System::Drawing::Size(235, 259);
			this->groupBox1->ResumeLayout(false);
			this->contextMenuStrip->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: ListViewItem^ FindNode(String^ value);
	private: System::Void BoneBoxTreeView_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ɾ��ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ���ӵ�ѡ����ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ����Ĭ�Ϲ���ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void ͷ�ǰ�Χ��ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
};
}
