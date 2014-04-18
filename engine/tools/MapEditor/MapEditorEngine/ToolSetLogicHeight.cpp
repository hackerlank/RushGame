#include "stdafx.h"
#include "ToolSetLogicHeight.h"
#include "CMapEditApp.h"
#include "TerrainMesh.h"
#include "WireBox.h"
#include "Render.h"
#include "Operator.h"
#include "Operator.inl"
#include "OperationLog.h"
#include "StringHelper.h"
#include "CEventState.h"

namespace sqr
{
	const int NEIGHBOR_NUM = 8;	// ��,��,��,��

	CToolSetLogicHeight::CToolSetLogicHeight()
	{
		m_fCopyLogicHeight			= 0.0f;
		m_fModelLogicHeight			= 0.0f;
		m_fLogicCheckDiffNum		= 1.0f;
		m_bCheckMapLogicHeight		= false;
		m_pRS						= NULL;
	}

	CToolSetLogicHeight::~CToolSetLogicHeight()
	{
		if(m_pRS)
			m_pRS = NULL;
	}

	void CToolSetLogicHeight::OnEvent(const CMsg& msg)
	{	
		COperator * pOperator = CMapEditApp::GetInst()->GetOperator();
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();

		switch ( msg.msgSource )
		{
		case MGS_KEYMSG:
			{
				break;
			}

		case MGS_MOUSEMSG :
			{
				switch(msg.msgType.mouse_type)
				{
				case MGT_MOUSE_LBNDOWN:
				case MGT_MOUSE_MOVE :	
					{
						if ( CEventState::GetInst()->GetState(MGT_MOUSE_LBUTTON) == MGT_MOUSE_LBNDOWN )
						{
							m_vecVertexIndicesLock = this->GetBrushVertexIndices();
						}
					}
					break;

				default:
					break;
				}
			}

		default:
			break;
		}
	}

	void CToolSetLogicHeight::RenderUnit(const vector<DWORD> & vertexIndexs)
	{
		if( vertexIndexs.empty() )
			return;

		CRenderDevice * pRender = CRenderDevice::GetInst();
		COperator * pOperator   = CMapEditApp::GetInst()->GetOperator();
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();

		// for brush
		CVector3f vertex[VERTEX_BATCH_SIZE];
		CVector2f uv0[VERTEX_BATCH_SIZE];
		VerColorTex rvertex[VERTEX_BATCH_SIZE];

		size_t cnt = vertexIndexs.size();
		DWORD  drawnCnt = 0;
		for ( DWORD n = 0; n < cnt; ++n, ++drawnCnt )
		{	
			if ( drawnCnt && !((drawnCnt) % GRID_BATCH_SIZE) )
			{	
				pRender->DrawLayer3DWireQuad(&m_pRS, rvertex, GRID_BATCH_SIZE);

				m_pRS->m_TexOP_S0 = TOP_MODULATE;
				m_pRS->m_TexArg1_S0 = TA_TEXTURE;
				m_pRS->m_TexArg2_S0 = TA_DIFFUSE;

				m_pRS->m_AlpOP_S0 = TOP_SELECTARG1;
				m_pRS->m_AlpArg1_S0 = TA_TEXTURE;

				m_pRS->m_AlphaTestEnable = true;
				m_pRS->m_Alpharef = 0x40;
				m_pRS->m_AlphaTestFun = CMP_GREATER; 
				m_pRS->m_AlphaBlendEnable = false;
				m_pRS->m_LightEnable = false;
				m_pRS->m_ZTestEnable = false;
				m_pRS->m_ZWrite = true;
				m_pRS->m_Cull = CULLT_CCW;
				drawnCnt = 0;
			}

			SVertex & s = pTerrain->GetVertex(vertexIndexs[n]);

			vertex[0] = s.vPosition + CVector3f(-GRID_SPACE/2,0.01f,-GRID_SPACE/2);
			vertex[1] = s.vPosition + CVector3f(-GRID_SPACE/2, 0.01f, GRID_SPACE/2);
			vertex[2] = s.vPosition + CVector3f(GRID_SPACE/2, 0.01f, -GRID_SPACE/2);
			vertex[3] = s.vPosition + CVector3f(GRID_SPACE/2,0.01f,GRID_SPACE/2);

			rvertex[6 * drawnCnt + 0] = VerColorTex(vertex[0].x, vertex[0].y, vertex[0].z, 0xff000000, 0.0f, 1.0f);
			rvertex[6 * drawnCnt + 1] = VerColorTex(vertex[1].x, vertex[1].y, vertex[1].z, 0xff000000, 0.0f, 0.0f);
			rvertex[6 * drawnCnt + 2] = VerColorTex(vertex[2].x, vertex[2].y, vertex[2].z, 0xff000000, 1.0f, 1.0f);

			rvertex[6 * drawnCnt + 3] = VerColorTex(vertex[1].x, vertex[1].y, vertex[1].z, 0xff000000, 0.0f, 0.0f);
			rvertex[6 * drawnCnt + 4] = VerColorTex(vertex[3].x, vertex[3].y, vertex[3].z, 0xff000000, 1.0f, 0.0f);
			rvertex[6 * drawnCnt + 5] = VerColorTex(vertex[2].x, vertex[2].y, vertex[2].z, 0xff000000, 1.0f, 1.0f);
		}
		if( drawnCnt > 0 )
		{
			pRender->DrawLayer3DWireQuad(&m_pRS, rvertex, drawnCnt);

			m_pRS->m_TexOP_S0 = TOP_MODULATE;
			m_pRS->m_TexArg1_S0 = TA_TEXTURE;
			m_pRS->m_TexArg2_S0 = TA_DIFFUSE;

			m_pRS->m_AlpOP_S0 = TOP_SELECTARG1;
			m_pRS->m_AlpArg1_S0 = TA_TEXTURE;

			m_pRS->m_AlphaTestEnable = true;
			m_pRS->m_Alpharef = 0x40;
			m_pRS->m_AlphaTestFun = CMP_GREATER; 
			m_pRS->m_AlphaBlendEnable = false;

			m_pRS->m_LightEnable = false;
			m_pRS->m_ZTestEnable = false;
			m_pRS->m_ZWrite = true;
			m_pRS->m_Cull = CULLT_CCW;
		}
	}

