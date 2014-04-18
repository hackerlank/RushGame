#include "StdAfx.h"
#include "CSkeletonPropCom.h"
#include "SkeletonEditCtrl/CSkeletonEditImp.h"
#include "CtrlImpHelper.h"
#include "CUIBase.h"
#include "dthelp.h"

#define SkeletonImp ((CSkeletonEditImp*)GetCoreCtrl())

sqr_tools::CSkeletonPropCom::CSkeletonPropCom()
{
}

sqr_tools::CSkeletonPropCom::~CSkeletonPropCom()
{

}

char* CSkeletonPropCom::GetImpTypeName(void)
{
	return GET_IMP_NAME(CSkeletonEditImp);
}

bool CSkeletonPropCom::��ʾ����::get()
{
	return m_bShowSkeleton;
}

void CSkeletonPropCom::��ʾ����::set(bool value)
{
	m_bShowSkeleton = value;
	if (SkeletonImp)
		SkeletonImp->SetShowSkeleton(value);
}

String^ CSkeletonPropCom::��ǰ�����ļ�::get()
{
	return m_strSkeletonName;
}

void CSkeletonPropCom::��ǰ�����ļ�::set(String^  value)
{
	if (!value)
		return;

	m_strSkeletonName = value;
}

String^ CSkeletonPropCom::��ǰѡ�й���::get()
{
	return m_strSeletedSkeletonName;
}

void CSkeletonPropCom::��ǰѡ�й���::set(String^  value)
{
	if (!value)
		return;

	m_strSeletedSkeletonName = value;
	std::string strCLR = MagStringToUnmagString(value);
	if (SkeletonImp)
		SkeletonImp->ChangeCurSelSkeBoneName(strCLR);
}

String^ CSkeletonPropCom::��������ļ�::get()
{
	return m_strSkeletonFileInfo;
}

void CSkeletonPropCom::��������ļ�::set(String^  value)
{
	if (!value)
		return;

	m_strSkeletonFileInfo = "�뱣��";
	if (SkeletonImp)
	{
		String^ msg = gcnew String(SkeletonImp->SaveSke().c_str());
		MessageBox::Show(msg, "��ʾ", MessageBoxButtons::OK);
	}
}

int CSkeletonPropCom::����������С::get()
{
	return m_iSkeletonSize;
}

void CSkeletonPropCom::����������С::set(int  value)
{
	if (!value)
		return;
	
	m_iSkeletonSize = value;
	int delta = m_iSkeletonSize - m_iLastSkeletonSize;
	if (SkeletonImp)
		SkeletonImp->SetSkeBoneSize(delta>0?1:-1);
	m_iLastSkeletonSize = value;
}

System::Windows::Forms::ContainerControl^ CSkeletonPropCom::ControlParent::get()
{
	return m_pUserCtrl;
}

void CSkeletonPropCom::ControlParent::set(System::Windows::Forms::ContainerControl^ value)
{
	m_pUserCtrl = value;
}

System::Void CSkeletonPropCom::_InitCtrl( void )
{

}

void sqr_tools::CSkeletonPropCom::Update( void )
{
	if (!SkeletonImp || !m_pUserCtrl)
		return;

	std::string skeName = SkeletonImp->GetCurSkeName();
	std::string curSelectedSkeName = SkeletonImp->GetCurSelSkeBoneName();
	��ǰ�����ļ� = gcnew String(skeName.c_str());
	��ǰѡ�й��� = gcnew String(curSelectedSkeName.c_str());
	if (skeName.empty())
		m_iSkeletonSize = m_iLastSkeletonSize = 0;
	m_pUserCtrl->Refresh();
}

