#include "stdafx.h"
#include "CTestSuiteTickMgr.h"
#include "CommonTest.h"
#include "CTick.h"
#include "CTickImp.h"
#include "CTickMgr.h"
#include "TTickHandler.h"


namespace sqr
{
	const char* RegTestSuiteTickMgr()
	{
		static TestSuiteFactory<CTestSuiteTickMgr> m_factory;
		const char* szSuiteName="CTestSuiteTickMgr";
		TestFactoryRegistry::getRegistry(szSuiteName).registerFactory(&m_factory);
		return szSuiteName;
	}

	class CZeroTick : public CTick
	{
	public:
		uint32	m_uCounter;
		CZeroTick():m_uCounter(0){};
		virtual ~CZeroTick(){};

		void OnTick(){UnRegister();}

	};

	class CMyTick : public CTick
	{
	public:
		uint32	m_uCounter;
		CMyTick():m_uCounter(0){};
		virtual ~CMyTick(){};

		void OnTick(){m_uCounter++;}

	};

	class CFreeThisTick : public CTick
	{
	public:
		uint32	m_uCounter;
		CFreeThisTick():m_uCounter(0){};
		virtual ~CFreeThisTick(){};

		void OnTick(){ m_uCounter++;delete this;}

	};

	class CRegisterTick : public CTick
	{
	public:
		CTickMgr * m_pTickMgr;
		uint32    m_uNum;
		uint32	m_uCounter;

		CRegisterTick(CTickMgr* pTickMgr,uint32 uNum ){m_pTickMgr = pTickMgr;m_uNum=uNum;m_uCounter=0;};

		virtual ~CRegisterTick(){};

		void OnTick(){m_pTickMgr->Register(this,m_uNum); m_uCounter++; };

	};

	class CClearOtherTick : public CTick
	{
	public:
		uint32	m_uCounter;
		CTick	*m_pTick;
		CClearOtherTick(){m_uCounter=0;};
		void ClearReg(CTick	*pTick){m_pTick=pTick;m_pTick=NULL;};
		virtual ~CClearOtherTick(){};
		void OnTick()
		{ 
			if(m_pTick!=NULL)
			{
				delete m_pTick;
				m_pTick = NULL;
			}
			m_uCounter++; 
		};
	};

	class CRegOtherTick : public CTick
	{
	public:
		CTickMgr * m_pTickMgr;
		uint32    m_uNum;
		uint32	m_uCounter;
		CTick	*m_pTick;
		CRegOtherTick(CTickMgr* pTickMgr,uint32 uNum){m_pTickMgr = pTickMgr;m_uNum=uNum;m_uCounter=0;m_pTick=NULL;};
		void RegOther(CTick	*pTick){m_pTick=pTick;}
		virtual ~CRegOtherTick(){};

		void OnTick()
		{
			if(m_pTick!=NULL&&m_uNum!=0)
				m_pTickMgr->Register(m_pTick,m_uNum); 
			m_uCounter++; 
		};
	};

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4355)
#endif

	class CPlayer
	{
	public:
		TTickHandler< CPlayer >	m_TickHandler1;
		TTickHandler< CPlayer >	m_TickHandler2;
		CTickMgr * m_pTickMgr;
		uint32 m_uCount1,m_uCount2;
	public:

		CPlayer(CTickMgr * pTickMgr):m_TickHandler1(this),m_TickHandler2(this),m_pTickMgr(pTickMgr)
		{
			m_uCount1 = 0;
			m_uCount2 = 0;
			m_TickHandler1.m_pArg = reinterpret_cast<void *> (1);
			m_TickHandler2.m_pArg = reinterpret_cast<void *> (2);
			m_pTickMgr->Register(&m_TickHandler1,10);
			m_pTickMgr->Register(&m_TickHandler2,20);
		};

		void OnTick(TTickHandler<CPlayer>* pHandler)
		{
			if(pHandler->m_pArg ==reinterpret_cast<void *> (1))
				m_uCount1++;
			else
				m_uCount2++;
		};
	};

