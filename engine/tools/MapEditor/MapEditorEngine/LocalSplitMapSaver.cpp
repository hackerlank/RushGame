//------------------------------------------------------------------------------
//  CLocalSplitMapSaver.cpp
//  (C) 2009 ThreeOGCMan
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "LocalSplitMapSaver.h"
#include "GraphicCommon.h"
#include "TerrainMesh.h"
#include "CMapEditApp.h"
#include "Operator.h"
#include "Operator.inl"
#include "TerrainMeshObject.h"
#include "CMapEditObject.h"
#include "ModelGroup.h"
#include "Engine.h"
#include "Render.h"
#include "River.h"
#include "TerrainMeshRegion.h"
#include "CMapEditCamera.inl"
#include "CCameraManager.h"
#include "LocalMapAudioSaver.h"
#include "LocalMapLoginSaver.h"
#include "TraceHelper.h"
#include "CodeCvs.h"
#include "CXmlConfig.inl"
#include "CMapEditModelObjPro.h"
#include "IoSystem.h"
#include "LocalVersion.h"
#include "SceneRegionMgr.h"
#include "SceneAudio.h"
#include "MapCommonDataSaver.h"
#include "Console.h"
#include "ToolRecord.h"
#include "SceneEnvMgr.h"
#include "SceneEnvMgr.inl"
#include "ToolMainPlayer.h"
#include "CMapEffect.h"

namespace sqr
{

	//------------------------------------------------------------------------------
	CLocalSplitMapSaver::CLocalSplitMapSaver()
		: terrain(NULL)
		, progress(NULL)
	{
	}

	//------------------------------------------------------------------------------
	///save �ָ���ͼ��.project file
	void
	CLocalSplitMapSaver::SaveAsSplitProject()
	{
		string strFileName	 = m_strMapPathName + ".project";
 		size_t setTileSize   = setTitleResourceName.size();
 		size_t setEffectSize = setEffecteResourceName.size();
 		set<string>::iterator iter = setTitleResourceName.begin();

		vector<string> vecAttribValueName, vecAttribValueValue;
		TCHAR s[64];
		sprintf(s,"%d",setTileSize);
		vecAttribValueName.push_back("ResSetCount");
		vecAttribValueValue.push_back(s);

		for ( size_t i = 0; i < setTileSize ; ++i )
		{
			TCHAR s[64];
			string str = *iter;
			sprintf(s,"ResSetFileName%d",i);
			
			vecAttribValueName.push_back(s);
			vecAttribValueValue.push_back(str);

			++iter;
		}

		CXmlConfig *pProjectXmlCfg = new CXmlConfig("x");
		pProjectXmlCfg->AttribSet("x", "ͼ�ؼ�", vecAttribValueName, vecAttribValueValue);
		pProjectXmlCfg->SaveConfig(strFileName.c_str());
		delete pProjectXmlCfg;
		pProjectXmlCfg = NULL;
	}

	//------------------------------------------------------------------------------
	///load ���ָ��ͼ��.project file
	void
	CLocalSplitMapSaver::LoadAsMapProject(const string& path)
	{
		if( path.empty() )
		{
			MessageBox(NULL, "���ָ�ĵ�ͼ��Ϊ��", "��ʾ", MB_OK);
			return;
		}
		
		string strFile = path;
		strFile += ".project";

		//////////////////////////////////////////////////////////////////////////
		///load map project
		LogConsole(strFile.c_str());
		
		SQR_TRY
		{
			locale::global(locale(""));
			ifstream in(strFile.c_str());
			
			CXmlConfig *pMapProjectFile = new CXmlConfig( "x", in );
			///x property
			TiXmlNode  *pResourceSetNode = pMapProjectFile->Get<TiXmlElement*>("ͼ�ؼ�");
			if( pResourceSetNode )
			{
				TiXmlElement *pElement = pResourceSetNode->ToElement();
				if( pElement )
				{
					int nCnt = atoi(pElement->Attribute("ResSetCount"));
					for ( int i = 0; i < nCnt; ++i )
					{
						TCHAR s[64];
						sprintf(s,"ResSetFileName%d",i);
						string str = pElement->Attribute(s);

						setTitleResourceName.insert(str);
					}
				}
			}

			in.close();

			delete pMapProjectFile;
			pMapProjectFile = NULL;
		}
		SQR_CATCH(exp)
		{
			MessageBox(NULL, exp.ErrorMsg(), "����", MB_OK);

			return;
		}
		SQR_TRY_END;

	}


	//------------------------------------------------------------------------------
	void
	CLocalSplitMapSaver::SaveAsSplitRgnLightInfo()
	{
		COperator *pOperator = NULL;
		pOperator = CMapEditApp::GetInst()->GetOperator();
		if ( pOperator == NULL )
			return;

		//////////////////////////////////////////////////////////////////////////
		//�������õ���Ϣ���б���

		string strRgnLightPath = m_strMapPathName + ".operaterinfo";
		FILE * fp = NULL;

		fp = fopen(strRgnLightPath.c_str(),"wb");
		if (fp)
		{
			bool flag = false;

			//�Ƿ�ȫ�ֹ�Ч
			flag = CSceneEnvMgr::GetInst()->GetIsTurnOnGaussTarget();
			fwrite(&flag, 1, 1, fp);

			//�Ƿ������ǹ�Ч
			flag = false;
			fwrite(&flag, 1, 1, fp);

			//�Ƿ�����Ч
			flag = CSceneEnvMgr::GetInst()->GetDoesEnableFog();
			fwrite(&flag, 1, 1, fp);

			//�Ƿ������͸��
			flag = false;
			fwrite(&flag, 1, 1, fp);

			//�Ƿ���ȫ���ӽ���Ӱ
			flag = CSceneEnvMgr::GetInst()->GetIsLightMapShadow();
			fwrite(&flag, 1, 1, fp);

			//�Ƿ�����׼�ӽ���Ӱ
			flag = CSceneEnvMgr::GetInst()->GetIsRealtimeShadow();
			fwrite(&flag, 1, 1, fp);

			CSceneRegionMgr::GetInst()->SaveSceneRegionInfo(fp, true);

			fclose(fp);
		}
		else
		{
			string log = strRgnLightPath;
			log += "���ļ��ѱ�����������ǰ�Ƚ��������򱣴���ļ�ʧ��";

			MessageBox( NULL, log.c_str() , "��ʾ", MB_OK );
		}
	}

