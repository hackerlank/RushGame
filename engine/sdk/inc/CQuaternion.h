#pragma once

/******************************************************************************
*! \file       CCQuaternion.h
*   \brief      ��Ԫ����Ķ���
*
*				������ֿ��ܵĹ�����Ԫ���ķ���,����ص���Ԫ������
*   \version	1.0
*   \date		04-8-2 11:42
*   \todo		��û���������Ĳ��Լ�������Ż� �������ǲ�������
*******************************************************************************/

#include "CMatrix.h"
#include "CVector3.h"
#include "CMathMallocObject.h"

namespace sqr
{
	class CQuaternion : public CMathMallocObject
	{
	public:
		union
		{
			//#ifdef SIMD
			//		__declspec(align(16)) float q[4];
			//#else
			float q[4];
			//#endif
			//��w����ǰ�� ��Ҫ���� SSE�ļ���
			struct
			{
				float w,z,y,x;
			};
		};
	public:
		CQuaternion( const CQuaternion& OriQuater );
		CQuaternion( float qw = 1.0f,float qx = 0.0f,float qy = 0.0f,float qz = 0.0f );

		void setNULL(void);
		void setIdentity(void);

		void FromRotationMatrix (const CMatrix& matRot);
		void ToRotationMatrix (CMatrix& matRot) const;
		void FromAngleAxis (const float& fAngle, const CVector3f& vAxis);
		void ToAngleAxis (float& fAngle, CVector3f& vAxis) const;

		void FromAxes (const CVector3f* avAxis);
		void FromAxes (const CVector3f& xAxis, const CVector3f& yAxis, const CVector3f& zAxis);
		void ToAxes (CVector3f* avAxis) const;
		void ToAxes (CVector3f& xAxis, CVector3f& yAxis, CVector3f& zAxis) const;

		void FromTwoVector( const CVector3f& vOrg,const CVector3f& vDest );

		bool		operator ==( const CQuaternion& cQ ) const;
		bool		operator !=( const CQuaternion& cQ ) const;
		CQuaternion& operator =( const CQuaternion& cQ );
		CQuaternion	operator +( const CQuaternion& cQ ) const;
		CQuaternion	operator -( const CQuaternion& cQ ) const;
		CQuaternion	operator *( const CQuaternion& cQ ) const;
		CQuaternion	operator *( const float cr ) const;

		CQuaternion& operator *=( const CQuaternion& cQ );
		CQuaternion& operator *=( const float cr );

		friend CQuaternion operator* (const float cr,
			const CQuaternion& cQ);
		CQuaternion operator- () const;

		//���ܺ���
		CVector3f		operator *(const CVector3f& cV) const; //��תһ��3D����

		//��ֵ
		CQuaternion& lerp(const CQuaternion& rkP, const CQuaternion& rkQ, float fT );
		//�����ֵ
		CQuaternion& Slerp(const CQuaternion& rkP, const CQuaternion& rkQ, float fT );

		CQuaternion& SlerpExtraSpins ( const CQuaternion& rkP, const CQuaternion& rkQ, int iExtraSpins , float fT);

		// �������淽�̲�ֵ
		CQuaternion& Squad ( const CQuaternion& rkP,
			const CQuaternion& rkA, const CQuaternion& rkB,
			const CQuaternion& rkQ, float fT );

		//��ѧ����
		float Dot (const CQuaternion& rkQ) const;
		float Norm () const;
		float normalize(void);
		CQuaternion Inverse () const;
		CQuaternion UnitInverse () const;
		CQuaternion Exp () const;
		CQuaternion Log () const;

		//��ֵ����
		// ��ԭ���Ĳ�ֵ
		//ֱ�߲�ֵ
		static void lerp( CQuaternion& outQ, const CQuaternion& rkP,
			const CQuaternion& rkQ, float fT );

		//�����ֵ
		static void Slerp ( CQuaternion& outQ, const CQuaternion& rkP,
			const CQuaternion& rkQ, float fT, bool shortestPath = true);

		static void SlerpExtraSpins ( CQuaternion& outQ, const CQuaternion& rkP, const CQuaternion& rkQ,
			int iExtraSpins , float fT);

		// ���ö������淽�̲�ֵ
		static void Intermediate (const CQuaternion& rkQ0,
			const CQuaternion& rkQ1, const CQuaternion& rkQ2,
			CQuaternion& rkA, CQuaternion& rkB);

