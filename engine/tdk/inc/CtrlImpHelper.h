#pragma once
#include "CCtrlBase.h"

namespace sqr_tools
{
	template <class T>
	T* CtrlImpPtr(void* pCtrl)
	{
		Ast(((CCtrlBase*)pCtrl)->GetTypeName() == T::st_TypeName && "CtrlImp����ת������ȷ");
		T* CtrlImp = static_cast<T*>(pCtrl);
		return CtrlImp;
	}
}