	//------------------------------------------------------------------------------
	void
	CLocalSplitMapSaver::SaveAsSplitLocal()
	{
		COperator *pOperator = NULL;
		pOperator = CMapEditApp::GetInst()->GetOperator();
		if( pOperator == NULL )
			return;

		CMapCommonDataSaver::GetInst()->SetOperator(pOperator);
		CMapCommonDataSaver::GetInst()->SetTerrainMesh(terrain);

		CMapEditApp::GetInst()->SetEditingMesh(sqr::EEM_TERRAIN);

		locale loc;
		use_facet<ctype<TCHAR> >( loc ).tolower
			( &m_strMapPathName[0], &m_strMapPathName[m_strMapPathName.length()] );

		string strLocalPath = m_strMapPathName + ".local";
		FILE * fp = fopen(strLocalPath.c_str(),"wb");
		int SceneLightCount = 0;	//��¼�������Դ����
		int nSpotLightCount = 0;    //��¼�����۹�Ƹ���

		if ( fp )
		{
			set<DWORD> eraseGrids;

			// header
			fwrite("LOCL",4,1,fp);

			// version
			DWORD dwVersion = LOCAL_VERSION;
			fwrite(&dwVersion,sizeof(DWORD),1,fp);

			///��Ϸ�ͻ��˷��������ļ�ID��
			DWORD dwGameClientID = 0;
			fwrite(&dwGameClientID,sizeof(DWORD),1,fp);

			DWORD dwGameServerID = 0;
			fwrite(&dwGameServerID,sizeof(DWORD),1,fp);

			// background music
			string str = CSceneEnvMgr::GetInst()->GetBackGroundMusic();

			DWORD dwFileNameLen = str.length();
			fwrite(&dwFileNameLen,sizeof(DWORD),1,fp);
			fwrite(str.c_str(),dwFileNameLen,1,fp);

			// main player
			CToolMainPlayer::GetInst()->Save(fp);

			// camera
			CMapEditCamera * pCamera = CCameraManager::GetInst()->GetMapEditCamera();
			CVector3f vView = pCamera->GetTarget();
			fwrite(&vView,sizeof(CVector3f),1,fp);

			//fov
			float fov = pCamera->GetFovY();
			fwrite(&fov,sizeof(float),1,fp);

			//yaw
			float yaw = pCamera->GetYaw();
			fwrite(&yaw,sizeof(float),1,fp);

			//pitch
			float pitch = pCamera->GetPitch();
			fwrite(&pitch,sizeof(float),1,fp);

			CMapCommonDataSaver::GetInst()->SaveEnvInfo(fp);

			///����ͼ�ƶ������Ϣ
			CMapCommonDataSaver::GetInst()->SaveBackPictureInfo(fp);

			// fog mode
			int fogMode = CSceneEnvMgr::GetInst()->GetFogMode();
			fwrite(&fogMode,sizeof(int),1,fp);

			//ȫ������
			CSceneEnvMgr::GetInst()->SaveGaussParamsInfo(fp);

			// wave info
			CMapCommonDataSaver::GetInst()->SaveWaveInfo(fp);

			// ambient fx info
			CMapEffect::GetInst()->SaveAmbientFXInfo(fp);

			// width depth
			fwrite(&m_dwSplitMapWidth,sizeof(DWORD),1,fp);
			fwrite(&m_dwSplitMapDepth,sizeof(DWORD),1,fp);

			//////////////////////////////////////////////////////////////////////////
			///write main road texture name and weight
			CMapCommonDataSaver::GetInst()->SaveMainRoadWeightInfo(fp);
			//////////////////////////////////////////////////////////////////////////

			// rgn mask
			int   newGridCnt = 0, SceneLightCount = 0;
			DWORD newVertexCnt = 0;

			DWORD dwMapWidth = m_dwSplitMapWidth * REGION_SPACE;
			DWORD dwMapDepth = m_dwSplitMapDepth * REGION_SPACE;
			newGridCnt		 = dwMapWidth * dwMapDepth;
			newVertexCnt     = (dwMapWidth + 1) * (dwMapDepth + 1);

			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			CTerrainMesh *pTerrainNew = NULL;
			pTerrainNew = new CTerrainMesh; 
			pTerrainNew->Create(m_dwSplitMapWidth, m_dwSplitMapDepth, m_strDefaultTerrainTexName, false, false);
			if( pTerrainNew == NULL )
				return;

			DWORD gridSplitedW		= pTerrainNew->GetWidth();
			DWORD gridSplitedH		= pTerrainNew->GetDepth();
			DWORD gridReadW			= terrain->GetWidth();

			int   nSplitGridStartX   = m_dwSplitStartX * REGION_SPACE;
			int   nSplitGridStartZ   = m_dwSplitStartZ * REGION_SPACE;

			int nNewVertexIndex = 0, nOldVertexIndex = 0;
			DWORD dwNewGridIndex = 0, dwOldGridIndex = 0;
			for ( DWORD z = 0 ; z < gridSplitedH; z++ )
			{
				for ( DWORD x = 0 ; x < gridSplitedW ; x++ )
				{
					dwNewGridIndex = x + z * gridSplitedW;
					dwOldGridIndex = x + nSplitGridStartX + (nSplitGridStartZ + z) * gridReadW;
					// 				int col = x + nSaveGridStartX;
					// 				int row = (nSaveGridStartZ + z) * gridSaveW;

					if( pTerrainNew->IsValid(dwNewGridIndex) )
					{
						SGrid * pGridNew = &pTerrainNew->GetGrid(dwNewGridIndex);
						SGrid * pGridOld = &terrain->GetGrid(dwOldGridIndex);

						this->mapSplitMapGridIndex[dwNewGridIndex]		     = dwOldGridIndex;
						this->mapRevertSplitMapGridIndex[dwOldGridIndex]     = dwNewGridIndex;
						pOperator->m_SplitOldMapGridIndexMap[dwOldGridIndex] = dwNewGridIndex;
	
						//DebugOut("Read reg index %d, save %d\n", nReadDivideRegionIndex, nSaveDivideRegionIndex);

						for( int m = 0; m < 4; ++m )
						{
							nOldVertexIndex = pGridOld->dwVertexIndex[m];
							nNewVertexIndex = pGridNew->dwVertexIndex[m];

							this->mapSplitMapVertexIndex[nNewVertexIndex] = nOldVertexIndex;

							//DebugOut("old vertex %d, new %d\n", nReadVertexIndex, nSaveVertexIndex);
						}
					}
					else
						MessageBox(NULL,"Խ�磬��ע��","��ʾ",MB_OK);
				}
			}
			//////////////////////////////////////////////////////////////////////////

#pragma region VERTEX
			///������Ϣ
			map<int, int>::iterator vertexiter;
			for ( DWORD n = 0; n < newVertexCnt; ++n )
			{
				DWORD dwVertexIndexOld = 0, oColor = VERTEX_COLOR, sColor = VERTEX_COLOR;
				float fPositionY = 0.0f;
				CVector3f vNormal(0.0, 1.0f, 0.0f);
				short sLogicHeight = 0;

				vertexiter = this->mapSplitMapVertexIndex.find(n);
				if ( vertexiter != this->mapSplitMapVertexIndex.end() )
				{
					SVertex &VertexOld = terrain->GetVertex(vertexiter->second);

					oColor		 = VertexOld.oColor;
					sColor		 = VertexOld.sColor;
					fPositionY   = VertexOld.vPosition.y;
					vNormal		 = VertexOld.vNormal;
					sLogicHeight = short(VertexOld.fLogicHeight);
				}
				else
				{
					char c[10];
					sprintf(c, "%d", n);
					string str = c;
					str = str + "�����и�ʧ��";

					MessageBox(NULL,str.c_str(),"��ʾ",MB_OK);
				}

				CMapCommonDataSaver::GetInst()->SaveEachVertexInfo(fp, fPositionY, vNormal, oColor, sColor, sLogicHeight);
			}
#pragma endregion VERTEX

			//////////////////////////////////////////////////////////////////////////
			///model property version
			CMapCommonDataSaver::GetInst()->SaveGridModeProVersion(fp);
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			///��¼������������
			int nGridCnt = terrain->GetGridCount();
			map<string, int16> terrainTextureNames;///���ڼ�¼��ͼ���õ���������������֣��������������Ϣ�򱣴�index����
			CMapCommonDataSaver::GetInst()->SaveTerrainGridTexIndex(fp, nGridCnt, terrainTextureNames);
			//////////////////////////////////////////////////////////////////////////

#pragma region GRID
			// grids
			string strName = "";
			SGrid * pGridOld = NULL;
			int16  nFirstTextureIndex = 0, nSecondTextureIndex = 0;
			DWORD dwGridIndexOld = 0;
			float delta = 0.001f;
			float U[2][4] =
			{
				{delta, 0.5f, 0.5f, delta},
				{0.5f, 1.0f - delta, 1.0f - delta, 0.5f}
			};
			float V[2][4] =
			{
				{1.0f - delta, 1.0f - delta, 0.5f, 0.5f},
				{0.5f, 0.5f, delta, delta}
			};

			map<int, int>::iterator griditer;
			for (int i = 0; i < newGridCnt; ++i )
			{
				int x = i % dwMapWidth;
				int z = i / dwMapWidth;
				int nX = x % 2;
				int nZ = z % 2;

				SGrid * pGridNew = &pTerrainNew->GetGrid(i);
				pGridNew->dwGridIndex = i;

				griditer = this->mapSplitMapGridIndex.find(i);
				if ( griditer != this->mapSplitMapGridIndex.end() )
				{
					dwGridIndexOld = griditer->second;
					pGridOld = &terrain->GetGrid(dwGridIndexOld);

					pGridNew->bGridHide = pGridOld->bGridHide;
					pGridNew->nBlockType = pGridOld->nBlockType;

					pGridNew->nodeInfo.strTextureNames[0] = pGridOld->nodeInfo.strTextureNames[0];
					pGridNew->nodeInfo.strTextureNames[1] = pGridOld->nodeInfo.strTextureNames[1];

					pGridNew->bCliffLayerOne = pGridOld->bCliffLayerOne;

					for (int m = 0; m < 4; ++m)
					{
						pGridNew->uv[0][m] = pGridOld->uv[0][m];
						pGridNew->uv[1][m] = pGridOld->uv[1][m];
					}

					pGridNew->vCenter.y				= pGridOld->vCenter.y;
					pGridNew->vecCenterObjects		= pGridOld->vecCenterObjects;
					pGridNew->bEffectByVertexColor	= pGridOld->bEffectByVertexColor;
				}
				else
				{
					char c[10];
					sprintf(c, "%d", i);
					string str = c;
					str = str + "�����и�ʧ��";

					MessageBox(NULL,str.c_str(),"��ʾ",MB_OK);

					pGridNew->nodeInfo.strTextureNames[0] = m_strDefaultTerrainTexName;
					pGridNew->nodeInfo.strTextureNames[1] = "";

					for (int m = 0; m < 4; ++m)
					{
						pGridNew->uv[0][m].x = U[nX][m];
						pGridNew->uv[0][m].y = V[nZ][m];
					}
				}

				//grid hide mask
				fwrite(&pGridNew->bGridHide,sizeof(bool),1,fp);

				//1
				strName = pGridNew->nodeInfo.strTextureNames[0];
				nFirstTextureIndex = terrainTextureNames[strName];
				fwrite(&nFirstTextureIndex,sizeof(nFirstTextureIndex),1,fp);

				//2
				strName = pGridNew->nodeInfo.strTextureNames[1];
				nSecondTextureIndex = terrainTextureNames[strName];
				fwrite(&nSecondTextureIndex,sizeof(nSecondTextureIndex),1,fp);

				/////////////////////////////////////////////////////////////////
				CMapCommonDataSaver::GetInst()->SaveEachGridInfo(fp, pGridNew, SceneLightCount, nSpotLightCount, false, true);
			}
#pragma endregion GRID
			//////////////////////////////////////////////////////////////////////////

			// block lines
			this->SaveSplitMapBlockInfo(newGridCnt);
			
			///pTerrain
			size_t erasebloclsize = 0;
			fwrite( &erasebloclsize, sizeof(size_t), 1, fp );

			//////////////////////////////////////////////////////////////////////////
			// water

			CTerrainMesh * pWater = terrain->GetWater();
			set<int> setGridIndicesVisibleWater = pWater->GetGridIndicesVisibleWater();
			map< CRiver*, vector<int> > mapRivers;
			map<int, int>::iterator splitRevertGriditer;

			for ( set<int>::iterator iter = setGridIndicesVisibleWater.begin(); iter != setGridIndicesVisibleWater.end(); )
			{
				int nGridIndex = *iter;
				SGrid & gridW = pWater->GetGrid(nGridIndex);

				++iter;

				Ast(gridW.bVisible);
				if ( gridW.pRiver )
				{
					setGridIndicesVisibleWater.erase(nGridIndex);

					splitRevertGriditer = this->mapRevertSplitMapGridIndex.find(nGridIndex);
					if( splitRevertGriditer != this->mapRevertSplitMapGridIndex.end() )
						nGridIndex = splitRevertGriditer->second;
					else
					{
						//DebugOut("Grid %d\n", nGridIndex);
						continue;
					}

					mapRivers[gridW.pRiver].push_back(nGridIndex);
				}
			}

			DWORD dwCnt = mapRivers.size();
			fwrite(&dwCnt,sizeof(DWORD),1,fp);

			// visible and occupied
			for ( map< CRiver*, vector<int> >::iterator iterMapRivers = mapRivers.begin(); iterMapRivers != mapRivers.end(); ++iterMapRivers )
			{
				CRiver * pRiver = iterMapRivers->first;

				string strItemName = pRiver->GetItemName();

				DWORD dw = strItemName.size();
				fwrite(&dw,sizeof(int),1,fp);
				fwrite(strItemName.c_str(),dw,1,fp);

				dwCnt = iterMapRivers->second.size();
				fwrite(&dwCnt,sizeof(DWORD),1,fp);

				for (size_t i = 0; i < iterMapRivers->second.size(); ++i  )
				{
					int nGridIndex = iterMapRivers->second[i];
					fwrite(&nGridIndex,sizeof(DWORD),1,fp);///�и���������

					///�õ����и��ͼ��Ӧ���������ø��ӵõ���ȡ��ˮ�ĸ߶�ֵ
					splitRevertGriditer = this->mapSplitMapGridIndex.find(nGridIndex);
					if( splitRevertGriditer != this->mapSplitMapGridIndex.end() )
						nGridIndex = splitRevertGriditer->second;
					else
					{
						nGridIndex = 0;
						MessageBox(NULL,"��ͼ�и�ˮ�ĵط������⣬��ע��","��ʾ",MB_OK);
					}

					SGrid & gridW = pWater->GetGrid(nGridIndex);
					fwrite(&gridW.vCenter.y,sizeof(float),1,fp);

					for ( int j = 0; j < 4; ++j )
					{
						int nVertexIndex = gridW.dwVertexIndex[j];

						SVertex & vertex = pWater->GetVertex(nVertexIndex);
						fwrite(&vertex.vPosition.y, sizeof(float),1,fp);
						fwrite(&vertex.oColor,sizeof(DWORD),1,fp);
					}
				}
			}

			set<int>::iterator end = setGridIndicesVisibleWater.end();
			int nOldGridIndex = 0, nNewGridIndex = 0;
			for ( set<int>::iterator beg = setGridIndicesVisibleWater.begin(); beg != end; ++beg )
			{
				nOldGridIndex = *beg;
				splitRevertGriditer = this->mapRevertSplitMapGridIndex.find(nOldGridIndex);
				if( splitRevertGriditer != this->mapRevertSplitMapGridIndex.end() )
				{
					nNewGridIndex = splitRevertGriditer->second;
					if( nNewGridIndex < newGridCnt )
						setGridIndicesVisibleWater.erase(nOldGridIndex);
				}
			}

			// unoccupied but visible
			dwCnt = setGridIndicesVisibleWater.size();
			fwrite(&dwCnt,sizeof(DWORD),1,fp);

			for ( set<int>::iterator iter = setGridIndicesVisibleWater.begin(); iter != setGridIndicesVisibleWater.end(); ++iter )
			{
				int nGridIndex = *iter;
				fwrite(&nGridIndex,sizeof(DWORD),1,fp);///�и���������

				///�õ����и��ͼ��Ӧ���������ø��ӵõ���ȡ��ˮ�ĸ߶�ֵ
				splitRevertGriditer = this->mapSplitMapGridIndex.find(nGridIndex);
				if( splitRevertGriditer != this->mapSplitMapGridIndex.end() )
					nGridIndex = splitRevertGriditer->second;
				else
				{
					nGridIndex = 0;
					MessageBox(NULL,"��ͼ�и�ˮ�ĵط������⣬��ע��","��ʾ",MB_OK);
				}

				SGrid & gridW = pWater->GetGrid(nGridIndex);
				fwrite(&gridW.vCenter.y,sizeof(float),1,fp);

				for ( int j = 0; j < 4; ++j )
				{
					int nVertexIndex = gridW.dwVertexIndex[j];

					SVertex & vertex = pWater->GetVertex(nVertexIndex);
					fwrite(&vertex.vPosition.y,sizeof(float),1,fp);
					fwrite(&vertex.oColor,sizeof(DWORD),1,fp);
				}
			}

			/////////////////// //--- ���� SceneLight ���ݿ�
			///save point light
			bool bSceneLightCntEqual = true;
			this->SavePointLightInfo(fp, SceneLightCount);


			///////////////////////--- ����cave���ݿ飬���������Ƿ��ǡ����ڡ�
			int tLength = 6;
			fwrite( &tLength, sizeof(int), 1, fp);
			fwrite( "bCave", 6, 1, fp);
			// cave�� no shadow
			bool bCave = false;
			fwrite(&bCave,sizeof(bool),1,fp);

			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			DWORD dwNewVersion = SPECULAR_LIGHT_PARA;
			fwrite(&dwNewVersion,sizeof(DWORD),1,fp);

			//////////////////////////////////////////////////////////////////////////
			//save specular light parameter
			for (size_t i = 0; i < 4; ++i)
			{
				// light
				DWORD dwSceneSpecurlarColor = CSceneEnvMgr::GetInst()->GetSceneSpecurlarColor();
				fwrite(&dwSceneSpecurlarColor,sizeof(DWORD),1,fp);

				DWORD dwPlayerSpecurlarColor = CSceneEnvMgr::GetInst()->GetPlayerSpecurlarColor();
				fwrite(&dwSceneSpecurlarColor, sizeof(DWORD), 1, fp);
			}

			float fSceneSPower = CSceneEnvMgr::GetInst()->GetSpecularReflectionPower(0);
			fwrite(&fSceneSPower, sizeof(fSceneSPower), 1, fp);

			float fNpcSPower = CSceneEnvMgr::GetInst()->GetSpecularReflectionPower(1);
			fwrite(&fNpcSPower, sizeof(fNpcSPower), 1, fp);
			//////////////////////////////////////////////////////////////////////////

			/// ������Ӳ�������ֵ
			this->SaveSplitMapGridMatIndexInfo(fp, newGridCnt);

			//���治���Ŷ�������Ϣ
			this->SaveOffAnimateModelIndex(fp);

			//���澫ȷ�赲��Ϣ
			this->SaveSplitMapExactBlockInfo(fp, newGridCnt);

			// ��������˻����
			this->SaveWaveSideInfo(fp, newGridCnt);

			///����۹�Ƶ���Ϣ
			this->SaveSpotLightInfo(fp, nSpotLightCount);

			//////////////////////////////////////////////////////////////////////////

			// ----------------------------------------------------------------------------
			// ���������·����Ϣ
			CToolRecord::GetInst()->Save(fp);

			fclose(fp);

			this->SaveAsAudio();

			MessageBox(NULL,"��ͼ�ѷָ���ɣ����Ҫ���ָ��ĵ�ͼ�������´򿪸õ�ͼ","��ʾ",MB_OK);
			MessageBox(NULL,"�򿪷ָ��ͼ������������lightmap��������������ߵ��ᱬ��","��ʾ",MB_OK);

			delete pTerrainNew;
			pTerrainNew = NULL;
		}
		else
		{
			MessageBox(NULL,"д���ļ�����,�п����ļ�����ס��","����",MB_OK);
		}
	}

