#pragma once
#include "IPipeHandler.h"
#include "CDynamicObject.h"
#include "CAddress.h"
#include "CLogOwner.h"
#include "TConnAllocator.h"

namespace sqr
{
	class IPipe;

	template<typename HolderType, void(HolderType::*pFun)() >
	class TTickFunctionHandler;

	template<typename Traits>
	class TConnectionMgr;

	class CTestSuiteCoreObjectServer;

	
	class CExpCounter;

	template<typename Traits>
	class TConnection
		: public CLogOwner
		, protected IPipeHandler
		, public CConnMallocObject

	{
		typedef TConnectionMgr<Traits>				ConnMgr_t;
		typedef typename Traits::ConnMgr_t			ImpConnMgr_t;
		typedef typename Traits::ConnMgrHandler_t	ImpConnMgrHandler_t;
		typedef TConnection<Traits>					Conn_t;
		typedef typename Traits::Conn_t				ImpConn_t;
		typedef typename Traits::TimeSystem_t		ImpTimeSystem_t;
		typedef typename Traits::CoreScene_t		impCoreScene_t;
		typedef typename Traits::AppConfig_t		ImpAppConfig_t;


		friend class CSyncCoreObjectDictator;
		friend class CSyncCoreObjectMediator;
		friend class CCoreObjectFollower;
		friend class CCoreObjectDirector;

		friend class CCacheVariantServerHolder;

		template<typename ImpClass,typename IdType,uint32 uId>
		friend class TBaseMsgDispatcher;

		friend class CTestSuiteCoreObjectServer;
		friend class TConnectionMgr<Traits>;
#ifdef _WIN32
		friend ImpConnMgr_t;
		friend impCoreScene_t;
#else
		friend class Traits::ConnMgr_t;
		friend class Traits::CoreScene_t;
#endif	

	public:
		bool IsShuttingDown()const;

		bool IsConnected()const;
		void* GetRecvData()const;
		size_t GetRecvDataSize()const;
		void PopRecvData(size_t stSize);
		//return false��ʾ���ӻ�û�н�����ֻ�пͻ��˵����Ӷ�����п���return false��
		template<typename CmdType>bool SendCmd(const CmdType* pCmd)
		{
			return static_cast<ImpConn_t*>(this)->Send(pCmd,sizeof(CmdType));
		}

		void OnCaughtNetException();

		bool GetRemoteAddress(CAddress& Address)const;
		bool GetLocalAddress(CAddress& Address)const;

	protected:
		TConnection();
		~TConnection(void);

		void Init(IPipe* pPipe);

		void CacheRecvData(char* pData,size_t stSize);
		void StoreRecvData();
		bool IntShutDown();

		bool DispatchIsCanceled()const;

		template<typename CmdType>
		void SendCoreCmd(const CmdType* pCmd);
		void SendCoreCmd(const void* pData,uint32 uSize);

		void DoShutDownAndCallback();

		//�����������ǿ�ƶ���
		void CoreShutDown(const char*);
		void LogOnOffLineMsg(const char* szMsg);
		bool RegDoShutDownTick();
		void UnRegDoShutdownTick();

		void SendCoreMsg(const void* pData,uint32 uSize);

		IPipe* GetPipe()const;

		// IPipeHandler���
		void OnConnected();
		void OnConnectFailed(EPipeConnFailedReason eReason);
		void OnDisconnected(EPipeDisconnectReason eReason,bool bGracefully);
		void OnAccepted(IPipe* pPipe) {};
		void OnDataReceived();
		void OnDataSent(uint32 uSentSize);
		
		typedef typename std::list<ImpConn_t*,TConnAllocator<ImpConn_t*> >::iterator ConnIter_t;
		ConnIter_t m_itListConn;
		bool		m_bIsGuidSet;

		void LogTime(ostringstream& stream);

	private:
		typedef basic_string<char, std::char_traits<char>, TConnAllocator<char> >	String_t;

		String_t	m_strRecvBuffer; // ���ջ�����
		
		typedef TTickFunctionHandler<Conn_t,&Conn_t::DoShutDownAndCallback> TickFuncHandler_t;
		TickFuncHandler_t* m_pTickDoShutDown;

		bool m_bShakedHand:1;
		bool m_bShutDownCalled:1; //�����ⲿ�Ƿ���ù�ShutDown����
		uint8		m_uConnCorruptedNum;

		IPipe* m_pPipe;

		static char*		ms_pCurRecvBuffer;	//���������һ���������ݵ�buffer��ʼ��ַ�����ڼ����������ݸ��ơ�
		static size_t		ms_uCurRecvBufferSize;
		
		unsigned int m_uExpNum;

		CExpCounter*	m_pExpCounter;


		unsigned OnNetMessage(void* pData,unsigned int uSize,void* pArg);
	};

}


