#include "stdafx.h"
#include "MainForm.h"
#include "CMapEditApp.h"
#include "IApplication.h"

using namespace System;
using namespace System::Windows::Forms;

using namespace sqr;

//�ر� ϣ�� �����������ȡ��������޶ȵķ��� 

// 1 UI����  (MapEditorForm)    ���������� Ҳ�ܹ�������ʾ ֻ�����޷�ʵ���κι���
// 2 ���沿��(MapEditorManage)  �����ʼ�� �ر໷����ʼ�� �����ഴ���ͳ�ʼ��  
// 3 UI�͹�����󶨲���
// 4 ��Ϣ����� (CEditToolApp)

#include "CTerrainMgr.h"
#include "PathManager/CPathManager.h"

[STAThreadAttribute]
int WINAPI WinMain(HINSTANCE hinst,HINSTANCE hpre, LPSTR lpCmd, int nShowCmd)
{
	CTerrainMgr::st_isEditor = true;

	IApplication::InitApp();
	CUIBase::Initialize(IApplication::GetInst()->GetCtrlImpFactory());
	//����������
	MapEditor::MainForm^ MapEditForm = gcnew MapEditor::MainForm();

	MapEditForm->Show();

	while(!CPathSetImp::GetInst()->HasSetConfigPath())
	{
		CPathSetDlg m_PathSetDlg;
		m_PathSetDlg.ShowDialog();
	}
	//��ʼ������
	if(!CMapEditApp::HasInst())
		CMapEditApp::Create();

	IApplication::GetInst()->Run();
	
	if(CMapEditApp::HasInst())
		CMapEditApp::Destroy();

	return 0;
}


