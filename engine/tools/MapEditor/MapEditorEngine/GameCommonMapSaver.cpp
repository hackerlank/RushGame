

#include "StdAfx.h"
#include "GameCommonMapSaver.h"
#include "GraphicCommon.h"
#include "TerrainMesh.h"
#include "Engine.h"
#include "Console.h"
#include "CLinkRegionBuild.h"
#include "TBinaryHeap.inl"
#include "IoSystem.h"

namespace sqr
{

	//------------------------------------------------------------------------------
	CGameCommonMapSaver::CGameCommonMapSaver()
		: terrain(NULL)
		, progress(NULL)
	{
	}

	//------------------------------------------------------------------------------
	bool
		CGameCommonMapSaver::SaveAsCommon(const string& path)
	{
		Ast(NULL != this->terrain);
		Ast(NULL != this->progress);

		locale loc;
		locale::global(locale(""));

		// region file
		// region file
		string strOldRgnFileName = "map\\" + path + ".lrg";
		if( IoSystem::GetInst()->FileExists(strOldRgnFileName) )
		{
			bool bDelete = IoSystem::GetInst()->DeleteFile(strOldRgnFileName);
			if( !bDelete )
			{
				if ( IoSystem::GetInst()->IsReadOnly(strOldRgnFileName) )
				{
					string log = strOldRgnFileName;
					log += "���ļ��ѱ�����������ǰ�Ƚ��������򱣴���ļ�ʧ��";

					MessageBox( NULL, log.c_str() , "��ʾ", MB_OK );
				}
			}
			MessageBox( NULL, "��ԭ��resbin/map�¶�Ӧ��lrg�ļ�ɾ��" , "��ʾ", MB_OK );
		}

		string strRgnFileName = "common\\map\\" + path + ".lrg";

		use_facet<ctype<TCHAR> >( loc ).tolower
			( &strRgnFileName[0], &strRgnFileName[strRgnFileName.length()] );


		CLinkRegionBuild::Inst().SetProcessCallback(progress);
		return CLinkRegionBuild::Inst().CreateLinkRegionFile(strRgnFileName, terrain, 1);
	}

}// namespace sqr
