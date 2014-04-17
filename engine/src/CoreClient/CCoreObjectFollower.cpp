#include "stdafx.h"
#include "CCoreObjectFollower.h"
#include "CObjMoveStateClient.h"
#include "CCoreSceneClient.h"
#include "ICoreSceneClientHandler.h"
#include "ICoreObjectFollowerHandler.h"
#include "CoreObjectClientCommon.h"
#include "CoreObjectCommon.h"
#include "PrtlGas2GacObjectDef.h"
#include "PrtlGac2GasObjectDef.h"
#include "CoreCommon.h"
#include "CFolObjFollowState.h"
#include "CFolMovement.h"
#include "CPixelPath.h"
#include "ErrLogHelper.h"
#include "CCOEventPositionSet.h"
#include "CDelayedObjCmdMgr.h"
#include "ExpHelper.h"
#include "CConnClient.inl"
#include "COEventClient.h"
#include "MoveCallArgs.h"
#include "CSyncVariantClient.h"
#include "CSyncVariantClientHolder.h"
#include "CObjVarDef.h"
#include "CObjVarDefCfg.h"
#include "CSyncVariantClientHolder.inl"
#include "IPathFinding.h"
#include <iomanip>
#include "CCoreMsgMgr.h"
#include "CMetaSceneClient.h"
#include "CDistortedTimeObjClient.h"
#include "TBindObject.inl"
#include "CSyncVariantClientHolderPool.h"

CCoreObjectFollower::MapPendingBindRelation				CCoreObjectFollower::m_mapPendingBindRelation;

//������������һ�����캯�����ڴ�����ֹ����ʱ����
CCoreObjectFollower::CCoreObjectFollower(const CGas2GacOC_Create_Still_Object* pCmd)
:CCoreObjectClient(NULL,pCmd->PixelPos)
,m_uPendingBindParentGID(0)
,m_bReceiveStopOrReachCmd(true)
,m_bEnteredSyncAoi(false)
,m_bEnteredViewAoi(false)
{
	m_pBindObj = new TBindObject<CCoreObjectFollower>(this);

	static_cast<CDistortedTimeObjClient*>(GetDistortedTimeObj())->m_uLastDistortedServerFrameTimeWhenTimeRatioChanged = pCmd->uobDistortedFrameTime;
	static_cast<CDistortedTimeObjClient*>(GetDistortedTimeObj())->m_uLastRealServerFrameTimeWhenTimeRatioChanged = pCmd->uobDistortedFrameTime;

	m_uGlobalID				=	pCmd->uqbGlobalID;
	m_uMoveDelay			=	pCmd->uqbDelay;
	m_fBarrierSize			=	0.0f;
	m_eBarrierType			=	eBT_NoBarrier;
	
	CreateSyncVariant( pCmd );
	ConstructHelper();
	//cout << m_uGlobalID << " so " << pCmd->PixelPos.x << " " << pCmd->PixelPos.y << endl;
	SetBarrier(pCmd->fBarrierSize, static_cast<EBarrierType>(pCmd->usbSelfBarrierType));
#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "CGas2GacOC_Create_Still_Object  pos " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y;
	LogMsg(strm.str());
#endif
}

//������������һ�����캯�����ڴ����ƶ��еĶ���ʱ����
CCoreObjectFollower::CCoreObjectFollower(const CGas2GacOC_Create_Moving_Object* pCmd)
:CCoreObjectClient(NULL,pCmd->BeginPixelPos)
,m_uGlobalID(pCmd->uqbGlobalID),m_uMoveDelay(pCmd->uqbDelay)
,m_fBarrierSize(0.0f)
,m_eBarrierType(eBT_NoBarrier)
,m_uPendingBindParentGID(0)
,m_bReceiveStopOrReachCmd(false)
,m_bEnteredSyncAoi(false)
,m_bEnteredViewAoi(false)
{
	m_pBindObj = new TBindObject<CCoreObjectFollower>(this);

	static_cast<CDistortedTimeObjClient*>(GetDistortedTimeObj())->m_uLastDistortedServerFrameTimeWhenTimeRatioChanged = pCmd->uobDistortedFrameTime;
	static_cast<CDistortedTimeObjClient*>(GetDistortedTimeObj())->m_uLastRealServerFrameTimeWhenTimeRatioChanged = pCmd->uobDistortedFrameTime;

	CreateSyncVariant( pCmd );
	ConstructHelper();
	SetBarrier(pCmd->fBarrierSize, static_cast<EBarrierType>(pCmd->usbSelfBarrierType));


#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "CGas2GacOC_Create_Moving_Object  pos " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y;
	LogMsg(strm.str());
#endif

	CFPos BeginPixelPos(pCmd->BeginPixelPos);
	CFPos EndPixelPos(pCmd->EndPixelPos);

	float fDist		= pCmd->fMovedDist;
	float fEndDist	= pCmd->fEndDist;
	float fSpeed	= pCmd->fSpeed;

	if(fDist < 0)
		GenErr("fDist < 0");
	if(fSpeed <= 0)
		GenErr("fSpeed <= 0");

	int16* pPathData = (int16*)(pCmd + 1);

	//cout << "CM " << CConnClient::Inst()->GetEstimatedServerFrameTime() << endl;
	CFolMovement* pMm = CFolMovement::Create(GetScene(),pPathData, pCmd->udbPathDataLen,BeginPixelPos,EndPixelPos,eFMT_MoveTo,fSpeed,fEndDist,CConnClient::Inst()->GetEstimatedServerFrameTime() - uint64(fDist/fSpeed/eGridSpanForObj*1000));

	//printf("goodman: create_moving_object from(%d,%d) to(%d,%d,%f)\n",BeginPixelPos.x,BeginPixelPos.y,EndPixelPos.x,EndPixelPos.y,pCmd->fEndDist);

	pMm->GetMovementBeginPixelPos();
	pMm->GetPathEndPixelPos();

	CPixelPath* pPath = pMm->ClonePixelPath();

	//�մ������ƶ��еĶ����Ȼ����·���ϣ�����ʹ�����߽���ͬ���취��
#ifdef LOG_COREOBJ_MOVE
	LogMsg("Follower Create m_pFolState");
#endif
	m_pFolState = new CFolObjFollowState( pMm , fSpeed, this );
	m_pFolState->OnStepped(fDist);

	//������ʱ���˾���
	fDist -= m_pFolState->CalIdealDistByTime(m_uMoveDelay);

	if( fDist < 0 )
		fDist = 0;

	Ver(pMm->GetPixelPos(BeginPixelPos,fDist));
	//if ( !pMm->GetPixelPos(BeginPixelPos,fDist) )
	//	BeginPixelPos = pMm->GetMovementEndPixelPos();

	//cout << m_uGlobalID << " mo  server_begin_pos=" << pCmd->BeginPixelPos.x << "," << pCmd->BeginPixelPos.y << " client_begin_pos=" << BeginPixelPos.x << "," << BeginPixelPos.y << endl;
	//cout << GetGlobalID() << " setpos " << BeginPixelPos.x << "," << BeginPixelPos.y << endl;
	IntSetPosition( BeginPixelPos );

	DoMove( pPath , m_pFolState->GetRealSpeed() , fDist );
	GetMovState()->SetEndDist( fEndDist );
	
}

template<typename CmdType>
void CCoreObjectFollower::CreateSyncVariant(const CmdType* pCmd)
{
	CObjVarDef* pDef=GetVarDefCfg()->Get(pCmd->usbVarDefCode);
	CCypherVariantCodeTable* pCodeTable;

	CSyncVariantClientHolderPackage* pData = CSyncVariantClientHolderPool::GetInst()->GetSyncVariantClientHolderById(m_uGlobalID);
	if (pData)
	{
		for (uint32 i=0; i<pData->m_vecSyncVariantHolder.size(); i++)
		{
			m_vecSyncVariantHolder.push_back(pData->m_vecSyncVariantHolder[i]);
		}

		for (uint32 i=0; i<pData->m_vecViewVariantHolder.size(); i++)
		{
			m_vecViewVariantHolder.push_back(pData->m_vecViewVariantHolder[i]);
		}
		delete pData;
	}
	else
	{
		for (uint32 i=0; i<pDef->GetSyncCodeTableCount(); i++)
		{
			pCodeTable=pDef?pDef->GetSyncCodeTable(ECodeTableIndex(i)):NULL;
			m_vecSyncVariantHolder.push_back(new CSyncVariantClientHolder(pCmd->usbVarDefCode,pCodeTable,true,ECodeTableIndex(i)));
		}
		for (uint32 i=0; i<pDef->GetViewCodeTableCount(); i++)
		{
			pCodeTable=pDef?pDef->GetViewCodeTable(ECodeTableIndex(i)):NULL;
			m_vecViewVariantHolder.push_back(new CSyncVariantClientHolder(pCmd->usbVarDefCode,pCodeTable,false,ECodeTableIndex(i)));
		}
	}
}

