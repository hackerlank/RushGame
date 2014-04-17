#include "stdafx.h"
#include "CTestSuiteRpcCall.h"
#include "ScriptRpc.h"
#include "CTestScriptMemoryPipe.h"
#include "ExpHelper.h"
#include <iostream>


namespace sqr
{

	class CTestScriptMemoryPipeMgr
	{
	public:
		static CTestScriptMemoryPipe *s_pPipe;
		static CTestScriptMemoryPipe * GetPipe(){ return s_pPipe; };
		static void CreatePipe(){ s_pPipe = new CTestScriptMemoryPipe;};
		static void DeletePipe(){delete s_pPipe;};
	};

	class InitReg
	{
	private:
		InitReg();
	public:
		static void Instance()
		{
			static InitReg _Instance;
		}
		static void LoadBefore()
		{
			sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
			Script.RunFile("common/Rpc/callbefore",NULL);
		}
		static void LoadAfter()
		{
			sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
			Script.RunFile("common/Rpc/callafter",NULL);
		}
	};



	const char* RegTestSuiteRpcCall()
	{
		static TestSuiteFactory<CTestSuiteRpcCall> m_factory;
		const char* szSuiteName="CTestSuiteRpcCall";
		TestFactoryRegistry::getRegistry(szSuiteName).registerFactory(&m_factory);
		return szSuiteName;
	}

	//typedef TUnPacker<IPipe> CUnPacker;
	//typedef TPackerTable<IPipe> CPackerTable;
	//typedef TUnPackerTable<IPipe> CUnPackerTable;
	CTestScriptMemoryPipe* CTestScriptMemoryPipeMgr::s_pPipe = 0;
	
	//int32 static RegisterRpc(CScript& Script)
	//{
	//	//	CScript& Script = *GetScript();

	//	REGIST_ABSTRACT_B_CLASS( Script, CPackerTable);
	//	REGIST_CLASSFUNCTION(Script,CPackerTable,RegistPackFunc, "sss");
	//	REGIST_STATICFUNCTION( Script, CPackerTable, Inst, ":(CPackerTable)" );

	//	REGIST_ABSTRACT_B_CLASS( Script, CUnPackerTable);
	//	REGIST_CLASSFUNCTION(Script,CUnPackerTable,RegistUnPackFunc, "sss");
	//	REGIST_STATICFUNCTION( Script, CUnPackerTable, Inst, ":(CUnPackerTable)" );

	//	return 1;
	//}

	InitReg::InitReg()
	{
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		REGIST_ABSTRACT_B_CLASS( Script, IPipe );
		REGIST_CLASSFUNCTION(Script,IPipe,GetRecvDataSize,":I")
		REGIST_STATICFUNCTION( Script, CTestScriptMemoryPipeMgr, GetPipe, ":(IPipe)" );
		CTestScriptMemoryPipeMgr::CreatePipe();
		CREATE_RPC_CHANNEL(Script,IPipe,IPipe);
	}

	typedef TRegisterRpc<IPipe,IPipe>::CUnPacker CUnPacker;
	typedef TRegisterRpc<IPipe,IPipe>::CPackerTable CPackerTable;
	typedef TRegisterRpc<IPipe,IPipe>::CUnPackerTable CUnPackerTable;

	sqr::CScript *CTestSuiteRpcCall::GetScript()
	{
		//static sqr::CScript instance;
		//return instance;

		//ת��ȫ�ֵ� GetScript, ��Ϊ CPackerTable::RegistPackFunc �õ� ȫ�ֵ� GetScript��
		//�޸ĳ�ʹ�þֲ���Script����Ҫ��һ����ʱ�俼��
		return CBaseScriptApp::Inst()->GetScript();
	}

	void CTestSuiteRpcCall::setUp()
	{

	}


