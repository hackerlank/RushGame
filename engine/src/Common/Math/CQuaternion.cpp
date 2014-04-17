//-------------------------------------------------------------
// CQuaternion.cpp
// ��Ԫ�� Դ�ļ�
// Ver:1.00
// ����:����
//
// ��������:2007/10/19
//
//-------------------------------------------------------------
#include "stdafx.h"
#include "CQuaternion.h"
#include "CMath.h"
#include "TSqrAllocator.inl"

namespace sqr
{

	const float CQuaternion::ms_fEpsilon = float(1e-03);

	void CQuaternion::FromRotationMatrix(const CMatrix& matRot)
	{
		/*
		����ToRotationMatrix�Ĺ�ʽ�����ǿ��Է�������
		M[1][2] + M[2][1] = 4xy �������Ĺ���
		���ԡ�������Ҫ�ҵ�һ�����ľ���ֵȥ����������ֵ
		����������һ���������ľ���ֵ������1/2
		*/

		float fTrace = matRot.m[0][0]+matRot.m[1][1]+matRot.m[2][2];
		float fRoot;

		if ( fTrace > 0.0 )
		{
			// |w| > 1/2
			fRoot = CMath::Sqrt(fTrace + 1.0f);  // 2w
			w = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;  // 1/(4w)
			x = (matRot.m[1][2]-matRot.m[2][1])*fRoot;
			y = (matRot.m[2][0]-matRot.m[0][2])*fRoot;
			z = (matRot.m[0][1]-matRot.m[1][0])*fRoot;
		}
		else
		{
			// |w| <= 1/2 ��wС��1/2��Ҫ�ȼ���x,y��z
			// �磺vi = sqrt(M[i][i] - M[j][j] - M[k][k] + 1)/2
			static int s_iNext[3] = { 1, 2, 0 };
			int i = 0;
			if ( matRot.m[1][1] > matRot.m[0][0] )
				i = 1;
			if ( matRot.m[2][2] > matRot.m[i][i] )
				i = 2;
			int j = s_iNext[i];
			int k = s_iNext[j];

			fRoot = CMath::Sqrt(matRot.m[i][i]-matRot.m[j][j]-matRot.m[k][k] + 1.0f);
			float* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;
			w = (matRot.m[j][k]-matRot.m[k][j])*fRoot;
			*apkQuat[j] = (matRot.m[j][i]+matRot.m[i][j])*fRoot;
			*apkQuat[k] = (matRot.m[i][k]+matRot.m[k][i])*fRoot;
		}
	}

	void CQuaternion::ToRotationMatrix(CMatrix& matRot) const
	{
		/*
		���� p'=upu*���
		[ 1-2(y^2 + z^2)    2(xy + wz)     2(xz - wy)   0 ]
		p' =[x y z 1]|   2(xy - wz)    1-2(x^2 + z^2)   2(yz + wz)   0 |
		|   2(xz + wy)      2(yz - wx)   1-2(x^2+y^2)   0 |
		[        0               0              0       1 ]
		*/
		float fTx  = 2.0f*x;
		float fTy  = 2.0f*y;
		float fTz  = 2.0f*z;
		float fTwx = fTx*w;
		float fTwy = fTy*w;
		float fTwz = fTz*w;
		float fTxx = fTx*x;
		float fTxy = fTy*x;
		float fTxz = fTz*x;
		float fTyy = fTy*y;
		float fTyz = fTz*y;
		float fTzz = fTz*z;

		matRot.m[0][0] = 1.0f-(fTyy+fTzz);
		matRot.m[0][1] = fTxy+fTwz;
		matRot.m[0][2] = fTxz-fTwy;
		matRot.m[1][0] = fTxy-fTwz;
		matRot.m[1][1] = 1.0f-(fTxx+fTzz);
		matRot.m[1][2] = fTyz+fTwx;
		matRot.m[2][0] = fTxz+fTwy;
		matRot.m[2][1] = fTyz-fTwx;
		matRot.m[2][2] = 1.0f-(fTxx+fTyy);

		matRot.m[0][3] = 0.0f;
		matRot.m[1][3] = 0.0f;
		matRot.m[2][3] = 0.0f;
		matRot.m[3][0] = 0.0f;
		matRot.m[3][1] = 0.0f;
		matRot.m[3][2] = 0.0f;
		matRot.m[3][3] = 1.0f;
	}

