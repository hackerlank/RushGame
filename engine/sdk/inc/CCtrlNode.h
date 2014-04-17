#pragma once

#include "CMatrix.h"
#include "CVector3.h"
#include "CQuaternion.h"
#include "CGraphicMallocObject.h"
#include "CDynamicObject.h"
#include "CRenderSpaceNode.h"

namespace sqr
{
	class CNodeController;
	class CCtrlNode 
		: public CGraphicMallocObject
		, public CRenderSpaceNode
		, public virtual CDynamicObject //�������CBaseObject�Ǻ���ģ�����˵��CCtrlNodeӦ����Scene��һ����
	{
		friend class CNodeController;
	protected:
		CQuaternion m_Quaternion;
		CVector3f	m_v3Position;
		CVector3f	m_vScale;
		///CVector3f	m_v3TargetPos;//CurCameraDest;
	public:

		CCtrlNode();
		virtual ~CCtrlNode();

		void move( float x, float y, float z ); //���ƽ��,��������ƽ��
		void move( const CVector3f& v3 );

		void moveRelative( const CVector3f& v3 ); //����������ƽ��
		void moveRelative( float x,float y,float z );//����������ƽ��

		//��������ת
		void Rotate(float alpha,float beta,float gamma); //�������ܿ��� ������ʹ��
		void Rotate(const CVector3f& axis,float angle);
		void Rotate(const CQuaternion& q);
		void Pitch( float alpha );
		void Yaw( float beta );
		void Roll( float gamma );
		void SetRotate(const CVector3f& axis,float angle);

		//�ƹ̶�����ת
		void RotateX(float alpha);
		void RotateY(float beta);
		void RotateZ(float gamma);

		void SetPosition( float x, float y, float z );//���þ�������
		void SetPosition( const CVector3f& v3 );
		const CVector3f& GetPosition() const; //��ȡ��ǰ����

		void setScale( float x,float y,float z);
		void setScale( const CVector3f& v3 );
		void setScale( float Scale); //����ͬһ�ֱ�������

		void Scale( float x,float y,float z);
		void Scale( const CVector3f& v3 );
		void Scale( float Scale); //����ͬһ�ֱ�������

		// ������������ת����������Ƹı�
		void centerRotate( const CVector3f& center,float alpha,float beta,float gamma ); //��������ת
		void centerRotate( const CVector3f& center,const CVector3f& axis,float angle); //��������ת
		void centerRotate( const CVector3f& center,const CQuaternion& q);
		void centerPitch( const CVector3f& center,float alpha );
		void centerYaw( const CVector3f& center,float beta );
		void centerRoll( const CVector3f& center,float gamma ); 

		void setTransferMatrix( const CMatrix& mat4  ); //���ñ任����
		const CMatrix& GetWorldMatrix();	//��ȡ��ǰ����

		void setVisible( bool IsVisible = true );
		bool getVisible();
		
		const CVector3f& getUP(void)		const;
		const CVector3f& getRIGHT(void)		const;
		const CVector3f& getDIRECTION(void)	const;
		//���ù۲췽��
		void setDIRECTION( const CVector3f& fVector );
		
		
	protected:			
				void	Update(void)		const;
		virtual void	_UpdateEx(void)		const{};
				bool	m_bVisible;

		mutable CMatrix	m_matCached; //���ڴ������
		mutable	bool	m_bNeedUpdate;
		CNodeController*	m_pController;
	private:
	};

	inline const CVector3f& CCtrlNode::getUP(void) const
	{
		Update();
		return *(CVector3f*)&m_matCached.m[1];
	}

	inline const CVector3f& CCtrlNode::getRIGHT(void) const
	{
		Update();
		return *(CVector3f*)&m_matCached.m[0];
	}

	inline const CVector3f& CCtrlNode::getDIRECTION(void) const
	{
		Update();
		return *(CVector3f*)&m_matCached.m[2];
	}
}