	//------------------------------------------------------------------------------
	void
	CLocalSplitMapSaver::SaveSpotLightInfo(FILE * fp, const int nSpotLightCnt)
	{
		int nSceneSpotLightCnt = 0;
		int nGridCnt = terrain->GetGridCount();
		map<int, int>::iterator iter;

		fwrite( &nSpotLightCnt, sizeof(int), 1, fp);

		map<int, int>::iterator revertgridIter = this->mapRevertSplitMapGridIndex.begin();
		map<int, int>::iterator revertgridend  = this->mapRevertSplitMapGridIndex.end();
		int nGridIndex = 0;
		for (revertgridIter; revertgridIter != revertgridend; ++revertgridIter )
		{
			nGridIndex = revertgridIter->first;
			SGrid * pGrid = &terrain->GetGrid(nGridIndex);
			if ( !pGrid->vecCenterObjects.empty())
			{
				size_t objectCnt = pGrid->vecCenterObjects.size();
				for ( size_t j = 0; j < objectCnt; ++j)
				{
					CTObjectPtr p = pGrid->vecCenterObjects[j];

					string strEffectName;

					// effect bound?
					if ( p->GetModelGroup()->GetIsEffectBased() && p->GetModelGroup()->GetEffectName(strEffectName) )
					{
						//---
						if (p->GetModelGroup()->m_SpotLight.m_bIAmbSceneSpotLight)
						{
							if ( nSceneSpotLightCnt < nSpotLightCnt )
							{
								int nLightGridIndex = 0;

								///�任�µĵ��θ�������
								iter = this->mapRevertSplitMapGridIndex.find(nGridIndex);
								if( iter != this->mapRevertSplitMapGridIndex.end() )
									nLightGridIndex = iter->second;
								else
								{
									MessageBox(NULL,"��ͼ�и���Դ����ע��","��ʾ",MB_OK);
									nLightGridIndex = 0;
								}

								fwrite( &nLightGridIndex,			sizeof(nLightGridIndex), 1, fp);   ///��������
								fwrite( &j,							sizeof(size_t),			 1, fp);///�ڼ���ģ�͵�����
								fwrite( &p->GetModelGroup()->m_SpotLight, sizeof(SceneSpotLight),  1, fp);

								++nSceneSpotLightCnt;
							}
						}
					}
				}
			}
		}
	}

