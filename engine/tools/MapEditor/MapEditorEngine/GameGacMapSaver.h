#pragma once
/**
	@class CGameGacMapSaver

	��Ϸ�ͻ��˵�amp,ars, arg�ļ�����

	//////////////////////////////////////////////////////////////////////////
	.amp�ļ�
	1.4��ʱ��εĹ����Ϣ
	2.ˮ�����˵���Ϣ
	3.����ͼ�����Ϣ
	4.��������Ϣ

	//////////////////////////////////////////////////////////////////////////
	.ars�ļ�
	1.ƴ����Ϸ������
	2.����ģ�ͣ���������Ч��Ϣ���Ա�argֱ�ӱ�����Ӧ������ֵ

	//////////////////////////////////////////////////////////////////////////
	.arg�ļ�����Chunk�ṹ, ÿ��Chunk�ṹ����:
	====================================
	0x0000: FourCC
	0x0004: Version
	0x0008: ChunkSize
	[ChunkSize bytes]
	====================================
	��14����֧Chunk
	REGN------------------------(�ļ�ͷ��־)
		SRGN--------------------(�ķ�֮һ����)
		VHGT--------------------(����߶�)
		VDIF--------------------(������ɫ)
		GATR--------------------(�赲)
		EBLK--------------------(��ȷ�赲)
		TEXT--------------------(����)
		MODL--------------------(����ģ��)
		TRAP--------------------(Trap�㣬�Ѿ�û����)
		LHGT--------------------(�߼���)
		SBVT--------------------(SubVertex����ʱû������)
		OBJT--------------------(Object)
		WATR--------------------(ˮ����ʱû������)
		SCLT--------------------(�������Դ)
		SPLT--------------------(�����۹��)
		AUDI--------------------(������Դ)
		LOGI--------------------(��¼ģ��)

	(C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"

namespace sqr
{
class CTerrainMesh;
class IProgressCallback;
SmartPointer(CGameGacMapSaver);

class CGameGacMapSaver : public CRefObject
{
public:
	CGameGacMapSaver();
	/// ����terrain mesh
	void SetTerrainMesh(CTerrainMesh* terrain);
	/// ���ý������ص�
	void SetProcessCallback(IProgressCallback* progress);

	/// Save GameGac File
	bool SaveAsClient(const string& path);

private:
	///.ars save
	void SaveResSetFile(const string& path);
	///.amp save
	void SaveAmpSetFile(const string& path);
	///.arg save
	void SaveRegionFile(const string& path);
	///ƴ�Ӵ�����
	void MakeAtlasTexture(const string& path);

private:
	CTerrainMesh* terrain;
	IProgressCallback* progress;
};

//------------------------------------------------------------------------------
inline void
CGameGacMapSaver::SetTerrainMesh( CTerrainMesh* t )
{
	Ast(NULL != t);
	this->terrain = t;
}

//------------------------------------------------------------------------------
inline void
CGameGacMapSaver::SetProcessCallback( IProgressCallback* p )
{
	Ast(NULL != p);
	this->progress = p;
}

}// namespace sqr