void CCoreObjectFollower::DelSyncVariant()
{
	CSyncVariantClientHolderPackage* pData = new CSyncVariantClientHolderPackage(GetGlobalID());
	for (uint32 i=0; i<m_vecSyncVariantHolder.size(); i++)
	{
		m_vecSyncVariantHolder[i]->SetHandler(NULL);
		m_vecSyncVariantHolder[i]->EnableDecode(true);
		pData->m_vecSyncVariantHolder.push_back(m_vecSyncVariantHolder[i]);
	}
	for (uint32 i=0; i<m_vecViewVariantHolder.size(); i++)
	{
		//delete m_vecViewVariantHolder[i];
		m_vecViewVariantHolder[i]->SetHandler(NULL);
		m_vecViewVariantHolder[i]->EnableDecode(true);
		pData->m_vecViewVariantHolder.push_back(m_vecViewVariantHolder[i]);
	}
	m_vecSyncVariantHolder.clear();
	m_vecViewVariantHolder.clear();
	CSyncVariantClientHolderPool::GetInst()->PushSyncVariantClientHolder(m_uGlobalID, pData);
}
//�������캯�����õ��Ĺ�����ʼ�����룬��˰�������ȡ��һ��������
void CCoreObjectFollower::ConstructHelper()
{
	m_pFolState = NULL;
	m_pDelayedCmdMgr=new CDelayedObjCmdMgr;

	if( AddObjectFollower(this) )
		return;

	delete GetObjectFollower( GetGlobalID() );

	Ver( AddObjectFollower(this) );

	ostringstream strm;
	strm<<"������id��:"<< GetGlobalID();
	LogErr("AddObjectFollowerʱ������ͬid�Ķ����Ѿ����ڣ��϶���ɾ��", strm.str());	
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Check_Object_ViewVariant* pCmd)
{
	bool bSyncStaticView = pCmd->uStaticVariantEncodeTimes > m_vecViewVariantHolder[eCTI_Static]->GetRealDecodeTimes();
	bool bSyncAgileView = pCmd->uAgileVariantEncodeTimes > m_vecViewVariantHolder[eCTI_Agile]->GetRealDecodeTimes();

	if (bSyncStaticView || bSyncAgileView)
	{
		m_vecViewVariantHolder[eCTI_Static]->EnableDecode(bSyncStaticView);
		m_vecViewVariantHolder[eCTI_Agile]->EnableDecode(bSyncAgileView);
		CGac2GasOC_Tell_Mediator_Sync_Target_ViewVariant cmd;
		cmd.uTargetGlobalID = pCmd->uqbGlobalID;
		cmd.bSyncStaticView = bSyncStaticView;
		cmd.bSyncAgileView = bSyncAgileView;
		cmd.uSessionID = pCmd->uSessionID;
		CConnClient::Inst()->SendCoreCmd(&cmd);
		return;
	}
	Ast( pCmd->uStaticVariantEncodeTimes == m_vecViewVariantHolder[eCTI_Static]->GetRealDecodeTimes() );
	Ast( pCmd->uAgileVariantEncodeTimes == m_vecViewVariantHolder[eCTI_Agile]->GetRealDecodeTimes() );
	SetViewVariantInitEnd();
	OnSyncViewVariantEnd(pCmd->uBindParentID,pCmd->uStaticVariantEncodeTimes,pCmd->uAgileVariantEncodeTimes);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Sync_View_Variant_End* pCmd)
{
	OnSyncViewVariantEnd(pCmd->uBindParentID,pCmd->uStaticVariantEncodeTimes,pCmd->uAgileVariantEncodeTimes);
}

void CCoreObjectFollower::OnSyncViewVariantEnd(uint32 uBindParentGID, uint32 uStaticVariantRealDecodeTimes, uint32 uAgileVariantRealDecodeTimes)
{
	if(uBindParentGID != 0)
	{
		CCoreObjectFollower* pBindParent = GetObjectFollower(uBindParentGID);
		if(!pBindParent)
		{
			m_mapPendingBindRelation.insert(make_pair(uBindParentGID, GetGlobalID()));
			m_uPendingBindParentGID = uBindParentGID;
			//cout << "����Ԥ�󶨹�ϵSID��" << GetGlobalID() << " -> " << uBindParentID << "\n";
		}
		else
		{
			//cout << GetGlobalID() << " Sync Global PreBindTo " << uBindParentGID << endl;
			BindTo(pBindParent);
		}
	}
	uint32 uSelfGID = GetGlobalID();
	for(;;)
	{
		MapPendingBindRelation::iterator itr = m_mapPendingBindRelation.find(uSelfGID);
		if(itr == m_mapPendingBindRelation.end())
			break;
		uint32 uBindChildGID = itr->second;
		m_mapPendingBindRelation.erase(itr);
		//cout << "����Ԥ���Ӷ���SID = " << uBindChildGID << "�����Ԥ�󶨹�ϵSID�� " << uBindChildGID << " -> " << uSelfGID << "\n";
		CCoreObjectFollower* pBindChild = GetObjectFollower(uBindChildGID);
		if(!pBindChild)
		{
			stringstream str;
			str << "�ͻ��˴���CCoreObjectFollower�����Ӱ󶨹�ϵʱ�Ӷ��󲻴��ڣ�" << "����ID = " << uSelfGID << ", �Ӷ���ID = " << uBindChildGID;
			GenErr(str.str().c_str());
			//GenErr("�ͻ��˴���CCoreObjectFollower�����Ӱ󶨹�ϵʱ�Ӷ��󲻴���", str.str().c_str());
		}
		pBindChild->m_uPendingBindParentGID = 0;
		//BindTo����������m_mapPendingBindRelation��Ԫ�أ����������ѭ��
		//cout << uBindChildGID << " Sync Global PreBindTo " << uSelfGID << endl;
		pBindChild->BindTo(this);	
	}

	m_vecViewVariantHolder[eCTI_Static]->SetRealDecodeTimes(uStaticVariantRealDecodeTimes);
	m_vecViewVariantHolder[eCTI_Agile]->SetRealDecodeTimes(uAgileVariantRealDecodeTimes);
	ICoreSceneClientHandler* pHandler=GetScene()->GetHandler();
	if( !pHandler )
		return;
	pHandler->OnObjectCreated(this);

	m_bEnteredViewAoi = true;
	if (m_bEnteredSyncAoi && GetHandler())
	{
		GetHandler()->OnEnteredSyncAoi();
	}
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Init_View_Variant* pCmd)
{
	m_bEnteredViewAoi = false;
	SetViewVariantInitEnd();
}

//��������
CCoreObjectFollower::~CCoreObjectFollower(void)
{
#ifdef LOG_COREOBJ_MOVE
	LogMsg("Delete CoreObjectFollower");
#endif
	CCoreObjectFollower::OnBeforeDisbindFromParent();
	CCoreObjectFollower::OnBeforeDisbindToAllChild();

	delete m_pDelayedCmdMgr;
	m_pDelayedCmdMgr=NULL;

	if(m_pFolState)
	{
		Ast(IsPassive());

		delete m_pFolState->m_pCurMovement;
		m_pFolState->m_pCurMovement=NULL;

		list<CFolMovement*>& lstMovement=m_pFolState->m_lstPendingMovement;

		while(!lstMovement.empty())
		{
			delete lstMovement.front();
			lstMovement.pop_front();
		}

#ifdef LOG_COREOBJ_MOVE
		LogMsg("Follower Delete m_pFolState");
#endif
		delete m_pFolState;
		m_pFolState=NULL;
	}

	DelSyncVariant();
	ClearBarrier();
	Ver(DelObjectFollower(GetGlobalID()));

	//cout << GetGlobalID() << " Global PreDisbindFromParent & AllChild\n";

	DisbindFromParent();
	DisbindAllChildren();

	delete m_pBindObj;
	m_pBindObj = NULL;
	m_bReceiveStopOrReachCmd = true;
}


//�ͻ������и������ע�������key�Ǹ�������GlobalID��ֵ�Ǹ��������
CCoreObjectFollower::MapObjFollower_t CCoreObjectFollower::ms_mapObjFollower;


//���ظ�������map,��const���ͣ�����޷��޸�
const map<uint32,CCoreObjectFollower*>& CCoreObjectFollower::GetObjectFollowerMap()
{
	return ms_mapObjFollower;
}

//���Follower��������ƶ��ٶȵ�ʱ��
float& CCoreObjectFollower::GetCorrectionSpeedRatio()
{
	//����˷���step������
	static float ls_fCorrectionSpeed=0.5f;	//0.25��֮�ھ������
	return ls_fCorrectionSpeed;
}

CCoreObjectFollower* CCoreObjectFollower::GetObjectFollower(uint32 uObjGlobalID)
{
	MapObjFollower_t::iterator it=ms_mapObjFollower.find(uObjGlobalID);
	if(it==ms_mapObjFollower.end())
		return NULL;
	return it->second;
}

bool CCoreObjectFollower::AddObjectFollower(CCoreObjectFollower* pObject)
{
	return ms_mapObjFollower.insert(make_pair(pObject->GetGlobalID(),pObject)).second;
}

bool CCoreObjectFollower::DelObjectFollower(uint32 uObjGlobalID)
{
	return ms_mapObjFollower.erase(uObjGlobalID)==1;
}

uint32 CCoreObjectFollower::GetGlobalID()const
{
	return m_uGlobalID;
}

bool CCoreObjectFollower::GetDestOfCurPath(CFPos& pos)
{
	if(!IsMoving())
		return false;

	CFolMovement* pFolMovement = m_pFolState->GetLastMovement();
	Ast(pFolMovement);
	pos = pFolMovement->GetMovementEndPixelPos();

	return true;
}

bool CCoreObjectFollower::IsMoving()const
{
	return m_pFolState != NULL;
}

EClientObjectType CCoreObjectFollower::GetType()const
{
	return eCOT_Follower;
}

ICoreObjectFollowerHandler* CCoreObjectFollower::GetHandler()const
{
	ICoreObjectClientHandler* pHandler=CCoreObjectClient::GetHandler();
	return pHandler?pHandler->CastToObjFolHandler():NULL;
}