	//------------------------------------------------------------------------------
	void
	CLocalSplitMapSaver::SaveWaveSideInfo(FILE *pRead, FILE *pSave, const int nSaveMapGridCnt, const int nReadMapGridCnt)
	{
		uint fourcc;
		fread(&fourcc, sizeof(uint), 1, pRead);
		Ast(fourcc == 'WVSD');
	
		vector<byte> waveGrids;
		waveGrids.resize(nReadMapGridCnt);
		fread(&waveGrids.front(), sizeof(byte), waveGrids.size(), pRead);

		map<int, bool> mapGridWaves;
		int nGridIndex = 0;
		for (int i = 0; i < nReadMapGridCnt; ++i)
		{
			nGridIndex = i;
			nGridIndex = this->mapReadMapGridIndex[i];
			mapGridWaves[nGridIndex] = waveGrids[i] == 1;
		}

		//////////////////////////////////////////////////////////////////////////
		// ��������˻����
		{
			CTerrainMesh* water = terrain->GetWater();
			SGrid* grids = water->GetGrids();

			uint fourcc = 'WVSD';
			//fwrite(&fourcc, sizeof(uint), 1, pSave);
			waveGrids.clear();
			waveGrids.resize(nSaveMapGridCnt);
			for (index_t i = 0; i < waveGrids.size(); ++i)
			{
				waveGrids[i] = 0;
			}

			map<int, bool>::iterator iter;
			for ( int i = 0; i < nSaveMapGridCnt; ++i )
			{
				bool bSize = false;
				iter = mapGridWaves.find(i);
				if( iter != mapGridWaves.end() )
					bSize = iter->second;
				else
					bSize = false;
		
				waveGrids[nGridIndex] = bSize ? 1 : 0;
				grids[i].bSide = bSize;

				if (!grids[i].bSide && NULL != grids[i].pWave)
				{
					index_t regionId = water->GetRegionIndexByGridIndex(grids[i].dwGridIndex);
					grids[i].pRiver->RemoveWave(regionId, grids[i].pWave);

					grids[i].pWave = NULL;
				}
			}
			//fwrite(&waveGrids.front(), sizeof(byte), waveGrids.size(), pSave);
		}
	}

