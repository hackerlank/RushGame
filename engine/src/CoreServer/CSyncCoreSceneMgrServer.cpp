#include "stdafx.h"
#include "CSyncCoreSceneServer.h"
#include "CSyncCoreSceneMgrServer.h"
#include "TCoreSceneMgr.inl"
#include "TWhole.inl"
#include "CAoiThreadMgr.h"
#include "CTimeCheckPoint.h"
#include "CSyncAppServer.h"
#include "CCoreSceneServer.h"
#include "ExpHelper.h"
#include "DebugHelper.h"

template class TCoreSceneMgr<CSyncTraitsServer>;
template class TWhole<CSyncCoreSceneMgrServer,CSyncCoreSceneServer>;

CSyncCoreSceneMgrServer::CSyncCoreSceneMgrServer()
:m_bHalting( false )
{
	CAoiThreadMgr::Init(1);
}

CSyncCoreSceneMgrServer::~CSyncCoreSceneMgrServer()
{
	ClearParts();

	CTimeCheckPoint tcp;
	cout << "�ر�aoi�߳�";
	CAoiThreadMgr::Unit();
	cout << "����ʱ��" << tcp.GetElapse() << endl;
}

CSyncCoreSceneServer* CSyncCoreSceneMgrServer::
CreateCoreScene(uint32 uScenID, CSyncMetaSceneServer* pMetaScene, const char* szSyncAoiFileName, CDimensionInfo* DimInfo, uint32 uSlot)
{
	if( Halting() )
		GenErr("Can't create CoreScene  while application is doing quit!");
	if(!pMetaScene || !szSyncAoiFileName)
		return NULL;

	CSyncCoreSceneServer* pSynCoreScene  = new CSyncCoreSceneServer(uScenID, pMetaScene, this, szSyncAoiFileName, DimInfo);
	pSynCoreScene->CreateQuadScene(uSlot);



	return pSynCoreScene;
}

bool CSyncCoreSceneMgrServer::DestroyCoreScene(CSyncCoreSceneServer* pScene)
{
	return pScene->BeginDestroyScene();
}

uint32 CSyncCoreSceneMgrServer::Send(const void* pData,uint32 uSize)
{
	for(list<CSyncCoreSceneServer*>::const_iterator it=GetParts().begin();it!=GetParts().end();++it)
		static_cast<ISend*>(*it)->Send(pData,uSize);

	return uSize;
}

bool CSyncCoreSceneMgrServer::Halting()const
{
	return m_bHalting;
}

bool CSyncCoreSceneMgrServer::Halt()
{
	if( Halting() )
		return false;

	m_bHalting = true;	

	if( HasPart() )
	{
		for(list<CSyncCoreSceneServer*>::const_iterator it=GetParts().begin();it!=GetParts().end();++it)
			(*it)->BeginDestroyScene();
	}
	else
	{
		//���е�CoreScene���Ѿ���ɾ��������������ɵ�֪ͨ��
		CSyncAppServer::Inst()->OnSceneMgrHalted();
	}

	CSyncAppServer::Inst()->Log("Halt SyncSceneMgr.");

	return true;
}

void CSyncCoreSceneMgrServer::OnACoreSceneDestroyed()
{
	//�����������Ǵ��ڹر�״̬
	if( !Halting() )
		return;

	//������Ȼû�б�ɾ�����
	if( HasPart() )
	{
		ostringstream strm;
		
		strm<<"ʣ��"<<GetParts().size()<<"������δɾ��";
		CSyncAppServer::Inst()->Log(strm.str().c_str());
	
		return;
	}

	CSyncAppServer::Inst()->Log("SyncSceneMgr halted.");

	CSyncAppServer::Inst()->OnSceneMgrHalted();
}
