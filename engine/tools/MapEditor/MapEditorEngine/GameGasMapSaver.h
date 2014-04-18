#pragma once
/**
	@class CGameGasMapSaver

	��Ϸ����˵�arg�ļ�����

	�ļ�����Chunk�ṹ, ÿ��Chunk�ṹ����:
	====================================
	0x0000: FourCC
	0x0004: Version
	0x0008: ChunkSize
	[ChunkSize bytes]
	====================================
	���ĸ���֧Chunk
	REGN------------------------(�ļ�ͷ��־)
		GHGT--------------------(���Ӹ߶�)
		GATR--------------------(�赲��Ϣ)
		EBLK--------------------(��ȷ�赲��Ϣ)
		TRAP--------------------(Trap����Ϣ���Ѿ�û����)
	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
SmartPointer(CGameGasMapSaver);

class CGameGasMapSaver : public CRefObject
{
public:
	CGameGasMapSaver();
	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);

	/// Save GameGac File
	bool SaveAsServer(const string& path);

private:
	CTerrainMesh* terrain;
	IProgressCallback* progress;
};

//------------------------------------------------------------------------------
inline void
CGameGasMapSaver::SetTerrainMesh( CTerrainMesh* t )
{
	Ast(NULL != t);
	this->terrain = t;
}

//------------------------------------------------------------------------------
inline void
CGameGasMapSaver::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

}// namespace sqr