	void CLocalSplitMapSaver::SaveWaveSideInfo( FILE* file, DWORD newGridCnt )
	{
		int nGridCount = terrain->GetGridCount();
		SGrid* grids = CMapEditApp::GetInst()->GetWater()->GetGrids();
		uint fourcc = 'WVSD';

		fwrite(&fourcc, sizeof(uint), 1, file);
		vector<byte> waveGrids;
		waveGrids.resize(newGridCnt);

		map<int, int>::iterator iter;
		int nOldGridIndex = 0;

		index_t size = waveGrids.size();
		for (index_t i = 0; i < size; ++i)
		{
			iter = this->mapSplitMapGridIndex.find(i);
			if( iter != this->mapSplitMapGridIndex.end() )
			{
				nOldGridIndex = iter->second;
				waveGrids[i]  = grids[nOldGridIndex].bSide ? 1 : 0;
			}
			else
			{
				char c[10];
				sprintf(c, "%d", i);
				string str = c;
				str = str + "�����и���˻�ʧ��";

				MessageBox(NULL,str.c_str(),"��ʾ",MB_OK);
			}
		}
		fwrite(&waveGrids.front(), sizeof(byte), waveGrids.size(), file);
	}
	//------------------------------------------------------------------------------
	void
	CLocalSplitMapSaver::SaveSplitMapGridMatIndexInfo(FILE * fp, DWORD newGridCnt)
	{
		map<int, int>::iterator beg, end = this->mapSplitMapGridIndex.end();

		int nGridIndex = 0;
		int8 nMatIndex = -1;
		for ( DWORD i = 0; i < newGridCnt; ++i )
		{
			beg = this->mapSplitMapGridIndex.find(i);
			if( beg != end )
			{
				nGridIndex = beg->second;
				SGrid &grid = terrain->GetGrid(nGridIndex);
				nMatIndex = grid.nMaterialIndex;
			}
			else
				nMatIndex = -1;

			fwrite( &nMatIndex, sizeof(nMatIndex), 1, fp);  
		}
	}

