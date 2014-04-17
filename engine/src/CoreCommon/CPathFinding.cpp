
#include "stdafx.h"

#include "IPathFinding.h"
#include "CPathFinding.h"
#include "CPkgFile.h"
#include "CoreCommon.h"
#include "CBufFile.h"
#include "CPath.inl"
#include "CNode.h"
#include "CNode.inl"
#include "CPath.h"
#include "ErrLogHelper.h"
#include "MapInfoManager.inl"
#include "TSqrAllocator.inl"
#include "CDumpEndDrawLine.h"
#include "CAStarDataMgr.h"
#include "CAStarData.inl"
#include "ErrLogHelper.h"
#include "TSqrAllocator.inl"
#include "TBinaryHeap.inl"



namespace sqr
{
	IMap* CreateMap(uint32 uWidthInRegion, uint32 uHeightInRegion, const VecBarrierMatrixSharedPtr* pVecBarrierMatrixSharedPtr)
	{
		return new CMap(uWidthInRegion, uHeightInRegion, pVecBarrierMatrixSharedPtr);
	}
}


CMap::CMap(uint32 uWidthInRegion, uint32 uHeightInRegion, const CBarrierManager::VecBarrierMatrixSharedPtr* pVecBarrierManager)
	: m_uWidthInRegion(uWidthInRegion)
	, m_uHeightInRegion(uHeightInRegion)
	, m_nMaxStep(-1) // ��ʼ��������ֵ
	, m_MapInfo(uWidthInRegion,uHeightInRegion)
	, m_BarrierMgr(&m_MapInfo, pVecBarrierManager)
	, m_DrawLine(&m_BarrierMgr, &m_MapInfo)
{
	m_LoadRegin.resize(uHeightInRegion);

	for( uint32 i=0; i<uHeightInRegion; ++i )
	{
		m_LoadRegin[i].resize( uWidthInRegion );
		m_LoadRegin[i].assign( uWidthInRegion,false ); // ��ʼ���ϰ�����Ϊfalse(���ϰ�)
	}

	m_uCountInLine = 0;

	CAStarDataMgr::Inst().UpdateSize(m_MapInfo.GetQuarWidth(),  m_MapInfo.GetQuarHeight());

	// ���й��ص�CNodeState����
}

CMap::~CMap()
{
}


const CBarrierManager::VecBarrierMatrixSharedPtr* CMap::GetBarrierMatrix()const
{
	return m_BarrierMgr.GetBarrierMatrix();
}

CPath* CMap::DumpEnd(CNode* pEndNode, CNode* pBeginNode)
{
	CAStarData& AStarData = *(CAStarDataMgr::Inst().GetAStarData());
	CNode* pCurrentNode = pEndNode;
	CPath* pPath = new CPath(this);
	CPos pos;
	while ( pCurrentNode != NULL )
	{
		AStarData.GetPos(pos.x, pos.y, pCurrentNode);
		m_deqPathPos.push_front(pos);
		pCurrentNode  = AStarData.GetFront(pCurrentNode);
	}

	PathPosList::iterator it = m_deqPathPos.begin();
	PathPosList::iterator itPos = it++;
	PathPosList::iterator itCur;
	PathPosList::iterator itBefore = itPos;

	if(eFPT_HypoAStarOptiUseWeight == m_EFindType || eFPT_AStarUseWeight == m_EFindType)
	{
		CDumpEndDrawLine dumpEndDrawLine(&m_BarrierMgr);
		dumpEndDrawLine.SetCheckType(m_BarrierMgr.GetBarrierType(itPos->x >>1, itPos->y>>1));
		for (; it != m_deqPathPos.end();++it)
		{
			itCur = it;
			if (! Line(itPos->x, itPos->y, itCur->x, itCur->y, dumpEndDrawLine) )
			{
				pPath->PushBackPath(*itPos);
				if (itBefore != itPos)
				{
					pPath->PushBackPath(*itBefore);
				}
				itPos = itCur;
				dumpEndDrawLine.SetCheckType(m_BarrierMgr.GetBarrierType(itPos->x >> 1, itPos->y >> 1));
			}
			itBefore = itCur;
		}
	}
	else
	{
		for (; it != m_deqPathPos.end();)
		{
			itCur = it;
			if ( Line(itPos->x, itPos->y, itCur->x, itCur->y, m_DrawLine) )
			{
				itBefore = itCur;
				++it;
			}
			else
			{
				pPath->PushBackPath(*itPos);
				itPos = itBefore;
			}
		}
	}

	pPath->PushBackPath(*itPos);
	AStarData.GetPos(pos.x, pos.y, pEndNode);
	if(pos != *itPos)
		pPath->PushBackPath(pos);

	m_deqPathPos.clear();

	return pPath;
}


