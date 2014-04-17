#pragma once
/******************************************************************************
/*! \file       CMatrix.h
*   \brief      ������Ķ���
*
*				������ֿ��ܵĹ������ķ���,����صľ������
*   \version	1.0
*   \date		01-2-10 9:40:12
*   \sa			CCoord, CVector3f
*   \todo		��û���������Ĳ��Լ�������Ż�
*******************************************************************************/


#include <cmath>
#include <cstring>
#include "CVector3.h"


namespace sqr
{
class CMatrix
{
public:

	union
	{
		struct
		{
			float	_11, _12, _13, _14;
			float	_21, _22, _23, _24;
			float	_31, _32, _33, _34;
			float	_41, _42, _43, _44;
		};
		float	m[4][4];
	};
	/**
	* we're using m[y][x] as our notation.
	*/

	//! ȱʡ���캯��
	CMatrix();
	//! �������캯��
	CMatrix(const CMatrix&	mat);
	//! ��Euler�ǹ������
	CMatrix(float fPitch, float fYaw, float fRoll);
	//! ������ĳ������תĳ���Ƕȵı任����
	CMatrix(const CVector3f& vDir, float fRads);
	//! ��������ʸ����ԭ��(������)(�����������������ϵ)������ת������
	CMatrix(const CVector3f& vX, const CVector3f& vY,
	        const CVector3f& vZ, const CVector3f& vOrg);
	//! ������ʼ����ֹʸ�������ϵ�ʸ���������Ӿ���
	CMatrix(const CVector3f& vFrom, const CVector3f& vAt,
	        const CVector3f& vUp);

	~CMatrix();


	//!�������þ���
	void SetWithRow(const CVector3f& r0,const CVector3f& r1,const CVector3f& r2);
	//!�������þ���
	void SetWithColumn(const CVector3f& c0,const CVector3f& c1,const CVector3f& c2);
	//!  ��ͬһ����
	inline void Identity();
	//! �������
	bool		Invert();
	//! �������
	void		InvertFast();
	//! ��ת�þ���
	void		Transpose();
	//! �������
	CMatrix	operator * (const CMatrix& mat) const;
	//! �����Գ�
	CMatrix&	operator *= (const CMatrix& mat);
	// �����ʸ�����
	const CVector3f operator*( const CVector3f &b);
	//! ����ƫ�ƾ���
	void SetTranslate( float tx, float ty, float tz );
	//! ����ƫ�ƾ���
	void SetTranslate( const CVector3f& v );
	//! �������ž���
	void SetScale( float sx, float sy, float sz );
	//! �������ž���
	void SetScale( const CVector3f& v );

	//! ����һ�����������ϵ(�������������ϵ),����һ��ת������
	//void SetTransform(const CCoord& crd, bool IsWorldToObj = true);

	//! ������ʼ����ֹʸ�������ϵ�ʸ��(�����������������ϵ)������ת������
	// ���ȥ����ֻʹ�õ�һ��
	bool SetTransform( const CVector3f& vFrom,
	                   const CVector3f& vAt,
	                   const CVector3f& vUp );

	//! ��������ʸ��(������)��ԭ��(�����������������ϵ)������ת������
	void SetTransform( const CVector3f& vX,
	                   const CVector3f& vY,
	                   const CVector3f& vZ,
	                   const CVector3f& vOrg);

	//! �����һ������ϵ����һ������ϵ��ת������
	//void SetTransform(const CCoord& crdSrc, const CCoord& crdDes);

	//! ���ݴ���Ĳ�������ͶӰ����
	bool SetProjection( float fFOV = 1.570795f,
	                    float fAspect = 1.0f,
	                    float fNearPlane = 1.0f,
	                    float fFarPlane = 1000.0f );

	//! ������X����תfRads���ȵı任����
	void    SetRotateX( float fRads );
	//! ������Y����תfRads���ȵı任����
	void    SetRotateY( float fRads );
	//! ������Z����תfRads���ȵı任����
	void    SetRotateZ( float fRads );
	//! ����Euler��ת����
	void	SetRotation(float fPitch, float fYaw, float fRoll);
	//! ������ĳ������תĳ�����ȵı任����
	void    SetRotation( const CVector3f& vDir, float fRads );