	//------------------------------------------------------------------------------
	void
	CLocalSplitMapSaver::SaveSplitMapExactBlockInfo( FILE * fp, DWORD newGridCnt )
	{
		int dwCnt = 0;
		///�����˾�ȷ�赲�ĸ���
		long lExactBlokGridCntPt = ftell(fp);
		fwrite(&dwCnt,sizeof(int),1,fp);

		///��չ��ɵ�ͼ�����������µ�ͼ�е�������ӳ�� first new grid   index second old grid index
		map<int, int>::iterator beg, end = this->mapSplitMapGridIndex.end();

		int nGridIndex = 0;
		for ( DWORD i = 0; i < newGridCnt; ++i )
		{
			beg = this->mapSplitMapGridIndex.find(i);
			if( beg != end )
			{
				nGridIndex = beg->second;
				SGrid & grid = terrain->GetGrid(nGridIndex);
				if ( grid.m_bSetExactBlock )
				{
					fwrite(&i, sizeof(beg->first), 1, fp);
					fwrite(&grid.subGridTypes, sizeof(grid.subGridTypes), 1, fp); // size

					++dwCnt;
				}
			}
		}

		long l = ftell(fp);
		fseek(fp, lExactBlokGridCntPt, SEEK_SET);
		fwrite(&dwCnt, sizeof(dwCnt), 1, fp); // size
		fseek(fp, l, SEEK_SET);
	}

	//------------------------------------------------------------------------------
	void
	CLocalSplitMapSaver::SaveOffAnimateModelIndex(FILE * fp)
	{
		int dwCnt = 0;
		///������Ҫ������Ϣ���ܸ����ĸ���
		long lOffAniModelIndexCntPt = ftell(fp);
		fwrite(&dwCnt,sizeof(int),1,fp);

		int nGridCount = terrain->GetGridCount();
		for ( int i = 0; i< nGridCount; ++i )
		{
			SGrid * pGrid = &terrain->GetGrid(i);
			if ( !pGrid->vecCenterObjects.empty())
			{
				size_t objectCnt = pGrid->vecCenterObjects.size();
				for ( size_t j = 0; j < objectCnt; ++j)
				{
					CTObjectPtr p = pGrid->vecCenterObjects[j];

					string strAgpName = p->GetModelGroup()->GetAnimationGroupName();
					bool bPlay = p->GetModelGroup()->GetIsPlayAnimation();

					if ( strAgpName != "" && !bPlay )
					{
						fwrite( &i, sizeof(int), 1, fp);   ///��������
						fwrite( &j, sizeof(size_t), 1, fp);///�ڼ���ģ�͵�����

						++dwCnt;
					}
				}
			}
		}

		long l = ftell(fp);
		fseek(fp, lOffAniModelIndexCntPt, SEEK_SET);
		fwrite(&dwCnt, sizeof(dwCnt), 1, fp); // size
		fseek(fp, l, SEEK_SET);
	}