IPath* CMap::CreatePath(const CPos& Begin, const CPos& End, EFindPathType EFindType, EBarrierType EBarrier,
						const IDynamicBarrierInfo* pBarrierInfo)
{
	// ���ö�̬�ϰ�
	m_BarrierMgr.SetDynamicBarrier(pBarrierInfo);
	IPath* pPath = NULL;

	SQR_TRY
	{
		pPath = CreatePath( Begin, End, EFindType, EBarrier );
	}
	SQR_CATCH(exp)
	{
		// ��ն�̬�ϰ�
		m_BarrierMgr.SetDynamicBarrier(NULL);
		exp.AppendMsg("û�����Ʋ���");
		LogExp(exp);
		return NULL;
	}
	SQR_TRY_END;

	// ��ն�̬�ϰ�
	m_BarrierMgr.SetDynamicBarrier(NULL);

	return pPath;
}

IPath* CMap::CreatePath(const CPos& Begin, const CPos& End, EFindPathType EFindType, EBarrierType EBarrier,
						int32 nMaxStep)
{
	uint32 nCacheMaxStep = m_nMaxStep;
	if ( nMaxStep > -1 )
		m_nMaxStep = nMaxStep << 1;

	IPath* pPath = CreatePath( Begin, End, EFindType, EBarrier );

	m_nMaxStep = nCacheMaxStep;

	return pPath;
}

IPath* CMap::CreatePath(const CPos& Begin, const CPos& End, EFindPathType EFindType, EBarrierType EBarrier,
						int32 nMaxStep, const IDynamicBarrierInfo* pBarrierInfo)
{
	int32 nCacheMaxStep = m_nMaxStep;
	if ( nMaxStep > -1 )
		m_nMaxStep = nMaxStep << 1;

	// ���ö�̬�ϰ�
	m_BarrierMgr.SetDynamicBarrier(pBarrierInfo);

	IPath* pPath = NULL;

	SQR_TRY
	{
		pPath = CreatePath( Begin, End, EFindType, EBarrier );
		m_nMaxStep = nCacheMaxStep;
	}
	SQR_CATCH(exp)
	{
		// ��ն�̬�ϰ�
		m_BarrierMgr.SetDynamicBarrier(NULL);
		ostringstream strm;
		strm << "�������� " << nMaxStep << endl;
		exp.AppendMsg(strm.str());
		LogExp(exp);
		return NULL;
	}
	SQR_TRY_END;

	// ��ն�̬�ϰ�
	m_BarrierMgr.SetDynamicBarrier(NULL);

	return pPath;
}

