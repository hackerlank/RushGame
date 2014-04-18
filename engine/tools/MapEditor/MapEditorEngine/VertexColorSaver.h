#pragma once
/**
	@class VertexColorSaver

	�決�������ɫ�ı���.

	�ļ�����Chunk�ṹ, ÿ��Chunk�ṹ����:
	====================================
	0x0000: FourCC
	0x0004: Version
	0x0008: ChunkSize
	[ChunkSize bytes]
	====================================
	���ļ�Chunk
	BKCL------------------------(�ļ�ͷ, �����grid��ƫ��)
		GRID--------------------(����, ���漸��ģ��)
			MODL----------------(ģ��, ����ԭ����, ModelGroup����, ����piece class)
				CLAS------------(piece class, �����piece����ɫֵ)

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
struct PieceClassColor;
SmartPointer(Stream);
SmartPointer(BinaryWriter);
SmartPointer(VertexColorSaver);

class VertexColorSaver : public CRefObject
{
public:
	VertexColorSaver();
	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);

	/// ���浽�ļ�
	bool Save(const string& path);

private:
	void WriteGridColor(index_t gridIndex);
	void WriteModelColor(index_t originGridIndex,
	                     index_t modelGroupIndex,
	                     const string& modelName,
	                     const vector<PieceClassColor>& classColors);
	void WriteClassColor(const PieceClassColor* classColor);

private:
	CTerrainMesh* terrain;
	IProgressCallback* progress;

	StreamPtr stream;
	BinaryWriterPtr writer;
};

//------------------------------------------------------------------------------
inline void
VertexColorSaver::SetTerrainMesh( CTerrainMesh* t )
{
	Ast(NULL != t);
	this->terrain = t;
}

//------------------------------------------------------------------------------
inline void
VertexColorSaver::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

}// namespace sqr