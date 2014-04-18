#pragma once


#include "CRefObject.h"

namespace sqr
{
	class CTerrainMesh;
	class IProgressCallback;
	SmartPointer(CGameCommonMapSaver);

	class CGameCommonMapSaver : public CRefObject
	{
	public:
		CGameCommonMapSaver();
		/// ����terrain mesh
		void SetTerrainMesh(CTerrainMesh* terrain);
		/// ���ý������ص�
		void SetProcessCallback(IProgressCallback* progress);

		/// Save Common File
		bool SaveAsCommon(const string& path);

	private:
		CTerrainMesh* terrain;
		IProgressCallback* progress;
	};

	//------------------------------------------------------------------------------
	inline void
		CGameCommonMapSaver::SetTerrainMesh( CTerrainMesh* t )
	{
		Ast(NULL != t);
		this->terrain = t;
	}

	//------------------------------------------------------------------------------
	inline void
		CGameCommonMapSaver::SetProcessCallback( IProgressCallback* p )
	{
		Ast(NULL != p);
		this->progress = p;
	}

}// namespace sqr