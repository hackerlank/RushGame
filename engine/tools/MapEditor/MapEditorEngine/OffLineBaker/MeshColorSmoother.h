#pragma once
//------------------------------------------------------------------------------
/**
    @class CMeshColorSmoother

	����Mesh����ɫ����Ĺ�����.
	��Ϊ��Щģ�͵��ڽӶ����λ�úͷ�����һ����, �����Ҫ���ǵ���ɫҲ��һ����.
	�������Ķ������ɫ����ƽ������, ʹģ�͵���ɫ���ɸ�Ϊƽ��.

    (C) 2009 ThreeOGCMan
*/
#include "CRefObject.h"
#include "CVector3.h"
#include "CColor4.h"

//------------------------------------------------------------------------------
namespace sqr
{

SmartPointer(CMeshColorSmoother);
class CMeshColorSmoother : public CRefObject
{
public:
	/// ���һ������, �����������ļ���
	void AddVertex(const CVector3f& pos, const CVector3f& nml, const CColor4& clr);
	/// ȡ��ƽ����Ķ�����ɫ
	D3DCOLOR GetSmoothedColor(const CVector3f& pos, const CVector3f& nml);

	/// ��ջ���
	void Clear();

private:
	struct Vertex
	{
	public:
		Vertex() {};
		Vertex(const CVector3f& pos, const CVector3f& nml);;
		 
		bool operator==(const Vertex& rhs) const;
	public:
		CVector3f position;
		CVector3f normal;
	};
	struct AverageColor
	{
	public:
		AverageColor();
		AverageColor(const CColor4& clr);
	public:
		uint count;			//< �ڽӶ�����Ŀ
		CColor4 colorSum;	//< ��ɫ��
	};

private:
	vector<Vertex> vertices;
	vector<AverageColor> colors;
};

//------------------------------------------------------------------------------
inline void
CMeshColorSmoother::Clear()
{
	this->vertices.clear();
	this->colors.clear();
}

//------------------------------------------------------------------------------
inline
CMeshColorSmoother::Vertex::Vertex( const CVector3f& pos, const CVector3f& nml )
: position(pos), normal(nml)
{
	// empty
}

//------------------------------------------------------------------------------
inline bool
CMeshColorSmoother::Vertex::operator==( const Vertex& rhs ) const
{
	return (this->position == rhs.position) && (this->normal == rhs.normal);
}

//------------------------------------------------------------------------------
inline
CMeshColorSmoother::AverageColor::AverageColor()
: count(0), colorSum(0, 0, 0, 0)
{
	// empty
}

//------------------------------------------------------------------------------
inline
CMeshColorSmoother::AverageColor::AverageColor( const CColor4& clr )
: count(1), colorSum(clr)
{
	// empty
}

}// namespace sqr