// ����·�������������Begin��EndΪGrid
IPath* CMap::CreatePath(const CPos& Begin, const CPos& End, EFindPathType EFindType, EBarrierType EBarrier)
{
	// �����ݴ���Ϊ�˷�ֹgmָ��Ĵ������롣������������GetIndex֮ǰ����飬��Ȼ�ᱻGetIndex�Ķ��Ե�
	if ( m_MapInfo.IndexError(Begin.x, Begin.y) == 1 || m_MapInfo.IndexError(End.x,End.y) == 1 )
		return NULL;

	// ��¼��ǰ��Ϣ
	m_EFindType   = EFindType;
	m_EBarrier    = EBarrier;

	Ast(EBarrier <= eBT_HighBarrier);

	// ����Ѱ·����ע���ϰ�����
	m_BarrierMgr.SetJumpBarrier(EBarrier, EFindType, m_MapInfo.GetIndex(Begin.x, Begin.y),  m_MapInfo.GetIndex(End.x, End.y));

	CPath* pPath = NULL; // �ڵ����㷨�ڲ���������Ϊ����㷨���ɹ��ڵ�ҪΪNULL

	// ���ܷ�֧
	switch (EFindType)
	{
		case eFPT_LineIgnoreEnd: // �����յ�Ϊ�ϰ���
			if ( ! m_BarrierMgr.CheckRangeBarrier(End) ) // ����ڽ��˸��Ƿ�Ϊ�ϰ�
				return NULL;

			pPath = LineAlgorithm(Begin, End);
			break;
		case eFPT_AStarIgnoreEnd:
			if ( !m_BarrierMgr.CheckRangeBarrier(End) )
				return NULL;

			if (!(pPath = LineAlgorithm(Begin, End))) // ֱ��Ѱ·��ͨ������A*
				pPath = AStarAlgorithm(Begin, End);   // A*Ѱ·��ͨ����NULL
			break;
		case eFPT_Line:  // ���������ֱ��Ѱ·��Ѱ��ͨ������ѭ��(eFPT_LineIgnoreEnd)
			pPath = LineAlgorithm(Begin, End);
			break;
		case eFPT_AStar: // ���������A*Ѱ·���Ƚ���ֱ��Ѱ·����ͨ����A*
			if (!(pPath = LineAlgorithm(Begin, End))) // ֱ��Ѱ·��ͨ������A*
				pPath = AStarAlgorithm(Begin, End);   // A*Ѱ·��ͨ����NULL
			break;
		case eFPT_HypoAStar: // ֱ��ʹ��HypoLineѰ·
			if (!(pPath = LineAlgorithm(Begin, End)))      // ֱ��Ѱ·��ͨ������A*
				if (!(pPath = AStarAlgorithm(Begin, End))) // A*Ѱ·��ͨʹ��HypoLine
					pPath = HypoLineAlgorithm(Begin, End); // HypoLineѰ·��һ���ɹ���
			break;
		case eFPT_HypoAStarOpti:
			if (!(pPath = LineAlgorithm(Begin, End))) // ֱ��Ѱ·��ͨ������A*
				pPath = AStarAlgorithm(Begin, End); // A*Ѱ·��ͨ����NULL
			break;
		case eFPT_AStarUseWeight:
			pPath = AStarAlgorithm(Begin, End); //ֱ�Ӿ�A*��,����Ԥ��ʹ��ֱ��Ѱ·(ֱ��Ѱ·�޷�����Ȩֵ, ��ֱ����ͨʱ��һ����Ԥ��Ľ��)
			break;
		case eFPT_HypoAStarOptiUseWeight:
			pPath = AStarAlgorithm(Begin, End); //ֱ�Ӿ�A*��,����Ԥ��ʹ��ֱ��Ѱ·(ֱ��Ѱ·�޷�����Ȩֵ, ��ֱ����ͨʱ��һ����Ԥ��Ľ��)
			break;
		//case eFPT_TerrainAStar: // ֱ��ʹ��TerrainAStar����Ѱ·����ͨ��ʹ��HypoLine
		//	if (!(pPath = TerrianAStarAlgorithm(QuarBegin, QuarEnd)))
		//		pPath = HypoLineAlgorithm(QuarBegin, QuarEnd);

		//	break;
		case eFPT_HypoLine: // ֱ��ʹ��HypoLineѰ·
			if (!(pPath = LineAlgorithm(Begin, End)))  // ֱ��Ѱ·��ͨ
				pPath = HypoLineAlgorithm(Begin, End); // HypoLineѰ·��һ���ɹ���
			break;
		default:
			break;
	}

	// ̽�������յ��Ƿ���ͬ
	if ( pPath != NULL )
	{
		if ( const_cast<const CPath*>(pPath)->GetPath().size() < 2 )
		{
			delete pPath;
			pPath = NULL;
		}
	}

	return pPath;
}


IPath* CMap::CreatePath(const int16* pPathData, uint16 uPathDataLen, const CPos& Begin, const CPos& End, const IDynamicBarrierInfo* pBarrierInfo)
{
	// ���ö�̬�ϰ�
	m_BarrierMgr.SetDynamicBarrier(pBarrierInfo);
	CPath* pPath = NULL;
	SQR_TRY
	{
		pPath = new CPath(pPathData, uPathDataLen, Begin, End, this);
	}
	SQR_CATCH(exp)
	{
		// ��ն�̬�ϰ�
		m_BarrierMgr.SetDynamicBarrier(NULL);
		LogExp(exp);
		return NULL;
	}
	SQR_TRY_END;

	// ��ն�̬�ϰ�
	m_BarrierMgr.SetDynamicBarrier(NULL);
	return pPath;
}


