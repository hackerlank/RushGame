/*
*	Line(�߶�)�Ķ���,�߶ξ���һ������һ���յ�(��ͬ��Ray),��һ����صķ���
*/
#ifndef _MATH_LINE_H_
#define _MATH_LINE_H_
#include "CMathMallocObject.h"

namespace sqr
{
class CLine : public CMathMallocObject
{
public:
	CLine();
	~CLine();
	CLine(const CLine& lnOrg);					//!< �������캯��
	CLine operator =(const CLine& lnRight);		//!< ��ֵ���캯��

	inline CVector3f GetStart()	const
	{
		return m_vStart;    //!< �õ����
	}
	inline CVector3f GetEnd()		const
	{
		return m_vEnd;    //!< �õ��յ�
	}
	inline CVector3f GetDir()		const
	{
		return m_vDir;    //!< �õ�����
	}
	inline float	  GetLen()		const
	{
		return m_fLen;    //!< �õ�����
	}

	//! �������յ㹹���߶�
	void MakeLineFrom2Point(const CVector3f& vStart, const CVector3f& vEnd);
	//! �ɷ�������һ�����빹��һ���߶�
	void MakeLineFromDir(const CVector3f& vDir, const CVector3f& vStart,
	                     float fLen);
	//! ��һ������Դ�㣬�߶�����Դ��ľ��룬�߶��յ��Դ��ľ��빹���߶�
	void MakeLineFronDir(const CVector3f& vDir, const CVector3f& vOrg,
	                     float fStartLen, float fEndLen);
	//!ʸ������һ���߶εľ���
	bool	Mirror(CVector3f& vec);

protected:
	CVector3f	m_vDir;			//!< �߶εķ����ڲ����˹�һ��
	CVector3f	m_vStart;		//!< �߶ε����
	CVector3f	m_vEnd;			//!< �߶ε��յ�
	float		m_fLen;			//!< �߶εĳ���
};


CLine::CLine() : m_vStart(0.0f, 0.0f, 0.0f), m_vEnd(1.0f, 0.0f, 0.0f),
		m_vDir(1.0f, 0.0f, 0.0f), m_fLen(1.0f)
{

}

CLine::~CLine()
{

}

//! �������캯��
CLine::CLine(const CLine& lnOrg)
{
	m_vStart	= lnOrg.m_vStart;
	m_vEnd		= lnOrg.m_vEnd;
	m_vDir		= lnOrg.m_vDir;
	m_fLen		= lnOrg.m_fLen;
}

//! ��ֵ���캯��
CLine CLine::operator = (const CLine& lnRight)
{
	m_vStart	= lnRight.m_vStart;
	m_vEnd		= lnRight.m_vEnd;
	m_vDir		= lnRight.m_vDir;
	m_fLen		= lnRight.m_fLen;

	return *this;
}

//******************************************************************************
/*! \fn     void CLine::MakeLineFrom2Point(const CVector3f& vStart,
const CVector3f& vEnd)
*   \brief  �������յ㹹���߶�
*
*           �ڲ�У�鴫���������ȷ�ԣ������������ȷ����һȱʡ�߶�,����ʸ��
*			�����˹�һ��
*   \param  const CVector3f& vStart �߶ε����
*   \param  const CVector3f& vEnd   �߶ε��յ�
*   \return void
*   \sa     MakeLineFromDir
*******************************************************************************/
void CLine::MakeLineFrom2Point(const CVector3f& vStart, const CVector3f& vEnd)
{
	if (vStart.IsEqual(vEnd))
	{
		m_vStart.Init(0.0f, 0.0f, 0.0f);
		m_vEnd.Init(1.0f, 0.0f, 0.0f);
		m_vDir.Init(1.0f, 0.0f, 0.0f);
		m_fLen = 1.0f;
		return;
	}
	m_vDir		= vStart - vEnd;
	m_fLen		= m_vDir.Mag();
	m_vDir		= m_vDir/m_fLen;
	m_vStart	= vStart;
	m_vEnd		= vEnd;
}

//******************************************************************************
/*! \fn     void CLine::MakeLineFromDir(const CVector3f& vDir,
const CVector3f& vStart, float fLen)
*   \brief  �ɷ�������һ�����빹��һ���߶�
*
*           �ɷ�������һ�����빹��һ���߶�,�ڲ�У�鴫���������ȷ�ԣ��������
*			����ȷ����һȱʡ�߶�,����ʸ�������˹�һ��.
*   \param  const CVector3f& vDir		�ߵķ���
*   \param  const CVector3f& vStart	�ߵ����
*   \param  float fLen					�ߵĳ���
*   \return void
*   \sa     MakeLineFrom2Point, MakeLineFromDir
*******************************************************************************/
void CLine::MakeLineFromDir(const CVector3f& vDir, const CVector3f& vStart,
                            float fLen)
{
	if (m_vDir.IsZero() || gIsZero(fLen))
	{
		m_vStart.Init(0.0f, 0.0f, 0.0f);
		m_vEnd.Init(1.0f, 0.0f, 0.0f);
		m_vDir.Init(1.0f, 0.0f, 0.0f);
		m_fLen	= 1.0f;
		return;
	}
	m_vDir		= vDir;
	m_vDir.Normalize();
	m_vStart	= vStart;
	m_fLen		= fLen;
	m_vEnd		= vStart + m_vDir*fLen;
}

//******************************************************************************
/*! \fn     void CLine::MakeLineFronDir(cont CVector3f& vDir,
const CVector3f& vOrg, float fStartLen, float fEndLen);
*   \brief  ��һ������Դ�㣬�߶�����Դ��ľ��룬�߶��յ��Դ��ľ��빹���߶�
*
*
*   \param  cont CVector3f&	vDir ����
*	\param  const CVector3f&	vOrg Դ��
*   \param  float fStartLen �߶�����Դ��ľ���
*   \param  float fEndLen	�߶��յ��Դ��ľ���
*   \return void
*   \sa     MakeLineFrom2Point, MakeLineFromDir
*******************************************************************************/
void CLine::MakeLineFronDir(const CVector3f& vDir, const CVector3f& vOrg,
                            float fStartLen, float fEndLen)
{
	if (m_vDir.IsZero() || gIsZero(fStartLen-fEndLen))
	{
		m_vStart.Init(0.0f, 0.0f, 0.0f);
		m_vEnd.Init(1.0f, 0.0f, 0.0f);
		m_vDir.Init(1.0f, 0.0f, 0.0f);
		m_fLen	= 1.0f;
		return;
	}

	m_vDir		= vDir;
	m_vDir.Normalize();
	m_fLen		= (float)fabs((float)(fEndLen - fStartLen));

	m_vStart	= vOrg + m_vDir*fStartLen;
	m_vEnd		= vOrg + m_vDir*fEndLen;
}

//******************************************************************************
/*! \fn     bool TVector3<T>::Mirror(const CLine& Line)
*   \brief  ʸ������һ���߶εľ���
*           ���������һ��������߶�������, �����ֵ���ı�
*   \param  const CLine& Line �߶�
*   \return bool false ���������߶���
*******************************************************************************/
bool CLine::Mirror(CVector3f& vec)
{
	CVector3f v1,v2,v3,st;
	CVector3f dir = GetDir();
	float t;

	st = Line.GetStart();
	v1 = vec - st;
	v2 = v1.Cross(dir);
	if (v2.IsZero())
		return false;
	v3 = v2.Cross(dir);
	t =	v3.Dot(st - vec) * 2 / v3.SquareMag();

	vec += v3 * t;

	return true;

}
} // end of namespace sqr
#endif