	 void CLocalSplitMapSaver::SaveAsAudio()
	 {
		 string strSaveAudioFile = m_strMapPathName + ".audio";

		 //save .audio file 
		 COperator *pOperator = CMapEditApp::GetInst()->GetOperator();

		 locale loc = locale::global(locale(""));
		 FILE * fp = NULL;
		 fp = fopen(strSaveAudioFile.c_str(),"wb");

		 if (!fp)
		 {
			 string log = strSaveAudioFile + "�޷�����";
			 MessageBox(NULL,log.c_str(),"��ʾ",MB_OK);
			 return;
		 }
		 else
		 {
			 DWORD version = AUDIO_PLAYTIME_INTERVAL;
			 fwrite(&version, sizeof(version), 1, fp);

			 //////////////////////////////////////////////////////////////////////////
			 ///save cue name
			 int nNameLen = 0;
			 int nGridCnt   = terrain->GetGridCount(), nSaveGridIndex = 0;
			 string strCueName = "";
			 size_t dwLen = 0;
			 set<int> nGrindIndexs;
			 map<int, int>::iterator splitRevertGriditer, splitGriditer;

			 for( int i = 0; i < nGridCnt; ++i )
			 {
				 SGrid * pGrid = &terrain->GetGrid(i);
				 if ( !pGrid->vecCenterObjects.empty() )
				 {
					 for ( vector< CTObjectPtr >::iterator iter = pGrid->vecCenterObjects.begin(); iter != pGrid->vecCenterObjects.end(); ++iter)
					 {
						 CTObjectPtr p = (*iter);

						 CEditModelGroup *pModelGroup = p->GetModelGroup();

						 if( pModelGroup->m_pSceneAudio )
						 {
							 splitRevertGriditer = this->mapRevertSplitMapGridIndex.find(i);
							 if( splitRevertGriditer != this->mapRevertSplitMapGridIndex.end() )
							 {
								 nSaveGridIndex = splitRevertGriditer->second;
								 nGrindIndexs.insert(i);
							 }
						 }
					 }
				 }
			 }

			 size_t size = nGrindIndexs.size();
			 fwrite(&size,sizeof(size),1,fp);

			 set<int>::iterator end = nGrindIndexs.end();
			 map<int, int>::iterator begsplit, endsplit = this->mapRevertSplitMapGridIndex.end();

			 for( set<int>::iterator beg = nGrindIndexs.begin(); beg != end; ++beg )
			 {
				 int nGridIndex = *beg;
				 int nSaveGridIndex = 0;

				 begsplit = this->mapRevertSplitMapGridIndex.find(nGridIndex);
				 if( begsplit != endsplit )
					 nSaveGridIndex = begsplit->second;
				 else
					 nSaveGridIndex = nGridIndex;

				 fwrite(&nSaveGridIndex,sizeof(nSaveGridIndex),1,fp);	

				 SGrid * pGrid = &terrain->GetGrid(nGridIndex);
				 if ( !pGrid->vecCenterObjects.empty() )
				 {
					 int8 nAudioObjectIndex = 0; 
					 DWORD dw = 0;
					 long lCntOffset = ftell(fp);
					 fwrite(&dw,sizeof(DWORD),1,fp);

					 for ( vector< CTObjectPtr >::iterator iter = pGrid->vecCenterObjects.begin(); iter != pGrid->vecCenterObjects.end(); ++iter)
					 {
						 CTObjectPtr p = (*iter);

						 CEditModelGroup *pModelGroup = p->GetModelGroup();

						 strCueName = pModelGroup->GetSceneCueName();
						 if( pModelGroup->m_pSceneAudio && !strCueName.empty() )
						 {
							 //////////////////////////////////////////////////////////////////////////
							 ///play time mechanism

							 ///һ��ʱ��Ĳ��ų�����С���ֵ
							 DWORD time = pModelGroup->GetPlayTimeLenMin();
							 fwrite(&time,sizeof(time),1,fp);

							 time = pModelGroup->GetPlayTimeLenMax();
							 fwrite(&time,sizeof(time),1,fp);

							 ///���β��ż��������С���ֵ
							 time = pModelGroup->GetPlayIntervalMin();
							 fwrite(&time,sizeof(time),1,fp);

							 time = pModelGroup->GetPlayIntervalMax();
							 fwrite(&time,sizeof(time),1,fp);

							 //////////////////////////////////////////////////////////////////////////

							 ///cue name
							 dwLen = strCueName.size();
							 fwrite(&dwLen,sizeof(int),1,fp);
							 fwrite(strCueName.c_str(),dwLen,1,fp);

							 dw++;

							 string strItemName = pModelGroup->GetItemName();

							 dwLen = strItemName.size();
							 fwrite(&dwLen,sizeof(int),1,fp);
							 fwrite(strItemName.c_str(),dwLen,1,fp);

							 //////////////////////////////////////////////////////////////////////////
							 ///offset
							 CVector3f vUserOffsetEx = pModelGroup->GetUserOffset();
							 fwrite(&vUserOffsetEx,sizeof(CVector3f),1,fp);
							 //////////////////////////////////////////////////////////////////////////

							 int n = p->GetCenterGridIndex();
							 int nSaveModelIndex = 0;

							 begsplit = this->mapRevertSplitMapGridIndex.find(nGridIndex);
							 if( begsplit != endsplit )
								 nSaveModelIndex = begsplit->second;
							 else
								 nSaveModelIndex = n;

							 fwrite(&nSaveModelIndex,sizeof(nSaveModelIndex),1,fp);

							 //y ��
							 float fRotation = pModelGroup->GetRotation();
							 fwrite(&fRotation,sizeof(float),1,fp);

							 int nRotationCnt = pModelGroup->GetRotationCount();
							 fwrite(&nRotationCnt,sizeof(int),1,fp);

							 //////////////////////////////////////////////////////////////////////////
							 //�°汾��������x��z����Ⱦ������������Ϣ
							 //z ��
							 float fPitch = pModelGroup->GetPitch();
							 fwrite(&fPitch,sizeof(float),1,fp);

							 int nPitchCnt = pModelGroup->GetPitchCount();
							 fwrite(&nPitchCnt,sizeof(int),1,fp);

							 //x ��
							 float fYaw = pModelGroup->GetYaw();
							 fwrite(&fYaw,sizeof(float),1,fp);

							 int nYawCnt = pModelGroup->GetYawCount();
							 fwrite(&nYawCnt,sizeof(int),1,fp);

							 //����
							 float xScale = pModelGroup->GetScaleX();
							 fwrite(&xScale,sizeof(float),1,fp);
							 float yScale = pModelGroup->GetScaleY();
							 fwrite(&yScale,sizeof(float),1,fp);
							 float zScale = pModelGroup->GetScaleZ();
							 fwrite(&zScale,sizeof(float),1,fp);
							 //////////////////////////////////////////////////////////////////////////

							 BYTE byRenderStyle = pModelGroup->GetRenderStyle();
							 fwrite(&byRenderStyle,sizeof(BYTE),1,fp);

							 int nAtten = pModelGroup->GetAtten();
							 fwrite(&nAtten,sizeof(int),1,fp);

							 bool bShadow = p->IsCastShadow();
							 fwrite(&bShadow,sizeof(bool),1,fp);

							 ///д�Ƿ������Ӱ��Ϣ
							 bool bReceiveShadow = pModelGroup->IsReceiveShadow();
							 fwrite(&bReceiveShadow,sizeof(bool),1,fp);

							 ///�Ƿ�����,��¼ģ��Ӧ�ò���Ҫ�����İ�
							 bool bTrans = false;
							 fwrite(&bTrans,sizeof(bTrans),1,fp);

							 ///mirror type
							 int nMirrorType = pModelGroup->GetMirrorType();
							 fwrite(&nMirrorType, sizeof(nMirrorType), 1, fp);

							 ///nAffectRadio
							 int nRatio = pModelGroup->GetAffectRatio();
							 fwrite( &nRatio, sizeof(int), 1, fp);
						 }

					 }

					 long lNow = ftell(fp);
					 fseek(fp,lCntOffset,SEEK_SET);
					 fwrite(&dw,sizeof(dw),1,fp);
					 fseek(fp,lNow,SEEK_SET);
				 }
				 else
				 {
					 int16 dw = 0;
					 fwrite(&dw,sizeof(dw),1,fp);
				 }
			 }

			 fclose(fp);
			 locale::global(loc);//�ָ�ȫ��locale	

			 MessageBox(NULL,"������Դ�������","��ʾ",MB_OK);
		}
	 }

