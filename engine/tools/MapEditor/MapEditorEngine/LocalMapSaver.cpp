//------------------------------------------------------------------------------
//  CLocalMapSaver.cpp
//  (C) 2009 ThreeOGCMan
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "LocalMapSaver.h"
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
#include "StringHelper.h"
#include "CMapEditModelObjPro.h"
#include "IoSystem.h"
#include "LocalVersion.h"
#include "CIniFile.h"
#include "MapCommonDataSaver.h"
#include "Console.h"
#include "CodeCvs.h"
#include "ToolSetTileBlock.h"
#include "ToolRecord.h"
#include "SceneEnvMgr.h"
#include "SceneEnvMgr.inl"
#include "ToolMainPlayer.h"
#include "SceneAudioMgr.h"
#include "LoadMapMgr.h"
#include "EditStateOpertiaonMgr.h"
#include "CMapEffect.h"
#include "TraceHelper.h"

namespace sqr
{

	//------------------------------------------------------------------------------
	CLocalMapSaver::CLocalMapSaver()
		: terrain(NULL)
		, pOperator(NULL)
		, progress(NULL)
	{
	}

	//------------------------------------------------------------------------------
	void
	CLocalMapSaver::SaveAsLocal(const string& path)
	{
		//�ж��Ƿ��ܹ����棬���ܼ�������ʾ
		if ( !CMapEditApp::GetInst()->m_bSave )
		{
			string log = CMapEditApp::GetInst()->savelog;
			log += ", �����޷������ͼ";

			//if ( IDNO == AfxMessageBox(log.c_str(), MB_YESNO) )
			return;
		}

		if( terrain->IsWater() )
		{
			MessageBox(NULL,"����ǰ״̬�ĳɵ���״̬������ˮ���״̬�������ܱ����ͼ��Ϣ","��ʾ",MB_OK);
			return;
		}

		CMapCommonDataSaver::GetInst()->nLoginGridIndexs.clear();

		Ast(NULL != this->terrain);
		//Ast(NULL != this->progress);

		m_strDefaultTerrainTexName = terrain->GetTerrainDefaultTextureName();

		terrain->WipeOffUnUseGridTextureInfo();
		pOperator = CMapEditApp::GetInst()->GetOperator();
		if ( pOperator == NULL )
			return;

		CMapCommonDataSaver::GetInst()->SetOperator(pOperator);
		CMapCommonDataSaver::GetInst()->SetTerrainMesh(terrain);

		// For ExtraData   ���ݾɵر��ļ�   ��ԭ����ĩβ�ӵ�Data��
		int SceneLightCount = 0;	//��¼�������Դ����
		int nSpotLightCount = 0;    //��¼�����۹�Ƹ���

		string strFileName = path;
		locale loc;
		use_facet<ctype<TCHAR> >( loc ).tolower
			( &strFileName[0], &strFileName[strFileName.length()] );

		//////////////////////////////////////////////////////////////////////////
		///��ֹ�Ƶ���Ϣ���������ȴ��˷ݱ����ļ�������Ƶ���Ϣû�����⣬�ͽ������ļ�copy��local
		string backupname = "";
		backupname = strFileName + "backup";

		//////////////////////////////////////////////////////////////////////////
		int nGridCnt   = terrain->GetGridCount();
		int nVertexCnt = terrain->GetVertexCount();

		locale::global(locale(""));
		FILE * fp = NULL;
		fp = fopen(backupname.c_str(),"wb");

		if (!fp)
		{
			string log = strFileName + "�޷�����";
			MessageBox(NULL,log.c_str(),"��ʾ",MB_OK);
			return;
		}

		//////////////////////////////////////////////////////////////////////////
		string autopath = IoSystem::GetInst()->GetFullPath("home:Auto");
		autopath = autopath + "\\";
		string strOpenBackUp = terrain->m_strMapName;
		strOpenBackUp = autopath + "��ǰ��ͼ.local";
		//strOpenBackUp = autopath + strOpenBackUp;

		CopyFile(strFileName.c_str(), strOpenBackUp.c_str(), false);
		//////////////////////////////////////////////////////////////////////////

// 		///������Ч����
// 		CSceneAudioMgr::GetInst()->CalTerrainMaterialIndex();

		size_t npos = strFileName.rfind("\\");
		string mapname = "";
		if ( npos != -1 )
		{
			mapname = strFileName.substr(npos+1, strFileName.length());

			npos = mapname.rfind(".");
			if ( npos != -1 )
				mapname = mapname.substr(0, npos);
		}
		terrain->m_strMapName = mapname;

		float Total = float(nGridCnt*1.5 + nVertexCnt + 1.0f);
		//progress->SetProgress( 0.0f );

		// header
		fwrite("LOCL",4,1,fp);

		// version
		DWORD dwVersion = LOCAL_VERSION;
		fwrite(&dwVersion,sizeof(DWORD),1,fp);

		///��Ϸ�ͻ��˷��������ļ�ID��
		DWORD dwGameClientID = terrain->GetGameGacMapVersion();
		fwrite(&dwGameClientID,sizeof(DWORD),1,fp);

		DWORD dwGameServerID = terrain->GetGameGasMapVersion();
		fwrite(&dwGameServerID,sizeof(DWORD),1,fp);

		LogConsole("Save GameClientID: " << dwGameClientID << " GameServerlientID: " << dwGameServerID <<"\n" );

		// background music
		string str = CSceneEnvMgr::GetInst()->GetBackGroundMusic();

		DWORD dwFileNameLen = str.length();
		fwrite(&dwFileNameLen,sizeof(DWORD),1,fp);
		fwrite(str.c_str(),dwFileNameLen,1,fp);

		// main player
		CToolMainPlayer::GetInst()->Save(fp);

		// camera
		//CMapEditCamera * pCamera = CCameraManager::GetInst()->GetMapEditCamera();
		CVector3f vView/* = pCamera->GetTarget()*/;
		fwrite(&vView,sizeof(CVector3f),1,fp);

		//fov
		float fov/* = pCamera->GetFovY()*/;
		fwrite(&fov,sizeof(float),1,fp);

		//yaw
		float yaw/* = pCamera->GetYaw()*/;
		fwrite(&yaw,sizeof(float),1,fp);

		//pitch
		float pitch/* = pCamera->GetPitch()*/;
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
		//////////////////////////////////////////////////////////////////////////

		// ambient fx info
		CMapEffect::GetInst()->SaveAmbientFXInfo(fp);

		//progress->SetProgress( 1.0f/Total );

		// width depth
		DWORD dwWidth = terrain->GetWidthInRegion();
		DWORD dwDepth = terrain->GetDepthInRegion();
		if ( dwWidth % 4 ) dwWidth+=4-dwWidth % 4;
		if ( dwDepth % 4 ) dwDepth+=4-dwDepth % 4;

		fwrite(&dwWidth,sizeof(DWORD),1,fp);
		fwrite(&dwDepth,sizeof(DWORD),1,fp);
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		///write main road texture name and weight
		CMapCommonDataSaver::GetInst()->SaveMainRoadWeightInfo(fp);
		//////////////////////////////////////////////////////////////////////////

		DWORD gridW = dwWidth * REGION_SPACE;
		DWORD gridH = dwDepth * REGION_SPACE;

		//////////////////////////////////////////////////////////////////////////
		//������Ϣ
		{
			for (int k = 0; k < nVertexCnt; ++k )
			{
				SVertex vertex = terrain->GetVertex(k);
				short s = (short)vertex.fLogicHeight;

				CMapCommonDataSaver::GetInst()->SaveEachVertexInfo(fp, vertex.vPosition.y, vertex.vNormal, vertex.oColor, vertex.sColor, s);

				//progress->SetProgress( (k+1.0f)/Total );
			}
		}

		//////////////////////////////////////////////////////////////////////////
		///model property version
		CMapCommonDataSaver::GetInst()->SaveGridModeProVersion(fp);
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		///��¼������������
		map<string, int16> terrainTextureNames;///���ڼ�¼��ͼ���õ���������������֣��������������Ϣ�򱣴�index����
		CMapCommonDataSaver::GetInst()->SaveTerrainGridTexIndex(fp, nGridCnt, terrainTextureNames);

		int16  nFirstTextureIndex = 0, nSecondTextureIndex = 0;
		string strName = "";

		// grids
		//������Ϣ
		{
			for ( int j = 0; j< nGridCnt; ++j )
			{
				SGrid * pGrid = &terrain->GetGrid(j);
				//hide mask
				fwrite(&pGrid->bGridHide,sizeof(bool),1,fp);
				///////////////////////////////////////////////////////////////
				//������Ϣ

				//1
				strName = pGrid->nodeInfo.strTextureNames[0];
				nFirstTextureIndex = terrainTextureNames[strName];
				fwrite(&nFirstTextureIndex,sizeof(nFirstTextureIndex),1,fp);

				//2
				strName = pGrid->nodeInfo.strTextureNames[1];
				nSecondTextureIndex = terrainTextureNames[strName];
				fwrite(&nSecondTextureIndex,sizeof(nSecondTextureIndex),1,fp);

				CMapCommonDataSaver::GetInst()->SaveEachGridInfo(fp, pGrid, SceneLightCount, nSpotLightCount, false, false);

				//progress->SetProgress( (j+1.0f+ + terrain->GetVertexCount())/Total );
			}
		}
		//////////////////////////////////////////////////////////////////////////

		// info from other places

		// block lines �������浽һ���ļ�
		CToolSetTileBlock::GetInst()->Save(path);

		///pTerrain
		size_t erasebloclsize = 0;
		fwrite( &erasebloclsize, sizeof(size_t), 1, fp );

		// water

		CTerrainMesh * pWater = terrain->GetWater();

		set<int> setGridIndicesVisibleWater = pWater->GetGridIndicesVisibleWater();

		map< CRiver*, vector<int> > mapRivers;

		for ( set<int>::iterator iter = setGridIndicesVisibleWater.begin(); iter != setGridIndicesVisibleWater.end();)
		{
			int nGridIndex = *iter;

			++iter;

			SGrid & gridW = pWater->GetGrid(nGridIndex);

			Ast(gridW.bVisible);

			if ( gridW.pRiver )
			{
				mapRivers[gridW.pRiver].push_back(nGridIndex);
				setGridIndicesVisibleWater.erase(nGridIndex);
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

				SGrid & gridW = pWater->GetGrid(nGridIndex);
				fwrite(&gridW.dwGridIndex,sizeof(DWORD),1,fp);
				fwrite(&gridW.vCenter.y,sizeof(float),1,fp);

				for ( int j = 0; j < 4; ++j )
				{
					int nVertexIndex = gridW.dwVertexIndex[j];

					SVertex & vertex = pWater->GetVertex(nVertexIndex);
					fwrite(&vertex.vPosition.y,sizeof(float),1,fp);
					fwrite(&vertex.oColor,sizeof(DWORD),1,fp);
				}
			}
		}

		// unoccupied but visible
		dwCnt = setGridIndicesVisibleWater.size();
		fwrite(&dwCnt,sizeof(DWORD),1,fp);

		for ( set<int>::iterator iter = setGridIndicesVisibleWater.begin(); iter != setGridIndicesVisibleWater.end(); ++iter )
		{
			int nGridIndex = *iter;

			SGrid & gridW = pWater->GetGrid(nGridIndex);
			fwrite(&gridW.dwGridIndex,sizeof(DWORD),1,fp);
			fwrite(&gridW.vCenter.y,sizeof(float),1,fp);

			for ( int j = 0; j < 4; ++j )
			{
				int nVertexIndex = gridW.dwVertexIndex[j];

				SVertex & vertex = pWater->GetVertex(nVertexIndex);
				fwrite(&vertex.vPosition.y,sizeof(float),1,fp);
				fwrite(&vertex.oColor,sizeof(DWORD),1,fp);
			}
		}

		//progress->SetProgress( (terrain->GetGridCount() * 1.125f +1.0f + terrain->GetVertexCount())/Total );

		//////////////////////////////////////////////////////////////////////////


		/////////////////// //--- ���� SceneLight ���ݿ�
		///save point light
		bool bSceneLightCntEqual = true;
		this->SavePointLightInfo(fp, SceneLightCount, bSceneLightCntEqual);

		///////////////////////--- ����cave���ݿ飬���������Ƿ��ǡ����ڡ�
		int tLength = 6;
		fwrite( &tLength, sizeof(int), 1, fp);
		fwrite( "bCave", 6, 1, fp);
		// cave�� no shadow
		bool bCave = false;
		fwrite(&bCave,sizeof(bool),1,fp);

		//progress->SetProgress( (terrain->GetGridCount() * 1.25f+1.0f+terrain->GetVertexCount())/Total );

		//////////////////////////////////////////////////////////////////////////
		/*�����ǰ�ĸ�ʽ�޸ĵ��µ�һϵ�����⣬�����ڿ�ʼ������Ч�еĹ���屣���ʽ��
		Ҳ�������ð汾�ż���switch�ķ�ʽ�����������ӵĵ�ͼ��Ϣ*/

		//////////////////////////////////////////////////////////////////////////
		DWORD dwNewVersion = VERTEX_BAKING_COLOR;
		fwrite(&dwNewVersion,sizeof(DWORD),1,fp);

		//////////////////////////////////////////////////////////////////////////
		//���涥�����ɫ
		for (int k = 0; k < nVertexCnt; ++k )
		{
			SVertex vertex = terrain->GetVertex(k);
			fwrite(&vertex.tColor,sizeof(DWORD),1,fp);
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		
		//����specular light parameter
		for (size_t i = 0; i < 4; ++i)
		{
			// light
			DWORD dwSceneSpecurlarColor = CSceneEnvMgr::GetInst()->GetSceneSpecurlarColor();
			fwrite(&dwSceneSpecurlarColor,sizeof(DWORD),1,fp);

			DWORD dwPlayerSpecurlarColor = CSceneEnvMgr::GetInst()->GetPlayerSpecurlarColor();
			fwrite(&dwPlayerSpecurlarColor, sizeof(DWORD), 1, fp);
		}

		float fSceneSPower = CSceneEnvMgr::GetInst()->GetSpecularReflectionPower(0);
		fwrite(&fSceneSPower, sizeof(fSceneSPower), 1, fp);

		float fNpcSPower = CSceneEnvMgr::GetInst()->GetSpecularReflectionPower(1);
		fwrite(&fNpcSPower, sizeof(fNpcSPower), 1, fp);
		//////////////////////////////////////////////////////////////////////////

		/// ������Ӳ�������ֵ
		SaveTerGridMatIndexInfo(fp);

		//���治���Ŷ�������Ϣ
		SaveOffAnimateModelIndex(fp);

		//���澫ȷ�赲��Ϣ
		SaveExactBlockInfo(fp);

		// ��������˻����
		SaveWaveSideInfo(fp);

		///����۹�Ƶ���Ϣ
		bool bSceneSpotLightCntEqual = true;

		SaveSpotLightInfo(fp, nSpotLightCount, bSceneSpotLightCntEqual);

		// ----------------------------------------------------------------------------
		// ���������·����Ϣ

		CToolRecord::GetInst()->Save(fp);
		
		fclose(fp);

		if ( bSceneLightCntEqual && bSceneSpotLightCntEqual )
			CopyFile(backupname.c_str(), strFileName.c_str(), false);
		else
			MessageBox(NULL,"����ĵ��Դ����ͳ�������⣬��ص��ر࣬���´򿪵�ͼ���鿴��ͼ�Ƿ�ȱʧ��Ϣ�����������","��ʾ",MB_OK);

		MessageBox(NULL,"�������","��ʾ",MB_OK);

		//save .audio file 
		this->SaveAsAudio(path, false);

		//save .login file
		this->SaveAsLogin(path, CMapCommonDataSaver::GetInst()->nLoginGridIndexs);
		
#ifdef USE_OLD_RESOURCEDIR_NOTCODER
		return;
#else
		if( !CLoadSavedMapMgr::HasInst() )
			CLoadSavedMapMgr::Create();

		//CLoadSavedMapMgr::GetInst()->SetProcessCallback(progress);
		CLoadSavedMapMgr::GetInst()->Save();
#endif

	}

	//------------------------------------------------------------------------------
	void
	CLocalMapSaver::SaveSpotLightInfo(FILE * fp, const int nSpotLightCnt, bool& bSaveLocalFile)
	{
		bool bReSizeMap = pOperator->GetIsResizeMap();
		int nSceneSpotLightCnt = 0;
		int nGridCnt = terrain->GetGridCount();

		fwrite( &nSpotLightCnt, sizeof(int), 1, fp);

		for ( int i = 0; i < nGridCnt; ++i )
		{
			SGrid * pGrid = &terrain->GetGrid(i);
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
								if ( bReSizeMap)
									nLightGridIndex = pOperator->m_OldMapGridIndexMap[i];
								else
									nLightGridIndex = i;

								fwrite( &nLightGridIndex,			sizeof(nLightGridIndex), 1, fp);   ///��������
								fwrite( &j,							sizeof(size_t),			 1, fp);///�ڼ���ģ�͵�����
								fwrite( &p->GetModelGroup()->m_SpotLight, sizeof(SceneSpotLight),  1, fp);
							}
							else
							{
								TCHAR temp[255];
								sprintf( temp, "%d", nSpotLightCnt );

								string log = "";
								log = temp;
								log = log + "���ӵľ۹��Դ��Ϣ����";
								MessageBox(NULL,log.c_str(),"",MB_OK);
								bSaveLocalFile = false;
							}

							++nSceneSpotLightCnt;
						}
					}
				}
			}
		}
	}

	//------------------------------------------------------------------------------
	void
	CLocalMapSaver::SaveExactBlockInfo(FILE * fp)
	{
		int dwCnt = 0;
		///�����˾�ȷ�赲�ĸ���
		long lExactBlokGridCntPt = ftell(fp);
		fwrite(&dwCnt,sizeof(int),1,fp);

		int nGridCount = terrain->GetGridCount();

		for ( int i = 0; i < nGridCount; ++i )
		{
			SGrid & grid = terrain->GetGrid(i);

			if ( grid.m_bSetExactBlock )
			{
				fwrite(&i, sizeof(i), 1, fp);
				fwrite(&grid.subGridTypes, sizeof(grid.subGridTypes), 1, fp); // size

				++dwCnt;
			}
		}

		long l = ftell(fp);
		fseek(fp, lExactBlokGridCntPt, SEEK_SET);
		fwrite(&dwCnt, sizeof(dwCnt), 1, fp); // size
		fseek(fp, l, SEEK_SET);
	}

	//------------------------------------------------------------------------------
	void
	CLocalMapSaver::SaveReSizeExactBlockInfo(FILE * fp, DWORD dwWidth, DWORD dwDepth )
	{
		int dwCnt = 0;
		///�����˾�ȷ�赲�ĸ���
		long lExactBlokGridCntPt = ftell(fp);
		fwrite(&dwCnt,sizeof(int),1,fp);

		DWORD dwMapWidth = dwWidth * REGION_SPACE;
		DWORD dwMapDepth = dwDepth * REGION_SPACE;
		DWORD newGridCnt = dwMapWidth * dwMapDepth;

		///��չ��ɵ�ͼ�����������µ�ͼ�е�������ӳ�� first new grid   index second old grid index
		map<int, int> m_NewMapGridIndexMap;
		map<int, int>::iterator beg, end = pOperator->m_OldMapGridIndexMap.end();
		for( beg = pOperator->m_OldMapGridIndexMap.begin(); beg != end; ++beg )
			m_NewMapGridIndexMap[beg->second] = beg->first;

		int nGridIndex = 0;
		for ( DWORD i = 0; i < newGridCnt; ++i )
		{
			beg = m_NewMapGridIndexMap.find(i);
			if( beg != m_NewMapGridIndexMap.end() )
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
	CLocalMapSaver::SaveWaveSideInfo(FILE * fp)
	{
		int nGridCount = terrain->GetGridCount();
		SGrid* grids = CMapEditApp::GetInst()->GetWater()->GetGrids();
		uint fourcc = 'WVSD';

		fwrite(&fourcc, sizeof(uint), 1, fp);
		vector<byte> waveGrids;
		waveGrids.resize(nGridCount);

		for (index_t i = 0; i < waveGrids.size(); ++i)
		{
			waveGrids[i] = grids[i].bSide ? 1 : 0;
		}
		fwrite(&waveGrids.front(), sizeof(byte), waveGrids.size(), fp);
	}

	//------------------------------------------------------------------------------
	void
	CLocalMapSaver::SaveTerGridMatIndexInfo(FILE* fp)
	{
		int nGridCount = terrain->GetGridCount();
		int8 nMatIndex  = -1;
		for ( int i = 0; i< nGridCount; ++i )
		{
			SGrid * pGrid = &terrain->GetGrid(i);
			nMatIndex = pGrid->nMaterialIndex;
			fwrite( &nMatIndex, sizeof(nMatIndex), 1, fp);  
		}
	}

	//------------------------------------------------------------------------------
	void
	CLocalMapSaver::SaveOffAnimateModelIndex(FILE * fp)
	{
		int dwCnt = 0;
		///������Ҫ������Ϣ���ܸ����ĸ���
		long lExactBlokGridCntPt = ftell(fp);
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
		fseek(fp, lExactBlokGridCntPt, SEEK_SET);
		fwrite(&dwCnt, sizeof(dwCnt), 1, fp); // size
		fseek(fp, l, SEEK_SET);
	}

	 //------------------------------------------------------------------------------
	 bool
	 CLocalMapSaver::ResizeAndSaveAsLocal(string strTitle, DWORD dwWidth, DWORD dwDepth)
	 {
		pOperator = CMapEditApp::GetInst()->GetOperator();
	 	CMapEditApp::GetInst()->SetEditingMesh(sqr::EEM_TERRAIN);
	 
	 	locale loc;
	 	use_facet<ctype<TCHAR> >( loc ).tolower
	 	( &strTitle[0], &strTitle[strTitle.length()] );
	 
	 	FILE * fp = fopen(strTitle.c_str(),"wb");
	 	int SceneLightCount = 0;	//��¼�������Դ����
	 	int nSpotLightCount = 0;    //��¼�����۹�Ƹ���
	 
	 	if ( fp )
	 	{
			CMapCommonDataSaver::GetInst()->SetOperator(pOperator);
			CMapCommonDataSaver::GetInst()->SetTerrainMesh(terrain);

	 		set<DWORD> eraseGrids;
	 
	 		string strTileName = strTitle;
	 		size_t npos = strTileName.rfind(".");
	 		strTileName = strTileName.substr(0, npos);
	 
	 		string strOldLocalName = strTileName + ".local";
	 		string strNewLocalName = strTileName + "_��չǰ����.local";
	 		CopyFile(strOldLocalName.c_str(), strNewLocalName.c_str(), false);
	 
	 		string strOldProjectName = strTileName + ".project";
	 		string strNewProjectName = strTileName + "_��չǰ����.project";
	 		CopyFile(strOldProjectName.c_str(), strNewProjectName.c_str(), false);
	 
	 		string strOldInfoName = strTileName + ".operaterinfo";
	 		string strNewInfoName = strTileName + "_��չǰ����.operaterinfo";
	 		CopyFile(strOldInfoName.c_str(), strNewInfoName.c_str(), false);
	 
	 		pOperator->m_OldMapGridIndexMap.clear();
	 		pOperator->m_OldMapRegionIndexMap.clear();
	 
	 		// header
	 		fwrite("LOCL",4,1,fp);
	 
	 		// version
	 		DWORD dwVersion = LOCAL_VERSION;
	 		fwrite(&dwVersion,sizeof(DWORD),1,fp);
	 
	 		///��Ϸ�ͻ��˷��������ļ�ID��
	 		DWORD dwGameClientID = terrain->GetGameGacMapVersion();
	 		fwrite(&dwGameClientID,sizeof(DWORD),1,fp);
	 
	 		DWORD dwGameServerID = terrain->GetGameGasMapVersion();
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
	 
			//////////////////////////////////////////////////////////////////////////
	 		// ambient fx info
			CMapEffect::GetInst()->SaveAmbientFXInfo(fp);
			//////////////////////////////////////////////////////////////////////////
	 
	 		// width depth
	 		fwrite(&dwWidth,sizeof(DWORD),1,fp);
	 		fwrite(&dwDepth,sizeof(DWORD),1,fp);
	 
			//////////////////////////////////////////////////////////////////////////
			///write main road texture name and weight
			CMapCommonDataSaver::GetInst()->SaveMainRoadWeightInfo(fp);
			//////////////////////////////////////////////////////////////////////////
	 
			m_strDefaultTerrainTexName = terrain->GetTerrainDefaultTextureName();

	 		// rgn mask
	 		int newRegionCnt = 0, newGridCnt = 0, SceneLightCount = 0;
	 		DWORD newVertexCnt = 0;
	 
	 		newRegionCnt = dwWidth * dwDepth;
	 		DWORD dwMapWidth = dwWidth * REGION_SPACE;
	 		DWORD dwMapDepth = dwDepth * REGION_SPACE;
	 		newGridCnt = dwMapWidth * dwMapDepth;
	 		newVertexCnt = (dwMapWidth + 1) * (dwMapDepth + 1);
	 
			CMapCommonDataSaver::GetInst()->dwNewMapWidth = dwMapWidth;
	 		//////////////////////////////////////////////////////////////////////////
	 		///������Ϣ
			uint8 uReSizeMapType = terrain->GetResizeMapType();
	 		for ( DWORD n = 0; n < newVertexCnt; ++n )
	 		{
	 			DWORD dwVertexIndexOld = 0, oColor = VERTEX_COLOR, sColor = VERTEX_SPECULAR_COLOR;
	 			float fPositionY = 0.0f;
	 			CVector3f vNormal(0.0, 1.0f, 0.0f);
	 			short sLogicHeight = 0;

	 			if ( terrain->IsOverlappingByVertexIndex(dwWidth, dwDepth, n, dwVertexIndexOld, uReSizeMapType) )
	 			{
	 				SVertex &VertexOld = terrain->GetVertex(dwVertexIndexOld);
	 
	 				oColor		 = VertexOld.oColor;
	 				sColor		 = VertexOld.sColor;
	 				fPositionY   = VertexOld.vPosition.y;
	 				vNormal		 = VertexOld.vNormal;
	 				sLogicHeight = short(VertexOld.fLogicHeight);
	 			}
	 
				CMapCommonDataSaver::GetInst()->SaveEachVertexInfo(fp, fPositionY, vNormal, oColor, sColor, sLogicHeight);
	 		}

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
	 
	 		// grids
	 		string strName = "";
	 		SGrid * pGridOld = NULL;
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

			map<DWORD, DWORD> mapNewTerrainDivideRegIndexs;
			DWORD dwGridCntInRegion = (REGION_SPACE)*(REGION_SPACE);
			DWORD dwRegionCnt = dwWidth * dwDepth;
			DWORD dwWidthInDivideReg = dwWidth * REGION_STEP_DIVIDE;
			DWORD dwDepthInDivideReg = dwDepth * REGION_STEP_DIVIDE;
			int nDiviceReginWidthGrids = REGION_SPACE / REGION_STEP_DIVIDE;
			int nDivideRegIndex = 0;
			int col = 0, row = 0, nGridIndex = 0, nDivideIndex = 0, nRgnGridIndex;
			int nDivideRgnGridCnt = nDiviceReginWidthGrids * nDiviceReginWidthGrids;
			int  index = 0;
			
			for (size_t i = 0; i < dwRegionCnt; i++ )
			{
				int nX = i % dwWidth;
				int nY = i / dwWidth;

				vector<int> vGridIndices;
				vGridIndices.resize(dwGridCntInRegion);

				int nGridIndex = nX*REGION_SPACE + nY*REGION_SPACE*dwMapWidth;
				for ( size_t j = 0; j < dwGridCntInRegion; j++ )
				{
					if ( j != 0 && j % (REGION_SPACE) == 0 )
					{
						nGridIndex += -1 + dwMapWidth - REGION_SPACE +1;
					}

					vGridIndices[j] = nGridIndex;

					nGridIndex++;
				}

				nDivideRegIndex = (i/dwWidth) * dwWidth * REGION_DIVIDE + (i%dwWidth) * REGION_STEP_DIVIDE;
				nGridIndex = 0;
				for ( int m = 0; m < REGION_STEP_DIVIDE; ++m )
				{
					for ( int n = 0;n < REGION_STEP_DIVIDE; ++n )
					{
						index = n + m * REGION_STEP_DIVIDE;
						nDivideIndex = nDivideRegIndex + n + m * dwWidthInDivideReg;

						for ( int k = 0; k < nDiviceReginWidthGrids; ++k )
						{
							for ( int j = 0; j < nDiviceReginWidthGrids; ++j )
							{
								nRgnGridIndex = m * REGION_SPACE * (nDiviceReginWidthGrids) + n * nDiviceReginWidthGrids + j + k * REGION_SPACE;
								nGridIndex = vGridIndices[nRgnGridIndex];
								
								mapNewTerrainDivideRegIndexs[nGridIndex] = nDivideIndex;
							}
						}
					}
				}
			}

			int16  nFirstTextureIndex = 0, nSecondTextureIndex = 0;
			bool   bOldMapStart = true;
			int	   nOldSatrtGridInNewIndex = 0;

			for ( DWORD z = 0 ; z < dwMapDepth; z++ )
			{
				for ( DWORD x = 0 ; x < dwMapWidth ; x++ )
				{
					DWORD dwNewGridIndex = x + z * dwMapWidth;

					SGrid *pGridNew = new SGrid;
					pGridNew->dwGridIndex = dwNewGridIndex;

					int nX = x % 2;
					int nZ = z % 2;

					for (int i = 0; i < 4; i++)
					{
						pGridNew->uv[0][i].x = U[nX][i];
						pGridNew->uv[0][i].y = V[nZ][i];
						pGridNew->uv[1][i].x = U[nX][i];
						pGridNew->uv[1][i].y = V[nZ][i];
					}

					DWORD dwGridIndexOld = 0;
					int   nOldDivideRegionIndex = 0, nNewDivideRegionIndex = 0;
					if ( terrain->IsOverlappingByGridIndex(dwWidth, dwDepth, dwNewGridIndex, dwGridIndexOld, uReSizeMapType) )
					{
						if( bOldMapStart )
						{
							nOldSatrtGridInNewIndex = i;
							bOldMapStart = false;
						}

						pGridOld = &terrain->GetGrid(dwGridIndexOld);
						nOldDivideRegionIndex = pGridOld->m_nDivideRegionIndex;

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

						pOperator->m_OldMapGridIndexMap[dwGridIndexOld] = dwNewGridIndex;
						pOperator->m_OldMapRegionIndexMap[nOldDivideRegionIndex] = mapNewTerrainDivideRegIndexs[dwNewGridIndex];
					}
					else
					{
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
					CMapCommonDataSaver::GetInst()->SaveEachGridInfo(fp, pGridNew, SceneLightCount, nSpotLightCount, true, false);

					delete pGridNew;
				}
			}
	 
	 		//////////////////////////////////////////////////////////////////////////
	 
	 		// block lines
	 		CToolSetTileBlock::GetInst()->ReSizeSave(strTitle, dwWidth, dwDepth);
	 
	 		///pTerrain
	 		size_t erasebloclsize = eraseGrids.size();
	 		fwrite( &erasebloclsize, sizeof(size_t), 1, fp );
	 		
			//////////////////////////////////////////////////////////////////////////
	 		// water
	 
	 		CTerrainMesh * pWater = terrain->GetWater();
	 
	 		set<int> setGridIndicesVisibleWater = pWater->GetGridIndicesVisibleWater();
	 		map< CRiver*, vector<int> > mapRivers;
	 
	 		for ( set<int>::iterator iter = setGridIndicesVisibleWater.begin(); iter != setGridIndicesVisibleWater.end();)
	 		{
	 			int nGridIndex = *iter;
	 
	 			++iter;
	 
	 			SGrid & gridW = pWater->GetGrid(nGridIndex);
	 
	 			Ast(gridW.bVisible);
	 
	 			if ( gridW.pRiver )
	 			{
	 				mapRivers[gridW.pRiver].push_back(nGridIndex);
	 				setGridIndicesVisibleWater.erase(nGridIndex);
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
	 
	 				SGrid & gridW = pWater->GetGrid(nGridIndex);
	 				///�任�µĵ��θ�������
	 				nGridIndex = pOperator->m_OldMapGridIndexMap[nGridIndex];
	 
	 				fwrite(&nGridIndex,sizeof(DWORD),1,fp);
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
	 
	 		// unoccupied but visible
	 		dwCnt = setGridIndicesVisibleWater.size();
	 		fwrite(&dwCnt,sizeof(DWORD),1,fp);
	 
	 		for ( set<int>::iterator iter = setGridIndicesVisibleWater.begin(); iter != setGridIndicesVisibleWater.end(); ++iter )
	 		{
	 			int nGridIndex = *iter;
	 
	 			SGrid & gridW = pWater->GetGrid(nGridIndex);
	 			///�任�µĵ��θ�������
	 			nGridIndex = pOperator->m_OldMapGridIndexMap[nGridIndex];
	 
	 			fwrite(&nGridIndex,sizeof(DWORD),1,fp);
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
			this->SavePointLightInfo(fp, SceneLightCount, bSceneLightCntEqual);
	 
	 
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
			SaveReSizeTerGridMatIndexInfo(fp, dwWidth, dwDepth);
	 
	 		//���治���Ŷ�������Ϣ
	 		SaveOffAnimateModelIndex(fp);
	 
	 		//���澫ȷ�赲��Ϣ
	 		SaveReSizeExactBlockInfo(fp, dwWidth, dwDepth);
	 
	 		// ��������˻����
	 		{
	 			SGrid* grids = pWater->GetGrids();
	 			uint fourcc = 'WVSD';
	 			fwrite(&fourcc, sizeof(uint), 1, fp);
	 			vector<byte> waveGrids;
	 			waveGrids.resize(newGridCnt);
	 			for (index_t i = 0; i < waveGrids.size(); ++i)
	 			{
	 				waveGrids[i] = 0;
	 			}

				int nWaterGridCnt = pWater->GetGridCount();
	 			for ( int i = 0; i < nWaterGridCnt; ++i )
	 			{
	 				int nGridIndex = pOperator->m_OldMapGridIndexMap[i];
	 				waveGrids[nGridIndex] = grids[i].bSide ? 1 : 0;
	 			}
	 			fwrite(&waveGrids.front(), sizeof(byte), waveGrids.size(), fp);
	 		}
	 
	 		///����۹�Ƶ���Ϣ
	 		bool bSceneSpotLightCntEqual = true;
	 
	 		SaveSpotLightInfo(fp, nSpotLightCount, bSceneSpotLightCntEqual);
	 
	 		//////////////////////////////////////////////////////////////////////////
	 
			// ----------------------------------------------------------------------------
			// ���������·����Ϣ

			CToolRecord::GetInst()->Save(fp);

	 		fclose(fp);
	 
			this->SaveAsAudio(strTitle, true);

	 		MessageBox(NULL,"��ͼ����չ��ɣ����Ҫ����չ��ĵ�ͼ�������´򿪸õ�ͼ","��ʾ",MB_OK);
			MessageBox(NULL,"������չ�ĵ�ͼ������������lightmap��������������ߵ��ᱬ��","��ʾ",MB_OK);

	 		pOperator->SetIsResizeMap(true);

			//////////////////////////////////////////////////////////////////////////
			//����ɵ�ͼ������������µ�ͼ�е�����ֵ
			this->SaveOldMapStartCoordinateInNewMap(dwWidth, dwDepth, nOldSatrtGridInNewIndex);
			//////////////////////////////////////////////////////////////////////////
	 	}
	 	else
	 	{
	 		MessageBox(NULL,"д���ļ�����,�п����ļ�����ס��","����",MB_OK);
	 		return false;
	 	}

	 	return true;
	 }

	 void CLocalMapSaver::SaveAsAudio( const string& path, bool bResize )
	 {
		 //save .audio file 
		 CLocalMapAudioSaverPtr audiosaver = new CLocalMapAudioSaver();
		 audiosaver->SetTerrainMesh(CMapEditApp::GetInst()->GetTerrain());
		 audiosaver->SaveAsSceneAudio(path, bResize);
	 }

	 void CLocalMapSaver::SaveAsLogin(const string& path, set<int> nGridIndexs)
	 {
		 //save .login file 
		 CLocalMapLoginSaverPtr loginsaver = new CLocalMapLoginSaver();
		 loginsaver->SetTerrainMesh(CMapEditApp::GetInst()->GetTerrain());
		 loginsaver->SaveAsSceneLogin(path, nGridIndexs);
	 }

    void CLocalMapSaver::SaveAsRgn( const string& path )
	{
		Ast(NULL != this->terrain);

		string szFullPath = path + ".rgn";

		// save .rgn file
		DWORD dwWidth = terrain->GetWidthInRegion();
		DWORD dwDepth = terrain->GetDepthInRegion();

		FILE * fp = NULL;
		fp = fopen(szFullPath.c_str(),"wb");
		if (!fp)
		{
			string log = szFullPath + "�޷�����";
			MessageBox(NULL,log.c_str(),"��ʾ",MB_OK);
			return;
		} 	
		fwrite( &dwWidth, sizeof(DWORD), 1, fp );
		fwrite( &dwDepth, sizeof(DWORD), 1, fp );
		fclose(fp);
	}

	void CLocalMapSaver::SaveReSizeTerGridMatIndexInfo( FILE * fp, DWORD dwWidth, DWORD dwDepth )
	{
		DWORD dwMapWidth = dwWidth * REGION_SPACE;
		DWORD dwMapDepth = dwDepth * REGION_SPACE;
		DWORD newGridCnt = dwMapWidth * dwMapDepth;

		///��չ��ɵ�ͼ�����������µ�ͼ�е�������ӳ�� first new grid   index second old grid index
		map<int, int> m_NewMapGridIndexMap;
		map<int, int>::iterator beg, end = pOperator->m_OldMapGridIndexMap.end();
		for( beg = pOperator->m_OldMapGridIndexMap.begin(); beg != end; ++beg )
			m_NewMapGridIndexMap[beg->second] = beg->first;

		int nGridIndex = 0;
		int8 nMatIndex = -1;
		for ( DWORD i = 0; i < newGridCnt; ++i )
		{
			beg = m_NewMapGridIndexMap.find(i);
			if( beg != m_NewMapGridIndexMap.end() )
			{
				nGridIndex = beg->second;
				SGrid & grid = terrain->GetGrid(nGridIndex);
				nMatIndex = grid.nMaterialIndex;
			}
			else
				nMatIndex = -1;

			fwrite( &nMatIndex, sizeof(nMatIndex), 1, fp);  
		}
	}

	void CLocalMapSaver::SavePointLightInfo( FILE * fp, const int SceneLightCount, bool& bSceneLightCntEqual )
	{
		int     tLength = 11;
		int		nSceneLightCnt = 0;///�����жϱ���ĳ������Դ�����Ƿ��SceneLightCountһ�µļ���
		int     nGridCnt = terrain->GetGridCount();
		bool    bReSizeMap = pOperator->GetIsResizeMap();

		fwrite( &tLength, sizeof(int), 1, fp);
		fwrite( "SceneLight", 11, 1, fp);
		fwrite( &SceneLightCount, sizeof(int), 1, fp);

		for (int i = 0; i < nGridCnt; ++i )
		{
			SGrid * pGrid = &terrain->GetGrid(i);
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
								if ( bReSizeMap)
									nLightGridIndex = pOperator->m_OldMapGridIndexMap[i];
								else
									nLightGridIndex = i;

								fwrite( &nLightGridIndex, sizeof(nLightGridIndex), 1, fp);
								fwrite( &iter, sizeof(int), 1, fp);
								fwrite( &p->GetModelGroup()->m_SL, sizeof(SceneLight), 1, fp);
							}
							else
							{
								TCHAR temp[255];
								sprintf( temp, "%d", SceneLightCount );

								string log = "";
								log = temp;
								log = log + "���ӵĵ��Դ��Ϣ����";
								MessageBox(NULL,log.c_str(),"",MB_OK);
								bSceneLightCntEqual = false;
							}

							++nSceneLightCnt;
						}
					}
				}
			}
		}
	}

	void CLocalMapSaver::SaveOldMapStartCoordinateInNewMap( const DWORD dwNewWidth, const DWORD dwNewDepth, const int nOldSatrtGridInNewIndex )
	{
		string mapEnlargePath  = IoSystem::GetInst()->GetFullPath("home:Enlarge");
		bool bExit = IoSystem::GetInst()->DirectoryExists(mapEnlargePath);
		if( !bExit )
			IoSystem::GetInst()->CreateDirectory(mapEnlargePath);
		string strMapName = terrain->m_strMapName;

		string filename;
		format(filename,"%s\\%s.ini", mapEnlargePath.c_str(), strMapName.c_str());

		DWORD dwNewMapWidth = dwNewWidth * REGION_SPACE;
		DWORD dwNewMapDepth = dwNewDepth * REGION_SPACE;

		int nX = nOldSatrtGridInNewIndex % dwNewMapWidth; // ��
		int nZ = nOldSatrtGridInNewIndex / dwNewMapWidth; // ��

		CIniFile inifile;

		inifile.Create(gbk_to_utf16(filename).c_str());
		inifile.SetValue("StartCoordinate", "X", nX);
		inifile.SetValue("StartCoordinate", "Y", nZ);
		inifile.SetValue("NewMapSize", "Width", dwNewMapWidth);
		inifile.SetValue("NewMapSize", "Depth", dwNewMapDepth);

		inifile.Save();
		inifile.Close();
	}
}