	const CVector3f GetLoc() const;

	/**
	* GetAxis only returns the correct axis if we're talking about a matrix that
	* transforms 'up' towards worldspace.  For example, this will return the
	* correct axis if called on a object->world matrix, but incorrect one if
	* called on the world->camera matrix.  (If one needs the axes for such a
	* matrix, invert the world->camera transform to get a camera->world
	* transform)
	*/
	CVector3f GetAxis( int axis ) const;

protected:


};

//! ȱʡ���캯��
inline CMatrix::CMatrix()
		: _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f)
		, _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f)
		, _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f)
		, _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
{
	//Identity();
}

//! ��������
inline CMatrix::~CMatrix()
{

}

//! �������캯��
inline CMatrix::CMatrix(const CMatrix&	mat)
{
	memcpy(this, &mat, sizeof(CMatrix));
}

//******************************************************************************
/*! \fn     CMatrix::CMatrix(float fPitch, float fYaw, float fRoll)
*   \brief  ��Euler�ǹ�����󣬲ο�SetRotation
*   \param  float fPitch	�����x����ת�Ļ���
*   \param  float fYaw		�����y����ת�Ļ���
*   \param  float fRoll		�����z����ת�Ļ���
*   \sa     SetRotation(float fPitch, float fYaw, float fRoll)
*******************************************************************************/
inline CMatrix::CMatrix(float fPitch, float fYaw, float fRoll)
{
	SetRotation(fPitch, fYaw, fRoll);
}

//******************************************************************************
/*! \fn     CMatrix::CMatrix(const CVector3f& vDir, float fRads)
*   \brief   ������ĳ������תĳ���Ƕȵı任����,�ο�SetRotation
*
*   \param  const CVector3f&	vDir	��ʾ�ռ��е�һ����
*   \param  float				fRads	��ת�Ļ���ֵ
*   \sa     SetRotion(const CVector3f& vDir, float fRads)
*******************************************************************************/
inline CMatrix::CMatrix(const CVector3f& vDir, float fRads)
{
	SetRotation(vDir, fRads);
}

//******************************************************************************
/*! \fn     CMatrix::CMatrix(const CVector3f& vX, const CVector3f& vY,
*					const CVector3f& vZ, const CVector3f& vOrg)
*   \brief  ��������ʸ����ԭ��(������)�����Ӿ���, �ο�SetView
*
*			�����ӿռ�������������ԭ���γ�����ռ䵽���ӿռ��4*4�任����
*   \param  const CVector3f& vX	�ӿռ��x��
*   \param  const CVector3f& vY	�ӿռ��y��
*   \param  const CVector3f& vZ	�ӿռ��z��
*   \param  const CVector3f& vOrg	�ӿռ��ԭ��
*   \sa     SetView
*******************************************************************************/
inline CMatrix::CMatrix(const CVector3f& vX, const CVector3f& vY,
                        const CVector3f& vZ, const CVector3f& vOrg)
{
	SetTransform(vX, vY, vZ, vOrg);
}

//******************************************************************************
/*! \fn     CMatrix::CMatrix(const CVector3f& vFrom, const CVector3f& vAt,
*									const CVector3f& vUp)
*   \brief  ������ʼ����ֹʸ�������ϵ�ʸ���������Ӿ���, �ο�SetView
*
*   \param  const CVector3f& vFrom		��ʼʸ��
*   \param  const CVector3f& vAt		��ֹʸ��
*   \param  const CVector3f& vUp		���ϵ�ʸ��
*   \sa     SetView
*******************************************************************************/
inline CMatrix::CMatrix(const CVector3f& vFrom, const CVector3f& vAt,
                        const CVector3f& vUp)
{
	SetTransform(vFrom, vAt, vUp);
}


//******************************************************************************
/*! \fn     void CMatrix::SetWithRow(const CVector3f& r0,const CVector3f& r1,const CVector3f& r2)
*   \brief  �������þ���
*
*   \param  const CVector3f& r0 ��һ��
*   \param  const CVector3f& r1 �ڶ���
*   \param  const CVector3f& r2 ������
*   \return ��
*******************************************************************************/
inline void CMatrix::SetWithRow(const CVector3f& r0,const CVector3f& r1,const CVector3f& r2)
{
	_11 = r0.x,_12 = r0.y,_13 = r0.z,_14 = 0;
	_21 = r1.x,_22 = r1.y,_23 = r1.z,_24 = 0;
	_31 = r2.x,_32 = r2.y,_33 = r2.z,_34 = 0;
	_41 = 0,   _42 = 0,   _43 = 0,   _44 = 1;
}

