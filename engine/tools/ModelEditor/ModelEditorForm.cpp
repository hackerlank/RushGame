#include "StdAfx.h"
#include "ModelEditorForm.h"
#include "IApplication.h"
#include "CtrlImpHelper.h"
#include "CCoreViewImp.h"
#include "CEditContext.h"
#include "TSqrAllocator.inl"
#include "ContextCtrl\CContextCtrl.h"

namespace ModelEditor
{
	System::Void ModelEditorForm::ModelEditorForm_FormClosed( System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e )
	{
		IApplication::GetInst()->Close();
	}

	System::Void ModelEditorForm::ModelEditorForm_Load( System::Object^ sender, System::EventArgs^ e )
	{
	} 

	System::Void ModelEditorForm::��ģ��ToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
	{
		this->cPieceGroupEditCtrl->AddMesh();
	}

	System::Void ModelEditorForm::����ģ��ToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
	{
		this->cPieceGroupEditCtrl->SaveMesh();
	}

	System::Void ModelEditorForm::����ģ��ΪToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
	{
		this->cPieceGroupEditCtrl->SaveMeshAs();
	}

	System::Void ModelEditorForm::����ȫ��ToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
	{
		this->cPieceGroupEditCtrl->SaveAllMesh();
	}

	System::Void ModelEditorForm::�ر�����ģ���ļ�ToolStripMenuItem_Click( System::Object^ sender, System::EventArgs^ e )
	{
		this->cPieceGroupEditCtrl->ClearAllMesh();
	}

	System::Void ModelEditorForm::����·��ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		CPathSetDlg^ pathDlg = gcnew CPathSetDlg;
		pathDlg->ShowDialog();
	}
}