#ifdef _WIN32
#pragma warning(pop)
#endif


	void CTestSuiteTickMgr::setUp()
	{

	}


	void CTestSuiteTickMgr::tearDown()
	{

	}


	void CTestSuiteTickMgr::TestCreation()
	{
		CTickMgr mgr(10,60);
	}

	void CTestSuiteTickMgr::TestCreationError()
	{
		CTickMgr mgr(0,10);
	}
	void CTestSuiteTickMgr::TestCreationError1()
	{
		CTickMgr mgr(10,0);
	}
	void CTestSuiteTickMgr::TestRegisterZero()
	{
		CTickMgr mgr(10,10);
		CZeroTick tick1;
		mgr.Register(&tick1,0);
		mgr.OnTick();
	}
	//ע��tickʱ�����ڱ�CTickMgrС��
	void CTestSuiteTickMgr::TestRegisterLittle()
	{
		CTickMgr mgr(10,10);
		CMyTick tick1;
		mgr.Register(&tick1,4);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,tick1.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)5,tick1.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)7,tick1.m_uCounter);
	}
	void CTestSuiteTickMgr::TestOnTickNormal()
	{
		CTickMgr mgr(10,10);
		for(uint32 i=0;i<2048;i++)
		{
			mgr.OnTick();
		}
		//	CPPUNIT_ASSERT(1==1);
	}
	//��˳��Register��UnRegister�Ƿ�����
	void CTestSuiteTickMgr::TestRegisterMore()
	{
		CTickMgr mgr(10,10);
		CMyTick tick1,tick2;
		//first
		mgr.Register(&tick1,10);
		mgr.Register(&tick1,10);
		mgr.Register(&tick1,20);
		mgr.UnRegister(&tick1);
		//second
		mgr.UnRegister(&tick2);
		mgr.UnRegister(&tick2);
		mgr.Register(&tick2,30);
		mgr.UnRegister(&tick2);
		//three
		mgr.Register(&tick1,10);
		mgr.Register(&tick2,20);
		mgr.UnRegister(&tick2);
		mgr.UnRegister(&tick1);

	}
	//Register��UnRegister�����Ƿ�����
	void CTestSuiteTickMgr::TestOnTickRU()
	{
		CTickMgr mgr(10,1024);
		CMyTick tick1,tick2,tick3;
		mgr.Register(&tick1,100);
		mgr.Register(&tick2,100);
		mgr.Register(&tick3,10240);
		for(uint32 i=0;i<1;i++)
		for(uint32 i=0;i<2048;i++)
		{
			mgr.OnTick();
		}
		mgr.UnRegister(&tick1);
		mgr.UnRegister(&tick2);
		mgr.UnRegister(&tick3);
	}
	//mgr��OnTick�����Ƿ�����
	void CTestSuiteTickMgr::TestOnTickRight()
	{
		CTickMgr mgr(10,3);
		CMyTick tick1;
		mgr.Register(&tick1,10);
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick1.m_uCounter);
	}
	//��������Register��ֻ�к������Ч
	void CTestSuiteTickMgr::TestDoubleRegister()
	{
		CTickMgr mgr(10,3);
		CMyTick tick1;
		mgr.Register(&tick1,10);
		mgr.Register(&tick1,20);
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick1.m_uCounter);
	}
	//delete����tickmgr��Ӧ���ͷ��Լ�
	void CTestSuiteTickMgr::TestFree()
	{
		CTickMgr mgr(10,3);
		//first
		CMyTick *tick1=new CMyTick;
		mgr.Register(tick1,10);
		delete tick1;
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
	}
	//��OnTick����Ȼû�иı䳲λ�õġ����Ƿ������ȷ
	void CTestSuiteTickMgr::TestMid()
	{
		CTickMgr mgr(10,2);
		CMyTick tick1,tick2;
		mgr.Register(&tick1,20);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,tick1.m_uCounter);
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick1.m_uCounter);

		mgr.Register(&tick2,20);
		for(int i = 0;i<4;i++)
			mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)3,tick1.m_uCounter);
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick2.m_uCounter);
	}

	//��OnTick�������Լ�
	void CTestSuiteTickMgr::TestFreeThis()
	{
		CFreeThisTick *freetick1,*freetick2;
		CMyTick tick1,tick2;
		CTickMgr mgr(10,3);

		freetick1 = new CFreeThisTick;
		mgr.Register(&tick1,10);
		mgr.Register(freetick1,10);
		mgr.Register(&tick2,10);

		freetick2 = new CFreeThisTick;
		mgr.Register(freetick2,20);
		for(int i = 0;i<3;i++)
			mgr.OnTick();
	}
	//��OnTick������ע���Լ�
	void CTestSuiteTickMgr::TestRegisterMyself()
	{
		CTickMgr mgr1(10,3),mgr2(10,5);
		CRegisterTick Registertick1(&mgr1,10),Registertick2(&mgr2,10);


		mgr1.Register(&Registertick1,10);

		for(int i = 1;i<4;i++)
			mgr1.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,Registertick1.m_uCounter);

		mgr2.Register(&Registertick2,10);
		mgr2.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,Registertick2.m_uCounter);
		mgr2.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,Registertick2.m_uCounter);
	}
	//��OnTick������ע���Լ�
	void CTestSuiteTickMgr::TestRegisterMyselfRing()
	{
		CTickMgr mgr1(10,3);
		CRegisterTick Registertick1(&mgr1,30);



		mgr1.Register(&Registertick1,30);
		mgr1.OnTick();
		mgr1.OnTick();
		mgr1.OnTick();


	}
	//��OnTick�аѱ�Ķ���ɾ��
	void CTestSuiteTickMgr::TestClearOther()
	{
		CTickMgr mgr(10,3);
		CClearOtherTick ClearOther;
		CMyTick * pMyTick;
		pMyTick =new CMyTick;
		ClearOther.ClearReg(pMyTick);
		mgr.Register(pMyTick,10);
		mgr.Register(&ClearOther,10);

		for(int i = 0;i<3;i++)
			mgr.OnTick();
	}

	//��OnTick������ע���Ķ���
	void CTestSuiteTickMgr::TestRegOther()
	{

		CMyTick MyTick;
		CTickMgr mgr(10,3);
		CRegOtherTick RegOtherTwo(&mgr,20);

		RegOtherTwo.RegOther(&MyTick);
		mgr.Register(&RegOtherTwo,10);
		mgr.Register(&MyTick,10);

		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,MyTick.m_uCounter);
		CPPUNIT_ASSERT_EQUAL((uint32)0,RegOtherTwo.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,MyTick.m_uCounter);
		CPPUNIT_ASSERT_EQUAL((uint32)1,RegOtherTwo.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,MyTick.m_uCounter);
		CPPUNIT_ASSERT_EQUAL((uint32)2,RegOtherTwo.m_uCounter);
	}
	//һ������������Ƶ�ʵ�
	void CTestSuiteTickMgr::TestTwoFrequency()
	{
		CTickMgr mgr(10,3);
		CPlayer Player(&mgr);
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,Player.m_uCount1);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,Player.m_uCount1);
		CPPUNIT_ASSERT_EQUAL((uint32)1,Player.m_uCount2);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)3,Player.m_uCount1);
	}
	//����ʱ�䳬���ܳ���tick
	void CTestSuiteTickMgr::TestNum()
	{
		CTickMgr mgr(10,3);
		CMyTick MyTick;
		mgr.Register(&MyTick,70);
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,MyTick.m_uCounter);
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,MyTick.m_uCounter);
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyTick.m_uCounter);

		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyTick.m_uCounter);
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyTick.m_uCounter);
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,MyTick.m_uCounter);
	}
	//����ʱ����ʣ��ʱ���tick
	void CTestSuiteTickMgr::TestAddRemain()
	{
		CTickMgr mgr(10,3);
		CMyTick MyTick;
		mgr.Register(&MyTick,17);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,MyTick.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,MyTick.m_uCounter);
		mgr.OnTick();	//��������ǰ���ģ�17���Ȳ���20
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyTick.m_uCounter);
		mgr.OnTick(); //���������
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyTick.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,MyTick.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,MyTick.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)3,MyTick.m_uCounter);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)4,MyTick.m_uCounter);
	}
	//������mgr��ע��mgr
	void CTestSuiteTickMgr::TestRegTickMgr()
	{
		CMyTick MyTick;
		CTickMgr mgrMain(10,3),mgr(10,3);
		mgr.Register(&MyTick,10);
		mgrMain.Register(&mgr,10);
		mgrMain.OnTick();
		mgrMain.OnTick();
		mgrMain.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyTick.m_uCounter);
		mgrMain.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,MyTick.m_uCounter);
		mgrMain.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)3,MyTick.m_uCounter);
		mgrMain.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)4,MyTick.m_uCounter);
	}
	//����mgr.m_uTickCount���������
	void CTestSuiteTickMgr::TestUpperLimit()
	{
		CTickMgr mgr(10,3);
		CMyTick tick;
		mgr.m_uRing = 4294967295u;	//32���޷��ŵ�����4294967295
		mgr.Register(&tick,70);
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick.m_pImp->m_uRing);
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,tick.m_uCounter);
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,mgr.m_uRing);
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)0,tick.m_uCounter);
		mgr.OnTick();
		mgr.OnTick();
		mgr.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick.m_uCounter);
	}
	//ֻ��һ�����ģ����в����������ķ�����TestRegisterMix��ȫһ����Ӧ�ý��Ҳ��һ���ġ�
	void CTestSuiteTickMgr::TestJustOneSlot()
	{
		//���Լ�����ע�ᵽͬ��һ��
		CTickMgr mgr1(10,1);
		CRegisterTick Registertick1(&mgr1,30);
		mgr1.Register(&Registertick1,10);
		mgr1.OnTick();
		mgr1.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,Registertick1.m_uCounter);
		mgr1.OnTick();
		mgr1.OnTick();
		mgr1.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,Registertick1.m_uCounter);

		//���µĶ���ע�ᵽ��ǰ
		CTickMgr mgr2(10,3);
		CMyTick tick;
		CRegOtherTick *MyRegOther=new CRegOtherTick(&mgr2,30);
		MyRegOther->RegOther(&tick);
		mgr2.Register(MyRegOther,10);
		mgr2.OnTick();
		mgr2.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyRegOther->m_uCounter);
		delete MyRegOther;
		mgr2.OnTick();
		mgr2.OnTick();
		mgr2.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick.m_uCounter);

		//���Լ�����ע�ᵽ��ĵط�
		CTickMgr mgr3(10,3);
		CRegisterTick Registertick3(&mgr3,20);
		mgr3.Register(&Registertick3,10);
		mgr3.OnTick();
		mgr3.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,Registertick3.m_uCounter);
		mgr3.OnTick();
		mgr3.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,Registertick3.m_uCounter);


		//�ѱ����ע�ᵽ��ĵط�
		CTickMgr mgr4(10,3);
		CMyTick tick4;
		CRegOtherTick *MyRegOther4=new CRegOtherTick(&mgr4,20);
		MyRegOther4->RegOther(&tick4);
		mgr4.Register(MyRegOther4,10);
		mgr4.Register(&tick4,10);
		mgr4.OnTick();
		mgr4.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyRegOther4->m_uCounter);
		delete MyRegOther4;
		mgr4.OnTick();
		mgr4.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick4.m_uCounter);

		//�ѱ�Ķ���ע�ᵽ��ǰ
		CTickMgr mgr5(10,3);
		CMyTick tick5;
		CRegOtherTick *MyRegOther5=new CRegOtherTick(&mgr5,30);
		MyRegOther5->RegOther(&tick5);
		mgr5.Register(MyRegOther5,10);
		mgr5.Register(&tick5,20);
		mgr5.OnTick();
		mgr5.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyRegOther5->m_uCounter);
		delete MyRegOther5;
		mgr5.OnTick();
		mgr5.OnTick();
		mgr5.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick5.m_uCounter);
	}
	//��ϲ���register��������������ӵ�
	void CTestSuiteTickMgr::TestRegisterMix()
	{
		//���Լ�����ע�ᵽͬ��һ��
		CTickMgr mgr1(10,3);
		CRegisterTick Registertick1(&mgr1,30);
		mgr1.Register(&Registertick1,10);
		mgr1.OnTick();
		mgr1.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,Registertick1.m_uCounter);
		mgr1.OnTick();
		mgr1.OnTick();
		mgr1.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,Registertick1.m_uCounter);

		//���µĶ���ע�ᵽ��ǰ
		CTickMgr mgr2(10,3);
		CMyTick tick;
		CRegOtherTick *MyRegOther=new CRegOtherTick(&mgr2,30);
		MyRegOther->RegOther(&tick);
		mgr2.Register(MyRegOther,10);
		mgr2.OnTick();
		mgr2.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyRegOther->m_uCounter);
		delete MyRegOther;
		mgr2.OnTick();
		mgr2.OnTick();
		mgr2.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick.m_uCounter);

		//���Լ�����ע�ᵽ��ĵط�
		CTickMgr mgr3(10,3);
		CRegisterTick Registertick3(&mgr3,20);
		mgr3.Register(&Registertick3,10);
		mgr3.OnTick();
		mgr3.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,Registertick3.m_uCounter);
		mgr3.OnTick();
		mgr3.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)2,Registertick3.m_uCounter);


		//�ѱ����ע�ᵽ��ĵط�
		CTickMgr mgr4(10,3);
		CMyTick tick4;
		CRegOtherTick *MyRegOther4=new CRegOtherTick(&mgr4,20);
		MyRegOther4->RegOther(&tick4);
		mgr4.Register(MyRegOther4,10);
		mgr4.Register(&tick4,10);
		mgr4.OnTick();
		mgr4.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyRegOther4->m_uCounter);
		delete MyRegOther4;
		mgr4.OnTick();
		mgr4.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick4.m_uCounter);

		//�ѱ�Ķ���ע�ᵽ��ǰ
		CTickMgr mgr5(10,3);
		CMyTick tick5;
		CRegOtherTick *MyRegOther5=new CRegOtherTick(&mgr5,30);
		MyRegOther5->RegOther(&tick5);
		mgr5.Register(MyRegOther5,10);
		mgr5.Register(&tick5,20);
		mgr5.OnTick();
		mgr5.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,MyRegOther5->m_uCounter);
		delete MyRegOther5;
		mgr5.OnTick();
		mgr5.OnTick();
		mgr5.OnTick();
		CPPUNIT_ASSERT_EQUAL((uint32)1,tick5.m_uCounter);
	}


}