//******************************************************************************
/*! \fn     void CMatrix::SetWithColumn(const CVector3f& c0,const CVector3f& c1,const CVector3f& c2);
*   \brief  �������þ���
*
*   \param  const CVector3f& c0 ��һ��
*   \param  const CVector3f& c1 �ڶ���
*   \param  const CVector3f& c2 ������
*   \return ��
*******************************************************************************/
inline void CMatrix::SetWithColumn(const CVector3f& c0,const CVector3f& c1,const CVector3f& c2)
{
	_11 = c0.x,_12 = c1.x,_13 = c2.x,_14 = 0;
	_21 = c0.y,_22 = c1.y,_23 = c2.y,_24 = 0;
	_31 = c0.z,_32 = c1.z,_33 = c2.z,_34 = 0;
	_41 = 0,   _42 = 0,   _43 = 0,   _44 = 1;
}

//******************************************************************************
/*! \fn     void CMatrix::Identity()
*   \brief  ����ͬһ����
*
*           ͬһ����Խ�Ϊһ,������ͬһ������˺󲻻�ı�
*   \return void
*******************************************************************************/
inline void CMatrix::Identity()
{
	_12 = _13 = _14 = _21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}


//******************************************************************************
/*! \fn     void CMatrix::SetTranslate( float tx, float ty, float tz )
*   \brief  ����ƫ�ƾ���
*   \param		float tx x�����ϵ�ƫ��
*   \param		float ty y�����ϵ�ƫ��
*   \param		float tz z�����ϵ�ƫ��
*   \return void
*******************************************************************************/
inline void CMatrix::SetTranslate( float tx, float ty, float tz )
{
	Identity();
	_41 = tx;
	_42 = ty;
	_43 = tz;
}

//******************************************************************************
/*! \fn     void CMatrix::SetTranslate( const CVector3f& v )
*   \brief  ����ƫ�ƾ���
*   \param  const CVector3f& v ����3�������ϵ�ƫ��
*   \return void
*******************************************************************************/
inline void CMatrix::SetTranslate( const CVector3f& v )
{
	SetTranslate( v.x, v.y, v.z );
}

//******************************************************************************
/*! \fn     void CMatrix::SetScale( float sx, float sy, float sz )
*   \brief  �������ž���
*   \param   float sx	x�����ϵ����ű���
*   \param   float sy	y�����ϵ����ű���
*   \param   float sz	z�����ϵ����ű���
*   \return void
*******************************************************************************/
inline void CMatrix::SetScale( float sx, float sy, float sz )
{
	Identity( );
	_11 = sx;
	_22 = sy;
	_33 = sz;
}

//******************************************************************************
/*! \fn     void CMatrix::SetScale( const CVector3f& v )
*   \brief  �������ž���
*           ��ϸ����
*   \param  const CVector3f& v ����3�������ϵ����ű���
*   \return void
*******************************************************************************/
inline void CMatrix::SetScale( const CVector3f& v )
{
	SetScale( v.x, v.y, v.z );
}