CObjVarDefCfg* CCoreObjectFollower::GetVarDefCfg()
{
	static CObjVarDefCfg ls_DefCfg;
	return &ls_DefCfg;
}

const CSyncVariantClientHolder* CCoreObjectFollower::GetSyncVariantHolder(ECodeTableIndex eIndex) const
{
	if (uint32(eIndex) > m_vecSyncVariantHolder.size())
		return NULL;
	return m_vecSyncVariantHolder[uint32(eIndex)];
}

const CSyncVariantClientHolder* CCoreObjectFollower::GetViewVariantHolder(ECodeTableIndex eIndex) const
{
	if (uint32(eIndex) > m_vecViewVariantHolder.size())
		return NULL;
	return m_vecViewVariantHolder[uint32(eIndex)];
}

const CSyncVariantClient* CCoreObjectFollower::GetSyncVariant(ECodeTableIndex eIndex) const
{
	if (uint32(eIndex) > m_vecSyncVariantHolder.size())
		return NULL;
	return m_vecSyncVariantHolder[uint32(eIndex)]->GetRootVariant();
}

const CSyncVariantClient* CCoreObjectFollower::GetViewVariant(ECodeTableIndex eIndex) const
{
	if (uint32(eIndex) > m_vecViewVariantHolder.size())
		return NULL;
	return m_vecViewVariantHolder[uint32(eIndex)]->GetRootVariant();
}

ICypherVariantHolderHandler* CCoreObjectFollower::GetSyncVariantHandler() const
{
	return m_vecSyncVariantHolder[(uint32)eCTI_Static]->GetHandler();
}

ICypherVariantHolderHandler* CCoreObjectFollower::GetViewVariantHandler() const
{
	return m_vecViewVariantHolder[(uint32)eCTI_Static]->GetHandler();
}

void CCoreObjectFollower::SetSyncVariantHandler(ICypherVariantHolderHandler* pHandler, ECodeTableIndex eIndex)
{
	if (uint32(eIndex) > m_vecSyncVariantHolder.size())
		return;
	m_vecSyncVariantHolder[uint32(eIndex)]->SetHandler(pHandler);;
}

void CCoreObjectFollower::SetViewVariantHandler(ICypherVariantHolderHandler* pHandler, ECodeTableIndex eIndex)
{	
	if (uint32(eIndex) > m_vecViewVariantHolder.size())
		return;
	m_vecViewVariantHolder[(uint32)eIndex]->SetHandler(pHandler);
}
bool CCoreObjectFollower::IsPassive()const
{
	return true;
}

//�յ��ӳ����������Ҫ��������ʼ�ƶ�
void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Move_Follower_To* pCmd)
{
	//printf("Move follower to,end dist:%f\n",pCmd->fEndDist);
	//cout<<GetLocalID()<<"\tCGas2GacOC_Move_Follower_To"<<endl;
	if(!m_bReceiveStopOrReachCmd)
	{
		ostringstream strmErr;
		strmErr << GetGlobalID();
		LogErr("Move_Follower_To Timing Err", strmErr.str());
	}
	m_bReceiveStopOrReachCmd = false;

	Ast( IsPassive() );

	int16* pPathData = (int16*)(pCmd + 1);

	CFPos EndPixelPos( pCmd->EndPixelPos );

	//printf("goodman: CGas2GacOC_Move_Follower_To to(%d,%d,%f)\n",EndPixelPos.x,EndPixelPos.y,pCmd->fEndDist);

#ifdef LOG_COREOBJ_MOVE
	ostringstream strm; 
	strm << "CGas2GacOC_Move_Follower_To\tm_pFolState ";
	if(m_pFolState)
	{
		strm << "!= NULL ";
	}
	else
	{
		strm << "== NULL ";
	}
	strm << " ServerPos = " << setprecision(12) << pCmd->BeginPixelPos.x << "," << pCmd->BeginPixelPos.y << " FolPos = " << GetPixelPos().x << "," << GetPixelPos().y;
	LogMsg(strm.str());
#endif

	if( !m_pFolState )
	{
		Ast( !GetMovState() );

		//֮ǰ��δ���ڸ���״̬������Ҫ�����κ����䶯����������Follower����ʼ�ƶ�

#ifdef LOG_COREOBJ_MOVE
		if( GetPixelPos() != CFPos(pCmd->BeginPixelPos) )
		{
			ostringstream oss;
			oss<<"("<< setprecision(12) <<GetPixelPos().x<<","<<GetPixelPos().y
				<<") != ("<<pCmd->BeginPixelPos.x<<","<<pCmd->BeginPixelPos.y<<")";
			LogMsg(oss.str());
			GenErr("BeginPixelPos not equal. ", oss.str());
		}
#endif


		float fEndDist = pCmd->fEndDist;

		//cout << "MFT " << pCmd->fSpeed << endl;
		//�����ƶ�·��
		//cout << "MFT " << CConnClient::Inst()->GetEstimatedServerFrameTime() << endl;
		//CFolMovement* pMm=CFolMovement::Create( GetScene(), pPathData, pCmd->udbPathDataLen, GetPixelPos(),EndPixelPos ,eFMT_MoveTo,pCmd->fSpeed, fEndDist, pCmd->uobServerFrameTime + (MoveDelayEnabled() ? GetMoveDelay() : 0));
		CFolMovement* pMm=CFolMovement::Create( GetScene(), pPathData, pCmd->udbPathDataLen, GetPixelPos(),EndPixelPos ,eFMT_MoveTo,pCmd->fSpeed, fEndDist, m_queMoveToBaseTime.front());
		m_queMoveToBaseTime.pop();

		if( !pMm )
		{
			//����ʧ��
			CGac2GasOC_Report_Invalid_Move cmd;
			CConnClient::Inst()->SendCoreCmd(&cmd);
#ifndef LOG_COREOBJ_MOVE
			GenErr("Invalid move");
#else
			return;
#endif
		}

		//�򿪶���ĸ���״̬
		//����MoveTo�����Ǳ��ӳٵ��õģ���˷���˶�������߽���Ӧ�ò���CalDelayDist(pCmd->fSpeed)��ô��
		//printf("�µ�follow����ľ���,%f\n", CalDelayDist(pCmd->fSpeed));
#ifdef LOG_COREOBJ_MOVE
		ostringstream strm;
		strm << "Follower Create m_pFolState path len = " << pMm->GetPathLength();
		LogMsg(strm.str());
#endif
		m_pFolState = new CFolObjFollowState(pMm,pCmd->fSpeed,this);
		m_pFolState->OnStepped(m_pFolState->CalIdealDistByTime(m_uMoveDelay));


		CFPos BeginPixelPos;
		pMm->GetOriginPath()->GetBeginPixelPos(BeginPixelPos);
		if( GetPixelPos() != BeginPixelPos )
		{
			ostringstream strm;
			strm<<"("<< setprecision(12) <<GetPixelPos().x<<","<<GetPixelPos().y<<") != ("<<BeginPixelPos.x<<","<<BeginPixelPos.y<<")"<<pMm->m_bChangePath;
#ifdef LOG_COREOBJ_MOVE
			LogMsg(strm.str());
#endif
			GenErr("BeginPixelPos not equal. ", strm.str());
		}
		//����·��ʱ��õ�EndPixelPos��ʵ�����ߵ���EndPixelPosλ�ò�ͬ



		//�ƶ�
		DoMove( pMm->ClonePixelPath(),m_pFolState->GetRealSpeed(), 0 );
		GetMovState()->SetEndDist(fEndDist);

#ifdef LOG_COREOBJ_MOVE
		if(fEndDist != GetMovState()->GetPathLength())	
		{
			ostringstream strm;
			strm << "pathLen != " << "s " << fEndDist << " c " << GetMovState()->GetPathLength();
			LogMsg(strm.str());
		}
#endif
		CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
			CCOEventMoveBeganClient(this));
	}
	else
	{
		//���GetMoveState()Ϊ�գ���m_pFolState����Ч����ôFollower
		//�϶���û���յ�Reached����Stopedָ����Ǵ�ʱ�����ڴ���Move_Toָ�Move_Toָ��֮
		//ǰ�Ǳ�����һ��Reached����Stopedָ��ģ�����GetMoveState()�ض���Ϊ�գ����߾��ǳ�����bug��
		if( !GetMovState() )
		{
#ifdef LOG_COREOBJ_MOVE
			LogMsg("GetMoveState() == NULL");
#endif
			GenErr("GetMoveState() == NULL");
		}
		//ȡ�õȴ��е�����·���Ķ��С�
		list<CFolMovement*>& lstMovement=m_pFolState->m_lstPendingMovement;

		//ȡ�����һ��·����
		CFolMovement* pMm=m_pFolState->GetLastMovement();

		pMm->SetMovedDistWhenRemoteReached(pMm->GetPathLength());

		//�����һ��·���Ľ����㣬�͵�ǰ��Ŀ��㴴��һ��·��
		//cout << "MFT1 speed " << pCmd->fSpeed << endl;
		//cout << "1MFT " << CConnClient::Inst()->GetEstimatedServerFrameTime() << endl;
		pMm=CFolMovement::Create( GetScene(),pPathData, pCmd->udbPathDataLen,pMm->GetMovementEndPixelPos(),EndPixelPos,eFMT_MoveTo,pCmd->fSpeed, pCmd->fEndDist, m_queMoveToBaseTime.front());
		m_queMoveToBaseTime.pop();

		pMm->m_bChangePath=false;

		//��·��ѹ�����
		lstMovement.push_back(pMm);

		m_pFolState->OnStepped(0);
	}
}