	CQuaternion CQuaternion::Exp () const
	{
		// ��� q(x,y,z) = A*(x*i+y*j+z*k)Ϊ��λ����:
		// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).
		// ���sinA������0���� exp(q) = cos(A)+A*(x*i+y*j+z*k) 
		// ��Ϊlimit(A->0)A/sin(A) = 1
		// sin(A)�Ƿ��������� ȡ����ϵͳ���������ѧ����
		float fAngle = CMath::Sqrt(x*x+y*y+z*z);
		float fSin = CMath::Sin(fAngle);

		CQuaternion kResult;
		kResult.w = CMath::Cos(fAngle);

		if ( 0 != fSin )
		{
			float fCoeff = fSin/fAngle;
			kResult.x = fCoeff*x;
			kResult.y = fCoeff*y;
			kResult.z = fCoeff*z;
		}
		else
		{
			kResult.x = x;
			kResult.y = y;
			kResult.z = z;
		}

		return kResult;
	}


	CQuaternion CQuaternion::Log () const
	{
		//q = con(a) + vsin(a) = exp( va )
		//�˲���ΪExp���������˵�������Exp
		CQuaternion kResult;
		kResult.w = 0.0f;

		if ( CMath::Abs(w) < 1.0f )
		{
			float fAngle = CMath::ACos(w);
			float fSin = CMath::SinTable(fAngle);
			if ( 0.0f!=fSin )
			{
				float fCoeff = fAngle/fSin;
				kResult.x = fCoeff*x;
				kResult.y = fCoeff*y;
				kResult.z = fCoeff*z;
				return kResult;
			}
		}

		kResult.x = x;
		kResult.y = y;
		kResult.z = z;

		return kResult;

	}


	//��ԭ�����β�ֵ
	void CQuaternion::lerp( CQuaternion& outQ, const CQuaternion& rkP,
		const CQuaternion& rkQ, float fT )
	{
		//���β�ֵ��ʽ
		//lerp(t;P,Q) = (1-t)P+tQ = t(Q-P) + P
		//ע��: ���β�ֵ����ֵ,��Ҫ��һ��
		outQ = (1.0f - fT) * rkP + fT * rkQ;
		outQ.normalize();
	}

	// ��ԭ�����������Բ�ֵ
	void CQuaternion::Slerp( CQuaternion& outQ, const CQuaternion& rkP,
		const CQuaternion& rkQ, float fT, bool shortestPath)
	{
		//�����ֵ��ʽ
		//Slerp(t;P,Q) = [Psin(a(1-t)) + Qsin(at)]/sin(a)
		//�������β�ֵ��ֵ������Ҫ��һ��
		float fCos = rkP.Dot(rkQ);
		CQuaternion rkT;

		// Do we need to invert rotation?
		if (fCos < 0.0f && shortestPath)
		{
			fCos = -fCos;
			rkT = -rkQ;
		}
		else
		{
			rkT = rkQ;
		}

		if (CMath::Abs(fCos) < 1 - ms_fEpsilon)
		{
			// Standard case (slerp)
			float fSin = CMath::Sqrt(1 - CMath::Sqr(fCos));
			float fAngle = CMath::ATan2(fSin, fCos);
			float fInvSin = 1.0f / fSin;
			float fCoeff0 = CMath::Sin((1.0f - fT) * fAngle) * fInvSin;
			float fCoeff1 = CMath::Sin(fT * fAngle) * fInvSin;
			outQ = fCoeff0 * rkP + fCoeff1 * rkT;
			return;
		}
		else
		{
			outQ = (1.0f - fT) * rkP + fT * rkT;
			outQ.normalize();
			return;
		}

	}

	//����Ķ�����ת
	void CQuaternion::SlerpExtraSpins ( CQuaternion& outQ, const CQuaternion& rkP, const CQuaternion& rkQ,
		int iExtraSpins , float fT)
	{
		float fCos = rkP.Dot(rkQ);
		float fAngle ( CMath::ACos(fCos) );

		if ( CMath::Abs(fAngle) < ms_fEpsilon )
		{
			outQ = rkP;
			return;
		}

		float fSin = CMath::Sin(fAngle);
		float fPhase ( CMath::pi*iExtraSpins*fT );
		float fInvSin = 1.0f/fSin;
		float fCoeff0 = CMath::Sin((1.0f-fT)*fAngle - fPhase)*fInvSin;
		float fCoeff1 = CMath::Sin(fT*fAngle + fPhase)*fInvSin;
		outQ = fCoeff0*rkP + fCoeff1*rkQ;
	}