//******************************************************************************
/*! \fn     void SetTransform(const CCoord& crd, bool IsWorldToObj = true)
*   \brief  ����һ�����������ϵ(�������������ϵ),����һ��ת������
*
*           ����һ�����������ϵ(�������������ϵ),����һ��ת������,�ڶ�������
*			Ϊtrue ������������������ϵ��������ϵ��ת������, Ϊfasle����������
*			������ϵ����������ϵ��ת������
*   \param  const CCoord& crd[in]		���������ϵ
*   \param  bool IsWorldToObj[in]		ȱʡΪtrue, ������������������ϵ��������
*										ϵ��ת������,Ϊfasle���������ɴ�����ϵ��
*										��������ϵ��ת������
*******************************************************************************/
/*
inline void CMatrix::SetTransform(const CCoord& crd, bool IsWorldToObj)
{
// Start building the matrix. The first three rows contains the basis
// vectors used to rotate the view to point at the lookat point
Identity( );
_11 = crd.GetXAxis().x;    _12 = crd.GetYAxis().x;    _13 = crd.GetZAxis().x;
_21 = crd.GetXAxis().y;    _22 = crd.GetYAxis().y;    _23 = crd.GetZAxis().y;
_31 = crd.GetXAxis().z;    _32 = crd.GetYAxis().z;    _33 = crd.GetZAxis().z;

// Do the translation values (rotations are still about the eyepoint)
_41 = - crd.GetOrg().Dot( crd.GetXAxis() );
_42 = - crd.GetOrg().Dot( crd.GetYAxis() );
_43 = - crd.GetOrg().Dot( crd.GetZAxis() );

if (!IsWorldToObj)
{
Invert();
}
}
*/

//******************************************************************************
/*! \fn     void CMatrix::SetView( const CVector3f& vX, const CVector3f& vY,
const CVector3f& vZ, const CVector3f& vOrg)
*   \brief  ��������ʸ��(������)��ԭ��(�����������������ϵ)������ת������
*
*			��������ʸ��(������)��ԭ��(�����������������ϵ)������һ������������
*			ϵ��������ϵ��ת������.һ�����õ�Ӧ���Ǹ����ӿռ�������������ԭ��
*			�γ�����ռ䵽���ӿռ��4*4�任����,
*   \param   const CVector3f& vX	�������x��
*   \param   const CVector3f& vY	�������y��
*   \param   const CVector3f& vZ	�������z��
*   \param   const CVector3f& vOrg �������ԭ��
*   \return void
*	\todo	û�д����⣬��������ʸ���Ƿ�Ҫ��λ��
*******************************************************************************/
inline void CMatrix::SetTransform( const CVector3f& vX, const CVector3f& vY,
                                   const CVector3f& vZ, const CVector3f& vOrg)
{
	// Start building the matrix. The first three rows contains the basis
	// vectors used to rotate the view to point at the lookat point
	_11 = vX.x;
	_12 = vY.x;
	_13 = vZ.x;
	_21 = vX.y;
	_22 = vY.y;
	_23 = vZ.y;
	_31 = vX.z;
	_32 = vY.z;
	_33 = vZ.z;

	// Do the translation values (rotations are still about the eyepoint)
	_41 = - vOrg.Dot( vX );
	_42 = - vOrg.Dot( vY );
	_43 = - vOrg.Dot( vZ );

	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;
}

//******************************************************************************
/*! \fn     bool CMatrix::SetView( const CVector3f& vFrom, const CVector3f& vAt,
const CVector3f& vUpAssumptive )
*   \brief  ��������ʸ����ԭ��(������)�����Ӿ���
*
*           ���ݸ�����Eye point, LooKAt point, ��һ�����ϵ�ʸ��ȷ��һ��4*4���Ӿ���
*			һ������ռ��ʸ�������������ͻ�任������ӿռ�
*   \param  const CVector3f& vFrom			Camera ����ʼ��
*   \param  const CVector3f& vAt			Camera ����ֹ��
*   \param  const CVector3f& vUpAssumptive ���ϵ�����
*   \return bool true �ɹ��� false ʧ��
*   \sa     �ο�����
*******************************************************************************/
inline bool CMatrix::SetTransform( const CVector3f& vFrom, const CVector3f& vAt,
                                   const CVector3f& vUpAssumptive )
{
	float	fLen;

	// �õ�z��
	CVector3f vView = vAt - vFrom;
	vView.Normalize();

	// ͨ���������Up(y)ʸ���� View(z)ʸ����ͶӰ,�ټ���������(View)z�ഹֱ
	// ��Up(y)ʸ��.��һ��ͼ�������ļ�������
	float fDotProduct = vUpAssumptive.Dot(vView);
	CVector3f vUp = vUpAssumptive - vView * fDotProduct;
	fLen = vUp.Mag();

	// ������Up(y)ʸ��̫��,��ʹ��ȱʡʸ������
	// ��Ϊ vView, (0.0f, 1.0f, 0.0f)���ǵ�λʸ��,����֤��(0.0f, 1.0f, 0.0f)
	// �� vView��ͶӰ�͵���vView.y(�� vView��(0.0f, 1.0f, 0.0f)��ͶӰ),��������
	// ���������ͬ.
	if ( gIsZero(fLen) )
	{
		vUp = CVector3f( 0.0f, 1.0f, 0.0f ) - vView * vView.y;
		fLen = vUp.Mag();
		if ( gIsZero(fLen) )
		{
			vUp = CVector3f( 0.0f, 0.0f, 1.0f ) - vView * vView.z;
			fLen = vUp.Mag();
			if ( gIsZero(fLen) )
			{
				//Ast(false);
				return false;
			}
		}
	}

	// ��Up(y)ʸ����λ��
	vUp /= fLen;

	// ͨ�� vUp(Y)ʸ�� ��� vView(Z)ʸ���õ� vRight(X)ʸ��,(ע����Up��ǰ)
	// ������λʸ���Ĳ�����ǵ�λʸ��
	CVector3f vRight = vUp.Cross( vView );

	SetTransform(vRight, vUp, vView, vFrom);

	return true;
}

