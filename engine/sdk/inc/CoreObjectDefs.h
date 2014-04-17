#pragma once


namespace sqr
{
	//SetSpeed�����ķ���ֵ
	enum ESetSpeedResult {eSSR_Success,eSSR_IsNotMoving,eSSR_InvalidSpeed,eSSR_NotAllowed};
	//MoveTo�����ķ���ֵ
	enum EMoveToResult {eMTR_Success,eMTR_FindPathFailed,eMTR_SuperPosition,eMTR_NotAllowed};

	enum ETraceResult {eTR_Success,eTR_MoveFailed,eTR_NotAllowed};
	enum ETraceEndedType { eTET_Traced, eTET_Stopped, eTET_Failed };

	//watch���ܵķ���ֵ
	enum EWatchResult {eWR_InScope, eWR_NotInScope};

	//StopMoving�����ķ���ֵ
	enum EStopMovingResult
	{
		eSMR_Success		=	eSSR_Success,
		eSMR_IsNotMoving	=	eSSR_IsNotMoving,
		eSMR_NotAllowed		=	eSSR_NotAllowed
	};

	//SetPos�Լ�SetGridPos�ķ���ֵ
	enum ESetPosResult {eSPR_Success,eSPR_IsMoving,eSPR_NotAllowed,eSPR_OutOfBound};

	//OnMoveMoveEnded�����ͣ�����Ŀ���ΪSuccess����StopMoving��ֹΪStopped
	enum EMoveEndedType {eMET_Reached,eMET_Stopped};

	enum EStillObjFlag{ eSOF_Follower,eSOF_Active_Director,eSOF_Passive_Director };

#define DIRECTOR_SEND_MOVED_DIST
}