	// ��������η��̲�ֵ
	void CQuaternion::Intermediate (const CQuaternion& rkQ0, const CQuaternion& rkQ1, const CQuaternion& rkQ2,
		CQuaternion& rkA, CQuaternion& rkB)
	{
		//// assert:  q0, q1, q2 Ϊ��λ4Ԫ��
		CQuaternion rkP0 = rkQ1*rkQ0.UnitInverse();
		CQuaternion rkP1 = rkQ2*rkQ1.UnitInverse();
		CQuaternion kArg = 0.25*(rkP0.Log()-rkP1.Log());
		CQuaternion kMinusArg = -kArg;

		rkA = rkQ1*kArg.Exp();
		rkB = rkQ1*kMinusArg.Exp();

	}

	// ���β�ֵ
	void CQuaternion::Squad ( CQuaternion& outQ, const CQuaternion& rkP,
		const CQuaternion& rkA, const CQuaternion& rkB, const CQuaternion& rkQ, float fT, bool shortestPath)
	{
		CQuaternion rKAA;
		CQuaternion rKBB;
		Slerp(rKAA, rkP, rkQ, fT, shortestPath);
		Slerp(rKBB, rkA, rkB, fT);
		Slerp( outQ,rKAA,rKBB,float(2.0*fT*(1.0-fT)));
	}

	CQuaternion CQuaternion::operator +( const CQuaternion &cQ ) const
	{
		CQuaternion rQ;
#ifdef SIMD
		__asm
		{
			mov esi,this
				mov edi,cQ
				movups xmm0,[esi]
			movups xmm1,[edi]
			addps xmm0,xmm1
				movups rQ,xmm0
		}
#else
		rQ.x=x+cQ.x; rQ.y=y+cQ.y; rQ.z=z+cQ.z; rQ.w=w+cQ.w;
#endif
		return rQ;
	}

	CQuaternion CQuaternion::operator -( const CQuaternion &cQ ) const
	{
		CQuaternion rQ;
#ifdef SIMD
		__asm
		{
			mov esi,this
				mov edi,cQ
				movups xmm0,[esi]
			movups xmm1,[edi]
			subps xmm0,xmm1
				movups rQ,xmm0
		}
#else
		rQ.x=x-cQ.x; rQ.y=y-cQ.y; rQ.z=z-cQ.z; rQ.w=w-cQ.w;
#endif 
		return rQ;
	}

	CQuaternion CQuaternion::operator *( const float cr ) const
	{	
#ifdef SIMD
		CQuaternion rQ;
		__asm
		{
			mov esi,this

				movups xmm0,[esi]
			movss  xmm1,cr
				shufps xmm1,xmm1,0x0
				mulps  xmm0,xmm1
				movups rQ,xmm0
		}
		return rQ;
#else
		return CQuaternion( w*cr,x*cr, y*cr, z*cr ); 
#endif
	}
	CQuaternion operator* (float cr, const CQuaternion& cQ)
	{ return	cQ*cr; }

	CQuaternion CQuaternion::operator *( const CQuaternion& cQ ) const
	{
		CQuaternion rQ;
#if 0//def SIMD
		__asm
		{
			mov esi,this
				mov edi,cQ

				movups xmm0,[esi]
			movups xmm1,xmm0
				movups xmm2,xmm0
				movups xmm3,xmm0 

				movups xmm4,[edi]
			movups xmm5,xmm4
				movups xmm6,xmm4
				movups xmm7,xmm4

				shufps xmm0,xmm3,0x0	//0xFF
				shufps xmm1,xmm3,0xE5	//0x24
				shufps xmm2,xmm3,0x7A	//0x49
				shufps xmm3,xmm3,0x9F	//0x92

				shufps xmm5,xmm4,0x1	//0x3F
				shufps xmm6,xmm4,0x9E	//0x53
				shufps xmm7,xmm4,0x7B	//0x89

				mulps xmm0,xmm4
				mulps xmm1,xmm5
				mulps xmm2,xmm6
				mulps xmm3,xmm7

				movss xmm4,xmm1 
				subss xmm1,xmm4   
				subss xmm1,xmm4   

				movss xmm4,xmm2 
				subss xmm2,xmm4   
				subss xmm2,xmm4   

				addps xmm0,xmm1
				addps xmm0,xmm2
				subps xmm0,xmm3

				movups rQ,xmm0
		}
#else
		/*
		rQ.w=w * cQ.w - x * cQ.x - y * cQ.y - z * cQ.z;
		rQ.x=w * cQ.x + x * cQ.w + y * cQ.z - z * cQ.y; 
		rQ.y=w * cQ.y + y * cQ.w + z * cQ.x - x * cQ.z; 
		rQ.z=w * cQ.z + z * cQ.w + x * cQ.y - y * cQ.x; 
		*/
		rQ.w=cQ.w * w - cQ.x * x - cQ.y * y - cQ.z * z;
		rQ.x=cQ.w * x + cQ.x * w + cQ.y * z - cQ.z * y; 
		rQ.y=cQ.w * y + cQ.y * w + cQ.z * x - cQ.x * z; 
		rQ.z=cQ.w * z + cQ.z * w + cQ.x * y - cQ.y * x;

#endif 
		return rQ;
	}

