#pragma once

#include "CClassCallBack.h"

template<typename Func>
void CClassCallBack::CheckCallBackFunc(Func pFunc)
{
	const char* szError = CheckRegisterCallBack(pFunc, GetParamType(), GetRetType(), GetParamLen(), GetRetLen());
	if(szError)
	{
		stringstream str;
		str << m_sClassName.c_str() << "::" << m_sFunName.c_str() << "ʵ�ʺ�����ע���lua������ \"" 
			<< m_szFormat.c_str() << "\" ��ƥ��\n" << szError << endl;
		GenErr(str.str().c_str());
	}
}

