#pragma once

#include "TFuncCall.h"
#include "CBaseObject.h"
#include "ErrLogHelper.h"
#include "ExpHelper.h"
#include "TFuncCallDispatcher.h"
#include "ScriptHelper.h"
#include "TFuncCallCheck.h"
#include <iostream>

template<typename Func>
TFuncCall<Func>::TFuncCall( CScript& Script, const char* szFormat, Func pFunc, bool bClass, const char* szFunName )
				: ParentType( Script, szFormat, szFunName ), m_pFunc(pFunc)
{
	this->SetBeClassFunc(bClass);

	const char* szError = CheckRegisterFunc(pFunc, GetParamType(), GetRetType(), GetParamLen(), GetRetLen());
	if(!szError)
		return;

	string strFormat(szFormat);
	string::size_type pos = strFormat.find_first_of(')');
	if(pos != string::npos)
		strFormat = strFormat.erase(0, pos + 1);
	stringstream str;
	str << m_sFunName.c_str() << "ʵ�ʺ�����ע���lua������ \"" 
		<< strFormat.c_str() << "\" ��ƥ��\n" << szError << endl;
	GenErr(str.str().c_str());
}

template<typename Func>
int32 TFuncCall<Func>::Call( lua_State* pState )
{
	int32 nRetNum = 0;
	
	SQR_TRY
	{
		TFuncCall<Func>* pFunction = (TFuncCall<Func>*)LuaToUserData( pState, LuaUpValueIndex(1) );
			
		char* pStackBuf	= (char*)alloca( pFunction->GetParamBufSize() );

		pFunction->GetParamFromLuaStack( pState, pStackBuf );

		//printf( "%s\n",pFunction->GetFunctionName());
		CallDispatcher(pFunction->m_pFunc, pStackBuf, pFunction->GetParamOffset(), pFunction->GetRetOffset() );

		pFunction->FreeParamFromCppStack(pStackBuf);

		nRetNum =  pFunction->PutResultToLuaStack( pState, pStackBuf );
	}
	SQR_CATCH(exp)
	{
		//���coredumpΪtrue����ô��������ֱ�ӵ����ˣ���Ȼ�������ϴ��ݣ�ʹ���Ժ���Ѳ���
		if (!CatchErrorEnabled())
		{
			ostringstream str;
			
			str<<exp.ErrorMsg()<<endl;
			str<<exp.StackInfo()<<endl;

			CScript::GetStack(str);

			GenErr(str.str().c_str());
		}
		else
		{
			ostringstream strm;
			CScript::GetStack(strm);
			exp.AppendStackInfo(strm.str().c_str());

			LogExp(exp, NULL, false);
			return 0;
		}
	}
	SQR_TRY_END;

	return nRetNum;
}

