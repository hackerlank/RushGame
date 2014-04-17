#pragma once
/*
*	CPlane�ṹ������3ά�ռ��е�ƽ��
*  ƽ�浽��ľ������������ģ��ͷ�ʸ�ķ���һ��Ϊ��������Ϊ��ֵ
if the normal pointing away from the origin, which causes d to be negative
if the normal pointing towards the origin, so d is positive.
if the plane goes through the origin, d is zero

*  ƽ��ķ�����	��	X*m_Normal.x + Y*Normal.y + Z*Normal.z + m_Dist = 0;
ƽ��ķ�ʸ�Ѿ���λ����

�м�������û����ɣ�����ƽ��ָ��߶Σ�ƽ��ָ����Σ���ƽ��Ľ��ߣ���ƽ��Ľ���
*/

#include "CMatrix.h"
#include "CMathMallocObject.h"

namespace sqr
{
	// Defines the three possible locations of a point in relation to a plane
	enum enumPoint2Plane
	{
		p2pFront,
		p2pBack,
		p2pCoplanar
	};

	class CPlane : public CMathMallocObject
	{
	public:
		//	ƽ��ķ���ʸ�����ǵ�λʸ��
		CVector3f		m_Normal;
		// negative of the signed distance from the origin to the plane
		float		m_Dist;

		//=======================================================================
		// ���캯��
		//=======================================================================
		CPlane();

		/*!
		\param  x    n_x
		\param  y    n_y
		\param  z    n_z
		\param  d   the negative of the signed distance from
		the origin to the plane

		Construct a plane with a unit normal  (x,y,z)  and
		the negative of the signed distance from the origin
		(0,0,0) to the plane.
		*/
		CPlane(float x, float y, float z, float d);

		/*!
		\param  n   a unit normal
		\param  P   a point on the plane

		Construct a plane with a unit normal n and
		the negative of the signed distance from the origin
		(0,0,0) to the plane.
		*/
		CPlane(const CVector3f& n, float d);

		/*!
		\param  n   a unit normal
		\param  P   a point on the plane

		Construct a plane with a unit normal  and a point on the plane.
		*/
		CPlane( const CVector3f& n,  const CVector3f& p );


		// ��ƽ���ϵ������㹹��һ��ƽ��, �����㰴��˳ʱ�����У����ֹ���
		CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c);

		//===================================================================================
		// ��ʼ������
		//===================================================================================
		/*!
		\param  n   a unit normal
		\param  P   a point on the plane

		Initialize a plane with a unit normal and
		a point on the plane.
		*/
		void Init(const CVector3f& n, const CVector3f& P);

		/*!
		\param  n   a unit normal
		\param  P   a point on the plane

		Initialize a plane with a unit normal  n and
		the negative of the signed distance from the origin
		(0,0,0) to the plane.
		*/
		void Init(const CVector3f& n, float d);


		// return the unit normal of the plane
		const CVector3f& GetNormal() const;

		// return the negative of the signed distance from the origin (0,0,0) to the plane
		float GetDist() const;

		/*!
		\param  p   a 3D point
		\return     the signed distance from the plane to the point p

		Compute the signed distance from the plane to the point p
		�㵽ƽ��ľ��룬������֮��
		*/
		float DistTo(const CVector3f& p) const;

		/*!
		\return     a plane with the values \f$ \left{ -{\bf n}, -dist \right} \f$

		Negate the values of m_Normal and m_Dist.  This creates a plane
		that is a reflection about the origin of the first (so they are parallel),
		and whose normal points the opposite direction.
		*/
		const CPlane operator-();

		// ����һ������ƽ���ǰ�棬���棬������ƽ����
		enumPoint2Plane GetSide( const CVector3f& point ) const; // it was between EP and -EP

		// ����һ���߶�ȷ����ֱ�ߺ�ƽ��Ľ���
		CVector3f Split( const CVector3f& a, const CVector3f& b ) const;

		/// ����任
		const CPlane& Transform(CMatrix matrix);
		/// ��λ��
		const CPlane& Normalize();

		//******************************************************************************
		/*! \fn     bool TVector3<T>::Mirror(const CPlane& Plane)
		*   \brief  ʸ������һ��ƽ��ľ���
		*           ���������һ�������ƽ��������, �����ֵ���ı�
		*   \param  const CPlane& Plane ƽ��
		*   \return bool	false ��������ƽ����
		*******************************************************************************/
		bool Mirror(CVector3f& vec);

