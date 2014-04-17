//------------------------------------------------------------------------------
//  CShadowedGrids.cpp
//  �⹦��̫������, �¸���Ŀ���ƾ�Ҫ�ɵ�
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "CShadowedGrids.h"
#include "StringHelper.h"
#include "CPkgFile.h"
#include "ErrLogHelper.h"
#include "ExpHelper.h"
const SWString PATH_ALIAS_BAKE = L"res";
namespace sqr
{

//------------------------------------------------------------------------------
CShadowedGrids::CShadowedGrids()
: widthInGrid(0)
, heightInGrid(0)
{
	// empty
}

//------------------------------------------------------------------------------
CShadowedGrids::~CShadowedGrids()
{
	// empty
}

//------------------------------------------------------------------------------
void
CShadowedGrids::Load( const string& path, uint w, uint h )
{
	this->widthInGrid = w;
	this->heightInGrid = h;
	this->lowGrids.clear();

	string low = format("%s/lightmap/shadow.sh1", path.c_str());
	CPkgFile lowFile;
	if (eFE_SUCCESS == lowFile.Open(PATH_ALIAS_BAKE.c_str(),low.c_str()))
	{
		this->lowGrids.resize(lowFile.Size());
		lowFile.Read(&this->lowGrids.front(), this->lowGrids.size());
	}
	else
	{
		ostringstream strm;
		strm<< low << "������\n";
		ArtErr(strm.str());
	}
}

//------------------------------------------------------------------------------
CColor
CShadowedGrids::GetShadowedColor( uint xGrid, uint yGrid )
{
	if (this->lowGrids.empty())
		return CColor::White;

	index_t index = xGrid + yGrid * this->widthInGrid;
	
	BYTE gray = 255; 
	if( index<lowGrids.size() )
		gray = this->lowGrids[index];
	else
		ArtErr("��Դ��ƥ��","lightmap/shadow.sh1");
	return CColor(255, gray, gray, gray);
}

//------------------------------------------------------------------------------
bool 
CShadowedGrids::ShadowedColorIsEmpty() const
{
	return lowGrids.empty();
}
}// namespace sqr
