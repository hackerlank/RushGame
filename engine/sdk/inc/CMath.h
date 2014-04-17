#ifndef __GMath_H__
#define __GMath_H__

//#include "GMacro.h"
//#include "GCommon.h"
#include <math.h>
#include "TSingleton.h"
#include "CVector3.h"

/*----------------------------------------
ʹ��Single Instruction Multiple Data
ʹ��Intel�Ļ��ָ����Ż�
��Ҫ������Pentium 3����õ�CPU
�����Ҫ ��ȡ�����SIMD���Ż� ȡ�����¶���
������SIMD ��ֻ��ʹ���ٶȽ����ĵ���������������
----------------------------------------*/
#if defined(_WIN32) || defined(__WIN32__)
#define SIMD
//����SIMD���뷽ʽ
#define _SIMD_ALIGNMENT __declspec(align(16)) 
#define _MATH_INLINE_ __forceinline
#else
#define _MATH_INLINE_ inline
#endif


//���廡��
#define TWO_PI	6.2831853071795864769252867665590
#define PI		3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916397
#define QTR_PI	0.7853981633974483096156608458198
#define SQRT_2  1.4142135623730950488016887242096
#define SQRT_3	1.7320508075688772935274463415058

namespace sqr
{

class CMath : public Singleton<CMath>
{
public:

	CMath();
	~CMath();

	//��2*PI��Χ�ڴ���SinTable��TanTable,���ڿ��ٲ�ѯ��������ֵ
	//uint32 trigTableSize�����趨������Ĵ�С����ֵԽ���򣬱�����Խ����ֵԽ��ȷ
	//bool mp�����趨�Ƿ�Ҫ��������
	//������ó�true����ֻ��trigTableSize���ڵ�ǰ����ʱ�Ÿ������Ǻ�����
	//������ó�false��������trigTableSize��󶼻����¸������Ǻ�����
	static void buildTrigTables(uint32 trigTableSize = 4096, bool mp = false);

	static float SinTable( float fRadian );
	static float CosTable( float fRadian );
	static float TanTable( float fRadian );

	static float Sin( float fRadian);
	static float Cos( float fRadian );
	static float Tan( float fRadian);

	static float ACos( float fValue );
	static float ASin( float fValue );
	static inline float ATan2(float fY, float fX) { return float(atan2(fY,fX)); }

	static float ToRadian( float fDegree );
	static float ToDegree( float fRadin );

	//��ѧ����
	static float Abs( float fValue );
	static float Sqr( float fValue );
	static float Sqrt( float fValue );
	static float InvSqrt( float fValue );

	static double Logbase( double a, double base );

	// ��������
	static bool IntersectTri( const CVector3f& v0, const CVector3f& v1, const CVector3f& v2, const CVector3f& vRayOrg, const CVector3f& vRayDir, float& u, float& v, float& d );

protected:
	static uint32 m_stTrigTableSize;
	static float m_stTrigTableFactor;

	static float* m_apSinTable;
	static float* m_apTanTable;

public:
	static const float pi;
	static const float half_pi;
	static const float two_pi;
	static const float qtr_pi;
public:
	static const float sqrt_2;
	static const float sqrt_3;
};

}
#endif //__GMath_H__