//******************************************************************************
/*! \fn     void SetTransform(const CCoord& crdSrc, const CCoord& crdDes)
*   \brief  �����һ������ϵ����һ������ϵ��ת������
*
*           �����һ������ϵ����һ������ϵ��ת������,��������ϵ�Ķ����������
*			����ϵ��,������ľ�����ɴ�Src����ϵ��Des����ϵ��ת��
*   \param  const CCoord& crdSrc Դ����ϵ
*   \param  const CCoord& crdDes Ŀ������ϵ
*   \return void
*   \sa     CCoord
*******************************************************************************/
/*
void CMatrix::SetTransform(const CCoord& crdSrc, const CCoord& crdDes)
{
CVector3f xSrc,ySrc,zSrc,oSrc;
CVector3f xDes,yDes,zDes,oDes;
CMatrix mSrc,mDes;

xSrc = crdSrc.GetXAxis();
ySrc = crdSrc.GetYAxis();
zSrc = crdSrc.GetZAxis();
oSrc = crdSrc.GetOrg();
xDes = crdDes.GetXAxis();
yDes = crdDes.GetYAxis();
zDes = crdDes.GetZAxis();
oDes = crdDes.GetOrg();

mSrc.SetTransform(xSrc,ySrc,zSrc,oSrc);
mDes.SetTransform(xDes,yDes,zDes,oDes);

mSrc.Invert();
mSrc *= mDes;

*this = mSrc;
}
*/

//******************************************************************************
/*! \fn     bool CMatrix::SetProjection(float fFOV,float fAspect,
float fNearPlane,float fFarPlane)
*   \brief  ����ͶӰ����
*   \param  float fFOV			= 1.570795f		Y���ϵ����� field-of-view,����
*   \param  float fAspect		= 1.3333f		x/y ��ı���
*   \param  float fNearPlane	= 1.0f			��ƽ��
*   \param  float fFarPlane		= 1000.0f		Զƽ��
*   \return bool
*******************************************************************************/
inline bool CMatrix::SetProjection(float fFOV, float fAspect, float fNearPlane,
                                   float fFarPlane)
{
	if ( gIsEqual(fNearPlane, fFarPlane, 0.01f) )
		return false;

	float fctg = tanf( fFOV/2 );
	if ( gIsZero( fctg, 0.01f ) )
		return false;

	float h = 1.0f/fctg;
	float w = h/fAspect;
	float Q = fFarPlane / ( fFarPlane - fNearPlane );

	memset( this, 0, sizeof(CMatrix) );
	_11 = w;
	_22 = h;
	_33 = Q;
	_34 = 1.0f;
	_43 = -Q*fNearPlane;

	return true;
}

//******************************************************************************
/*! \fn     void CMatrix::SetRotateX( float fRads )
*   \brief  ������X����תfRads���ȵı任����
*   \param  float fRads		��ת�Ļ���ֵ
*******************************************************************************/
inline void CMatrix::SetRotateX( float fRads )
{
	Identity( );

	float c = cosf(fRads);
	float s = sinf( fRads );

	_22 =  c;
	_23 =  s;
	_32 = -s;
	_33 =  c;
}

