#pragma once
//$id$
//����ϵͳʹ�õ����Ӷ���,�������ϵͳ��ص�������Ϣ���ɷ��Լ���װ����Э�鲢���͵��ͻ���

#include "CDynamicObject.h"
#include "ISend.h"
#include "CLogOwner.h"
#include "CAddress.h"
#include "TConnAllocator.h"

namespace sqr
{
	class CCoreObjectMediator;

	class CConnServer
		:public CLogOwner
		,public ISend
		,public CConnMallocObject
	{
		typedef vector<CConnServer*,TConnAllocator<CConnServer*> > IDVecConnServer;
	public:
		friend class CCoreDispatchShellMsgResult;
		friend class CCoreSetTestingConnResult;

		CConnServer(uint32 uID, char* pAddress, uint32 uPort);
		~CConnServer(void);
		static CConnServer* GetConnServer(uint32 uID);
		/*IPipe* GetPipe()const;*/

		uint32 Send(const void* pData,uint32 uSize);

		void ShutDown(const char* szMsg);
		void LogOnOffLineMsg(const char* msg);
		uint32 GetGlobalID(){return m_uID;};
		CCoreObjectMediator* GetObjectMediator();
		void SetMediator(CCoreObjectMediator* pMediator);

		void* GetRecvData()const;
		size_t GetRecvDataSize()const;
		void PopRecvData(size_t stSize);
		bool IsShuttingDown()const;
		bool GetRemoteAddress(CAddress& Address)const;

		void SendErrMsg(const char* szError);

		//����key value�����Ҹ�sync conn������Ϣ����
		void SetConnValue(const char* szKey, const char* szValue);

		//�����ӳ�
		void SetLatency(uint32 uLatency);
		uint32 GetLatency()const;

	private:
		static IDVecConnServer& GetIDVecConnServer();
		void CacheRecvData(char* pData,size_t stSize);
		void StoreRecvData();

	private:
		uint32					m_uID;		
		bool					m_bShutDownCalled;
		uint32					m_uLatency;
		bool					m_bTestingConn;

		CCoreObjectMediator*	m_pMedObj;

		static char*			ms_pCurRecvBuffer;			//���������һ���������ݵ�buffer��ʼ��ַ�����ڼ����������ݸ��ơ�
		static size_t			ms_uCurRecvBufferSize;
		basic_string<char,std::char_traits<char>,TConnAllocator<char> >				m_strRecvBuffer;			// ���ջ�����
		CAddress				m_Address;

	};
}