void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Change_Follower_Move_Target* pCmd)
{
	//printf("Change target\n");
	if(m_bReceiveStopOrReachCmd)
	{
		ostringstream strmErr;
		strmErr << GetGlobalID();
		LogErr("Change_Follower_Move_Target Timing Err", strmErr.str());
	}
	m_bReceiveStopOrReachCmd = false;

	Ast( IsPassive() );
	Ast( m_pFolState );

	int16* pPathData = (int16*)(pCmd + 1);

#ifdef LOG_COREOBJ_MOVE
	ostringstream stream;
	stream << "CGas2GacOC_Change_Follower_Move_Target " << pCmd->uqbSessionID << " datalen " << pCmd->udbPathDataLen;
	uint16 uWayPointNum = pCmd->udbPathDataLen / (2 * sizeof(int16));
	for(uint16 i = 0; i < uWayPointNum; ++i)
	{
		CPos CurWayPoint(*(pPathData + 2 * i), *(pPathData + 2 * i + 1));
		stream << " " << CurWayPoint.x << "," << CurWayPoint.y;
	}
	LogMsg(stream.str());
#endif

	//����˶����������·�����ƶ�����������Ϊ0
	m_pFolState->OnStepped(0);

	list<CFolMovement*>& lstMovement = m_pFolState->m_lstPendingMovement;

	CFolMovement* pLastMm=m_pFolState->GetLastMovement();

	//const CPixelPath* pOriginPath	=	pLastMm->GetOriginPath();

	const float fChangeDist=pCmd->fMovedDist;

	pLastMm->SetMovedDistWhenRemoteReached(fChangeDist);

	//������һ��·��������·����·���л�������꣬��������Ϊ����·������㡣
	CFPos BeginPixelPos;
	Ver( pLastMm->GetPixelPos( BeginPixelPos , fChangeDist ) );
#ifdef LOG_COREOBJ_MOVE
	if(BeginPixelPos != pCmd->BeginPixelPos)
	{
		ostringstream strm;
		strm<<"change_follower_move_target begin pos not equal. ("<< setprecision(12) <<pCmd->BeginPixelPos.x<<","<<pCmd->BeginPixelPos.y<<") != ("<<BeginPixelPos.x<<","<<BeginPixelPos.y<<")"; 
		CFPos beginPathPos;
		pLastMm->GetOriginPath()->GetBeginPixelPos(beginPathPos);
		strm << "client path: len = " << pLastMm->GetOriginPath()->GetPathPixelLength() << " beginpos " << beginPathPos.x << "," << beginPathPos.y; 
		const PathList& pathList = pLastMm->GetOriginPath()->GetGridPath()->GetPath();
		for(uint32 u = 0; u < pathList.size(); ++u)
			strm << " " << pathList[u].x << "," << pathList[u].y;
		LogMsg(strm.str());
	}
#endif

	CFPos EndPixelPos( pCmd->EndPixelPos );

	//cout << "CFMT " << pCmd->fSpeed << endl;
	//cout << "CMFT " << CConnClient::Inst()->GetEstimatedServerFrameTime() << endl;
	CFolMovement* pNewMm=CFolMovement::Create( GetScene(),pPathData, pCmd->udbPathDataLen,BeginPixelPos,EndPixelPos,eFMT_ChangePath,pCmd->fSpeed,pCmd->fEndDist, CConnClient::Inst()->GetEstimatedServerFrameTime());

	pNewMm->m_bChangePath=true;

	if( !pNewMm )
	{
		CGac2GasOC_Report_Invalid_Move cmd;
		CConnClient::Inst()->SendCoreCmd(&cmd);
#ifndef LOG_COREOBJ_MOVE
		GenErr("Invalid move");
#else
		return;
#endif
	}

	if( lstMovement.empty() )
	{
		//��ǰ����·���������һ��·������˵�ǰ���߽����п��ܳ����л���

		float fMovedDist=m_pFolState->GetMovedDist(GetMovState());

		if( fMovedDist >= fChangeDist )
		{
			//��ǰ���߽��ȳ������л��㣬ǿ�о���

			//���ĵ�ǰ·��
			delete m_pFolState->m_pCurMovement;
			m_pFolState->m_pCurMovement=pNewMm;

			//cout << GetGlobalID() << " setpos " << BeginPixelPos.x << "," << BeginPixelPos.y << endl;

#ifdef LOG_COREOBJ_MOVE
			ostringstream strm;
			strm << "CGas2GacOC_Change_Follower_Move_Target ��ǰ���߽��ȳ������л��㣬ǿ�о������� " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y << " -> " << BeginPixelPos.x << "," << BeginPixelPos.y;
			CFPos beginPathPos;
			CFPos EndPos;
			pNewMm->GetOriginPath()->GetBeginPixelPos(beginPathPos);
			pNewMm->GetOriginPath()->GetEndPixelPos(EndPos);
			strm << " new path: len = " << pNewMm->GetOriginPath()->GetPathPixelLength() << " beginpos " << beginPathPos.x << "," << beginPathPos.y << " endpos " << EndPos.x << "," << EndPos.y; 
			const PathList& pathList = m_pFolState->m_pCurMovement->GetOriginPath()->GetGridPath()->GetPath();
			for(uint32 u = 0; u < pathList.size(); ++u)
				strm << " " << pathList[u].x << "," << pathList[u].y;
			LogMsg(strm.str());
#endif

			if(!IntSetPosition(BeginPixelPos))
				GenErrAndLogInvalidPos(BeginPixelPos);
			//cout<<GetLocalID()<<"\tCGas2GacOC_Change_Follower_Move_Target"<<endl;
			//m_pFolState->ModifySpeed(0);	//ȷ��m_pFolState->GetRealSpeed()������0
			DoMove( pNewMm->ClonePixelPath(),m_pFolState->GetRealSpeed(),0 );
			if( !GetMovState() )
			{
#ifdef LOG_COREOBJ_MOVE
				LogMsg("Change_Follower_Move_Target GetMoveState() == NULL");
#endif
				GenErr("Change_Follower_Move_Target GetMoveState() == NULL");
			}
			GetMovState()->SetEndDist( pCmd->fEndDist );

			CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
				CCOEventMovePathChangedClient(this));
			
			//cout << "pass" << endl;
			return;
		}

		//cout << "in " << endl;
		//�ƶ�û�г���·���л��㣬��ȻҲ��û�г����ص㣬����GetMovStateҲ�Ͳ�������NULL��
		Ast( GetMovState() );

		Ver( GetMovState()->SetEndDist(fChangeDist) );
	}

#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "push new path. len = "  << pNewMm->GetPathLength();
	LogMsg(strm.str());

	strm.clear();
	strm << "cut path " << pLastMm->GetPathLength() << " -> " << fChangeDist;
	LogMsg(strm.str());
#endif
	lstMovement.push_back( pNewMm );

	//�޸����һ��δִ��·���ĳ��ȡ�����������������·����ִ�е�ʱ��Ͳ����ƶ�����·���л��㡣
	pLastMm->CutPath( fChangeDist );
}

void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Change_Follower_Move_Speed* pCmd)
{
	Ast( IsPassive() );
	if(! m_pFolState )
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("Change_Follower_Move_Speed m_pFolState == NULL");
#endif
		GenErr("Change_Follower_Move_Speed m_pFolState == NULL");
	}
	//cout << "med chane speed " << pCmd->fSpeed << endl;
	m_pFolState->SetIdealSpeed(pCmd->fSpeed);
}

//Mediator����FollowerĿǰ���ƶ�����
void CCoreObjectFollower::TellFollowerStepped(float fMovedDist)
{
	/*static uint32 uTime=uint32(GetProcessTime());
	uint32 uNewTime=uint32(GetProcessTime());

	//printf("%u\n",uNewTime-uTime);
	uTime=uNewTime;*/


	Ast(IsPassive());
	if(!m_pFolState)
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("Tell_Follower_Stepped m_pFolState == NULL");
#endif
		GenErr("Tell_Follower_Stepped m_pFolState == NULL");
	}
	if(fMovedDist < 0)
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("Tell_Follower_Stepped fMovedDist < 0");
#endif
	}
	m_pFolState->OnStepped(fMovedDist);
}

void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stepped_SmallDist* pCmd)
{
	TellFollowerStepped( ( (float)pCmd->uMovedDist )/1000 );
}

void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stepped_MiddleDist* pCmd)
{
	TellFollowerStepped( ( (float)pCmd->uMovedDist )/1000 );
}

