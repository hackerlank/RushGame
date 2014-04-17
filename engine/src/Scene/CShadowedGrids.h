#pragma once
/**
	@class CShadowedGrids

	 ��Ӱ�ڸǵĸ���.
	 ÿ�����ӱ���һ���Ҷ�ֵ

	 (C) 2009 ThreeOGCMan
 */
#include "CRefObject.h"
#include "CColor.h"
#include "CSceneMallocObject.h"

namespace sqr
{

SmartPointer(CShadowedGrids);
class CShadowedGrids 
	: public CRefObject
	, public CSceneMallocObject
{
public:
	CShadowedGrids();
	virtual ~CShadowedGrids();

	/// ��map/[MAPNAME]/lightmap/�¼����ļ�����
	void Load(const string& path, uint w, uint h);

	/// ��������������Ƿ�����Ӱ��, �����ػ�����ɫֵ
	CColor GetShadowedColor(uint xGrid, uint yGrid);
	bool   ShadowedColorIsEmpty() const;

private:
	SVector<BYTE> lowGrids;

	uint widthInGrid;
	uint heightInGrid;
};

}// namespace sqr