// ��ȡ�����ϰ���Ϣ(Grid������ϢҲ���Կ����������)
void CMap::LoadRegionData(CPkgFile& File, uint32 uXInRegion, uint32 uYInRegion)
{
	uint8 nVal = 0, roadweight = 0;
	Ast(uYInRegion < m_LoadRegin.size() && uXInRegion < m_LoadRegin[0].size());
	if ( m_LoadRegin[uYInRegion][uXInRegion] == true )
		return;
	else
		m_LoadRegin[uYInRegion][uXInRegion] = true;

	// ����һ������
	for(uint32 i = 0; i < eRegionSpan; ++i)
	{
		for(uint32 j = 0; j < eRegionSpan; ++j)
		{
			File.read( &(nVal) , sizeof(uint8) ); // ͨ���ڴ�鿽���ķ�ʽ����ȡ�ϰ���Ϣ������(0, 1, 2, 3)
			File.read( &roadweight, sizeof(uint8));
			m_BarrierMgr.SetBarrier((uXInRegion * eRegionSpan) + j, (uYInRegion * eRegionSpan) + i, nVal);
		}
	}
}

// ��ȡ����ȷ�ϰ���Ϣ(Grid������ϢҲ���Կ����������)
void CMap::LoadRgnExactBlockData(CPkgFile& File, uint32 uXInRegion, uint32 uYInRegion)
{
	uint8 bExactBlockType[4];

	// ����һ������
	for(uint32 i = 0; i < eRegionSpan; i++)
	{
		for(uint32 j = 0; j < eRegionSpan; j++)
		{
			File.read( &bExactBlockType, sizeof(bExactBlockType));
		}
	}
}

void CMap::LoadLinkRegionData(CBufFile& File)
{
	m_LinkRegionMgr.LoadData(File);
	
	CAStarDataMgr::Inst().UpdateAllNodeNum(m_LinkRegionMgr.GetAllNodeNum());
}

void CMap::SetSharedData(IMap* pOtherMap)
{
	m_LinkRegionMgr.SetSharedData(&(static_cast<CMap*>(pOtherMap)->m_LinkRegionMgr));
}

EBarrierType CMap::GetBarrierType(int32 x, int32 y)
{
	return m_BarrierMgr.GetBarrierType(x,y);
}

CPos CMap::GetBeginPos(const CPos& pos)
{
	CPos temp = pos;
	if(m_BarrierMgr.IsBarrier(temp.x, temp.y))
	{
		if (temp.x % 2 == 0)
		{
			temp.x = temp.x -1;
		}
		else
		{
			temp.x = temp.x +1;
		}
		if (!m_BarrierMgr.IsBarrier(temp.x, temp.y))
		{
			return temp;
		}
	}
	return pos;
}

// Ŀǰû��ʹ��
bool CMap::IsIntegrate()const
{
	for(uint32 i = 0; i < m_uHeightInRegion; i++)
	{
		for( uint32 j = 0; j < m_uWidthInRegion; j++ )
		{
			if(m_LoadRegin[i][j] == false)
				return false;
		}
	}

	return true;
}

// ���ٵ�ǰ�ڵ�
void CMap::Release()
{
	delete this;
}


// �㷨��
// ֱ��Ѱ·
CPath* CMap::LineAlgorithm(const CPos& Begin, const CPos& End)
{
	if( Line(Begin.x, Begin.y, End.x, End.y, m_DrawLine) ) // ֱ��Ѱ·
	{
		m_uCountInLine++;
		CPath* pPath = new CPath(this);
		pPath->AddPath(End);
		pPath->AddPath(Begin);
		return pPath;
	}

	return NULL; // Ѱ·ʧ��
}