void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stepped_BigDist* pCmd)
{
	TellFollowerStepped(pCmd->fMovedDist);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Check_Object_SyncVariant* pCmd)
{
	bool bSyncStaticSync = pCmd->uStaticVariantEncodeTimes > m_vecSyncVariantHolder[eCTI_Static]->GetRealDecodeTimes();
	bool bSyncAgileSync = pCmd->uAgileVariantEncodeTimes > m_vecSyncVariantHolder[eCTI_Agile]->GetRealDecodeTimes();

	if (bSyncStaticSync || bSyncAgileSync)
	{
		m_vecSyncVariantHolder[eCTI_Static]->EnableDecode(bSyncStaticSync);
		m_vecSyncVariantHolder[eCTI_Agile]->EnableDecode(bSyncAgileSync);
		if (bSyncStaticSync)
			m_vecSyncVariantHolder[eCTI_Static]->RecycleSubTree();
		if (bSyncAgileSync)
			m_vecSyncVariantHolder[eCTI_Agile]->RecycleSubTree();

		CGac2GasOC_Tell_Mediator_Sync_Target_SyncVariant cmd;
		cmd.uTargetGlobalID = pCmd->uqbGlobalID;
		cmd.bSyncStaticSync = bSyncStaticSync;
		cmd.bSyncAgileSync = bSyncAgileSync;
		cmd.uSessionID = pCmd->uSessionID;
		CConnClient::Inst()->SendCoreCmd(&cmd);
		return;
	}
	Ast( pCmd->uStaticVariantEncodeTimes == m_vecSyncVariantHolder[eCTI_Static]->GetRealDecodeTimes() );
	Ast( pCmd->uAgileVariantEncodeTimes == m_vecSyncVariantHolder[eCTI_Agile]->GetRealDecodeTimes() );
	SetSyncVariantInitEnd();
	m_bEnteredSyncAoi = true;
	if(GetHandler())
	{
		GetHandler()->OnEnteredSyncAoi();
	}
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Left_Sync_Aoi* pCmd)
{
	for (uint32 i=0; i < m_vecSyncVariantHolder.size(); i++)
	{
		m_vecSyncVariantHolder[i]->Reset();
	}
	m_bEnteredSyncAoi = false;
	if(GetHandler())
		GetHandler()->OnLeftedSyncAoi();
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Entered_Sync_Aoi* pCmd)
{
	if(GetHandler())
	{
		GetHandler()->OnEnteredSyncAoi();
	}
	m_vecSyncVariantHolder[eCTI_Static]->SetRealDecodeTimes(pCmd->uStaticVariantEncodeTimes);
	m_vecSyncVariantHolder[eCTI_Agile]->SetRealDecodeTimes(pCmd->uAgileVariantEncodeTimes);
	m_bEnteredSyncAoi = true;
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Init_Sync_Variant* pCmd)
{
	m_bEnteredSyncAoi = false;
	SetSyncVariantInitEnd();
}

void CCoreObjectFollower::SetViewVariantInitEnd()
{
	for (uint32 i=0; i<m_vecViewVariantHolder.size(); i++)
	{
		m_vecViewVariantHolder[i]->EnableDecode(false);
	}
}

void CCoreObjectFollower::SetSyncVariantInitEnd()
{
	for (uint32 i=0; i<m_vecSyncVariantHolder.size(); i++)
	{
		m_vecSyncVariantHolder[i]->EnableDecode(false);
	}
}

void CCoreObjectFollower::FlushAllPassiveMovement()
{
	Ast(IsPassive());

	m_pDelayedCmdMgr->FlushAllCommand();

	if(!m_pFolState)
		return;

#ifdef LOG_COREOBJ_MOVE
	LogMsg("IntStopDoingMove");
#endif
	IntStopDoingMove();	//����ȷʵ�ᴥ��OnMoveStopped�ص�������Follower������ص�ʲô����Ҳ��������

	CFolMovement* pMm=m_pFolState->m_pCurMovement;

	bool bPrevPathIsCut=pMm->PathIsCut();

	//cout << GetGlobalID() << " setpos " << pMm->GetMovementEndPixelPos().x << "," << pMm->GetMovementEndPixelPos().y << endl;
#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "FlushAllPassiveMovement " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y << " -> " << pMm->GetMovementEndPixelPos().x << "," << pMm->GetMovementEndPixelPos().y;  
	LogMsg(strm.str());
#endif

	const CFPos& EndPixelPos = pMm->GetMovementEndPixelPos();
	if(!IntSetPosition(EndPixelPos))
		GenErrAndLogInvalidPos(EndPixelPos);

	CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
		CCOEventMoveEndedClient(this,eMET_Reached));

	m_pFolState->m_pCurMovement=NULL;
	delete pMm;

	list<CFolMovement*>& lstMovement=m_pFolState->m_lstPendingMovement;

	while(!lstMovement.empty())
	{
		pMm=lstMovement.front();
		lstMovement.pop_front();

		//cout << GetGlobalID() << " setpos " << pMm->GetMovementBeginPixelPos().x << "," << pMm->GetMovementBeginPixelPos().y << endl;
		const CFPos& BeginPos = pMm->GetMovementBeginPixelPos();
		if(!IntSetPosition(BeginPos))
			GenErrAndLogInvalidPos(BeginPos);

		if(bPrevPathIsCut)
		{
			//����Ϊ�˱�֤����ص��Լ�rpc��Ϣ��ʱ�򣬱��������ص������ܷ���CO��Ϣ����
			CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
				CCOEventMovePathChangedClient(this));
		}
		else
		{
			//����Ϊ�˱�֤����ص��Լ�rpc��Ϣ��ʱ�򣬱��������ص������ܷ���CO��Ϣ����
			CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
				CCOEventMoveBeganClient(this));
		}

		bPrevPathIsCut=pMm->PathIsCut();

		//cout << GetGlobalID() << " setpos " << pMm->GetMovementEndPixelPos().x << "," << pMm->GetMovementEndPixelPos().y << endl;
#ifdef LOG_COREOBJ_MOVE
		ostringstream ostrm;
		ostrm << "FlushAllPassiveMovement ��������δ�����·�� SetPos = " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y << " -> " << pMm->GetMovementEndPixelPos().x << "," << pMm->GetMovementEndPixelPos().y;  
		LogMsg(ostrm.str());
#endif

		const CFPos& EndPos = pMm->GetMovementEndPixelPos();
		if(!IntSetPosition(EndPos))
			GenErrAndLogInvalidPos(EndPos);
		CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
			CCOEventMoveEndedClient(this,eMET_Reached));

		delete pMm;
	}

	if( bPrevPathIsCut )
	{
		CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
			CCOEventMoveEndedClient(this,eMET_Reached));
	}

#ifdef LOG_COREOBJ_MOVE
	LogMsg("Follower Delete m_pFolState");
#endif
	delete m_pFolState;
	m_pFolState=NULL;
}


void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Sync_Object_Pos* pCmd)
{
	FlushAllPassiveMovement();

	CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
		CCOEventPositionSet(this));
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Enable_Follower_Delay* pCmd)
{
#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "CGas2GacOC_Enable_Follower_Delay " << pCmd->uqbDelay;
	LogMsg(strm.str());
#endif

	if(pCmd->uqbDelay == 0 && MoveDelayEnabled())	//��ʱ��Ҫ�����������ӳ�����flush,�������ʱ������
		m_pDelayedCmdMgr->FlushAllCommand();

	m_uMoveDelay = pCmd->uqbDelay;
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Set_Object_Pos* pCmd)
{
	FlushAllPassiveMovement();

	CFPos Pos( pCmd->PixelPos );
	//cout << GetGlobalID() << " setpos " << Pos.x << "," << Pos.y << endl;
#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "CGas2GacOC_Set_Object_Pos " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y << " -> " << Pos.x << "," << Pos.y;
	LogMsg(strm.str());
#endif

	if(!IntSetPosition(Pos))
		GenErrAndLogInvalidPos(Pos);

	
	CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
		CCOEventPositionSet(this));
}

void CCoreObjectFollower::StopMovingWhenPassThrough()
{
#ifdef LOG_COREOBJ_MOVE
	LogMsg("IntStopDoingMove");
#endif
	IntStopDoingMove();

	Ast(m_pFolState->m_lstPendingMovement.empty());

	CFolMovement* pMm=m_pFolState->m_pCurMovement;

	//����ǿ��λ�þ���
	CFPos StopPixelPos;
	Ver( pMm->GetPixelPos(StopPixelPos,m_pFolState->GetRemoteLastPathMovedDist()) );
	//cout << GetGlobalID() << " setpos " << StopPixelPos.x << "," << StopPixelPos.y << endl;

#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "StopMovingWhenPassThrough " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y << " -> " << StopPixelPos.x << "," << StopPixelPos.y << " remote_moved_dist " << m_pFolState->GetRemoteLastPathMovedDist() << " " << pMm->GetMovedDistWhenRemoteSuspended();
	LogMsg(strm.str());
#endif

	m_pFolState->m_pCurMovement=NULL;

	if(!IntSetPosition(StopPixelPos))
		GenErrAndLogInvalidPos(StopPixelPos);

#ifdef LOG_COREOBJ_MOVE
	LogMsg("Follower Delete m_pFolState");
#endif
	delete pMm;
	delete m_pFolState;
	m_pFolState=NULL;

	CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
		CCOEventMoveEndedClient(this,eMET_Reached));
}

//void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Suspend_Follower_Moving* pCmd)
//{
//	if(!m_pFolState)
//		GenErr("Suspend_Follower_Moving m_pFolState == NULL");
//#ifdef LOG_COREOBJ_MOVE
//	LogMsg("SuspendMoving");
//#endif
//	m_pFolState->SuspendMoving(pCmd);
//}

//void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Resume_Follower_Moving* pCmd)
//{
//	if(!m_pFolState)
//		GenErr("Resume_Follower_Moving m_pFolState == NULL");
//
//#ifdef LOG_COREOBJ_MOVE
//	LogMsg("ResumeMoving");
//#endif
//	m_pFolState->ResumeMoving(pCmd->uqbSuspendedTime);
//}

void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Stopped* pCmd)
{
	if(m_bReceiveStopOrReachCmd)
	{
		ostringstream strmErr;
		strmErr << GetGlobalID();
		LogErr("Tell_Follower_Stopped Timing Err", strmErr.str());
	}
	m_bReceiveStopOrReachCmd = true;

	Ast(IsPassive());
	if(!m_pFolState)
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("Tell_Follower_Stopped m_pFolState == NULL");
#endif
		GenErr("Tell_Follower_Stopped m_pFolState == NULL");
	}

	list<CFolMovement*>& lstMovement=m_pFolState->m_lstPendingMovement;

	const float fStopDist=pCmd->fMovedDist;

	CFolMovement* pMm;

