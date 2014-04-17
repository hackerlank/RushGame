#include "stdafx.h"
#include "CTestSuiteMetaSceneServer.h"
#include "CMetaSceneServer.h"
#include "CMetaSceneMgrServer.h"
#include "CPathFinding.h"
#include <ctime>
#include "CDirSearch.h"
#include <iostream>

using namespace sqr::FindPath;
using namespace sqr;

namespace sqr
{
	const char* RegTestSuiteMetaSceneServer()
	{
		static TestSuiteFactory<CTestSuiteMetaSceneServer> m_factory;
		const char* szSuiteName="CTestSuiteMetaSceneServer";
		TestFactoryRegistry::getRegistry(szSuiteName).registerFactory(&m_factory);
		return szSuiteName;
	}
}


void CTestSuiteMetaSceneServer::setUp(void)
{
	m_pMetaSceneMgr=new CMetaSceneMgrServer;
}

void CTestSuiteMetaSceneServer::tearDown(void)
{
	delete m_pMetaSceneMgr;
}

// blocktestΪ2*2��region�������ϰ���Ϣ���£�
//��0,0���� ����31,0���У���31,31��Ϊ�ߣ���0,31����
//��8,5��~��24,5���� ����12,10��~��12,26����
void CTestSuiteMetaSceneServer::TestReadBlock()
{
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->
		CreateMetaScene( "map/newmapa" );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	//����
	CPPUNIT_ASSERT_EQUAL((int32)3,int32(pFindPath->m_BarrierMgr.GetQuarBarrierType(0, 0)));
	//����
	CPPUNIT_ASSERT_EQUAL((int32)2,int32(pFindPath->m_BarrierMgr.GetQuarBarrierType(31, 0)));
	//����
	CPPUNIT_ASSERT_EQUAL((int32)3,int32(pFindPath->m_BarrierMgr.GetQuarBarrierType(31, 31)));
	//����
	CPPUNIT_ASSERT_EQUAL((int32)1,int32(pFindPath->m_BarrierMgr.GetQuarBarrierType(0, 31)));
	for(uint32 i = 0; i <= 16; i++)
	{
		CPPUNIT_ASSERT_EQUAL((int32)3,int32(pFindPath->m_BarrierMgr.GetQuarBarrierType(8 + i, 5)));
	}
	for(uint32 i = 0; i <= 16; i++)
	{
		CPPUNIT_ASSERT_EQUAL((int32)1,int32(pFindPath->m_BarrierMgr.GetQuarBarrierType(12, 10 + i)));
	}
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
void CTestSuiteMetaSceneServer::TestGetBarrier()
{
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->
		CreateMetaScene( "map/newmapa" );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	CPPUNIT_ASSERT_EQUAL(pFindPath->GetBarrierType(0,0),eBT_HighBarrier);
	CPPUNIT_ASSERT_EQUAL(pFindPath->GetBarrierType(-1,-1),eBT_OutRange);
	CPPUNIT_ASSERT_EQUAL(pFindPath->GetBarrierType(0,31),eBT_LowBarrier);
	CPPUNIT_ASSERT_EQUAL(pFindPath->GetBarrierType(31,0),eBT_MidBarrier);
	CPPUNIT_ASSERT_EQUAL(pFindPath->GetBarrierType(13,13),eBT_NoBarrier);
}
void CTestSuiteMetaSceneServer::TestCreateMap()
{
	IMap* pMap = CreateMap(10,10);
	pMap->Release();
}
//��load��ͼֱ��ʹ��imap��
void CTestSuiteMetaSceneServer::TestFindPathNoLoad()
{
	IMap* pMap = CreateMap(10,10);
	IPath* pPath = pMap->CreatePath(CPos(0, 0), CPos(10, 10), eFPT_AStar, eBT_NoBarrier);
	CPPUNIT_ASSERT(pPath == NULL);
	pMap->Release();
}
//�м�û���ϰ��ģ����Ƿ�ֱ���û��ߵİ취�ж�
void CTestSuiteMetaSceneServer::TestSearchLine()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(0, 1),CPos(0, 10), eFPT_AStar , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(0,1));

	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(0,10));
	pPath->Release();
	//���ﲻ�ܵ���pFindPath->Release();��Ϊ��CMetaSceneServer�л���ã�����ӿ�Ҳû�б�����ȥ
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}

