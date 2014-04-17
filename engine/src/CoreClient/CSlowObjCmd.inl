#pragma once
#include "CSlowObjCmd.h"
#include "ErrLogHelper.h"
#include "ExpHelper.h"

template<typename CmdType>
TSlowObjCmd<CmdType>::TSlowObjCmd(CCoreObjectFollower* pObj,const void* pData,uint32 uSize)
:CSlowObjCmd(pObj,pData,uSize)
{
}

template<typename CmdType>
void TSlowObjCmd<CmdType>::OnTick()
{
	list<CFastObjCmd*> lstObjCmd;

	uint32 uObjId=GetObj()->GetGlobalID();
	lstObjCmd.swap(m_lstNestedCmd);

#ifdef LOG_COREOBJ_MOVE
	GetObj()->LogMsg("E_S_C");
#endif
	SQR_TRY
	{
		GetObj()->OnDelayedServerCommand( static_cast<const CmdType*>(GetData()) );
	}
	SQR_CATCH(exp)
	{
		if(CConnClient::Inst())
		{
			LogExp(exp, CConnClient::Inst());		
		}
		else
		{
			LogExp(exp);
		}
	}
	SQR_TRY_END;

	CCoreObjectFollower* pObj;

	while(!lstObjCmd.empty())
	{
		//�������ÿ��ѭ��������һ�飬��������Ѿ���������
		pObj=CCoreObjectFollower::GetObjectFollower(uObjId);
		if(!pObj)
			return;	//�˴�����delete this
		lstObjCmd.front()->Call(pObj);
		delete lstObjCmd.front();
		lstObjCmd.pop_front();
	}		
	delete this;
}