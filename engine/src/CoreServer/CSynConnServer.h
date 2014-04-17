#pragma once
//$id$
//����ϵͳʹ�õ����Ӷ���,�������ϵͳ��ص�������Ϣ���ɷ��Լ���װ����Э�鲢���͵��ͻ���

#include "TMsgDispatcher.h"
#include "TConnection.h"
#include "CTick.h"
#include "IPipeHandler.h"
#include "CSyncTraitsServer.h"
#include "GuidTypes.h"
#include "ISend.h"
#include "TIDPtrMap.h"


namespace sqr
{
	template<class T> struct TPos;
	typedef TPos<int32> CPos;
	class CCoreObjectMediator;
	class CSynConnMgrServer;

	class CGac2GasOC_Report_Invalid_Step;
	class CGac2GasOC_Report_Invalid_Move;
	class CGac2GasGC_Update_Latency_Server;
	class CGac2GasGC_Ping_Server;
	class CGac2GasGC_Small_Shell_Message;
	class CGac2GasGC_Middle_Shell_Message;
	class CGac2GasGC_Tell_BeTesting;
	class CGac2GasGC_Main_Scene_Set;
	//class CGac2GasGC_Tell_OwnerInfo;
	class CGac2GasGC_Init_Conn_Guid;
	class CGac2GasGC_Client_Guid;
	//class CGac2GasGC_Update_Server_Known_Time;
	class CLogOwner;
	class CGac2GasOC_Tell_Mediator_Sync_Target_ViewVariant;
	class CGac2GasOC_Tell_Mediator_Sync_Target_SyncVariant;