	void CToolSetLogicHeight::Render()
	{
		vector<DWORD> & vecVertexIndices = this->GetBrushVertexIndices();
		this->RenderUnit(vecVertexIndices);
		//////////////////////////////////////////////////////////////////////////
	
		// for lock
		this->RenderUnit(m_vecVertexIndicesLock);
	}

	DWORD CToolSetLogicHeight::GetVertexBrushSizeInX() 
	{ 
		return m_processParams.dwVertexBrushSizeInX; 
	}

	void CToolSetLogicHeight::SetVertexBrushSizeInX(DWORD dw) 
	{ 
		m_processParams.dwVertexBrushSizeInX = dw;	
	}

	void CToolSetLogicHeight::IncVertexBrushSizeInX() 
	{ 
		++m_processParams.dwVertexBrushSizeInX; 
	}

	void CToolSetLogicHeight::DecVertexBrushSizeInX() 
	{ 
		--m_processParams.dwVertexBrushSizeInX;
	}

	DWORD CToolSetLogicHeight::GetVertexBrushSizeInZ() 
	{ 
		return m_processParams.dwVertexBrushSizeInZ; 
	}

	void CToolSetLogicHeight::SetVertexBrushSizeInZ(DWORD dw) 
	{ 
		m_processParams.dwVertexBrushSizeInZ = dw;	
	}

	void CToolSetLogicHeight::IncVertexBrushSizeInZ() 
	{ 
		++m_processParams.dwVertexBrushSizeInZ; 
	}

	void CToolSetLogicHeight::DecVertexBrushSizeInZ() 
	{ 
		--m_processParams.dwVertexBrushSizeInZ;
	}