#ifdef LOG_COREOBJ_MOVE
	pMm = m_pFolState->GetLastMovement();
	CFPos stoppedPos;
	pMm->GetPixelPos(stoppedPos, fStopDist);
	m_pFolState->m_ServerReachedPos = stoppedPos;
	ostringstream strm;

	strm << "CGas2GacOC_Tell_Follower_Stopped server stopped at " << stoppedPos.x << "," << stoppedPos.y << " stopdist " << fStopDist;
	LogMsg(strm.str());
#endif


	if( lstMovement.empty() )
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("CGas2GacOC_Tell_Follower_Stopped\tδ���ߵ�·������Ϊ��");
#endif
		//δ���ߵ�·������Ϊ�գ���˾���Ҫֹͣ��ǰ�������ߵ�·����
		pMm=m_pFolState->m_pCurMovement;

		//cout << GetMovState() << endl;
		if( (!GetMovState()) || (m_pFolState->GetMovedDist(GetMovState())>=fStopDist) )
		{
			pMm->SetMovedDistWhenRemoteReached(fStopDist);
			m_pFolState->OnStepped(fStopDist);
			//�����˷���˶����ƶ����ȣ�����ǿ��λ�þ���
			StopMovingWhenPassThrough();
			return;
		}

		//�ͻ��˶����ƶ�����û�г�������˶����޸����һ������·����ֹͣ���뼴�ɡ�
		GetMovState()->SetEndDist(fStopDist);
	}
	else
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("CGas2GacOC_Tell_Follower_Stopped\tδ���ߵĶ��� * �� * Ϊ��");
#endif
		//δ���ߵĶ��в�Ϊ�գ��޸����һ������·����ֹͣ���뼴�ɡ�
		pMm=lstMovement.back();
	}

	pMm->CutPath( fStopDist );
	//��Ƿ���˶�������������·��
	m_pFolState->OnStepped(-1);
	pMm->SetMovedDistWhenRemoteReached(pMm->GetPathLength());
}


void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Tell_Follower_Reached* pCmd)
{
	if(m_bReceiveStopOrReachCmd)
	{
		ostringstream strmErr;
		strmErr << GetGlobalID();
		LogErr("Tell_Follower_Reached Timing Err", strmErr.str());
	}
	m_bReceiveStopOrReachCmd = true;

	Ast(IsPassive());
	if(!m_pFolState)
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("Tell_Follower_Reached m_pFolState == NULL");
#endif
		GenErr("Tell_Follower_Reached m_pFolState == NULL");
	}
	//cout<<GetLocalID()<<"\tCGas2GacOC_Tell_Follower_Reached"<<endl;
	//-1����˼�Ƿ���˶����Ѿ����������·���������ƶ����Ⱦ���·�����������ȣ��������޸ĺ�ĳ̶ȣ���
	m_pFolState->OnStepped(-1);
	CFolMovement* pMm = m_pFolState->GetLastMovement();
	pMm->SetMovedDistWhenRemoteReached(pMm->GetPathLength());

#ifdef LOG_COREOBJ_MOVE
	m_pFolState->m_ServerReachedPos = pCmd->ReachedPixelPos;
	m_pFolState->m_fServerReachedDist = pCmd->EndDist;
#endif

#ifdef LOG_COREOBJ_MOVE
	ostringstream strm;
	strm << "CGas2GacOC_Tell_Follower_Reached ReachedPixelPos = " << setprecision(12) << GetPixelPos().x << "," << GetPixelPos().y << " -> " << pCmd->ReachedPixelPos.x << "," << pCmd->ReachedPixelPos.y;
	LogMsg(strm.str());
#endif

	if( GetMovState() )
		return;

#ifdef LOG_COREOBJ_MOVE
	LogMsg("Follower Delete m_pFolState");
#endif
	delete m_pFolState->m_pCurMovement;
	m_pFolState->m_pCurMovement=NULL;
	delete m_pFolState;
	m_pFolState=NULL;
	CCoreObjectClient::OnEndMoveDerived();

#ifdef LOG_COREOBJ_MOVE
	if( GetPixelPos() != pCmd->ReachedPixelPos )
	{
		ostringstream oss;
		oss<<"("<< setprecision(12) <<GetPixelPos().x<<","<<GetPixelPos().y
			<<"),("<<pCmd->ReachedPixelPos.x<<","<<pCmd->ReachedPixelPos.y<<")";
		LogMsg(oss.str());
		GenErr("Reached Pixelpos not equal. ", oss.str());
	}
#endif
}

void CCoreObjectFollower::OnEndMoveDerived()
{
	//printf("follower move ended\n");
	//cout<<GetLocalID()<<"\tCCoreObjectFollower::OnEndMoveDerived\t"<<GetGlobalID()<<endl;
	CFolMovement* pMm=m_pFolState->m_pCurMovement;

	list<CFolMovement*>& lstMovement=m_pFolState->m_lstPendingMovement;

	if( lstMovement.empty() )
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("Follower move ended\tû�к���·��");
#endif
		//û�к���·��
		if( !m_pFolState->RemoteObjectReached() )
		{
			//����˶���û�������·����ʲôҲ�������ȴ�Reached��ChangePath֪ͨ
			return;
		}

#ifdef LOG_COREOBJ_MOVE
		LogMsg("Follower Delete m_pFolState");
#endif
		//����˶����Ѿ�������ϣ�����Ӧ�ô���������ϵĻص�
		delete pMm;
		m_pFolState->m_pCurMovement=NULL;

#ifdef LOG_COREOBJ_MOVE
		if( GetPixelPos() != m_pFolState->m_ServerReachedPos )
		{
			ostringstream oss;
			oss<<"("<< setprecision(12) <<GetPixelPos().x<<","<<GetPixelPos().y
				<<"),("<<m_pFolState->m_ServerReachedPos.x<<","<<m_pFolState->m_ServerReachedPos.y<<")";
			LogMsg(oss.str());
			GenErr("Reached Pixelpos not equal. ", oss.str());
		}
#endif
		delete m_pFolState;
		m_pFolState=NULL;

		CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
			CCOEventMoveEndedClient(this,eMET_Reached));
	}
	else
	{
#ifdef LOG_COREOBJ_MOVE
		LogMsg("Follower move ended\t�к���·��");
#endif
		//�к���·��
		CFolMovement* pNextMm=lstMovement.front();
		lstMovement.pop_front();

		delete pMm;
		pMm=m_pFolState->m_pCurMovement=pNextMm;

		bool bNewMoveTo=pNextMm->m_eType==eFMT_MoveTo;

		CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
			CCOEventMoveEndedClient(this,eMET_Reached));

		CFPos BeginPixelPos;
		pMm->GetOriginPath()->GetBeginPixelPos(BeginPixelPos);
		if( GetPixelPos() != BeginPixelPos )
		{
			ostringstream strm;
			strm<<"("<< setprecision(12) <<GetPixelPos().x<<","<<GetPixelPos().y<<") != ("<<BeginPixelPos.x<<","<<BeginPixelPos.y<<")"<<pMm->m_bChangePath;
			strm << " pathlen = " << pMm->GetOriginPath()->GetPathPixelLength(); 
			CPos beginGridPos;
			pMm->GetOriginPath()->GetGridPath()->GetPos(beginGridPos, 0);
			strm << " beginGridPos = " << beginGridPos.x << "," << beginGridPos.y;
#ifdef LOG_COREOBJ_MOVE
			LogMsg(strm.str());
#else
			GenErr("BeginPixelPos not equal. ", strm.str());
#endif
		}
		//����·��ʱ��õ�EndPixelPos��ʵ�����ߵ���EndPixelPosλ�ò�ͬ
		//m_pFolState->ModifySpeed(0);	//ȷ��m_pFolState->GetRealSpeed()������0
		DoMove(pMm->ClonePixelPath(),m_pFolState->GetRealSpeed(),0);

		//���·�����ضϹ�����Ҫ�޸���·���������ƶ��������
		Ver( GetMovState()->SetEndDist(pMm->GetPathLength()) );


		if( bNewMoveTo )
		{
			CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
				CCOEventMoveBeganClient(this));
		}
		else
		{
			CCoreMsgMgr::Inst().PostMsg(new (CCoreMsgMgr::Inst())
				CCOEventMovePathChangedClient(this));
		}
	}
}

float CCoreObjectFollower::GetRealMovedDistDerived()const
{
	float fRealMovedDist;
	if(m_pFolState)
	{
		CFolMovement* pMm = m_pFolState->m_pCurMovement;
		if(pMm->RemoteReached())
			fRealMovedDist = min(CCoreObjectClient::GetRealMovedDistDerived(), pMm->GetMovedDistWhenRemoteReached());

		float fRemoteStoppedDistOfCurMov = pMm->GetMovedDistWhenRemoteSuspended();
		if(fRemoteStoppedDistOfCurMov >= 0)
			fRealMovedDist = min(CCoreObjectClient::GetRealMovedDistDerived(), fRemoteStoppedDistOfCurMov);
	}

#ifdef USE_RENDER_SLOT
	CObjMoveStateClient* pMoveState = GetMovState();
	fRealMovedDist = limit2(pMoveState->IntGetNewMoveDistDerived(), pMoveState->GetMovedDist(), pMoveState->GetPathLength());
	//cout << "GetRealMovedDistDerived " << fRealMovedDist << endl;
#else
	fRealMovedDist = CCoreObjectClient::GetRealMovedDistDerived();
#endif
	return fRealMovedDist;
}

