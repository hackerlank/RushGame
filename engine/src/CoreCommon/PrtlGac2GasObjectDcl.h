#pragma once
//$id$
//����ϵͳgac��gas��Э�鶨��ͷ�ļ�

#include "PrtlGac2GasSegment.h"

namespace sqr
{
	enum EGac2GasObjectCmdId
	{
		eGac2GasOC_Tell_Mediator_Sync_Target_SyncVariant		=eGac2GasPS_Object_Begin,
		eGac2GasOCI_Tell_Mediator_Max_Speed_Set,
		eGac2GasOCI_Tell_Mediator_Stepped,
		eGac2GasOCI_Tell_Mediator_SetPosition,
		eGac2GasOCI_Report_Invalid_Step,		
		eGac2GasOC_Tell_Mediator_Sync_Target_ViewVariant,
		eGac2GasOCI_Tell_Mediator_Stopped,
		eGac2GasOCI_Report_Invalid_Move,					//���������ƶ�Ŀ������Ч��		
		eGac2GasOCI_Change_Mediator_Move_Target,
		eGac2GasOCI_Notify_Mediator_Run_Mode_Switched,
		eGac2GasOCI_Move_Mediator_To,			//��������step·����ʵ��·��������
		eGac2GasOCI_Tell_Mediator_Reached,
		eGac2GasOCI_Change_Mediator_Move_Speed,

		eGac2GasOCI_Count
	};
}