	class CSynConnServer
		:public TConnection<CSyncTraitsServer>
		,public TBaseMsgDispatcher<CSynConnServer,uint8>
		,private CTick	//�������ӵ�ʱ����
	{
		friend class CSynConnMgrServer;
		friend class CSynMultiMsgSender;
		friend class CConnMgrServer;
		friend class CConnServer;
		friend class CSyncCoreObjectMediator;
		friend class CCoreUpdateCheatScoreJob;
		friend class CCoreConnServerSendJob;
		friend class CCoreLogOnOffLineMsgJob;
		template<typename Traits> friend class TConnectionMgr;
		
		friend class CCoreFlushShellMsgJob;
		friend class CSyncAppServer;

	public:
		CSynConnServer(IPipe* pPipe);
		~CSynConnServer(void);

		static CSynConnServer* GetSynConn(uint32 uSynConnID);

		CSyncCoreObjectMediator* GetObjectMediator()const;
		uint32 GetLatency() const;
		static void RegisterMsgHandler();

		int32 GetCheatScore()const;

		bool IsConnected()const;

		void ShutDown(const char* szMsg);
		uint GetGlobalID(){return m_uID;};
		void SetMediator(CSyncCoreObjectMediator* pMediator);
		void AddConnBlockCount(); 

		//����˳��ִ����ʱ�������ӵĿͻ��˷�����Ϣ
		void SendErrMsg(const char* szError);
		void SendErrMsg(const char* szError, uint32 uLen);

		//����key value�����Ҹ�asyc conn������Ϣ����
		void SetConnValue(const char* szKey, const char* szValue);

		uint64 GetConnTime()const;
		uint64 GetClientKnownServerFrameTime()const;
		uint64 GetClientTime()const;

		CLogOwner* GetLogOwner();
	private:
		
		typedef TIDPtrMap<CSynConnServer>	MapSynConn;		
		static MapSynConn& GetSynConnMap();
		static MapSynConn*& IntGetSynConnMap();
		static void InitSynConnMap();
		static void UninitSynConnMap();
		
		bool	m_bGuidSet;
		guid_t	m_guidConn;
		uint8	m_uModSignCounter;

		int32	m_nCheatScore;		//���׷֣����׷�Խ�ӽ�0�������׵Ŀ�����ԽС�����׷ֿ����Ǹ��ġ�

		void SetClientPingTime(void);

		void SendCurrentVersion();
		void SendOwnerInfo(const char* szKey, const char* szValue);

		template<typename CmdType>
		void FastVerifyData(const void* pData,size_t stDataSize,const CmdType* pCmd);
		void FastVerifyData(const void* pData,size_t stDataSize,uint8 usbCounter,uint16 udbSeed,uint8 usbModCheckSum);

		//ʵ��ISend�ӿ�
		void UpdateCheatScore(int32 nCheatScoreModifier);//AddJob

		ConnIter_t	m_itScene;	//���������󶨵�Mediator�������ڵĳ������б�ĵ�����

		void SendShellMessage(const void* pData, uint32 uSize);

		void DoShutDownAndCallbackDerived();
		void LogOnOffLineMsg(const char*);

		// ÿ��һ����,���һ��,�ͻ����Ƿ����
		void OnTick();
		static uint8 GetMaxCheckTimeInOneTurn();	//ÿ�ּ���������
		bool UpdateTimeError();

		template<typename CmdClass>
		static void RegisterClientCommand();
		template<typename CmdClass>
		static void RegisterDirectorCommand();
		template<typename CmdClass>
		static void RegisterDirectorCommandBlockByMediator();
		template<typename CmdClass>
		static void RegisterDirectorCommandWithTimeStamp();

		template<typename CmdClass>
		void OnClientCommandWithTimeInfo(const CmdClass* pCmd, void* pParam);

		template<typename CmdClass>
		void OnDirectorCommand(const CmdClass* pCmd, void* pParam);
		template<typename CmdClass>
		void OnDirectorCommandWithTimeStamp(const CmdClass* pCmd, void* pParam);
		template<typename CmdClass>
		void OnDirectorCommandBlockByMediator(const CmdClass* pCmd, void* pParam);
		
		inline void OnClientCommand(const CGac2GasOC_Report_Invalid_Step* pCmd, void* pParam);
		inline void OnClientCommand(const CGac2GasOC_Report_Invalid_Move* pCmd, void* pParam);

		inline void OnClientCommand(const CGac2GasGC_Ping_Server* pCmd, void * pParam);
		inline void OnClientCommand(const CGac2GasGC_Update_Latency_Server* pCmd, void * pParam);
		inline void OnClientCommand(const CGac2GasGC_Small_Shell_Message* pCmd, void * pParam);
		inline void OnClientCommand(const CGac2GasGC_Middle_Shell_Message* pCmd, void * pParam);
		inline void OnClientCommand(const CGac2GasGC_Main_Scene_Set* pCmd, void * pParam);
		inline void OnClientCommand(const CGac2GasGC_Tell_BeTesting* pCmd, void* pParam);
		//inline void OnClientCommand(const CGac2GasGC_Tell_OwnerInfo* pCmd, void* pParam);
		inline void OnClientCommand(const CGac2GasGC_Init_Conn_Guid* pCmd, void* pParam);
		inline void OnClientCommand(const CGac2GasGC_Client_Guid* pCmd, void* pParam);
		inline void OnClientCommand(const CGac2GasOC_Tell_Mediator_Sync_Target_ViewVariant* pCmd, void* pParam);
		inline void OnClientCommand(const CGac2GasOC_Tell_Mediator_Sync_Target_SyncVariant* pCmd, void* pParam);
		//inline void OnClientCommand(const CGac2GasGC_Update_Server_Known_Time* pCmd, void* pParam);

		typedef TConnection<CSyncTraitsServer>	ParentConn_t;
		friend class TConnection<CSyncTraitsServer>;
		
		uint64	m_uClientLastPingTime;
		uint64	m_uPingClientLastTime;
		uint32	m_uCurrentDelay;
		CSyncCoreObjectMediator*	m_pMedObj;

		bool	m_bTestingConn; //����������ӵĿͻ����Ƿ��ǲ��Կͻ���

		//ÿ��SetConnection���BlockCount���ӣ��ȵ��ͻ��˷���OnMainSceneSetʱ��1��������һ��SetConneciton������������͵���һ��SetConnection��MedObj��
		uint32					m_uBlockCount;
		uint32					m_uID;

		uint32					m_uTotalCheckedNum;	//�ۼƼ�����
		int32					m_nTotalTimeError;	//�ۼ�ʱ������
		typedef vector<int32, TConnAllocator<int32> > VecTimeErr;
		VecTimeErr				m_vecTimeErr;		//����ʱ������,���ڰ�������

		typedef std::list<CSynConnServer*,
			TConnAllocator<CSynConnServer*> >::iterator TestingConnIter_t;
		TestingConnIter_t	m_itTestingConn;

		template<typename CmdClass>
		void UpdateClientTimeInfo(const CmdClass* pCmd);
		void UpdateClientTimeInfo(uint64 uClientKnownServerFrameTime, uint64 uClientTime);
		void VerifyClientKnownServerFrameTime(uint64 uClientKnownServerFrameTime);
		void PushServerTime(const uint64 uCurServerFrameTime);
		uint64 GetServerTime()const;

		uint64					m_uLastClientKnownServerFrameTime;

		uint64					m_uLastDifferentServerFrameTime;
		uint64					m_uLastClientTime;
		uint64					m_uLastDifferentFrameClientTime;
		uint64					m_uDiscardedConnTime;
		mutable	uint64			m_uLastConnTime;
		uint64					m_uCurFrameDiscardedConnTime;
		uint64					m_uCurFrameTimeSpanThreshold;

		bool					m_bServerTimeInited;
		int32					m_iServerClientTimeDiff;
		uint64					m_uServerTime;
		uint64					m_uDiscardedServerTime;
	};
}