bool CCoreObjectFollower::HasUnfinishedPassiveMovement()const
{
	return m_pFolState || !m_pDelayedCmdMgr->IsEmpty();
}


void CCoreObjectFollower::OnDelayedServerCommand(const CGas2GacOC_Validate_Follower_Pos* pCmd)
{
	Ast(m_pFolState);

	CFPos PixelPos=m_pFolState->GetServerObjectPixelPos();
	if(  PixelPos != pCmd->PixelPos )
	{
		CGac2GasOC_Report_Invalid_Step cmd;
		CConnClient::Inst()->SendCoreCmd(&cmd);
	}
}

bool CCoreObjectFollower::MoveDelayEnabled()const
{
	return m_uMoveDelay != 0;
}

uint32 CCoreObjectFollower::GetMoveDelay()const
{
	return m_uMoveDelay;
}

void CCoreObjectFollower::GenVariantDecodeError(const char* szVariantTypeName,uint32 uEncodeTimes,uint32 uDecodeTimes)
{
	ostringstream strm;
	strm << szVariantTypeName << "����������˳��ƥ��" << endl
		<< "����ʱ������" << uEncodeTimes << endl
		<< "����ʱ������" << uDecodeTimes << endl;		
	LogErr(strm.str());
}

template<typename CmdType>
void CCoreObjectFollower::UpdateSyncVariant(const CmdType* pCmd, ECodeTableIndex eIndex)
{
	const CSyncVariantClientHolder* pHolder = GetSyncVariantHolder(eIndex);
	if ( const_cast<CSyncVariantClientHolder*>(pHolder)->Decode( pCmd , m_bEnteredSyncAoi, "SyncVariant") )
		return;

	//GenVariantDecodeError("SyncVariant" , pCmd->GetEncodeTimes() , pHolder->GetDecodeTimes() );
}

template<typename CmdType>
void CCoreObjectFollower::DecompressUpdateSyncVariant(const CmdType* pCmd)
{
	//if ( m_pSyncVariantHolder->DecompressDecode( pCmd ) )
	//	return;
}

template<typename CmdType>
void CCoreObjectFollower::UpdateViewVariant(const CmdType* pCmd, ECodeTableIndex eIndex)
{
	//const CSyncVariantClientHolder* pHolder = GetViewVariantHolder(eCTI_Static);
	const CSyncVariantClientHolder* pHolder = GetViewVariantHolder(eIndex);
	if ( const_cast<CSyncVariantClientHolder*>(pHolder)->Decode( pCmd , m_bEnteredViewAoi, "ViewVariant") )
		return;

	//GenVariantDecodeError("ViewVariant" , pCmd->GetEncodeTimes() , pHolder->GetDecodeTimes() );
}

template<typename CmdType>
void CCoreObjectFollower::DecompressUpdateViewVariant(const CmdType* pCmd)
{
	//if ( m_pViewVariantHolder[]->DecompressDecode( pCmd ) )
	//	return;
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Static_Sync_Variant_Small_Data* pCmd)
{
	UpdateSyncVariant(pCmd, eCTI_Static);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Static_Sync_Variant_Middle_Data* pCmd)
{
	UpdateSyncVariant(pCmd, eCTI_Static);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Static_Sync_Variant_Big_Data* pCmd)
{
	UpdateSyncVariant(pCmd, eCTI_Static);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Agile_Sync_Variant_Small_Data* pCmd)
{
	UpdateSyncVariant(pCmd, eCTI_Agile);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Agile_Sync_Variant_Middle_Data* pCmd)
{
	UpdateSyncVariant(pCmd, eCTI_Agile);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Agile_Sync_Variant_Big_Data* pCmd)
{
	UpdateSyncVariant(pCmd, eCTI_Agile);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Static_View_Variant_Small_Data* pCmd)
{
	UpdateViewVariant(pCmd, eCTI_Static);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Static_View_Variant_Middle_Data* pCmd)
{
	UpdateViewVariant(pCmd, eCTI_Static);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Static_View_Variant_Big_Data* pCmd)
{
	UpdateViewVariant(pCmd, eCTI_Static);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Agile_View_Variant_Small_Data* pCmd)
{
	UpdateViewVariant(pCmd, eCTI_Agile);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Agile_View_Variant_Middle_Data* pCmd)
{
	UpdateViewVariant(pCmd, eCTI_Agile);
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Agile_View_Variant_Big_Data* pCmd)
{
	UpdateViewVariant(pCmd, eCTI_Agile);
}


void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Sync_Dynamic_Barrier* pCmd)
{
	//���¶�̬�ϰ���Ϣ
	SetBarrier(pCmd->fBarrierSize, static_cast<EBarrierType>(pCmd->usbBarrierType));
}

void CCoreObjectFollower::SetBarrier(float fBarrierSize, EBarrierType eBarrierType)
{
	if(fBarrierSize < 0)
		GenErr("error::BarrierSize is minus");
	if(fBarrierSize > 0 && eBarrierType <= eBT_NoBarrier)
		GenErr("error::BarrierType is illegal");

	//cout << "sb " << fBarrierSize << " " << uint32(eBarrierType) << endl;
	//if(eBarrierType == eBT_HighBarrier)
	//	cout << "sb" << endl;

	if(fBarrierSize != m_fBarrierSize)
	{
		UpdateSceneBarrier(m_fBarrierSize, eBT_NoBarrier);
		UpdateSceneBarrier(fBarrierSize, m_eBarrierType);
		m_fBarrierSize = fBarrierSize;
	}

	if(eBarrierType != m_eBarrierType)
	{
		if(m_eBarrierType != eBT_NoBarrier)
		{
			UpdateSceneBarrier(m_fBarrierSize, eBT_NoBarrier);
		}
		UpdateSceneBarrier(m_fBarrierSize, eBarrierType);
		m_eBarrierType = eBarrierType;
	}
}

void CCoreObjectFollower::UpdateSceneBarrier(float fBarrierSize, EBarrierType eBarrierType)
{
	//uint8 uDBRadius = fBarrierSize == 0.0f ? 0 : uint8(fBarrierSize) + 1;
	uint8 uDBRadius = uint8(fBarrierSize * 2);

	BarrierArea barrierArea;
	CMetaSceneClient* pMetaScene = GetScene()->GetMetaScene();
	CalcBarrierArea(barrierArea, GetPixelPos(), uDBRadius, pMetaScene->GetWidthInGrid(), pMetaScene->GetHeightInGrid());

	GetScene()->SetDynamicBarrierInfo(GetGlobalID(), barrierArea, eBarrierType);
}

void CCoreObjectFollower::GetServerPixelPos(CFPos& PixelPos)const
{
	if (!m_pFolState)
	{
		PixelPos=GetPixelPos();
	}
	else
	{
		PixelPos=m_pFolState->GetServerObjectPixelPos();
	}
}

float CCoreObjectFollower::GetBarrierSize()const
{
	return m_fBarrierSize;
}

bool CCoreObjectFollower::HasBarrier()const
{
	return m_fBarrierSize > 0;
}

void CCoreObjectFollower::ClearBarrier()
{
	if(HasBarrier())
	{
		SetBarrier(0, eBT_NoBarrier);
	}
}

void CCoreObjectFollower::IntSetPositionDerived(const CFPos& PixelPos)
{
	CPos posOld, posNew;
	GetGridPos(posOld);
	posNew.x = GetGridByPixel(PixelPos.x);
	posNew.y = GetGridByPixel(PixelPos.y);

	if(posOld != posNew)
	{
		if( HasBarrier() )
			UpdateSceneBarrier(m_fBarrierSize, eBT_NoBarrier);

		CCoreObjectClient::IntSetPositionDerived(PixelPos);

		if( HasBarrier() )
			UpdateSceneBarrier(m_fBarrierSize, m_eBarrierType);
	}
	else
	{
		CCoreObjectClient::IntSetPositionDerived(PixelPos);
	}
}

#ifdef USE_RENDER_SLOT
void CCoreObjectFollower::OnSpeedAdjust(uint32 uMoveInterval)
{
	if(m_pFolState)
	{
		m_pFolState->AdjustSpeed(uMoveInterval);
	}
}
#endif

float CCoreObjectFollower::Distance(const CCoreObjectFollower* pCoreObj)const
{
	Ast(pCoreObj);
	CFPos posSelf, posTarget;
	posSelf = GetPixelPos();
	posTarget = pCoreObj->GetPixelPos();
	float fDist = posSelf.Dist(posTarget);
	return fDist;
	//float fOffSet = (GetBarrierSize() + pCoreObj->GetBarrierSize()) * eGridSpanForObj;
	//fDist -= fOffSet;
	//return max(0.0f, fDist);
}

CPixelPath* CCoreObjectFollower::GetClonedMovePath()const
{
	if (m_pFolState)
	{
		return m_pFolState->m_pCurMovement->ClonePixelPath();
	}
	return NULL;
}

void CCoreObjectFollower::LogMsg(const string& log)
{
	ostringstream strm;
	strm << GetGlobalID() << "\t" << log;
	CLogCoreObject::Inst().LogMsg(strm.str());
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Bind_To* pCmd)
{
	if(pCmd->bRet)
	{
		CCoreObjectFollower* pBindParent = GetObjectFollower(pCmd->uParentID);
		if(pBindParent &&
			(m_uPendingBindParentGID == 0 ||
			m_mapPendingBindRelation.find(m_uPendingBindParentGID) != m_mapPendingBindRelation.end()))
		{
			//cout << GetGlobalID() << " Global PreBindTo " << pCmd->uParentID << "\n";
			BindTo(pBindParent);
			return;
		}
	}
	//��ʾ��ʧ�ܵ���Ϣ
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Disbind_From_Parent* pCmd)
{
	//cout << GetGlobalID() << " Global PreDisbindFromParent\n";
	DisbindFromParent();
}

