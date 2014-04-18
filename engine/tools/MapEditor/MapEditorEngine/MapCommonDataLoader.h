#pragma once
#include "TSingleton.h"
#include "CVector3.h"
namespace sqr
{
	class CTerrainMesh;
	class COperator;
	class SVertex;
	class SGrid;

	class MapCommonDataLoader : public TSingleton<MapCommonDataLoader>
	{
		COperator		*pOperator;
		CTerrainMesh	*terrain;
		vector<string>	ErrObjectList;
	public:
		friend class TSingleton<MapCommonDataLoader>;
		MapCommonDataLoader();
		virtual ~MapCommonDataLoader();

		void SetOperator(COperator *pOper);
		void SetTerrainMesh(CTerrainMesh *pTerrain);

		// ��ȡenv ������Ϣ
		void LoadEnvInfo(FILE * fp);

		// ��ȡ����ͼ��Ϣ
		void LoadBackPictureInfo( FILE * fp );

		// ��ȡwave��Ϣ
		void LoadWaveInfo(FILE * fp);

		// ��ȡÿ��������Ϣ
		void LoadEachVertexInfo(FILE * fp, SVertex* pVertex);

		void PrintErrorObject()
		{
			if (ErrObjectList.size()>0)
			{
				TCHAR tt[255];
				sprintf( tt, "�޳����󳡾���� %d ��", ErrObjectList.size() );
				::MessageBox( 0, tt, "��ʾ", MB_ICONINFORMATION);
				vector<string>::iterator eoitor = ErrObjectList.begin();
				while (eoitor!=ErrObjectList.end())
				{
					string pr;
					for (int i=0;eoitor!=ErrObjectList.end() && i<10;++i)
					{
						pr = pr + (*eoitor) + "\n";
						eoitor++;
					}
					::MessageBox( 0, pr.c_str(), "��ʾ", MB_ICONINFORMATION);
				}

				ErrObjectList.clear();
			}
		}
	};
}
