#pragma once
//$id$
//����ϵͳʹ�õ����Ӷ���,���������ϵͳ��ص�������Ϣ���ɷ�

#include "CConnStress.h"
#include "CCoreObjectStress.h"
#include "CTimeSystemStress.h"

template<typename CmdClass>
void CConnStress::OnServerCommand(const CmdClass* pCmd,void* pParam)
{
	OnServerCommand(pCmd);
	//	return ls_Inst;
}

//template<typename CmdClass>
//void CConnStress::OnServerCommand(const CmdClass* pCmd,void* pParam)
//{
//	OnServerCommand(pCmd);
//}

template<typename CmdClass>
void CConnStress::RegisterServerCommand()
{
	CConnStress::SetHandler<CmdClass>(&CConnStress::OnServerCommand);
}


template<typename CmdClass,typename ObjectClass,void (ObjectClass::*pFun)(const CmdClass*)>
void CConnStress::OnObjectCommand(CmdClass* pCmd,void* pParam)
{
	//(static_cast<ObjectClass*>(CCoreObjectFollower::GetObjectFollower(pCmd->uqbGlobalID))->*pFun)(pCmd);
}

template<typename CmdClass,typename ObjectClass,void (ObjectClass::*pFun)(const CmdClass*)>
void CConnStress::RegisterObjectCommand()
{
	CConnStress::SetHandler<CmdClass>(&CConnStress::OnObjectCommand<const CmdClass,ObjectClass,pFun>);
}

template<typename CmdClass>
void CConnStress::OnSceneCommand(const CmdClass* pCmd,void* pParam)
{
	(CCoreObjectStress::MainObject()->OnServerCommand)(this, pCmd);
}

template<typename CmdClass>
void CConnStress::RegisterSceneCommand()
{
	CConnStress::SetHandler<CmdClass>(&CConnStress::OnSceneCommand<CmdClass>);
}

template<typename Cmd>
void CConnStress::SendStressCmd(Cmd *cmd)
{
	cmd->uobClientKnownServerFrameTime = GetLastSyncedServerFrameTime();
	cmd->uobClientTime = CTimeSystemStress::Inst()->GetFrameTime();
	//cout << "send " << typeid(Cmd).name() << GetLastSyncedServerFrameTime() << endl;
	TConnection<CTraitsStress>::SendCoreCmd(reinterpret_cast<const void*>(cmd),sizeof(Cmd));
}