	void CLocalSplitMapSaver::SaveSplitMapBlockInfo( const int nSaveMapGridCnt )
	{
		COperator *pOperator = CMapEditApp::GetInst()->GetOperator();
		if( terrain->IsWater() )
		{
			MessageBox(NULL,"����ǰ״̬�ĳɵ���״̬������ˮ���״̬�������ܱ����ͼ�赲��Ϣ","��ʾ",MB_OK);
			return ;
		}

		CTerrainMesh* pTerrain = NULL;
		pTerrain =CMapEditApp::GetInst()->GetTerrain();
		if( pTerrain == NULL )
			MessageBox(NULL,"����Ϊ��","��ʾ",MB_OK);

		string strFileName = m_strMapPathName;
		locale loc;
		use_facet<ctype<TCHAR> >( loc ).tolower
			( &strFileName[0], &strFileName[strFileName.length()] );

		size_t npos = strFileName.rfind(".");
		if( npos != -1 )
			strFileName = strFileName.substr(0, npos);

		strFileName += ".block";

		FILE * fp = NULL;
		fp = fopen(strFileName.c_str(),"wb");

		if (!fp)
		{
			string log = strFileName + "�޷�����";
			MessageBox(NULL,log.c_str(),"��ʾ",MB_OK);
		}

		//////////////////////////////////////////////////////////////////////////
		///version
		DWORD dwVersion = BLOCK_VERSION;
		fwrite(&dwVersion, sizeof(dwVersion), 1, fp);

		int	nBlockType = 0, nGridIndex = 0;
		map<int, int>::iterator iter;
		for ( int i  = 0; i < nSaveMapGridCnt; ++i )
		{
			iter = this->mapSplitMapGridIndex.find(i);
			if ( iter != this->mapSplitMapGridIndex.end() )
			{
				nGridIndex = iter->second;
				SGrid &grid = terrain->GetGrid(i);
				nBlockType = grid.nBlockType;
			}

			fwrite(&nBlockType, sizeof(nBlockType), 1, fp);
		}
		//////////////////////////////////////////////////////////////////////////
		fclose(fp);
	}

	void CLocalSplitMapSaver::SavePointLightInfo( FILE * fp, const int SceneLightCount )
	{
		int     tLength = 11;
		int		nSceneLightCnt = 0;///�����жϱ���ĳ������Դ�����Ƿ��SceneLightCountһ�µļ���
		int     nGridCnt = terrain->GetGridCount();

		fwrite( &tLength, sizeof(int), 1, fp);
		fwrite( "SceneLight", 11, 1, fp);
		fwrite( &SceneLightCount, sizeof(int), 1, fp);
		map<int, int>::iterator griditer;

		map<int, int>::iterator revertgridIter, end = this->mapRevertSplitMapGridIndex.end();
		int nGridIndex = 0;
		for (revertgridIter = this->mapRevertSplitMapGridIndex.begin(); revertgridIter != end; ++revertgridIter )
		{
			nGridIndex = revertgridIter->first;
			SGrid * pGrid = &terrain->GetGrid(nGridIndex);
			if ( !pGrid->vecCenterObjects.empty())
			{
				int size = 0;
				size = (int)(pGrid->vecCenterObjects.size());
				for ( int iter = 0; iter < size; ++iter)
				{
					CTObjectPtr p = pGrid->vecCenterObjects[iter];

					string strEffectName;

					// effect bound?
					if ( p->GetModelGroup()->GetIsEffectBased() && p->GetModelGroup()->GetEffectName(strEffectName) )
					{
						//---
						if (p->GetModelGroup()->m_SL.m_bIAmbScenePointLight)
						{
							if ( nSceneLightCnt < SceneLightCount )
							{
								int nLightGridIndex = 0;
								
								///�任�µĵ��θ�������
								griditer = this->mapRevertSplitMapGridIndex.find(nGridIndex);
								if( griditer != this->mapRevertSplitMapGridIndex.end() )
									nLightGridIndex = griditer->second;
								else
									MessageBox(NULL,"��ͼ�и���Դ����ע��","��ʾ",MB_OK);

								fwrite( &nLightGridIndex, sizeof(nLightGridIndex), 1, fp);
								fwrite( &iter, sizeof(int), 1, fp);
								fwrite( &p->GetModelGroup()->m_SL, sizeof(SceneLight), 1, fp);
								++nSceneLightCnt;
							}
						}
					}
				}
			}
		}
	}

	void CLocalSplitMapSaver::SetTerrainMesh( CTerrainMesh* t )
	{
		Ast(NULL != t);
		this->terrain = t;
		m_strDefaultTerrainTexName = terrain->GetTerrainDefaultTextureName();
	}
}