	void CTestSuiteRpcCall::tearDown()
	{

	}
	void CTestSuiteRpcCall::TestReg()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
	}
	void CTestSuiteRpcCall::TestCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();

		Script.RunFile("common/Rpc/ByteCall",NULL);

		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();
		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)6,len);
	}
	void CTestSuiteRpcCall::TestWordCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();

		Script.RunFile("common/Rpc/WordCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();
		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)8,len);
	}
	void CTestSuiteRpcCall::TestDWordCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/DWordCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");

		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)12,len);
	}
	void CTestSuiteRpcCall::TestDDWordCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/DDWordCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");

		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)20,len);
	}
	//�޲����ĵ���
	void CTestSuiteRpcCall::TestNoArgCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/NoArgCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");

		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)4,len);
	}
	//�ַ�������'\0'������
	void CTestSuiteRpcCall::TestStringCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/StringCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");

		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)16,len);
	}
	void CTestSuiteRpcCall::TestBStringCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/BStringCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");

		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)17,len);
	}

	//���ַ����ĵ���
	void CTestSuiteRpcCall::TestMoreStrCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/BStringCall",NULL);
		Script.RunFile("common/Rpc/StringCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		int32 len = 0;
		while (pPipe->GetRecvDataSize() >= 2)
		{
			size_t dataSize = pPipe->GetRecvDataSize();
			len	+= RegistUnPack.UnPack(pPipe);
			if (dataSize == pPipe->GetRecvDataSize())
			{
				break;
			}
		}
	
		CPPUNIT_ASSERT_EQUAL((int32)33,len);
	}

	//�����ֵĵ���
	void CTestSuiteRpcCall::TestMoreNumCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/ByteCall",NULL);
		Script.RunFile("common/Rpc/WordCall",NULL);
		Script.RunFile("common/Rpc/DWordCall",NULL);
		Script.RunFile("common/Rpc/DDWordCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		int32 len = 0;
		while (pPipe->GetRecvDataSize() >= 2)
		{
			size_t dataSize = pPipe->GetRecvDataSize();
			len	+= RegistUnPack.UnPack(pPipe);
			if (dataSize == pPipe->GetRecvDataSize())
			{
				break;
			}
		}
		CPPUNIT_ASSERT_EQUAL((int32)46,len);
	}

	//��ϵ���
	void CTestSuiteRpcCall::TestMixCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();


		Script.RunFile("common/Rpc/WordCall",NULL);
		Script.RunFile("common/Rpc/BStringCall",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		int32 len = 0;
		while (pPipe->GetRecvDataSize() >= 2)
		{
			size_t dataSize = pPipe->GetRecvDataSize();
			len	+= RegistUnPack.UnPack(pPipe);
			if (dataSize == pPipe->GetRecvDataSize())
			{
				break;
			}
		}
		CPPUNIT_ASSERT_EQUAL((int32)25,len);
	}

	//id������Ч��Χ��
	void CTestSuiteRpcCall::TestErrorIdCall()
	{
		int32 len = 100,xx;
		InitReg::Instance();
		InitReg::LoadBefore();
		CTestScriptMemoryPipeMgr::CreatePipe();

		xx = len ^ 55555;
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(uint16));
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&xx),sizeof(uint16));
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(char));
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();
		len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)-1,len);
		pPipe->ClearRecvBuffer();
	}
	//��֤�����
	void CTestSuiteRpcCall::TestErrorXXCall()
	{
		int32 len = 0;
		InitReg::Instance();
		InitReg::LoadBefore();
		CTestScriptMemoryPipeMgr::CreatePipe();


		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(uint16));
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(uint16));
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(char));
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(char));
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();
		len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)-1,len);
		pPipe->ClearRecvBuffer();
	}

	//�Ҳ���������
	void CTestSuiteRpcCall::TestNoFoundFun()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();
		Script.RunFile("common/Rpc/NoFoundFun",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)-1,len);
		pPipe->ClearRecvBuffer();
	}
	//Э����ǲ�������
	void CTestSuiteRpcCall::TestNotAllCall()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/ByteCall",NULL);
		Script.RunFile("common/Rpc/WordCall",NULL);

		int32 len = 2;
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(uint16));

		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		len = 0;
		while (pPipe->GetRecvDataSize() >= 2)
		{
			size_t dataSize = pPipe->GetRecvDataSize();
			len	+= RegistUnPack.UnPack(pPipe);
			if (dataSize == pPipe->GetRecvDataSize())
			{
				break;
			}
		}
		CPPUNIT_ASSERT_EQUAL((int32)14,len);

		len = CTestScriptMemoryPipeMgr::GetPipe()->GetRecvDataSize();
		CPPUNIT_ASSERT_EQUAL((int32)2,len);
		pPipe->ClearRecvBuffer();
	}

	//Э����ǲ�������,��������2
	void CTestSuiteRpcCall::TestNotAllCall2()
	{
		int32 len,xx;
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();

		Script.RunFile("common/Rpc/ByteCall",NULL);
		Script.RunFile("common/Rpc/StringCall",NULL);

		len = 2;
		xx = len ^ 55555;
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(uint16));
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&xx),sizeof(uint16));
		CTestScriptMemoryPipeMgr::GetPipe()->Send(reinterpret_cast<char *>(&len),sizeof(char));
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		len = 0;
		while (pPipe->GetRecvDataSize() >= 2)
		{
			size_t dataSize = pPipe->GetRecvDataSize();
			len	+= RegistUnPack.UnPack(pPipe);
			if (dataSize == pPipe->GetRecvDataSize())
			{
				break;
			}
		}
		CPPUNIT_ASSERT_EQUAL((int32)22,len);

		len = CTestScriptMemoryPipeMgr::GetPipe()->GetRecvDataSize();
		CPPUNIT_ASSERT_EQUAL((int32)5,len);
		pPipe->ClearRecvBuffer();
	}
	//�û��Լ����� -1����ʾ�Ͽ�����
	void CTestSuiteRpcCall::TestClosePipe()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();
		Script.RunFile("common/Rpc/ClosePipe",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)-1,len);
		pPipe->ClearRecvBuffer();
	}
	//�û��Լ�>0�������Զ�������������
	void CTestSuiteRpcCall::TestCallError()
	{
		InitReg::Instance();
		InitReg::LoadBefore();
		sqr::CScript &Script = *CTestSuiteRpcCall::GetScript();
		IPipe * pPipe = CTestScriptMemoryPipeMgr::GetPipe();
		Script.RunFile("common/Rpc/CallError",NULL);
		InitReg::LoadAfter();
		CUnPacker & RegistUnPack = CUnPackerTable::Inst().GetUnPacker("Gas2Gac");
		int32 len = RegistUnPack.UnPack(pPipe);
		CPPUNIT_ASSERT_EQUAL((int32)-1,len);
		pPipe->ClearRecvBuffer();
	}
}