// A*Ѱ·;
CPath* CMap::AStarAlgorithm(const CPos& Begin, const CPos& End)
{
	//LARGE_INTEGER nFreq;
	//QueryPerformanceFrequency(&nFreq);
	//LARGE_INTEGER   nBeginTime;
	//LARGE_INTEGER   nEndTime;
	//double useTime = 0.f;
	//QueryPerformanceCounter(&nBeginTime);

	// ����յ����ϰ��㣬ֱ�ӷ���NULL
	
	CPathDirect* pPathDirect;
	pPathDirect = m_LinkRegionMgr.GetPathDirect(GetBeginPos(Begin), End, m_EFindType, m_EBarrier);
	if (! pPathDirect)
	{
		return NULL;
	}
	int32 nEndX = End.x, nEndY = End.y;
	if( pPathDirect->GetEndPos() != End)
	{
		nEndX = pPathDirect->GetEndPos().x;
		nEndY = pPathDirect->GetEndPos().y;
		m_BarrierMgr.SetJumpBarrier(m_EBarrier, m_EFindType, m_MapInfo.GetIndex(Begin.x, Begin.y),  m_MapInfo.GetIndex(nEndX, nEndY));
	}
	
	
	//if(!m_BarrierMgr.JumpBarrier(m_MapInfo.GetIndex(nEndX, nEndY)))
	//	return NULL;

	CAStarData& AStarData = *(CAStarDataMgr::Inst().GetAStarData()); 
	CAStarData::MinHeap& heapF =  AStarData.GetHeapF();
	uint32 nSearchId = AStarData.GetSearchId();
	int* nOffset = AStarData.GetOffsetArray();
	heapF.clear();

	
	CPath* pPath = NULL;
	CNode* pNode = NULL;
	CNode* pBestNode;
	CNode* pEndNode;
	CNode* pBeginNode;
	CPathDirectNode* pDirectNode;

	int32 x, y;
	int dx1, dy1, dx2, dy2, targetX, targetY;
	int32 cross;
	uint32 nTargetIndex;
	float fRemainderLength;
	float fNewG;// ��㵽��ǰ����ƶ����� 
	float fH; // �ӵ�ǰ���յ���ƶ�����
	float fTargetH; //�ӵ�ǰ��ָ������ƶ�����
	float fF; // F = G + H
	float nDiagonal; // �Խ���
	float nStraight; // ������
	float fNearSize = pPathDirect->GetNearSize();
	uint32 nMaxIndex = pPathDirect->GetPosNum() -1;

	// eFPT_HypoLine �����ж����һ���Ƿ����ϰ���ֱ�ӷ����ϰ�ǰ��Ŀ��
	// �� eFPT_Line ������һ�����ϰ�Ҳ��Ѱ��ͨ��

	pEndNode = AStarData.GetNodeAddress(nEndX, nEndY);
	pBeginNode = AStarData.GetNodeAddress(Begin.x, Begin.y);
	
	float fWeight = 1.f;

	nDiagonal = static_cast<float>(min(abs(nEndX - Begin.x), abs(nEndY - Begin.y)));
	nStraight = static_cast<float>((abs(nEndX - Begin.x) + abs(nEndY - Begin.y)));
	fH = 1.414f * nDiagonal + (nStraight - 2 * nDiagonal);
	float MinH = fH;// ���浱ǰ��ӽ�Ŀ���Ľ�, ��ʼ��Ϊ�������ֵ

	pBeginNode->SetG(0);
	pBeginNode->Open(nSearchId);
	pBeginNode->SetFrontDir(8); // 8�������
	pBeginNode->SetTargetId(0);
	heapF.push(0.0f,pBeginNode);

	m_OptNode = pBeginNode;
	int32 nBestX = 0;
	int32 nBestY = 0;
	const int AroundSize = 8;
	int nOffNum = 0;
	uint32 uExploreTime = 0;
	
	if ( m_nMaxStep != -1 )
		uExploreTime = static_cast<uint32>(m_nMaxStep * m_nMaxStep);
	else
		uExploreTime = static_cast<uint32>(m_nMaxStep);

	//int num = 0;
	for ( uint32 i=0; i<uExploreTime; ++i )
	{
		//++num;
		pBestNode = AStarData.ReturnBestNode();      // ���������б���F���ֵ
		pBestNode->Close(nSearchId);		// �ر�
		AStarData.GetPos(nBestX, nBestY, pBestNode);
		nTargetIndex = pBestNode->GetTargetId();
		pDirectNode = pPathDirect->GetDirectNode(nTargetIndex);
		targetX = pDirectNode->GetPos().x;
		targetY = pDirectNode->GetPos().y;
		fRemainderLength = pDirectNode->GetRemainderLength();
		dx2 =  nBestX - targetX;
		dy2 =  nBestY - targetY;
		for (int j = 0; j < AroundSize; ++j)
		{
			nOffNum = j << 1;
			x = nBestX + nOffset[nOffNum];
			y = nBestY + nOffset[nOffNum+1];
			// �ڵ� x, y ���ڷ�Χ��
			if(!m_MapInfo.CheckXYInRegion(x, y))
				continue;
			pNode = pBestNode + AStarData.GetNodeOffset(j);

			// ���Խڵ��Ƿ��ڹر��б�
			if ( pNode->IsClose(nSearchId) )
				continue;

			// �Ƿ�Ϊ����Խ�ϰ��ȼ�
			if ( !m_BarrierMgr.JumpBarrierAndGetWeight(m_MapInfo.GetIndex(x, y), fWeight))
				continue;

			j< 4 ? fNewG = 1.414f * fWeight : fNewG = (float)fWeight;
			fNewG += pBestNode->GetG();
			if (!pNode->IsOpen(nSearchId) ) // �ڵ㲻�ڿ����б�
			{
				dx1 = targetX - x;
				dy1 = targetY - y;

				nDiagonal = static_cast<float>(min(abs(dx1), abs(dy1)));
				nStraight = static_cast<float>((abs(dx1) + abs(dy1)));
				cross = abs(dx1 * dy2 - dx2 * dy1);
				
				fTargetH = 1.414f * nDiagonal + (nStraight - 2 * nDiagonal);
				fH = fTargetH + fRemainderLength + static_cast<float>(cross) * 0.001f;

				pNode->Open(nSearchId);
				pNode->SetG(fNewG); // ��¼��Gֵ
				pNode->SetTargetId((fTargetH < fNearSize && nTargetIndex < nMaxIndex) ? nTargetIndex + 1: nTargetIndex);
				pNode->SetFrontDir(j);       // ��¼���ڵ� ����

				fF = fNewG + fH;
				heapF.push(fF,pNode); // ��¼Fֵ
				
				// �������ŵ�
				if ( fH < MinH )
				{
					MinH = fH;
					m_OptNode = pNode;
				}
			}
			else // ���ڿ����б��У��ж�Gֵ�Ƿ�Ƚ�С�������޸ĸ��ڵ�Ϊ��ǰ�ڵ�
			{
				if(fNewG < pNode->GetG())
				{
					pNode->SetG(fNewG);
					pNode->SetFrontDir(j);
				}
			}
		}

		// ����յ��Ѿ��ڿ����б��ر��б���
		if( pEndNode->IsOpen(nSearchId) )
		{
			pPath = DumpEnd(pEndNode, pBeginNode);
			break;
		}
		else if(heapF.empty()) // �����б��ѿ�
		{
			break;
		}
	}

	if ( pPath == NULL && (m_EFindType == eFPT_HypoAStarOpti || m_EFindType == eFPT_HypoAStarOptiUseWeight) && m_OptNode != pBeginNode )
		pPath = DumpEnd(m_OptNode, pBeginNode);
	
	//QueryPerformanceCounter(&nEndTime);
	//useTime = static_cast<double>(nEndTime.QuadPart - nBeginTime.QuadPart) / nFreq.QuadPart;
	//printf("use time: %f\n", useTime);
	//printf("ָ����A* ���� %d  ָ������ %d \n", num, pPathDirect->GetPosNum());
	return pPath;
}

// ���ǵ��ε�Ѱ·�㷨
CPath* CMap::TerrianAStarAlgorithm(const CPos& Begin, const CPos& End)
{
	return NULL;
}

// ���ص�һ���ϰ�����㷨
CPath* CMap::HypoLineAlgorithm(const CPos& Begin, const CPos& End)
{
	CPath* pPath = new CPath(this);
	pPath->AddPath(CPos(m_DrawLine.GetLastX(), m_DrawLine.GetLastY())); // LastX��LastY����ֱ��Ѱ·
	pPath->AddPath(Begin);

	return pPath;
}