//Ѱ·���ܹ�����
void CTestSuiteMetaSceneServer::TestAStarNoBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(11, 10),CPos(13, 10), eFPT_AStar , eBT_NoBarrier);

	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(11,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(13,10));
	pPath->GetPos(Pos, 2);
	CPPUNIT_ASSERT(Pos == CPos(13,10));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}

void CTestSuiteMetaSceneServer::TestAStarNoBarrier2()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa" );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	CPath * pPath = (CPath*)pFindPath->CreatePath(CPos(11, 10),CPos(24, 4), eFPT_AStar , eBT_NoBarrier);

	//for(uint32 i = 0; i < pPath->GetPathSize(); i++)
	//{
	//	pPath->GetPos(Pos,i);
	//	cout<< "x = " << Pos.x << " y = " << Pos.y<<endl; 
	//}
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(11,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(24,4));
	pPath->GetPos(Pos, 2);
	CPPUNIT_ASSERT(Pos == CPos(24,4));
	pPath->GetPos(Pos, 3);
	CPPUNIT_ASSERT(Pos == CPos(24,4));
	pPath->GetPos(Pos, 4);
	CPPUNIT_ASSERT(Pos == CPos(24,4));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}

//Ѱ·�ܹ�����
void CTestSuiteMetaSceneServer::TestLineCanPassLowBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(11, 10),CPos(13, 10), eFPT_AStar , eBT_LowBarrier);

	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(11,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(13,10));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}


void CTestSuiteMetaSceneServer::TestAStarCanPassLowBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene( "map/newmapa" );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(12, 10),CPos(24, 4), eFPT_AStar , eBT_LowBarrier);

	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(12,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(24,6));
	pPath->GetPos(Pos, 2);
	CPPUNIT_ASSERT(Pos == CPos(25,5));
	pPath->GetPos(Pos, 3);
	CPPUNIT_ASSERT(Pos == CPos(24,4));
	pPath->Release();

	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}

//��ֱ�߷�ʽѰ·�����ڴ�����Ծ����,�ܹ�����
void CTestSuiteMetaSceneServer::TestUseLinePassLowBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(15, 10), eFPT_Line , eBT_LowBarrier);

	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(10,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(15,10));
	pPath->Release();

	pPath = pFindPath->CreatePath(CPos(22, 4),CPos(22, 8), eFPT_Line , eBT_LowBarrier);
	CPPUNIT_ASSERT(pPath == NULL);

	m_pMetaSceneMgr->DestroyMetaScene(pLoader);

}

//��ֱ�߷�ʽѰ·�����ڴ�����Ծ����,���ܹ�����
void CTestSuiteMetaSceneServer::TestUseLineNoPassLowBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(15, 10), eFPT_Line , eBT_NoBarrier);

	CPPUNIT_ASSERT(pPath == NULL);

	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}

//Ѱ·�����ڴ�����Ծ����,�ܹ�ȫ�����ϰ�
void CTestSuiteMetaSceneServer::TestAStarCanPassAllBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(8, 5),CPos(24, 5), eFPT_AStar , eBT_HighBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(8,5));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(24,5));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}


//���ɵ�ipath����reflush����
void CTestSuiteMetaSceneServer::TestReFlush()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(15, 10), eFPT_AStar , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(10,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(12,9));
	pPath->GetPos(Pos, 2);
	CPPUNIT_ASSERT(Pos == CPos(15,10));

	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//����ֱ��Ѱ·��reflush
void CTestSuiteMetaSceneServer::TestLineReFlush()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(15, 10), eFPT_HypoLine , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(10,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(11,10));

	m_pMetaSceneMgr->DestroyMetaScene(pLoader);

}
//����ֱ��Ѱ·�������ϰ��󷵻�ǰһ������λ��
void CTestSuiteMetaSceneServer::TestHypoLine()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa" );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(15, 10), eFPT_HypoLine , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(10,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(11,10));
	pPath->Release();

	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//�������һ�����ϰ��ĸ������
void CTestSuiteMetaSceneServer::TestLastIsBarrier()
{

	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(12, 10), eFPT_HypoLine , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(10,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(11,10));
	pPath->Release();

	pPath = pFindPath->CreatePath(CPos(10, 10),CPos(12, 10), eFPT_Line , eBT_NoBarrier);
	CPPUNIT_ASSERT(pPath == NULL);

	pPath = pFindPath->CreatePath(CPos(10, 10),CPos(12, 10), eFPT_AStar , eBT_NoBarrier);
	CPPUNIT_ASSERT(pPath == NULL);
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//ֱ�ߺ��Ե�һ���ϰ�
void CTestSuiteMetaSceneServer::TestLineFirstIsBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(12, 10),CPos(13, 10), eFPT_Line , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(12,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(13,10));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//a*���Ե�һ���ϰ�
void CTestSuiteMetaSceneServer::TestAFirstIsBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(12, 10),CPos(12, 27), eFPT_AStar , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(12,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(11,11));
	pPath->GetPos(Pos, 2);
	CPPUNIT_ASSERT(Pos == CPos(11,26));
	pPath->GetPos(Pos, 3);
	CPPUNIT_ASSERT(Pos == CPos(12,27));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//����Ѱ·���Ե�һ���ϰ�
void CTestSuiteMetaSceneServer::TestHypoLineFirstIsBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(12, 10),CPos(13, 10), eFPT_HypoLine , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(12,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(13,10));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}

void CTestSuiteMetaSceneServer::TestLineIgnoreEnd()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(12,10 ), eFPT_LineIgnoreEnd , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(10,10));
	pPath->GetPos(Pos, 1);
	CPPUNIT_ASSERT(Pos == CPos(12,10));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//ֱ�ߺ���Ŀ���Ϊ�ϰ���,���ǲ����ҵ�·����
void CTestSuiteMetaSceneServer::TestLineIgnoreEndNoPassBarrier()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(13,10 ), eFPT_LineIgnoreEnd , eBT_NoBarrier);
	CPPUNIT_ASSERT(pPath == NULL);
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//a*����Ŀ������ϰ���
void CTestSuiteMetaSceneServer::TestAIgnoreEnd()
{
	CPos Pos;
	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene(  "map/newmapa"  );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(12, 10),CPos(12,26), eFPT_AStarIgnoreEnd , eBT_NoBarrier);
	pPath->GetPos(Pos, 0);
	CPPUNIT_ASSERT(Pos == CPos(12,10));
	pPath->GetPos(Pos, 3);
	CPPUNIT_ASSERT(Pos == CPos(12,26));
	pPath->Release();
	m_pMetaSceneMgr->DestroyMetaScene(pLoader);
}
//����a�Ǻ����ߵ�ʱ��Ƚ�
void CTestSuiteMetaSceneServer::TestUseTime()
{
	/*int32 nPathNum,nXB,nYB,nXE,nYE,nNum = 32;	
	CSearcher<CNodeState,10000> search;

	time_t t;
	srand(std::time(&t));
	CTimeCheckPoint timego;

	CMetaSceneServer* pLoader=m_pMetaSceneMgr->CreateMetaScene( "test/CTestSuiteMetaScene/blocktest" );
	CMap* pFindPath = static_cast<CMap*>(pLoader->m_pMapPath);
	IPath * pPath = pFindPath->CreatePath(CPos(10, 10),CPos(15, 10), eFPT_AStar , eBT_NoBarrier);
	timego.SetCheckPoint();
	for(int i = 0;i < 2000;)
	{
	nXB = rand()%nNum;
	nYB = rand()%nNum;
	nXE = rand()%nNum;
	nYE = rand()%nNum;
	IPath * pPath = pFindPath->CreatePath(CPos(nXB, nYB),CPos(nXE, nYE), eFPT_AStar , eBT_NoBarrier);
	if(pPath != NULL)
	{
	i++;
	pPath->Release();
	}

	}
	cout <<"Line:"<<pFindPath->m_uCountInLine <<" A* time go =" << timego.GetElapse() << endl;


	timego.SetCheckPoint();
	for(int i = 0;i < 2000;)
	{
	nXB = rand()%nNum;
	nYB = rand()%nNum;
	nXE = rand()%nNum;
	nYE = rand()%nNum;
	if(pFindPath->m_NodeState.IsBarrier(nXB,nYB) || pFindPath->m_NodeState.IsBarrier(nXE,nYE))
	continue;

	search.SearchRoad( &(pFindPath->m_NodeState),sqr::CPathPos(nXB,nYB),sqr::CPathPos(nXE,nYE),nPathNum,eOL_Normal,NULL);
	if(nPathNum >= 0 )
	i++;	
	}
	cout<< "foword time go =" << timego.GetElapse()<<endl;*/
}
