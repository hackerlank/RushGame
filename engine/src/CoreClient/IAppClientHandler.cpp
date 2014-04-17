#include "stdafx.h"
#include "IAppClientHandler.h"
#include "CCoreSceneClient.h"
#include "CFileWatcherData.h"
#include "CRenderSystem.h"

IAppClientHandler::IAppClientHandler() //����ӿ�Set��Appд����TApp�У���ʱ�ȴ�����ط����ý�ȥ���Ժ����뷽������˳����ط�
{
	CRenderSystem::GetInst()->SetProcessCallBack(this);
}

void IAppClientHandler::OnMainSceneCreated(CCoreSceneClient* pCoreScene) 
{
	CALL_CLASS_CALLBACK_1(pCoreScene) 
}

void IAppClientHandler::OnFileChanged(CFileWatcherData* pFileDataVec)
{
	CALL_CLASS_CALLBACK_1(pFileDataVec); 
}
