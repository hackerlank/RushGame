#pragma once

#define Max_Far_Dist 20000.0f
#include "CCtrlNode.h"
#include "CFrustum.h"

namespace sqr
{
class CCamera :	public CCtrlNode
{
public:
	static const DWORD st_CameraType;

	CCamera(void);
	virtual ~CCamera(void);

	virtual void						GetBillboardNoneX( CMatrix& Billboard ) const;
	virtual void						GetBillboardAll( CMatrix& Billboard )	const;
	virtual void						GetCameraInvert( CMatrix& ViewInvert )	const;
public:
	//����Y�����ϵ���Ұ
	void setFOVy(float fov);
	//��ȡ��ǰ��Ұ
	float getFOVy(void);

	//������������˾���
	void setNearDist(float nd);
	//�����������˾���
	float getNearDist(void)						const;

	//���������Զ�˾���
	void setFarDist(float fd);
	//��������Զ�˾���
	float getFarDist(void)						const;

	//������ʾ����
	void setAspectRatio(float ratio);
	//��ȡ��ǰ��ʾ����
	float getAspectRatio(void)					const;

	const CMatrix& getViewProjectMatrix(void)	const;
	const CMatrix& getProjectionMatrix(void)	const;
	const CMatrix& getViewMatrix(void)			const;

	/// ƽ��ͶӰ
	bool IsOrthoProject() const;
	void SetOrthoProject(bool b);
	void SetOrthoSize(float width,float height);
	void SetDeltaViewMatrix(CMatrix& matDView);
	const CFrustum& GetFrustum() const;
	DWORD GetNodeType()	{ return CCamera::st_CameraType; };
public:

	//���ù۲��
	void lookAt( const CVector3f& fPoint );
	void lookAt( float x, float y, float z );

	//���ù۲췽��
	void lookTo( const CVector3f& fVector );

	ClipStatus::Type cull( const CAxisAlignedBox& box ) const;
	ClipStatus::Type cull( const CSphere& sph )			const;
	ClipStatus::Type cull( const CVector3f& vec )		const;

	bool	IsVisible( const CAxisAlignedBox& box )		const;
	bool	IsVisible( const CSphere& sphere )			const;
	bool	IsVisible( const CVector3f& point )			const;

	void setFixedYawAxis( bool useFixed,const CVector3f& fixedYaw );
	/*void updateView(void);*/
protected:
	//���¾���
	void _UpdateEx(void)		const;
	void updateFrustum(void)	const;
protected:

	//˳���ܱ�
	mutable	CMatrix			m_matProj;
	mutable	CMatrix			m_matView;
	mutable	CMatrix			m_matViewProject;

	mutable	CFrustum		m_Frustum;
	mutable	float			m_rAspectScr;
	mutable	float			m_rCos[2];
	mutable	float			m_rSin[2];

	//ͶӰ�����Ƿ񱻸ı�
	mutable	bool			m_bRecalcFrustum;

	//�����Y����
	CVector3f			m_vYawFixed;
	//Y�����Ƿ�̶�
	bool				m_bYawFixed;
	mutable	CMatrix		m_matDeltaView;
	mutable	CMatrix		m_matBillboardNoneX;
	mutable	CMatrix		m_matBillboardAll;
};
//------------------------------------------------------------------------------
inline bool
CCamera::IsOrthoProject() const
{
	return m_Frustum.isOrtho;
}

//------------------------------------------------------------------------------
inline void
CCamera::SetOrthoProject( bool b )
{
	if (m_Frustum.isOrtho != b)
	{
		m_Frustum.isOrtho = b;
		m_bRecalcFrustum = true;
		m_bNeedUpdate = true;
	}
}

inline void
CCamera::SetOrthoSize( float width, float height )
{
	m_bNeedUpdate		= true;
	m_bRecalcFrustum	= true;
	m_Frustum.orthoWidth	= width;
	m_Frustum.orthoHeight	= height;
}

inline void CCamera::setFOVy(float fov)
{
	if (m_Frustum.fovY != fov)
	{
		m_Frustum.fovY = fov;
		m_bRecalcFrustum = true;
		m_bNeedUpdate = true;
	}
}

inline float CCamera::getFOVy(void)
{
	return m_Frustum.fovY;
}

inline void CCamera::setNearDist(float nd)
{
	if (m_Frustum.zNear != nd)
	{
		m_Frustum.zNear = nd;
		m_bRecalcFrustum = true;
		m_bNeedUpdate = true;
	}
}

inline float CCamera::getNearDist(void) const
{
	return m_Frustum.zNear;
}

inline void CCamera::setFarDist(float fd)
{
	if (m_Frustum.zFar != fd)
	{
		m_Frustum.zFar = fd;
		m_bRecalcFrustum = true;
		m_bNeedUpdate = true;
	}
}

inline float CCamera::getFarDist(void)  const
{
	return m_Frustum.zFar;
}

inline void CCamera::setAspectRatio(float ratio)
{
	if (m_Frustum.aspect != ratio)
	{
		m_Frustum.aspect = ratio;
		m_bRecalcFrustum = true;
		m_bNeedUpdate = true;
	}
}

inline float CCamera::getAspectRatio(void) const
{
	return m_Frustum.aspect;
}

inline const CMatrix& CCamera::getViewProjectMatrix(void) const
{
	Update();
	return m_matViewProject;
}

inline const CMatrix& CCamera::getProjectionMatrix(void) const
{
	updateFrustum();
	return m_matProj;
}
inline const CMatrix& CCamera::getViewMatrix(void) const
{
	Update();
	return m_matView;
}

inline ClipStatus::Type CCamera::cull(const CVector3f& vec) const
{
	//updateView();
	return m_Frustum.Cull(vec);
}

inline ClipStatus::Type CCamera::cull(const CSphere& sph) const
{
	//updateView();
	return m_Frustum.Cull(sph);
}

inline ClipStatus::Type CCamera::cull(const CAxisAlignedBox& box) const
{
	//updateView();
	if (box.isNull())
		return ClipStatus::Outside;
	return m_Frustum.Cull(box);
}

inline bool CCamera::IsVisible( const CAxisAlignedBox& box ) const
{
	return m_Frustum.IsVisible(box);
}

inline bool CCamera::IsVisible( const CSphere& sphere ) const
{
	return m_Frustum.IsVisible(sphere);
}

inline bool CCamera::IsVisible( const CVector3f& point ) const
{
	return m_Frustum.IsVisible(point);
}

inline void CCamera::lookAt(float x, float y, float z)
{
	lookAt(CVector3f(x,y,z));
}

inline void CCamera::GetBillboardNoneX( CMatrix& Billboard ) const
{
	Update();
	Billboard = m_matBillboardNoneX;
}

inline void CCamera::GetBillboardAll( CMatrix& Billboard ) const
{
	Update();
	Billboard = m_matBillboardAll;
}

inline void CCamera::GetCameraInvert( CMatrix& ViewInvert ) const
{
	Update();
	ViewInvert = m_matCached;
}

//------------------------------------------------------------------------------
inline const CFrustum&
CCamera::GetFrustum() const
{
	return m_Frustum;
}

inline void CCamera::SetDeltaViewMatrix(CMatrix& matDView)
{
	m_matDeltaView = matDView;
}

}