	vector<DWORD> & CToolSetLogicHeight::GetBrushVertexIndices()
	{
		static vector<DWORD> vecBrushVertexIndices;

		if( m_bCheckMapLogicHeight )
		{
			return m_vecErrorLogicHeightVertexs;
		}
		else
		{
			CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
			COperator * pOperator   = CMapEditApp::GetInst()->GetOperator();

			vecBrushVertexIndices.clear();

			int nVertexIndexHot = pOperator->GetVertexIndexHot();
			DWORD dwBrushSizeInX = this->GetVertexBrushSizeInX();
			DWORD dwBrushSizeInZ = this->GetVertexBrushSizeInZ();
			DWORD dwTerrainWidth = pTerrain->GetWidth();

			if ( pTerrain->IsValid(pOperator->GetGridIndexHot()) )
			{
				DWORD dwCnt = dwBrushSizeInX * dwBrushSizeInZ;

				int nVertexIndex = nVertexIndexHot - dwBrushSizeInX/2 - (dwBrushSizeInZ/2) * (pTerrain->GetWidth()+1);

				for ( DWORD j = 0; j < dwCnt; j++)
				{
					if ( j )
					{
						if ( !(j % dwBrushSizeInX) ) 
							nVertexIndex += dwTerrainWidth + 1 - dwBrushSizeInX + 1;
						else
							++nVertexIndex;
					}

					if( pTerrain->IsValidVertexIndex(nVertexIndex) )
						vecBrushVertexIndices.push_back(nVertexIndex);
				}
			}
		}

		return vecBrushVertexIndices;
	}

	void CToolSetLogicHeight::CopyLogicHeight()
	{
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		int nHotVertexIndex		= CMapEditApp::GetInst()->GetOperator()->GetVertexIndexHot();
		if(pTerrain->IsValidVertexIndex(nHotVertexIndex))
		{
			SVertex vertex = pTerrain->GetVertex(nHotVertexIndex);
			m_fCopyLogicHeight = vertex.fLogicHeight;

			char szHeightBuf[64];
			sprintf(szHeightBuf, "��������¸����߼��� %f", m_fCopyLogicHeight);
			COpearationLog::GetInst()->WriteOperationLog(szHeightBuf);
		}
	}

	void CToolSetLogicHeight::PastateLogicHeight()
	{
		if(m_fCopyLogicHeight <= 0.0f) 
			return;

		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		m_vecVertexIndicesLock.clear();
		m_vecVertexIndicesLock = this->GetBrushVertexIndices();
		for ( size_t i = 0; i < m_vecVertexIndicesLock.size(); ++i )
		{
			DWORD dwVertexIndex = m_vecVertexIndicesLock[i];
			if(pTerrain->IsValidVertexIndex(dwVertexIndex))
			{
				SVertex & vertex = pTerrain->GetVertex(dwVertexIndex);
				vertex.fLogicHeight = m_fCopyLogicHeight; 
				CVector3f vCenter = vertex.vPosition;	
				
				vertex.GetWireBoxLogicHeight()->SetHeight(m_fCopyLogicHeight);					
				vertex.GetWireBoxLogicHeight()->SetRelativePos(vCenter);
				vertex.GetWireBoxLogicHeight()->UpdateAbsolute();
			}
		}

		char szHeightBuf[64];
		sprintf(szHeightBuf, "���������ճ���߼��� %f", m_fCopyLogicHeight);
		COpearationLog::GetInst()->WriteOperationLog(szHeightBuf);
	}

	void CToolSetLogicHeight::EraseLogicHeight()
	{
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		if( m_bCheckMapLogicHeight )
		{
			m_vecVertexIndicesLock.clear();
			m_vecVertexIndicesLock = this->GetBrushVertexIndices();
		}

		for ( size_t i = 0; i < m_vecVertexIndicesLock.size(); ++i )
		{
			DWORD dwVertexIndex = m_vecVertexIndicesLock[i];
			if(pTerrain->IsValidVertexIndex(dwVertexIndex))
			{
				SVertex & vertex = pTerrain->GetVertex(dwVertexIndex);
				vertex.fLogicHeight = 0.0f; 
				vertex.GetWireBoxLogicHeight()->Remove();
				vertex.pWireBoxLogicHeight = NULL;
			}
		}

		COpearationLog::GetInst()->WriteOperationLog("ɾ���߼���");
	}

