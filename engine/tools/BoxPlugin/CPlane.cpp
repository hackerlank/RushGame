#include "stdafx.h"

#include "CAxisAlignedBox.h"
#include "CSphere.h"
#include "CPlane.h"

namespace sqr
{
	CPlane::CPlane() : m_Dist(0) {}

	CPlane::CPlane(float x, float y, float z, float d) : m_Normal(x, y, z),m_Dist(d){}

	CPlane::CPlane(const CVector3f& n, float d) : m_Normal(n), m_Dist(d){}

	CPlane::CPlane( const CVector3f& n,  const CVector3f& p ) : m_Normal(n), m_Dist( -n.Dot(p)){}


	// ��ƽ���ϵ������㹹��һ��ƽ��, �����㰴��˳ʱ�����У����ֹ���
	CPlane::CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c)
	{
		m_Normal	= (b-a)^(c-a);	// ���
		m_Normal.Normalize();
		m_Dist		= -(m_Normal*a);		// ���
	}


	void CPlane::Init(const CVector3f& n, const CVector3f& P) 
	{
		m_Normal	= n;
		m_Dist		= n.Dot(P);
	}

	void CPlane::Init(const CVector3f& n, float d) 
	{
		m_Normal = n;
		m_Dist = d;
	}


	float CPlane::DistTo(const CVector3f& p) const 
	{
		return m_Normal.Dot(p) + m_Dist;
	}

	const CPlane CPlane::operator-() 
	{
		return CPlane(-m_Normal, -m_Dist);
	}

	// ����һ������ƽ���ǰ�棬���棬������ƽ����
	enumPoint2Plane CPlane::GetSide( const CVector3f& point ) const
	{
		float dp = m_Normal.Dot(point) + m_Dist;

		if(dp > MATH_EPSILON)
		{
			return p2pFront;
		}
		if(dp < -MATH_EPSILON )
		{
			return p2pBack;
		}

		return p2pCoplanar; // it was between EP and -EP
	}

	CVector3f CPlane::Split( const CVector3f& a, const CVector3f& b ) const
	{
		float aDot = (a * m_Normal);
		float bDot = (b * m_Normal);

		float scale = ( -m_Dist - aDot) / ( bDot - aDot );

		return a + (scale * (b - a));
	}

	bool CPlane::Mirror(CVector3f& vec)
	{
		float		fDist		= DistTo(vec);//GetPointDist(vec);     //GetPointDist()��ô�����ˣ���lily
		CVector3f	vNormal		= GetNormal();	

		if ( /*gBMIsZero(fDist)*/ gIsZero(fDist) )//gBMIsZero()��ô�����ˣ���lily
			return false;
		else if (fDist > 0.0f)
		{
			vNormal = -vNormal;
		}

		vec = vec + vNormal*fDist*2.0f;
		return true;
	}
}