#pragma once
//------------------------------------------------------------------------------
/**
@class CAxisAlignedBox

������Ķ��㶨�壺
// ע��:��ϵͳʹ����������ϵ:
// Z�����ƽ�� ����С�� ����0 ��ʼ ��ʱ��Χ�����ƽ��
// Z����Զƽ�� ������ ����4 ��ʼ ��ʱ��Χ�����ƽ��
	   5-----4
	  /|    /|
	 / |   / |
	3-----2  |
y	|  6--|--7
|  z| /   | /
| /	|/    |/
|/	0-----1
O-----x
���������� ���㶨��˵��
Z�����ƽ�� 0 - 3 �ĸ�����
Z����Զƽ�� 4 - 7 �ĸ�����

Z�����ƽ�� ���� N+4 (NΪ0 - 3) �ɵ�Զƽ�� �Զ���
Z����Զƽ�� ���� N-4 (NΪ4 - 7) �ɵý�ƽ�� �Զ���

����0 Ϊ ��С��
����4 Ϊ ����
*/

#include "CSphere.h"
#include "CVector4.h"
#include "CMathMallocObject.h"

namespace sqr
{
	struct CBaseAABB : public CMathMallocObject
	{
		CVector3f m_vMinimum;
		CVector3f m_vMaximum;
	};
	class CAxisAlignedBox : public CBaseAABB
	{
	public:
		static const CAxisAlignedBox MAX_BOX;
	protected:
		mutable CVector4f m_vCorners[8];
		bool m_bNull;
		mutable bool m_bUpdated;
	public:
		CAxisAlignedBox();
		CAxisAlignedBox( const CVector3f& min, const CVector3f& max );
		CAxisAlignedBox(	float mx, float my, float mz,
			float Mx, float My, float Mz );

		void setNull(void);
		bool isNull(void) const;

		const CVector3f& getMinimum(void) const;
		const CVector3f& getMaximum(void) const;

		void setMinimum( const CVector3f& vec );
		void setMinimum( float x, float y, float z );
		void setMaximum( const CVector3f& vec );
		void setMaximum( float x, float y, float z );

		void setExtents( const CVector3f& min, const CVector3f& max );
		void setExtents( float mx, float my, float mz,
			float Mx, float My, float Mz );

		const CVector4f* getAllCorners(void) const;
		const CVector3f getCenter(void) const;
		const CVector3f getSize(void) const;
		const CVector3f getPositiveCorner(const CVector3f& normal) const;
		const CVector3f getNegativeCorner(const CVector3f& normal) const;

		void mergeBox( const CAxisAlignedBox& rhs );
		void mergeBox( const CVector3f& point );
		void transformBox( const CMatrix& matrix);
		void transformBoxWithProj( const CMatrix& matrix);
		void scaleBox(const CVector3f& s);

		bool intersects(const CAxisAlignedBox& b2) const;
		bool intersects(const CSphere& s) const;
		bool intersects(const CPlane& p) const;
		bool intersects(const CVector3f& v) const;
		bool intersects(float* distance, const CVector3f&origin, const CVector3f& direction) const;

		inline bool operator != (const CAxisAlignedBox& box) const;
		inline bool operator == (const CAxisAlignedBox& box) const;
	protected:
		void _updateCorners(void) const;
	};

	//------------------------------------------------------------------------------
	inline bool CAxisAlignedBox::operator != (const CAxisAlignedBox& box) const
	{
		return m_vMinimum!=box.m_vMinimum||m_vMaximum!=box.m_vMaximum;
	}

	//------------------------------------------------------------------------------
	inline bool CAxisAlignedBox::operator == (const CAxisAlignedBox& box) const
	{
		return m_vMinimum==box.m_vMinimum&&m_vMaximum==box.m_vMaximum;
	}

	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::setNull(void)
	{
		m_bNull = true;
		m_bUpdated = false;
	}

	//------------------------------------------------------------------------------
	inline bool CAxisAlignedBox::isNull(void) const
	{
		return m_bNull;
	}

	//------------------------------------------------------------------------------
	inline const CVector3f& CAxisAlignedBox::getMaximum(void) const
	{
		return m_vMaximum;
	}

	//------------------------------------------------------------------------------
	inline const CVector3f& CAxisAlignedBox::getMinimum(void) const
	{
		return m_vMinimum;
	}

	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::setMaximum( const CVector3f& vec )
	{
		m_bNull = false;
		m_bUpdated = false;
		m_vMaximum = vec;
	}

	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::setMaximum(float x, float y, float z)
	{
		m_bNull = false;
		m_bUpdated = false;
		m_vMaximum.x = x;
		m_vMaximum.y = y;
		m_vMaximum.z = z;
	}

	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::setMinimum( const CVector3f& vec )
	{
		m_bNull = false;
		m_bUpdated = false;
		m_vMinimum = vec;
	}

	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::setMinimum(float x, float y, float z)
	{
		m_bNull = false;
		m_bUpdated = false;
		m_vMinimum.x = x;
		m_vMinimum.y = y;
		m_vMinimum.z = z;
	}


	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::setExtents( const CVector3f& min, const CVector3f& max )
	{
		m_bNull = false;
		m_bUpdated = false;
		m_vMinimum = min;
		m_vMaximum = max;
	}

	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::setExtents( float mx, float my, float mz,
		float Mx, float My, float Mz )
	{
		m_bNull = false;
		m_bUpdated = false;
		m_vMinimum.x = mx;
		m_vMinimum.y = my;
		m_vMinimum.z = mz;
		m_vMaximum.x = Mx;
		m_vMaximum.y = My;
		m_vMaximum.z = Mz;
	}

	//------------------------------------------------------------------------------
	inline const CVector4f* CAxisAlignedBox::getAllCorners(void) const
	{
		_updateCorners();
		return (const CVector4f*)(m_vCorners);
	}

	//------------------------------------------------------------------------------
	inline const CVector3f
		CAxisAlignedBox::getPositiveCorner( const CVector3f& normal ) const
	{
		CVector3f pos = m_vMinimum;
		if (normal.x >= 0)
			pos.x = m_vMaximum.x;
		if (normal.y >= 0)
			pos.y = m_vMaximum.y;
		if (normal.z >= 0)
			pos.z = m_vMaximum.z;
		return pos;
	}

	//------------------------------------------------------------------------------
	inline const CVector3f
		CAxisAlignedBox::getNegativeCorner( const CVector3f& normal ) const
	{
		CVector3f neg = m_vMaximum;
		if (normal.x >= 0)
			neg.x = m_vMinimum.x;
		if (normal.y >= 0)
			neg.y = m_vMinimum.y;
		if (normal.z >= 0)
			neg.z = m_vMinimum.z;
		return neg;
	}

	//------------------------------------------------------------------------------
	inline const CVector3f CAxisAlignedBox::getCenter(void) const
	{
		return CVector3f((m_vMinimum+m_vMaximum) * 0.5f );
	}

	//------------------------------------------------------------------------------
	inline const CVector3f  CAxisAlignedBox::getSize(void) const
	{
		return CVector3f(m_vMaximum-m_vMinimum);
	}

	//------------------------------------------------------------------------------
	inline void CAxisAlignedBox::scaleBox(const CVector3f& s)
	{
		m_vMaximum *= s;
		m_vMinimum *= s;
	}
}
