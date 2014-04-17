#pragma once
//******************************************************************************
/*! \file       BaseMath.h
*   \brief      ��������ѧ����
*
*				�����ϰ��� ��������ع��ܣ����ȽǶ���ع��ܣ��������ع��ܣ�
*				ʸ����ع��ܣ���ѧ�����Ķ��壬�������ģ���еĹ��ܱ������
*				Math.lib
*   \version	1.0
*   \date
*   \sa			CVector3, KLELine, KLEPlane, KLEQuaternion
*   \todo		��Ҫ�Ż����ٶ��ǵ�һλ��
*******************************************************************************/

#include <cmath>
#include <cstdlib>
#include "BaseTypes.h"

namespace sqr
{
using namespace std;
//-----------------------------------------------------------------------------
// Useful Math constants
//-----------------------------------------------------------------------------
#define		MATH_PI				3.14159265358979323846		// Pi
#define		MATH_2_PI			6.28318530717958623200		// 2 * Pi
#define		MATH_PI_DIV_2		1.57079632679489655800		// Pi / 2
#define		MATH_PI_DIV_4		0.78539816339744827900		// Pi / 4
#define		MATH_PI_DIV_8		0.39269908169872415480		// Pi / 8
#define		MATH_PI_DIV_16		0.19634954084936207740		// Pi / 16


#define		MATH_INV_PI			0.31830988618379069122		// 1 / Pi
#define		MATH_DEG2RAD		0.01745329251994329547		// Degrees to Radians
#define		MATH_RAD2DEG		57.29577951308232286465		// Radians to Degrees
#define		MATH_FLOATHUGE		1.0e+38f					// Huge number for float
#define		MATH_EPSILON		1.0e-5f						// Tolerance for float


//******************************************************************************
/*! \fn     inline bool gIsZero( float a, float fTol = MATH_EPSILON)
*   \brief  �ж�һ���������Ƿ������
*
*   \param  float a		Ҫ�жϵĸ�����
*   \param  float fTol	���ȣ�ȱʡΪMATH_EPSILON
*   \return bool		�����㷵��true, ���򷵻�false
*   \sa     gIsEqual
*******************************************************************************/
inline bool gIsZero( float a, float fTol = MATH_EPSILON)
{
	return ( a <= 0.0f ) ? ( a >= -fTol ) : ( a <= fTol );
}

//******************************************************************************
/*! \fn     inline bool gIsEqual(float a, float b, float fTol = MATH_EPSILON)
*   \brief  �ж������������Ƿ����
*
*   \param  float a		Ҫ�Ƚϵĸ�����
*   \param  float b		Ҫ�Ƚϵĸ�����
*   \param  float fTol 	���ȣ�ȱʡΪMATH_EPSILON
*   \return bool		��ȷ���true, ���򷵻�false
*   \sa     gIsZero
*******************************************************************************/
inline bool gIsEqual(float a, float b, float /*fTol*/ = MATH_EPSILON)
{
	if ((float)fabs((float)(a - b)) > MATH_EPSILON)
		return false;
	else
		return true;
}

//******************************************************************************
/*! \fn     inline int gRandRage(int nMin, int nMax)
*   \brief  ����һ����Χ�ڵ������,����nMin �� nMax
*
*   \param  int nMin	��Χ���½�
*   \param  int nMax	��Χ���Ͻ�
*   \return int			��õ������
*   \sa     gRandRage(int nMin, int nMax, int nExclude)
*******************************************************************************/
inline int gRandRage(int nMin, int nMax)
{
	int nSwap;
	if (nMin > nMax)
	{
		nSwap	= nMax;
		nMax	= nMin;
		nMin	= nSwap;
	}

	return nMin + rand()%(nMax - nMin + 1);
}

//!
//!
//******************************************************************************
/*! \fn     inline int gRandRage(int nMin, int nMax, int nExclude)
*   \brief  ����һ����Χ�ڵ������, ����nMin �� nMax.�������� nExclude,
*			nExclude ������nMin, nMax��
*
*   \param  int nMin		��Χ���½�
*   \param  int nMax		��Χ���Ͻ�
*   \param  int nExclude	��������ֵ
*   \return inline int		��õ������
*   \sa     gRandRage(int nMin, int nMax)
*******************************************************************************/
inline int gRandRage(int nMin, int nMax, int nExclude)
{
	int nTemp;

	if (nMin > nMax)
	{
		nTemp	= nMax;
		nMax	= nMin;
		nMin	= nTemp;
	}

	if ( nExclude < nMin || nExclude > nMax)
	{
		nTemp = gRandRage(nMin, nMax);
	}
	else
	{
		nTemp	= gRandRage(nExclude + 1, nMax + nExclude - 1);
		nTemp	= (nTemp > nMax) ? (nTemp - nMax) : nTemp;
	}
	return nTemp;
}

//******************************************************************************
/*! \fn     inline float gRandOneZero()
*   \brief  ����һ����0.0��1.0֮����ȷֲ������������
*
*   \return inline float ���ص����������
*******************************************************************************/
inline float gRandOneZero()
{
	return ((float)rand())/((float)RAND_MAX+1.0f);
}

//******************************************************************************
/*! \fn     inline float gCos(float angle)
*   \brief  ��һ���Ƕ�ֵ������ֵ
*
*   \param  float angle ����ĽǶ�ֵ
*   \return float		��õ�����ֵ
*   \sa     gSin, Deg2Rad, Rad2Deg
*******************************************************************************/
inline float gCos(float angle)
{
	return (float) cos(MATH_PI * ((double) angle / 180));
}


//******************************************************************************
/*! \fn     inline float gSin(float angle)
*   \brief  ��һ���Ƕ�ֵ������ֵ
*
*   \param  float angle		����ĽǶ�ֵ
*   \return float			��õ�����ֵ
*   \sa     gBMCos, gBMDeg2Rad, gBMRad2Deg
*******************************************************************************/
inline float gSin(float angle)
{
	return (float) sin(MATH_PI * ((double) angle / 180));
}

//******************************************************************************
/*! \fn     inline float gDeg2Rad(float fDeg)
*   \brief  �����򻡶�ת��
*
*   \param  float fDeg		Ҫת���ĽǶ�ֵ
*   \return float			ת����Ľ��
*   \sa     gSin, gCos, gRad2Deg
*******************************************************************************/
inline float gDeg2Rad(float fDeg)
{
	return fDeg*(float)MATH_DEG2RAD;
}


//******************************************************************************
/*! \fn     inline float gRad2Deg(float fRad)
*   \brief  ���������ת��
*
*   \param  float fRad		Ҫת���Ļ���ֵ
*   \return inline float	ת����Ľ��
*   \sa     gSin, gCos, gDeg2Rad
*******************************************************************************/
inline float gRad2Deg(float fRad)
{
	return fRad*(float)MATH_RAD2DEG;
}


//******************************************************************************
/*! \fn     inline int32 gLog2(int32 n)
*   \brief  ��������2�Ķ���
*
*   \param  int32 n			Ҫ�������
*   \return inline int32	������2�Ķ���
*******************************************************************************/
inline uint8 gLog2(uint32 n)
{
	uint8 r = 0;
	for ( ; n > 1; n = n>>1 )
		r++;
	return r;
}


//******************************************************************************
/*! \fn     T TriInterpolate( T& T0, T& T1, T& T2, T& T3, float x, float y )
*   \brief  ��T0~T3֮����������β�ֵ
*
*   \param  T& T0, T& T1, T& T2, T& T3		��ֵ����		T1 _______________________ T3 ( x = 1.0f, y = 1.0f )
*			float x, float y				��ֵλ��		|\					  	  |
*															|	 \				  	  |
*															|		  \ 		  	  |
*															|			   \	  	  |
*															|( x = 0, y = 0 )   \	  |
*															T0 ______________________\|T2
*
*
*   \return T								��ֵ���
*******************************************************************************/
template< class T >
T TriInterpolate( const T& T0, const T& T1, const T& T2, const T& T3, float x, float y )
{
	if ( y > 1.0f - x )
	{
		x -= 1.0f;
		y -= 1.0f;
		return (T)( ( T3 - T1 )*x + ( T3 - T2 )*y + T3 );
	}
	else
	{
		return (T)( ( T2 - T0 )*x + ( T1 - T0 )*y + T0 );
	}
}

// ***********************************************************************
// ��������������
// ���� a>=min && a<= max
// ***********************************************************************
template<class T>
void limit( T& a, const T& min, const T& max )
{
	if ( a < min )
		a = min;
	else if ( a > max )
		a = max;
}

template<class T>
T limit2(const T& a, const T& min, const T& max)
{
	if ( a < min)
		return min;
	else if ( a > max)
		return max;

	return a;
}

template<class T>
T limit3(const T& a, const T& min)
{
	return a < min ? min : a;
}

template<class T>
T limit4(const T& a, const T& max)
{
	return a > max ? max : a;
}

}

