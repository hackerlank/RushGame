#include "stdafx.h"
#include "../ChunkData/CEditorAudioCoder.h"
#include "CMapEditApp.h"
#include "TerrainMesh.h"
#include "CMapEditObject.h"
#include "ModelGroup.h"
#include "TerrainMeshObject.h"
#include "LocalMapResMgr.h"

BEGIN_VERSION_MAP(CEditorAudioCoder)
	ON_VERSION( 1 , DeCode_1 )
END_VERSION_MAP()

bool sqr::CEditorAudioCoder::Code( WriteDataInf& wdi , OBJ_HANDLE handle )
{
	CTerrainMesh  *pTerrain = CMapEditApp::GetInst()->GetTerrain();
	DWORD		  dwGridCntInTileRegion = pTerrain->GetGridCountInTileRegion();
	int			  nGridIndex = 0;
	int			  nRegionIndex = *(static_cast<int*>(handle));
	STileRegion   *pTileRegion = pTerrain->GetTileRegion(nRegionIndex);

	vector<CTObjectPtr> vecAudioObejects;
	size_t		  size = vecAudioObejects.size();
	wdi.Write(&size,		sizeof(size));

	vector< CTObjectPtr >::iterator beg;
	vector< CTObjectPtr >::iterator end = vecAudioObejects.end();
	for (beg = vecAudioObejects.begin(); beg != end; ++beg)
	{
		CTObject * p = (*beg);
		CEditModelGroup *pModelGroup = p->GetModelGroup();

		CLoadMapResInfo::EditorOffsetInfo emodelInfo;
		emodelInfo.vUserOffsetEx  = pModelGroup->GetUserOffsetEx();

		//DebugOut("Load vOffsetOverall %f, %f, %f\n", emodelInfo.vOffsetOverall.x, emodelInfo.vOffsetOverall.y, emodelInfo.vOffsetOverall.z);

		wdi.Write(&emodelInfo,		sizeof(emodelInfo));
	}
	return true;
}

bool sqr::CEditorAudioCoder::DeCode_1( const CDataChunk& inData , OBJ_HANDLE handle )
{
	CBufFile tData( inData.GetBuffer(), inData.GetSize() );

	size_t size = 0;
	tData.SafeRead( &size,	sizeof(size) );
	for (size_t i = 0; i < size; ++i)
	{
		//tData.SafeRead( &regionInfo,	sizeof(regionInfo) );
		CLoadMapResInfo::EditorOffsetInfo *pEditorOffsetInfo = new CLoadMapResInfo::EditorOffsetInfo;
		tData.SafeRead( pEditorOffsetInfo,	sizeof(CLoadMapResInfo::EditorOffsetInfo) );
		CLoadMapResInfo::GetInst()->vecEditorAudioOffsetInfo.push_back(pEditorOffsetInfo);
	}

	return true;
}

class CAnyObject;


CEditorAudioFormat::CEditorAudioFormat(void)
: CFileFormatMgr( 'EAFT', 0 )
{
	InitAudioCode();
}

CEditorAudioFormat::~CEditorAudioFormat(void)
{ /*do nothing~~~*/ }

void sqr::CEditorAudioFormat::InitAudioCode()
{
	RegisterCoder(new CEditorAudioCoder);
}

bool sqr::CEditorAudioFormat::LoadAudioFile( const string& strFile, int nRegionIndex )
{
	return Load( strFile, static_cast<OBJ_HANDLE>(&nRegionIndex) );
}

bool sqr::CEditorAudioFormat::SaveAudioFile( const string& strFile, int nRegionIndex )
{
	return Save( strFile, static_cast<OBJ_HANDLE>(&nRegionIndex) );
}