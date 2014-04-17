#pragma once



namespace sqr
{
	class CTickMgr;
	class CTick;

	template<typename ImpTimeSystem_t>
	class TTimeSystem
	{
	private:
		CTickMgr*		m_pTickMgr;

		uint64				m_uLogicBaseTime;
		volatile uint64		m_uLogicTime;
		uint64				m_uPushTime;
		volatile uint64		m_uRealTime;
		bool				m_bOnTick;

	protected:
		TTimeSystem(uint32 uBaseCyc);
		~TTimeSystem();
	public:
		uint32 GetBaseCyc()const;
		void Register(CTick*,uint32 uCyc);
		void UnRegister(CTick*);

		size_t GetRegisterTickNum();
		void GetRegisterTickInfo(std::string& strInfo);

		volatile uint64* GetRealTimePtr();

		uint64 GetFrameTime()const;
		uint64 GetBaseTime()const;
		uint64 GetPushTime()const;
		int32 GetTimeError()const;	//TimeError<0��˵������׷֡

		void Reset(uint64 uProcessTime);
		void Reset();
		
		//return true��ʾʱ���Ѿ�û�����
		bool PushLogicTime(uint64 uProcessTime);
		bool PushLogicTime();	//�ƶ��߼�ʱ��ǰ��,�����߼�ʱ������ʵʱ������
		bool IsOnTick(void);

		static void Init(uint32 uBaseCyc);
		static void Unit();
		static ImpTimeSystem_t*& Inst();
	};

}