		/// �Ƿ�Ϊ�Ϸ�ƽ��(abcd��Ϊ��ʱ������)
		bool IsValid() const;
	};

	//------------------------------------------------------------------------------
	inline bool
		CPlane::IsValid() const
	{
		return !(m_Normal.IsZero() && gIsZero(m_Dist));
	}

	//------------------------------------------------------------------------------
	// return the unit normal of the plane
	inline const CVector3f&
		CPlane::GetNormal() const
	{
		return m_Normal;
	}

	//------------------------------------------------------------------------------
	// return the negative of the signed distance from the origin (0,0,0) to the plane
	inline float
		CPlane::GetDist() const
	{
		return m_Dist;
	}
	//------------------------------------------------------------------------------
	inline const CPlane&
		CPlane::Transform( CMatrix matrix )
	{
		Normalize();
		float a = m_Normal.x * matrix._11 + m_Normal.y * matrix._21 + m_Normal.z * matrix._31 + m_Dist * matrix._41;
		float b = m_Normal.x * matrix._12 + m_Normal.y * matrix._22 + m_Normal.z * matrix._32 + m_Dist * matrix._42;
		float c = m_Normal.x * matrix._13 + m_Normal.y * matrix._23 + m_Normal.z * matrix._33 + m_Dist * matrix._43;
		float d = m_Normal.x * matrix._14 + m_Normal.y * matrix._24 + m_Normal.z * matrix._34 + m_Dist * matrix._44;
		m_Normal.x = a;
		m_Normal.y = b;
		m_Normal.z = c;
		m_Dist = d;

		return *this;
	}

	//------------------------------------------------------------------------------
	inline const CPlane&
		CPlane::Normalize()
	{
		float tLen = m_Normal.normalize();
		m_Dist = tLen > 0.0f ? m_Dist / tLen : 0.0f;
		return *this;
	}
	//------------------------------------------------------------------------------
	inline CPlane::CPlane() : m_Dist(0) {}

	//------------------------------------------------------------------------------
	inline CPlane::CPlane(float x, float y, float z, float d) : m_Normal(x, y, z),m_Dist(d){}

	//------------------------------------------------------------------------------
	inline CPlane::CPlane(const CVector3f& n, float d) : m_Normal(n), m_Dist(d){}

	//------------------------------------------------------------------------------
	inline CPlane::CPlane( const CVector3f& n,  const CVector3f& p ) : m_Normal(n), m_Dist( -n.Dot(p)){}


	// ��ƽ���ϵ������㹹��һ��ƽ��, �����㰴��˳ʱ�����У����ֹ���
	//------------------------------------------------------------------------------
	inline CPlane::CPlane(const CVector3f& a, const CVector3f& b, const CVector3f& c)
	{
		m_Normal	= (b-a).Cross(c-a);	// ���
		m_Normal.Normalize();
		m_Dist		= -(m_Normal.Dot(a));		// ���
	}


	//------------------------------------------------------------------------------
	inline void CPlane::Init(const CVector3f& n, const CVector3f& P) 
	{
		m_Normal	= n;
		m_Dist		= n.Dot(P);
	}

	//------------------------------------------------------------------------------
	inline void CPlane::Init(const CVector3f& n, float d) 
	{
		m_Normal = n;
		m_Dist = d;
	}


	//------------------------------------------------------------------------------
	inline float CPlane::DistTo(const CVector3f& p) const 
	{
		return m_Normal.Dot(p) + m_Dist;
	}

	//------------------------------------------------------------------------------
	inline const CPlane CPlane::operator-() 
	{
		return CPlane(-m_Normal, -m_Dist);
	}

	// ����һ������ƽ���ǰ�棬���棬������ƽ����
	//------------------------------------------------------------------------------
	inline enumPoint2Plane CPlane::GetSide( const CVector3f& point ) const
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

	//------------------------------------------------------------------------------
	inline CVector3f CPlane::Split( const CVector3f& a, const CVector3f& b ) const
	{
		float aDot = (a.Dot(m_Normal));
		float bDot = (b.Dot(m_Normal));

		float scale = ( -m_Dist - aDot) / ( bDot - aDot );

		return a + (scale * (b - a));
	}

	//------------------------------------------------------------------------------
	inline bool CPlane::Mirror(CVector3f& vec)
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

} // sqr