	void CToolSetLogicHeight::QuickAddLogicHeight()
	{
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		if( m_bCheckMapLogicHeight )
		{
			m_vecVertexIndicesLock.clear();
			m_vecVertexIndicesLock = this->GetBrushVertexIndices();
		}

		float fLogicHeight = this->GetMeshLogicHeight();
		for ( size_t i = 0; i < m_vecVertexIndicesLock.size(); ++i )
		{	
			DWORD dwVertexIndex = m_vecVertexIndicesLock[i];

			if( !pTerrain->IsValidVertexIndex(dwVertexIndex) )
				return;

			SVertex & vertex = pTerrain->GetVertex(dwVertexIndex);
			CVector3f vCenter = vertex.vPosition;	

			vertex.fLogicHeight = fLogicHeight - vCenter.y + 1.5f;
			if(vertex.fLogicHeight < 0.0f) 
			{
				vertex.fLogicHeight = 0.0f;
				vertex.pWireBoxLogicHeight = NULL;
				continue;
			}

			vertex.GetWireBoxLogicHeight()->SetHeight(vertex.fLogicHeight);					
			vertex.GetWireBoxLogicHeight()->SetRelativePos(vCenter);
			vertex.GetWireBoxLogicHeight()->UpdateAbsolute();
		}

		COpearationLog::GetInst()->WriteOperationLog("���������߼���");
	}

	void CToolSetLogicHeight::AddLogicHeight()
	{
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		if( m_bCheckMapLogicHeight )
		{
			m_vecVertexIndicesLock.clear();
			m_vecVertexIndicesLock = this->GetBrushVertexIndices();
		}

		float fLogicHeight = this->GetMeshLogicHeight();
		for ( size_t i = 0; i < m_vecVertexIndicesLock.size(); ++i )
		{	
			DWORD dwVertexIndex = m_vecVertexIndicesLock[i];

			if( !pTerrain->IsValidVertexIndex(dwVertexIndex) )
				return;

			SVertex & vertex = pTerrain->GetVertex(dwVertexIndex);
			CVector3f vCenter = vertex.vPosition;	

			vertex.fLogicHeight += 4.0f;

			if(vertex.fLogicHeight < 0.0f) 
			{
				vertex.fLogicHeight = 0.0f;
				vertex.pWireBoxLogicHeight = NULL;
				continue;
			}

			vertex.GetWireBoxLogicHeight()->SetHeight(vertex.fLogicHeight);					
			vertex.GetWireBoxLogicHeight()->SetRelativePos(vCenter);
			//UpdateAbsolute����wirebox����������ϵ
			vertex.GetWireBoxLogicHeight()->UpdateAbsolute();
		}

		COpearationLog::GetInst()->WriteOperationLog("�����߼��߸߶�");
	}

	void CToolSetLogicHeight::DeduceLogicHeight()
	{
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		if( m_bCheckMapLogicHeight )
		{
			m_vecVertexIndicesLock.clear();
			m_vecVertexIndicesLock = this->GetBrushVertexIndices();
		}

		for ( size_t i = 0; i < m_vecVertexIndicesLock.size(); ++i )
		{
			DWORD dwVertexIndex = m_vecVertexIndicesLock[i];

			if( !pTerrain->IsValidVertexIndex(dwVertexIndex) )
				return;

			SVertex & vertex = pTerrain->GetVertex(dwVertexIndex);
			vertex.fLogicHeight -= 4.0f;
			if(vertex.fLogicHeight < 0.0f) 
			{
				vertex.fLogicHeight = 0.0f;
				vertex.pWireBoxLogicHeight = NULL;
				continue;
			}

			CVector3f vCenter = vertex.vPosition;

			vertex.GetWireBoxLogicHeight()->SetHeight(vertex.fLogicHeight);
			vertex.GetWireBoxLogicHeight()->SetRelativePos(vCenter);
			vertex.GetWireBoxLogicHeight()->UpdateAbsolute();
		}

		COpearationLog::GetInst()->WriteOperationLog("�����߼��߸߶�");
	}

