#include "stdafx.h"
#include "CCfgTempVarCheck.h"

CCfgTempVarCheck::SetImmuneTaskAttackName CCfgTempVarCheck::m_setImmuneTaskAttackName;

bool CCfgTempVarCheck::Check()
{
	m_setImmuneTaskAttackName.insert("�����ս��");
	m_setImmuneTaskAttackName.insert("����ը��");
	m_setImmuneTaskAttackName.insert("��������");
	m_setImmuneTaskAttackName.insert("�ǹ�ս����");

	return true;
}

bool CCfgTempVarCheck::IsImmuneTaskAttackName(const string& strName)
{
	return m_setImmuneTaskAttackName.find(strName) != m_setImmuneTaskAttackName.end();
}

void CCfgTempVarCheck::EndCheck()
{
	m_setImmuneTaskAttackName.clear();
}
