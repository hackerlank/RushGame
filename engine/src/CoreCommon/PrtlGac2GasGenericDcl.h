#pragma once
//$id$
//gac��gas��ͨ��Э�鶨��ͷ�ļ�

#include "PrtlGac2GasSegment.h"

namespace sqr
{
	enum EGac2GasGenericCmdId
	{
		eGac2GasGCI_Tell_BeTesting =eGac2GasPS_Generic_Begin,
		eGac2GasGCI_Main_Scene_Set,
		eGac2GasGCI_Middle_Shell_Message,
		eGac2GasGCI_Ping_Server, 
		eGac2GasGCI_Client_Guid,
		eGac2GasGCI_Update_Latency_Server,
		eGac2GasGCI_Small_Shell_Message,
		//eGac2GasGCI_Tell_OwnerInfo,
		//eGac2GasGCI_Update_Server_Known_Time,
		eGac2GasGCI_Count
	};
}


