#pragma once
//------------------------------------------------------------------------------
/**
    @class VertexColorLoader

	�決����ɫ������.

	@see VertexColorSaver

    (C) 2009 ThreeOGCMan
*/
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
struct PieceClassColor;
SmartPointer(Stream);
SmartPointer(BinaryReader);
SmartPointer(VertexColorLoader);

class VertexColorLoader : public CRefObject
{
public:
	VertexColorLoader();

	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);

	/// ����
	bool Load(const string& path);

private:
	bool ReadGridColor(index_t gridIndex);
	void ReadModelColor(index_t& originGridIndex,
						index_t& modelGroupIndex,
						string& modelName,
						vector<PieceClassColor>& classColors);
	void ReadClassColor(PieceClassColor* classColor);

private:
	CTerrainMesh* terrain;
	IProgressCallback* progress;

	StreamPtr stream;
	BinaryReaderPtr reader;
};

//------------------------------------------------------------------------------
inline void
VertexColorLoader::SetTerrainMesh( CTerrainMesh* t )
{
	this->terrain = t;
}

//------------------------------------------------------------------------------
inline void
VertexColorLoader::SetProcessCallback( IProgressCallback* p )
{
	this->progress = p;
}

}// namespace sqr
