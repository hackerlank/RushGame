#include "stdafx.h"
#include "CConnMgrStrHandler.h"
#include "BaseHelper.h"
#include "CStrMsgDispatcher.h"
#include "CBaseMsgDispatcher.inl"
#include "CBaseConnMgrHandler.inl"
#include "IRpcCommon.h"
#include "TMsgDispatcher_inl.inl"
#include "TMsgHandler.inl"
#include "TComMsgDispatcher.inl"


CConnMgrStressHandler::CConnMgrStressHandler()
	: m_pFighterMsgDispatcher(new CStressFighterMsgDispatcher)
	, m_pCharacterMsgDispatcher(new CStressCharacterMsgDispatcher)
{
	CStressFighterMsgDispatcher::InitMsgHandlerContainer();
	CStressFighterMsgDispatcher::RegisterMsgHandler();
	CStressCharacterMsgDispatcher::InitMsgHandlerContainer();
	CStressCharacterMsgDispatcher::RegisterMsgHandler();
}

CConnMgrStressHandler::~CConnMgrStressHandler()
{
	SafeDelete(m_pFighterMsgDispatcher);
	SafeDelete(m_pCharacterMsgDispatcher);

	CStressFighterMsgDispatcher::UninitMsgHandlerContainer();
	CStressCharacterMsgDispatcher::UninitMsgHandlerContainer();
}

void CConnMgrStressHandler::OnDataReceived(CConnStress *pConn)
{
	
	while(pConn->GetRecvDataSize() >= 2)
	{
		char *pBuf = reinterpret_cast<char *>(pConn->GetRecvData());
		size_t dataSize = pConn->GetRecvDataSize();

		uint16 id = *(uint16*)(pBuf);

		//���Э���С��32767������lua���͵�rpcЭ�飬ת�Ƶ�lua������������c++����ַ�
		if(id < eLua_CPP_Split_ID)
		{
			SQR_TRY{
				GetScriptHandler()->OnDataReceived(pConn);
			}
			//�˴��ӵ����쳣ֻ�����ǵ���rpc����ʱ��������������⵼�µ�
			//�������쳣���������unpack��ʱ����Ѿ��Ͽ�������
			SQR_CATCH(exp){
				clog << exp.ErrorTitle() << endl << exp.ErrorMsg() << endl;
				clog << exp.StackInfo() << endl;
				pConn->OnCaughtNetException();
			}
			SQR_TRY_END;

			//�����ȣ���֤�����ݳ��Ȳ�����������Ҫ�����ȴ�������������
			if(dataSize == pConn->GetRecvDataSize())
				break;
		}
		else
		{
			SQR_TRY
			{
				if (id > eGas2GacCPP_Fighter_Begin && id < eGas2GacCPP_Fighter_End)
				{
					if(!DoDispatch(m_pFighterMsgDispatcher, pConn, pBuf))
						break;
				}
				else if (id > eGas2GacCPP_Character_Begin && id < eGas2GacCPP_Character_End)
				{
					if(!DoDispatch(m_pCharacterMsgDispatcher, pConn, pBuf))
						break;
				}
				else if (id > eGas2GacCpp_ApexProxyMsg_Begin && id < eGas2GacCpp_ApexProxyMsg_End) 
				{
					pConn->PopRecvData(dataSize); //ѹ������,���ÿ�ApexServer,����ᱻ��
					break;
				}
				else
				{
					std::ostringstream oss;
					oss << "Invalid Msg ID:" << id;
					GenErr(oss.str());
				}
			}
			//��������Ϣ��λ��Щ�쳣��ʱ���ǲ����ܺ��Եģ���������ֱ�Ӷ���
			SQR_CATCH(exp) {
				clog << exp.ErrorTitle() << endl << exp.ErrorMsg() << endl;
				clog << exp.StackInfo() << endl;
				pConn->ShutDown();
				break;
				
			}
			SQR_TRY_END;
		}
	}

}

void CConnMgrStressHandler::OnConnected(CConnStress* pConn)
{
	GetScriptHandler()->OnConnected(pConn);
}

void CConnMgrStressHandler::OnConnectFailed(CConnStress* pConn)
{
	GetScriptHandler()->OnConnectFailed(pConn);
}		

void CConnMgrStressHandler::OnDisconnect(CConnStress* pConn)
{
	GetScriptHandler()->OnDisconnect(pConn);
}		

void CConnMgrStressHandler::OnError(CConnStress* pConn,const char* szError)
{
	GetScriptHandler()->OnError(pConn, szError);
}