	void CToolSetLogicHeight::QucikDeduceLogicHeight()
	{
		CTerrainMesh * pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		float fLogicHeight = this->GetMeshLogicHeight();
		if( m_bCheckMapLogicHeight )
		{
			m_vecVertexIndicesLock.clear();
			m_vecVertexIndicesLock = this->GetBrushVertexIndices();
		}

		for ( size_t i = 0; i < m_vecVertexIndicesLock.size(); ++i )
		{
			DWORD dwVertexIndex = m_vecVertexIndicesLock[i];

			if( !pTerrain->IsValidVertexIndex(dwVertexIndex) )
				return;

			SVertex & vertex = pTerrain->GetVertex(dwVertexIndex);
			CVector3f vCenter = vertex.vPosition;	
			float ffLogicHeight = fLogicHeight - vCenter.y;
			vertex.fLogicHeight = vertex.fLogicHeight - ffLogicHeight - 1.5f;

			if(vertex.fLogicHeight < 0.0f) 
			{
				vertex.fLogicHeight = 0.0f;
				vertex.pWireBoxLogicHeight = NULL;
				continue;
			}

			vertex.GetWireBoxLogicHeight()->SetHeight(vertex.fLogicHeight);
			vertex.GetWireBoxLogicHeight()->SetRelativePos(vCenter);
			vertex.GetWireBoxLogicHeight()->UpdateAbsolute();
		}

		COpearationLog::GetInst()->WriteOperationLog("���ٽ����߼��߸߶�");
	}

	void CToolSetLogicHeight::CheckUpMapLogic( map<DWORD, DWORD>& errorVertexIndexs )
	{
		errorVertexIndexs.empty();
		this->m_bCheckMapLogicHeight = true;

		CTerrainMesh *pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		
		float fCompareValue = GRID_SPACE * m_fLogicCheckDiffNum;
		int dwWidth		  = pTerrain->GetWidth();
		int dwVertexWidth = dwWidth + 1;
		int nVertexCount = pTerrain->GetVertexCount();
		for (int i = 0; i < nVertexCount; ++i)
		{
			DWORD dwVertexIndex = i;
			SVertex &vertex = pTerrain->GetVertex(dwVertexIndex);

			bool bBelongBlock = false;
			for( int m = 0; m < 4; ++m )
			{
				const SGrid &grid = pTerrain->GetGrid(vertex.dwGridsBelongto[m]);
				/*������и��ϣ��Ͳ������߼��ߵļ�飬��Ϊ�и��ϣ������޷�����*/
				if( grid.nBlockType > 1 )
					bBelongBlock = true;
			}

			if( !bBelongBlock )
			{
				const SGrid &grid = pTerrain->GetGrid(vertex.dwGridsBelongto[0]);
				float fCurLogicHeight = vertex.fLogicHeight;
				float fCurFinalHeight = vertex.vPosition.y  + fCurLogicHeight;
				DWORD dwVertexIndex   = vertex.dwVertexIndex;

				///���ַ����8�����ڵ���бȽ�
				vector<int> neighborIndex;
				neighborIndex.resize(NEIGHBOR_NUM);
				neighborIndex[0] = dwVertexIndex - dwVertexWidth - 1;
				neighborIndex[1] = dwVertexIndex - dwVertexWidth;
				neighborIndex[2] = dwVertexIndex - dwVertexWidth + 1;

				neighborIndex[3] = dwVertexIndex - 1;
				neighborIndex[4] = dwVertexIndex + 1;

				neighborIndex[5] = dwVertexIndex + dwVertexWidth - 1;
				neighborIndex[6] = dwVertexIndex + dwVertexWidth;
				neighborIndex[7] = dwVertexIndex + dwVertexWidth + 1;

				if( dwVertexIndex % dwVertexWidth == 0 )
				{
					neighborIndex[0] = -1;
					neighborIndex[3] = -1;
					neighborIndex[5] = -1;
				}

				if( (dwVertexIndex + 1) % dwVertexWidth == 0 )
				{
					neighborIndex[2] = -1;
					neighborIndex[4] = -1;
					neighborIndex[7] = -1;
				}

				for ( int m = 0; m < NEIGHBOR_NUM; ++m )
				{
					int nNeiVertexIndex = neighborIndex[m];
					if( !pTerrain->IsValidVertexIndex(nNeiVertexIndex) )
						continue;

					SVertex neivertex = pTerrain->GetVertex(nNeiVertexIndex);

					float fNeiLogicHeight = neivertex.fLogicHeight;
					float fNeiFinalHeight = neivertex.vPosition.y  + fNeiLogicHeight;
					float fDiff			  = fCurFinalHeight - fNeiFinalHeight;

					if( fNeiLogicHeight == 0.0f && fCurLogicHeight == 0.0f )
						continue;

					if( fDiff < 0.0f )
					{
						///�����ǰ���ʵ�ʸ߶ȣ���ĸ߶�+�߼��ߣ��ľ���ֵС�����õ��޶ȣ���ô��Ϊ�ĵ���߼��������⣬�Ͳ��ټ���ʣ������ڵ�
						if( fabs(fDiff) >= fCompareValue )
						{
							errorVertexIndexs.insert(make_pair(dwVertexIndex, grid.dwGridIndex));
							break;
						}
					}
					else
					{
						///�����ǰ�����߼��ߣ����ǵ�ǰ���ʵ�ʸ߶ȣ���ĸ߶�+�߼��ߣ��������õ��޶ȣ���ô��Ϊ�ĵ���߼��������⣬�Ͳ��ټ���ʣ������ڵ�
						if( fCurLogicHeight > 0.0f )
						{
							if( fDiff >= fCompareValue )
							{
								errorVertexIndexs.insert(make_pair(dwVertexIndex, grid.dwGridIndex));
								break;
							}
						}
					}
				}
			}
		}

		size_t size = errorVertexIndexs.size();
		string log = format("�Զ���⵽��ͼ���У�%d��������߼���", size);
		MessageBox(NULL,log.c_str(),"��ʾ",MB_OK);
	}