		// �������淽�̲�ֵ
		static void Squad (	CQuaternion& outQ, const CQuaternion& rkP,
			const CQuaternion& rkA, const CQuaternion& rkB,
			const CQuaternion& rkQ, float fT, bool shortestPath = true);
	protected:
		static const float ms_fEpsilon;
	private:
	};


	_MATH_INLINE_ CQuaternion::CQuaternion(float qw /* = 1.0f  */,float qx /* = 0.0f */,float qy /* = 0.0f */,
		float qz /* = 0.0f */)
	{ w = qw; x = qx; y = qy; z = qz; }

	_MATH_INLINE_ CQuaternion::CQuaternion( const CQuaternion& OriQuater )
	{
		x = OriQuater.x; y = OriQuater.y; 
		z = OriQuater.z; w = OriQuater.w;
	}

	_MATH_INLINE_ void CQuaternion::setNULL()
	{ x=y=z=w=0; }

	_MATH_INLINE_ void CQuaternion::setIdentity()
	{ x=y=z=0; w=1;}

	_MATH_INLINE_ void CQuaternion::FromAngleAxis (const float& fAngle, const CVector3f& vAxis)
	{
		float fHalfAngle = float(0.5f*fAngle);
		float fSin = CMath::Sin(fHalfAngle);
		w = CMath::Cos(fHalfAngle);
		x = fSin*vAxis.x;
		y = fSin*vAxis.y;
		z = fSin*vAxis.z;
	}

	_MATH_INLINE_ void CQuaternion::ToAngleAxis (float& fAngle, CVector3f& vAxis) const
	{
		float fSqrLength = x*x+y*y+z*z;
		if ( fSqrLength > 0.0f )
		{
			fAngle = float(2.0f*CMath::ACos(w));
			float fInvLength = CMath::InvSqrt(fSqrLength);
			vAxis.x = x*fInvLength;
			vAxis.y = y*fInvLength;
			vAxis.z = z*fInvLength;
		}
		else
		{
			// �Ƕ�Ϊ0
			fAngle = 0.0;
			vAxis.x = 1.0;
			vAxis.y = 0.0;
			vAxis.z = 0.0;
		}
	}

	_MATH_INLINE_ void CQuaternion::FromAxes (const CVector3f* avAxis)
	{
		CMatrix kRot;

		for (int iCol = 0; iCol < 3; ++iCol)
		{
			kRot.m[iCol][0] = avAxis[iCol].x;
			kRot.m[iCol][1] = avAxis[iCol].y;
			kRot.m[iCol][2] = avAxis[iCol].z;
		}

		FromRotationMatrix(kRot);
	}

	_MATH_INLINE_ void CQuaternion::FromAxes (const CVector3f& xAxis, const CVector3f& yAxis, const CVector3f& zAxis)
	{
		CMatrix kRot;

		kRot.m[0][0] = xAxis.x;
		kRot.m[0][1] = xAxis.y;
		kRot.m[0][2] = xAxis.z;

		kRot.m[1][0] = yAxis.x;
		kRot.m[1][1] = yAxis.y;
		kRot.m[1][2] = yAxis.z;

		kRot.m[2][0] = zAxis.x;
		kRot.m[2][1] = zAxis.y;
		kRot.m[2][2] = zAxis.z;

		FromRotationMatrix(kRot);
	}

	_MATH_INLINE_ void CQuaternion::ToAxes (CVector3f* avAxis) const
	{
		CMatrix kRot;

		ToRotationMatrix(kRot);

		for (int iCol = 0; iCol < 3; ++iCol)
		{
			avAxis[iCol].x = kRot.m[iCol][0];
			avAxis[iCol].y = kRot.m[iCol][1];
			avAxis[iCol].z = kRot.m[iCol][2];
		}
	}

	_MATH_INLINE_ void CQuaternion::ToAxes (CVector3f& xAxis, CVector3f& yAxis, CVector3f& zAxis) const
	{
		CMatrix kRot;

		ToRotationMatrix(kRot);

		xAxis.x = kRot.m[0][0];
		xAxis.y = kRot.m[0][1];
		xAxis.z = kRot.m[0][2];

		yAxis.x = kRot.m[1][0];
		yAxis.y = kRot.m[1][1];
		yAxis.z = kRot.m[1][2];

		zAxis.x = kRot.m[2][0];
		zAxis.y = kRot.m[2][1];
		zAxis.z = kRot.m[2][2];
	}