//******************************************************************************
/*! \fn     void CMatrix::SetRotateY( float fRads )
*   \brief  ������Y����תfRads���ȵı任����
*   \param  float fRads ��ת�Ļ���ֵ
*******************************************************************************/
inline void CMatrix::SetRotateY( float fRads )
{
	Identity( );

	float c = cosf(fRads);
	float s = sinf( fRads );

	_11 =  c;
	_13 = -s;
	_31 =  s;
	_33 =  c;
}

//******************************************************************************
/*! \fn     void CMatrix::SetRotateZ( float fRads )
*   \brief  ������Z����תfRads���ȵı任����
*   \param  float fRads ��ת�Ļ���ֵ
*******************************************************************************/
inline void CMatrix::SetRotateZ( float fRads )
{
	Identity( );

	float c = cosf(fRads);
	float s = sinf( fRads );

	_11  =  c;
	_12  =  s;
	_21  = -s;
	_22  =  c;
}

//******************************************************************************
/*! \fn     void CMatrix::SetRotation(float fPitch, float fYaw, float fRoll)
*   \brief  ����Euler��ת����
*           ��ת��˳���� x, y z,Ӧ�ÿ���ֱ�ӹ���
*   \param  float fPitch	�����x����ת�Ļ���
*   \param  float fYaw		�����y����ת�Ļ���
*   \param  float fRoll		�����z����ת�Ļ���
*******************************************************************************/
inline void CMatrix::SetRotation(float fPitch, float fYaw, float fRoll)
{
	CMatrix	matTemp;

	Identity();

	matTemp.SetRotateX(fPitch);
	*this *= matTemp;
	matTemp.SetRotateY(fYaw);
	*this *= matTemp;
	matTemp.SetRotateZ(fRoll);
	*this *= matTemp;

	/*
	int i;
	float sinangx,cosangx,sinangy,cosangy,sinangz,cosangz;
	float xold,yold,zold,xnew,ynew,znew;//temporary rotation results to prevent round off error
	sinangx=(float)sin(yaw);
	cosangx=(float)cos(yaw);
	sinangy=(float)sin(pitch);
	cosangy=(float)cos(pitch);
	sinangz=(float)sin(roll);
	cosangz=(float)cos(roll);
	for(i=0; i<3; i++)
	{
	//init points
	xold=m[i][0];
	yold=m[i][1];
	zold=m[i][2];
	//rotate point locally
	//xrot
	xnew=xold;
	ynew=(yold*cosangx)-(zold*sinangx);
	znew=(yold*sinangx)+(zold*cosangx);
	xold=xnew;
	yold=ynew;
	zold=znew;
	//yrot
	xnew=((xold*cosangy)+(zold*sinangy));
	ynew=yold;
	znew=((-1*(xold*sinangy))+(zold*cosangy));
	xold=xnew;
	yold=ynew;
	zold=znew;
	//zrot
	xnew=((xold*cosangz)-(yold*sinangz));
	ynew=((yold*cosangz)+(xold*sinangz));
	znew=zold;
	//change object location
	m[i][0]=xnew;
	m[i][1]=ynew;
	m[i][2]=znew;
	}
	*/
}

//******************************************************************************
/*! \fn     void CMatrix::SetRotation( const CVector3f& vDir, float fRads )
*   \brief  ������ĳ������תĳ�����ȵı任����
*   \param   const CVector3f& vDir ��ʾ�ռ��е�һ����
*   \param  float fRads				��ת�Ļ���ֵ(�����������ԭ�㿴ȥ����ʱ��Ϊ��)
*   \return void
*******************************************************************************/
inline void CMatrix::SetRotation( const CVector3f& vDir, float fRads )
{
	float		fCos	= cosf( fRads );
	float		fSin	= sinf( fRads );
	CVector3f	v		= vDir;
	v.Normalize();

	_11 = ( v.x * v.x ) * ( 1.0f - fCos ) + fCos;
	_12 = ( v.x * v.y ) * ( 1.0f - fCos ) - (v.z * fSin);
	_13 = ( v.x * v.z ) * ( 1.0f - fCos ) + (v.y * fSin);

	_21 = ( v.y * v.x ) * ( 1.0f - fCos ) + (v.z * fSin);
	_22 = ( v.y * v.y ) * ( 1.0f - fCos ) + fCos ;
	_23 = ( v.y * v.z ) * ( 1.0f - fCos ) - (v.x * fSin);

	_31 = ( v.z * v.x ) * ( 1.0f - fCos ) - (v.y * fSin);
	_32 = ( v.z * v.y ) * ( 1.0f - fCos ) + (v.x * fSin);
	_33 = ( v.z * v.z ) * ( 1.0f - fCos ) + fCos;

	_14 = _24 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_44 = 1.0f;
}

