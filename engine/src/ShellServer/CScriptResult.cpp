#include "stdafx.h"
#include "CScriptResult.inl"
#include "CScriptAppServer.h"
#include "CScriptSerialHelper.h"
#include "CScriptThread.h"


CScriptPostPackRes::CScriptPostPackRes(const char* strType, size_t stTypeSize,
									   const char* vecBuf, size_t stBufSize,
									   CScriptThread* pThread)
: m_stTypeSize(stTypeSize)
, m_stBufSize(stBufSize)
{
	m_szType = (char*)( CloneData(strType, stTypeSize, pThread) );
	m_pBuffer = CloneData(vecBuf, m_stBufSize, pThread);
}


CScriptPostPackRes::~CScriptPostPackRes()
{
	CScript* pScript = CScriptAppServer::Inst()->GetScript();
	CSerialHelper::ms_OnUnPackFun(pScript, m_szType, m_stTypeSize, (char*)m_pBuffer, m_stBufSize);
}

CScriptTellErrMsgResult::CScriptTellErrMsgResult(const char* szError, CScriptThread* pThread)
{
	m_szMsg = CloneString(szError, pThread);
}


CScriptTellErrMsgResult::~CScriptTellErrMsgResult()
{
	//�����߼����̵߳�tell err to all����
	CallLogCallBackFunc(m_szMsg, NULL);
}

