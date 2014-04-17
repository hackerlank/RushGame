#pragma once
#include "TSingleton.h"
#include "CAudioMallocObject.h"

namespace sqr
{

/// 3D�ռ������������.
/// λ�������������ʶ
/// ȫ��ֻ��һ��������
/// ��������λ��, ����, �ٶ�, һ��������ͬ��
class IAudioListener 
	: public Singleton<IAudioListener>
	, public CAudioMallocObject
{
public:
	virtual ~IAudioListener(){};
	/// ����λ�úͳ���
	virtual void SetTransform(const CVector3f& orientFront, const CVector3f& orientTop, const CVector3f& position) = 0;
	/// ����λ��
	virtual const CVector3f& GetPosition() const = 0;
	/// ����λ��
	virtual void SetPosition(const CVector3f& pos) = 0;
	/// ���س���(��)
	virtual const CVector3f& GetOrientTop() const = 0;
	/// ���ó���(��)
	virtual void SetOrientTop(const CVector3f& top) = 0;
	/// ���س���(ǰ)
	virtual const CVector3f& GetOrientFront() const = 0;
	/// ���ó���(ǰ)
	virtual void SetOrientFront(const CVector3f& front) = 0;
	/// �����ٶ�
	virtual void SetVelocity(const CVector3f& velocity) = 0;
	/// �����ٶ�
	virtual const CVector3f& GetVelocity() const = 0;
};

}// namespace sqr