	_MATH_INLINE_ void CQuaternion::FromTwoVector(const CVector3f& vOrg,const CVector3f& vDest )
	{
		//assert:vOrg��vDest �����Ϊ��λ����
		CVector3f axis = vOrg.Cross(vDest);
		//axis��ģΪ|axis| = |vOrg|*|vDest|*sin2a = 2sina*cosa
		//ע��2aΪvOrg��vDest�м�ļн�
		//������нǵ�cos2a ��cos2a = 1ʱ ���������غ�
		//����cos2a = -1Ҳʲô������
		float c = vOrg.Dot(vDest);

		if (c >= 1.0f || c<=-1.0f)
		{
			x=0;y=0;z=0;w=1.0;
			return;
		}

		//����2���ǹ�ʽ��s=2cosa
		float s = CMath::Sqrt( (1+c)*2 );
		float invs = 1 / s;
		x = axis.x * invs;
		y = axis.y * invs;
		z = axis.z * invs;
		w = s * 0.5f;
		return;
	}

	//---------�����--------
	_MATH_INLINE_ bool CQuaternion::operator ==( const CQuaternion& cQ ) const
	{
		return (cQ.x == x) && (cQ.y == y) &&
			(cQ.z == z) && (cQ.w == w);
	}

	_MATH_INLINE_ bool CQuaternion::operator !=( const CQuaternion& cQ ) const
	{
		return (cQ.x != x) || (cQ.y != y) ||
			(cQ.z != z) || (cQ.w != w);
	}

	_MATH_INLINE_ CQuaternion& CQuaternion::operator =( const CQuaternion& cQ )
	{  
		x = cQ.x; y = cQ.y; z = cQ.z; w = cQ.w;
		return *this; 
	}

	

	_MATH_INLINE_ CQuaternion CQuaternion::operator -() const
	{
		return CQuaternion( -w, -x, -y, -z );
	}

	//-----���ܺ���------
	_MATH_INLINE_ CVector3f CQuaternion::operator *(const CVector3f& cV) const
	{
		//������OGRE���㷨
		//��ѧ�Ƶ�ʽ
		//ret = v + 2*w*uv + 2*uuv + (u��v)u
		//��OGRE�㷨Ϊ
		//ret = v + 2*w*uv + 2*uuv
		//�д��о�����ʱӦ��OGRE�㷨
		//������OGRE��ȷ���ѽ� 
		CVector3f uv, uuv; 
		CVector3f qvec(x, y, z);
		uv = qvec.Cross(cV); 
		uuv = qvec.Cross(uv); 
		uv *= (2.0f * w); 
		uuv *= 2.0f; 

		return cV + uv + uuv;
	}

	_MATH_INLINE_ CQuaternion& CQuaternion::lerp(const CQuaternion& rkP, const CQuaternion& rkQ, float fT )
	{
		lerp( *this , rkP, rkQ, fT );
		return *this;
	}

	//�����ֵ
	_MATH_INLINE_ CQuaternion& CQuaternion::Slerp(const CQuaternion& rkP, const CQuaternion& rkQ, float fT )
	{
		Slerp( *this,rkP,rkQ,fT,true );
		return *this;
	}

	_MATH_INLINE_ CQuaternion& CQuaternion::SlerpExtraSpins ( const CQuaternion& rkP, const CQuaternion& rkQ, int iExtraSpins , float fT)
	{
		SlerpExtraSpins( *this,rkP,rkQ,iExtraSpins,fT );
		return *this;
	}

	// �������淽�̲�ֵ
	_MATH_INLINE_ CQuaternion& CQuaternion::Squad ( const CQuaternion& rkP,
		const CQuaternion& rkA, const CQuaternion& rkB,
		const CQuaternion& rkQ, float fT )
	{
		Squad( *this,rkP,rkA,rkB,rkQ,fT,true );
		return *this;
	}


	//-----��ѧ����------
	_MATH_INLINE_ float CQuaternion::Dot (const CQuaternion& cQ) const
	{ return x*cQ.x+y*cQ.y+z*cQ.z+w*cQ.w; }

	_MATH_INLINE_ float CQuaternion::Norm () const
	{ return x*x+y*y+z*z+w*w; }

	_MATH_INLINE_ float CQuaternion::normalize(void)
	{ 
		float len = Norm();
		*this = *this * CMath::InvSqrt(len);
		return len;
	}

	_MATH_INLINE_ CQuaternion CQuaternion::UnitInverse () const
	{
		//����Ϊ��λ����
		return CQuaternion(w,-x,-y,-z);
	}
};