	void CToolSetLogicHeight::SelecErrorLogicVertexIndex( map<DWORD, DWORD> errorVertexIndexs )
	{
		if( errorVertexIndexs.empty() )
			return;

		CTerrainMesh* pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		if ( pTerrain == NULL )
			return;

		m_vecErrorLogicHeightVertexs.clear();

		set<DWORD> bufferindexs;
		DWORD dwVertexIndex = 0, dwGridIndex = 0;
		map<DWORD, DWORD>::iterator beg = errorVertexIndexs.begin();
		map<DWORD, DWORD>::iterator end = errorVertexIndexs.end();
		for ( beg; beg != end; ++beg )
		{
			dwVertexIndex = beg->first;
			dwGridIndex   = beg->second;

			SGrid &grid = pTerrain->GetGrid(dwGridIndex);
			bufferindexs.insert(grid.dwBufferIndex);

			m_vecErrorLogicHeightVertexs.push_back(dwVertexIndex);
		}

		COperator *pOperator = NULL;
		pOperator =  CMapEditApp::GetInst()->GetOperator();
		if ( pOperator == NULL )
			return;

		int nX = 0, nZ = 0;
		pOperator->GetMapCoordinate(dwGridIndex, nX, nZ);
		pOperator ->MoveTo(nX,nZ);

		pTerrain->ReWriteTerrainBuffer( bufferindexs, 10 );
	}

	void CToolSetLogicHeight::ClearErrorLogicVertexIndex()
	{
		m_vecErrorLogicHeightVertexs.clear();

		CTerrainMesh* pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
		if ( pTerrain == NULL )
			return;

		pTerrain->ReWriteTerrainBuffer(pTerrain->m_TerrainBufferIndexs, 2);
	}

	void CToolSetLogicHeight::SaveMapLogicHeight()
	{
		locale::global(locale(""));
		FILE * fp = NULL;
		fp = fopen("f:/1.heightinfo","wb");
		if (fp)
		{
			CTerrainMesh *pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
			int nVertexCnt = pTerrain->GetVertexCount();
			for ( int m = 0; m < nVertexCnt; ++m )
			{
				SVertex &vertex = pTerrain->GetVertex(m);
				float fLogic = vertex.fLogicHeight;
				fwrite(&fLogic, sizeof(fLogic), 1, fp);
			}
		}

		fclose(fp);
		MessageBox(NULL,"�߼�����Ϣ�������","��ʾ",MB_OK);
	}

	void CToolSetLogicHeight::LoadMapLogicHeight()
	{
		FILE * fp = NULL;
		fp = fopen("f:/1.heightinfo","rb");
		if (fp )
		{
			CTerrainMesh *pTerrain = CMapEditApp::GetInst()->GetEditingMesh();
			int nVertexCnt = pTerrain->GetVertexCount();
			for ( int m = 0; m < nVertexCnt; ++m )
			{
				SVertex &vertex = pTerrain->GetVertex(m);
				float fLogic = 0;
				fread(&fLogic,sizeof(fLogic),1,fp);
				vertex.fLogicHeight = fLogic;
			}
		}

		fclose(fp);
		MessageBox(NULL,"�߼�����Ϣ�����","��ʾ",MB_OK);
	}
}