//******************************************************************************
/*! \fn     bool CMatrix::Invert()
*   \brief  �������
*           ֻ���ھ���ĵ�����Ϊ[0 0 0 1] �������������������������ı�
*   \return bool true �������� false �󲻳������
*   \sa     �ο�����
*******************************************************************************/
inline bool CMatrix::Invert()
{
	CMatrix	InMat( *this );

	//Verify the matrices with [0 0 0 1] for the 4th column.
	if ( !gIsEqual(_44, 1.0f, .001f) ||
	        !gIsEqual(_14, 0.0f, .001f) ||
	        !gIsEqual(_24, 0.0f, .001f) ||
	        !gIsEqual(_34, 0.0f, .001f) )
		return false;

	float fDetInv = 1.0f / ( InMat._11 * ( InMat._22 * InMat._33 - InMat._23 * InMat._32 ) -
	                         InMat._12 * ( InMat._21 * InMat._33 - InMat._23 * InMat._31 ) +
	                         InMat._13 * ( InMat._21 * InMat._32 - InMat._22 * InMat._31 ) );

	_11 =  fDetInv * ( InMat._22 * InMat._33 - InMat._23 * InMat._32 );
	_12 = -fDetInv * ( InMat._12 * InMat._33 - InMat._13 * InMat._32 );
	_13 =  fDetInv * ( InMat._12 * InMat._23 - InMat._13 * InMat._22 );
	_14 =  0.0f;

	_21 = -fDetInv * ( InMat._21 * InMat._33 - InMat._23 * InMat._31 );
	_22 =  fDetInv * ( InMat._11 * InMat._33 - InMat._13 * InMat._31 );
	_23 = -fDetInv * ( InMat._11 * InMat._23 - InMat._13 * InMat._21 );
	_24 =  0.0f;

	_31 =  fDetInv * ( InMat._21 * InMat._32 - InMat._22 * InMat._31 );
	_32 = -fDetInv * ( InMat._11 * InMat._32 - InMat._12 * InMat._31 );
	_33 =  fDetInv * ( InMat._11 * InMat._22 - InMat._12 * InMat._21 );
	_34 = 0.0f;

	_41 = -( InMat._41 * _11 + InMat._42 * _21 + InMat._43 * _31 );
	_42 = -( InMat._41 * _12 + InMat._42 * _22 + InMat._43 * _32 );
	_43 = -( InMat._41 * _13 + InMat._42 * _23 + InMat._43 * _33 );
	_44 = 1.0f;

	return true;
}