	CQuaternion& CQuaternion::operator *=( const float cr )
	{

#ifdef SIMD
		CQuaternion rQ;
		__asm
		{
			mov esi,this
				movups xmm0,[esi]
			movss  xmm1,cr
				shufps xmm1,xmm1,0x0
				mulps  xmm0,xmm1
				movups rQ,xmm0
		};
#else
		CQuaternion rQ( w*cr,x*cr, y*cr, z*cr );
#endif
		x = rQ.x; y = rQ.y; z = rQ.z; w = rQ.w;
		return *this;		
	}

	CQuaternion& CQuaternion::operator *=( const CQuaternion& cQ )
	{
		CQuaternion rQ;
#if 0//def SIMD
		__asm
		{
			mov esi,this
				mov edi,cQ

				movups xmm0,[esi]
			movups xmm1,xmm0
				movups xmm2,xmm0
				movups xmm3,xmm0

				movups xmm4,[edi]
			movups xmm5,xmm4
				movups xmm6,xmm4
				movups xmm7,xmm4

				shufps xmm0,xmm3,0x0	//0xFF
				shufps xmm1,xmm3,0xE5	//0x24
				shufps xmm2,xmm3,0x7A	//0x49
				shufps xmm3,xmm3,0x9F	//0x92

				shufps xmm5,xmm4,0x1	//0x3F
				shufps xmm6,xmm4,0x9E	//0x53
				shufps xmm7,xmm4,0x7B	//0x89

				mulps xmm0,xmm4
				mulps xmm1,xmm5
				mulps xmm2,xmm6
				mulps xmm3,xmm7

				movss xmm4,xmm1 
				subss xmm1,xmm4   
				subss xmm1,xmm4   

				movss xmm4,xmm2 
				subss xmm2,xmm4   
				subss xmm2,xmm4   

				addps xmm0,xmm1
				addps xmm0,xmm2
				subps xmm0,xmm3
				movups rQ,xmm0
		}
#else
		/*
		rQ.w=w * cQ.w - x * cQ.x - y * cQ.y - z * cQ.z;
		rQ.x=w * cQ.x + x * cQ.w + y * cQ.z - z * cQ.y; 
		rQ.y=w * cQ.y + y * cQ.w + z * cQ.x - x * cQ.z; 
		rQ.z=w * cQ.z + z * cQ.w + x * cQ.y - y * cQ.x; 
		*/
		rQ.w=cQ.w * w - cQ.x * x - cQ.y * y - cQ.z * z;
		rQ.x=cQ.w * x + cQ.x * w + cQ.y * z - cQ.z * y; 
		rQ.y=cQ.w * y + cQ.y * w + cQ.z * x - cQ.x * z; 
		rQ.z=cQ.w * z + cQ.z * w + cQ.x * y - cQ.y * x;

#endif 
		x = rQ.x; y = rQ.y; z = rQ.z; w = rQ.w;
		return *this;	
	}

	CQuaternion CQuaternion::Inverse () const
	{
		float InvNorm = Norm();
		if( InvNorm  > 0.0 )
		{	
			InvNorm = float(1.0/InvNorm);
#ifdef SIMD
			CQuaternion rQ;
			__asm
			{
				mov esi,this

					movups xmm0,[esi]
				subps  xmm1,xmm1
					movss  xmm1,xmm0
					subps  xmm0,xmm1
					subps  xmm1,xmm0

					movss  xmm0,InvNorm
					shufps xmm0,xmm0,0x0

					mulps  xmm0,xmm1
					movups  rQ,xmm0
			}
			return rQ;
#else
			return CQuaternion(w*InvNorm,-x*InvNorm,-y*InvNorm,-z*InvNorm);
#endif
		}
		else
			return CQuaternion( 0,0,0,0 );
	}
}