void CCoreObjectFollower::OnServerCommand(const CGas2GacOC_Disbind_All_Children* pCmd)
{
	//cout << GetGlobalID() << " Global PreDisbindToAllChild\n";
	DisbindAllChildren();
}

bool CCoreObjectFollower::BindTo(uint32 uParentLID)
{
	return m_pBindObj->BindTo(uParentLID);
}

bool CCoreObjectFollower::BindTo(CCoreObjectFollower* pParent)
{
	return m_pBindObj->BindTo(pParent);
}

bool CCoreObjectFollower::DisbindFromParent()
{
	return m_pBindObj->DisbindFromParent();
}

bool CCoreObjectFollower::DisbindChild(uint32 uChildLID)
{
	return m_pBindObj->DisbindChild(uChildLID);
}

bool CCoreObjectFollower::DisbindAllChildren()
{
	return m_pBindObj->DisbindAllChildren();
}

bool CCoreObjectFollower::IsBindingObj() const
{
	return m_pBindObj->IsBindingObj();
}

void CCoreObjectFollower::OnBindTo(uint32 uParentLID, bool bRet)
{
	//���´�BindTo�ƹ���
	if(bRet)
	{
		CCoreObjectClient* pParent = this->GetObject(uParentLID);
		if(pParent)
		{
			CFPos fParentPos;
			pParent->GetPixelPos(fParentPos);
			IntSetPosition(fParentPos);
		}
	}
}

void CCoreObjectFollower::OnBeforeBindTo(uint32 uParentLID, bool bRet)
{
	if(m_uPendingBindParentGID != 0)
	{
		if(m_mapPendingBindRelation.erase(m_uPendingBindParentGID) > 0)
		{
			//cout << "OnBindTo���Ԥ�󶨹�ϵ��SID = " << GetGlobalID() << " -> " << m_uPendingBindParentGID << "\n";
		}
		m_uPendingBindParentGID = 0;
	}
	//��BindTo�ƹ�����
	if(bRet && GetMovState())
		IntStopMoving();
}

void CCoreObjectFollower::OnBeforeDisbindFromParent()
{
	if(m_uPendingBindParentGID != 0)
	{
		if(m_mapPendingBindRelation.erase(m_uPendingBindParentGID) > 0)
		{
			//cout << "OnDisbindFromParent���Ԥ�󶨹�ϵ��SID = " << GetGlobalID() << " -> " << m_uPendingBindParentGID << "\n";
		}
		m_uPendingBindParentGID = 0;
	}
}

void CCoreObjectFollower::OnBeforeDisbindToChild(uint32 uChildLID)
{
	uint32 uSelfGID = GetGlobalID();
	for(MapPendingBindRelation::iterator itr = m_mapPendingBindRelation.find(uSelfGID);
		itr != m_mapPendingBindRelation.end() && itr->first == uSelfGID; ++itr)
	{
		//��uChildID ת����ȡ CCoreObjectFollower��GetGlobalID();
		CCoreObjectClient* pObjClient = this->GetObject(uChildLID);
		if(pObjClient)
		{
			switch(pObjClient->GetType())
			{
			case eCOT_Follower:
			case eCOT_Director:
				{
					CCoreObjectFollower* pObjFollower = class_cast<CCoreObjectFollower*>(pObjClient);
					if(pObjFollower && itr->second == pObjFollower->GetGlobalID())
					{
						m_mapPendingBindRelation.erase(itr);
						return;
					}
				}
			}
		}
	}
}

void CCoreObjectFollower::OnBeforeDisbindToAllChild()
{
	//ע�⣺����������ʱ���õĴ˺��������Ӷ���ȸ�����Size�󣨻���������ĸ���������ͬʱ�ܿ����Ӷ��󿴲���������Ŀ����ԣ�������£�
	//���뽫ʵ�󶨹�ϵ��ת����󶨹�ϵ�����򸸶����ٴν��루��������ʱ���޷�����ʵ�󶨹�ϵ��
	//��������������Ȼ�����⣬������������ǿ�е���DisbindAllChildren���Ӷ��󲢲�����������Ϊ�����󷢵�DisbindAllChildren��Ϣû�б��յ�
	//������󶨹�ϵδ����󣨳��ǿ���DisbindAllChildren�Ĺ��ܣ����߷��������ʱ���ӷ���Ϣ��������Ҫ����ƣ�
	//��ˣ�Ŀǰ������������AOI���Ӷ����ܱ��˿���ʱ��������Ҳǿ�б�������ͨ���ĸ���Size��Stealth���Լ��Ը�����Size == 0 �͸��Ӷ���ViewGroup����ȵ��쳣�жϣ�
	if(m_mapPendingBindRelation.erase(GetGlobalID()) > 0)
	{
		//cout << "OnBeforeDisbindToAllChild���Ԥ�󶨹�ϵ��SID = " << GetGlobalID() << "\n";
	}
}

bool CCoreObjectFollower::BindObjIntSetPosition(const CFPos& PixelPos)
{
	return m_pBindObj->BindObjIntSetPosition(PixelPos);
}

//void CCoreObjectFollower::BindObjNotifyPosChangedIfNeed()
//{
//	m_pBindObj->BindObjNotifyPosChangedIfNeed();
//}

CCoreObjectFollower* CCoreObjectFollower::GetBindObjParent() const
{
	uint32 uParentID = m_pBindObj->GetBindParentID();
	if(uParentID == 0)
		return NULL;
	
	CCoreObjectClient* pParent = CCoreObjectFollower::GetObject(uParentID);
	if(!pParent)
		return NULL;

	CCoreObjectFollower* pParentFollower = class_cast<CCoreObjectFollower*>(pParent);
	return pParentFollower;
}

CCoreObjectFollower* CCoreObjectFollower::MoveFirstBindObjChild()
{
	TBindObject<CCoreObjectFollower>::SetUInt32& setBindChildID = m_pBindObj->GetBindChildIDSet();

	m_itrBindObjChild = setBindChildID.begin();
	if(m_itrBindObjChild == setBindChildID.end())
		return NULL;		//EOS
	CCoreObjectClient* pCurChild = CCoreObjectFollower::GetObject((uint32)(*m_itrBindObjChild));
	CCoreObjectFollower* pCurChildFollower = pCurChild ? static_cast<CCoreObjectFollower*>(pCurChild) : NULL;
	while(!pCurChildFollower)
	{
		++m_itrBindObjChild;
		if(m_itrBindObjChild == setBindChildID.end())
			return NULL;	//EOS

		pCurChild = CCoreObjectFollower::GetObject(*m_itrBindObjChild);
		pCurChildFollower = pCurChild ? static_cast<CCoreObjectFollower*>(pCurChild) : NULL;
	}
	return pCurChildFollower;
}

CCoreObjectFollower* CCoreObjectFollower::MoveNextBindObjChild()
{
	TBindObject<CCoreObjectFollower>::SetUInt32& setBindChildID = m_pBindObj->GetBindChildIDSet();

	++m_itrBindObjChild;
	if(m_itrBindObjChild == setBindChildID.end())
		return NULL;		//EOS
	CCoreObjectClient* pCurChild = CCoreObjectFollower::GetObject(*m_itrBindObjChild);
	CCoreObjectFollower* pCurChildFollower = pCurChild ? static_cast<CCoreObjectFollower*>(pCurChild) : NULL;
	while(!pCurChildFollower)
	{
		++m_itrBindObjChild;
		if(m_itrBindObjChild == setBindChildID.end())
			return NULL;	//EOS

		pCurChild = CCoreObjectFollower::GetObject(*m_itrBindObjChild);
		pCurChildFollower = pCurChild ? static_cast<CCoreObjectFollower*>(pCurChild) : NULL;
	}
	return pCurChildFollower;
}

bool CCoreObjectFollower::EndOfBindObjChild() const
{
	TBindObject<CCoreObjectFollower>::SetUInt32& setBindChildID = m_pBindObj->GetBindChildIDSet();
	return m_itrBindObjChild == setBindChildID.end();
}

CCoreObjectFollower* CCoreObjectFollower::GetCurBindObjChild() const
{
	uint32 uCurBindObjChildID = *m_itrBindObjChild;
	if(uCurBindObjChildID == 0)
		return NULL;

	CCoreObjectClient* pCurBindObjChild = CCoreObjectFollower::GetObject(uCurBindObjChildID);
	if(!pCurBindObjChild)
		return NULL;

	CCoreObjectFollower* pCurBindObjChildFollower = class_cast<CCoreObjectFollower*>(pCurBindObjChild);
	return pCurBindObjChildFollower;
}

void CCoreObjectFollower::GenErrAndLogInvalidPos(const CFPos& pos)const
{
	CMetaSceneClient* pMetaScene=GetScene()->GetMetaScene();
	const int32 nRatio = eGridSpanForObj / eGridSpan;

	ostringstream strm;
	strm << "(" << pos.x<< "," << pos.y << ")>(" << float(pMetaScene->GetWidthInPixel()) * nRatio << "," << float(pMetaScene->GetHeightInPixel()) * nRatio << " in " << pMetaScene->GetSceneName();
	GenErr("SetPos failed", strm.str());
}