//******************************************************************************
/*! \fn     void CMatrix::InvertFast()
*   \brief  �������
*           ֻ���ھ���ĵ�����Ϊ[0 0 0 1]�������������Ŵ�Сһ���������������������������ı�
*   \return
*   \sa     �ο�����
*******************************************************************************/
inline void CMatrix::InvertFast()
{
	/*
	// ժ��Advanced 3D Game Programming with DirectX7
	// first transpose the rotation matrix
	_11 = in._11;
	_12 = in._21;
	_13 = in._31;
	_21 = in._12;
	_22 = in._22;
	_23 = in._32;
	_31 = in._13;
	_32 = in._23;
	_33 = in._33;

	// fix right column
	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;

	// now get the new translation vector
	point3 temp = in.GetLoc();

	_41 = -(temp.x * in._11 + temp.y * in._12 + temp.z * in._13);
	_42 = -(temp.x * in._21 + temp.y * in._22 + temp.z * in._23);
	_43 = -(temp.x * in._31 + temp.y * in._32 + temp.z * in._33);
	*/

	// first transpose the rotation matrix
	CMatrix	InMat( *this );
	CVector3f v( _11, _12, _13 );
	float fScale = _11*_11 + _12*_12 + _13*_13;

	if ( gIsZero( fScale - 1.0f ) )
	{
		_11 = InMat._11;
		_12 = InMat._21;
		_13 = InMat._31;
		_21 = InMat._12;
		_22 = InMat._22;
		_23 = InMat._32;
		_31 = InMat._13;
		_32 = InMat._23;
		_33 = InMat._33;
	}
	else
	{
		fScale = 1.0f/fScale;
		_11 = InMat._11*fScale;
		_12 = InMat._21*fScale;
		_13 = InMat._31*fScale;
		_21 = InMat._12*fScale;
		_22 = InMat._22*fScale;
		_23 = InMat._32*fScale;
		_31 = InMat._13*fScale;
		_32 = InMat._23*fScale;
		_33 = InMat._33*fScale;
	}

	// fix right column
	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;

	// now get the new translation vector
	_41 = -( InMat._41 * _11 + InMat._42 * _21 + InMat._43 * _31 );
	_42 = -( InMat._41 * _12 + InMat._42 * _22 + InMat._43 * _32 );
	_43 = -( InMat._41 * _13 + InMat._42 * _23 + InMat._43 * _33 );
}

//******************************************************************************
/*! \fn     void CMatrix::Transpose()
*   \brief  ��ת�þ���
*           ת�þ�����Ǿ��������Ԫ�ػ���������ı�
*   \return void
*******************************************************************************/
inline void CMatrix::Transpose()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			float fTemp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = fTemp;
		}
	}
}

inline const CVector3f CMatrix::GetLoc() const
{
	return CVector3f( _41, _42, _43 );
}

inline CVector3f CMatrix::GetAxis( int axis ) const
{
	return CVector3f
	       (
	           m[axis][0],
	           m[axis][1],
	           m[axis][2]
	       );
}
//******************************************************************************
/*! \fn     CMatrix CMatrix::operator * (const CMatrix& mat) const
*   \brief  �������
*           �����˳˺�,����봫��ľ�����˵Ĳ����������ı�
*   \param  const CMatrix& mat Ϊ�ڶ�(��)������
*   \return CMatrix CMatrix::operator ������˺�Ľ������
*******************************************************************************/
inline CMatrix CMatrix::operator * (const CMatrix& mat) const
{
	CMatrix	MatRet;

	float* pA = (float*)this;
	float* pB = (float*)&mat;
	float* pM = (float*)&MatRet;

	memset( pM, 0, sizeof(CMatrix) );

	for ( unsigned int i = 0; i < 4; i++ )
		for ( unsigned int j = 0; j < 4; j++ )
			for ( unsigned int k = 0; k < 4; k++ )
				pM[4*i+j] +=  pA[4*i+k] * pB[4*k+j];

	return MatRet;
}

//******************************************************************************
/*! \fn     CMatrix& CMatrix::operator *= (const CMatrix& mat)
*   \brief  �����Գ�
*
*           ������*=��,����봫��ľ�����˵Ĳ���������ı�
*   \param  const CMatrix& mat Ϊ�ڶ�(��)������
*   \return CMatrix& CMatrix::operator ���ص�����˺�Ľ��Ҳ�����������
*******************************************************************************/
inline CMatrix& CMatrix::operator *= (const CMatrix& mat)
{
	CMatrix	MatRet;

	float* pA = (float*)this;
	float* pB = (float*)&mat;
	float* pM = (float*)&MatRet;

	memset( pM, 0, sizeof(CMatrix) );

	for ( unsigned int i = 0; i < 4; i++ )
		for ( unsigned int j = 0; j < 4; j++ )
			for ( unsigned int k = 0; k < 4; k++ )
				pM[4*i+j] +=  pA[4*i+k] * pB[4*k+j];

	memcpy( this, pM, sizeof(CMatrix) );

	return *this;
}

inline const CVector3f CMatrix::operator*( const CVector3f& b)
{
	return CVector3f(
	           b.x*_11 + b.y*_21 + b.z*_31 + _41,
	           b.x*_12 + b.y*_22 + b.z*_32 + _42,
	           b.x*_13 + b.y*_23 + b.z*_33 + _43
	       );
};
}// End of namespace sqr
