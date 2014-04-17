#pragma once
#include "CRefObject.h"
#include "CAxisAlignedBox.h"
/**
	@class CShadowMatrixBuilder

	��Ӱ���������㷨.

	�������������ɶ�Ӧ�㷨
 */

namespace sqr
{

class CCamera;
class CShadowClipper;
SmartPointer(CShadowMatrixBuilder);
class CShadowMatrixBuilder : public CRefObject
{
public:
	CShadowMatrixBuilder();
	virtual ~CShadowMatrixBuilder();

	/// ���ó��������
	void SetCamera(CCamera* camera);
	/// ȡ�ó��������
	CCamera* GetCamera() const;
	/// ���ù�Դ����
	void SetLightDir(const CVector3f& dir);
	/// ����shadow�����С
	void SetShadowMapSize(uint size);
	/// ȡ�������С
	uint GetShadowMapSize() const;

	/// ȡ��light�ռ���ͼ����
	const CMatrix& GetViewMatrix() const;
	/// ȡ��light�ռ�ͶӰ����
	const CMatrix& GetProjectMatrix() const;
	/// ȡ����Ӱ�����������ɾ���
	const CMatrix& GetShadowMatrix() const;

	/// ���������Ϣ
	virtual void BuildMatrix(CShadowClipper* clipper) = 0;

	//������չ����
	virtual float BuildMatrixEx( CShadowClipper* clipper)
	{
		return 0;
	};

protected:
	CCamera* m_Camera;
	CVector3f m_LightDir;
	uint m_ShadowMapSize;

	CMatrix m_ViewMatrix;
	CMatrix m_ProjMatrix;
	CMatrix m_ShadowMatrix;
};

/// �򵥵�ƽ��ͶӰ.
class COrthoMatrixBuilder : public CShadowMatrixBuilder
{
public:
	/// ���������Ϣ
	void BuildMatrix(CShadowClipper* clipper);
	float BuildMatrixEx( CShadowClipper* clipper);
};

/// Trapezoidal Shadow Map.
class CTsmMatrixBuilder : public CShadowMatrixBuilder
{
public:
	CTsmMatrixBuilder();
	/// ���ý���λ��(0~1)
	void SetFocusRegion(float focus);
	/// ���������Ϣ
	void BuildMatrix(CShadowClipper* clipper);

private:
	float m_FocusRegion;
};


//------------------------------------------------------------------------------
inline void
CShadowMatrixBuilder::SetCamera( CCamera* cam )
{
	m_Camera = cam;
}

//------------------------------------------------------------------------------
inline void
CShadowMatrixBuilder::SetLightDir( const CVector3f& dir )
{
	m_LightDir = -dir;
}

//------------------------------------------------------------------------------
inline void
CShadowMatrixBuilder::SetShadowMapSize( uint size )
{
	m_ShadowMapSize = size;
}

//------------------------------------------------------------------------------
inline  CCamera*
CShadowMatrixBuilder::GetCamera() const
{
	return m_Camera;
}

//------------------------------------------------------------------------------
inline uint
CShadowMatrixBuilder::GetShadowMapSize() const
{
	return m_ShadowMapSize;
}

//------------------------------------------------------------------------------
inline const CMatrix&
CShadowMatrixBuilder::GetViewMatrix() const
{
	return m_ViewMatrix;
}

//------------------------------------------------------------------------------
inline const CMatrix&
CShadowMatrixBuilder::GetProjectMatrix() const
{
	return m_ProjMatrix;
}

//------------------------------------------------------------------------------
inline const CMatrix&
CShadowMatrixBuilder::GetShadowMatrix() const
{
	return m_ShadowMatrix;
}

//------------------------------------------------------------------------------
inline void
CTsmMatrixBuilder::SetFocusRegion( float focus )
{
	m_FocusRegion = focus;
}
}// namespace sqr

