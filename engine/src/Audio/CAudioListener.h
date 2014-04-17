#pragma once
#include "IAudioListener.h"

namespace sqr
{

/// 3D�ռ������������.
/// λ�������������ʶ
/// ֻ��һ�������� //?Ϊʲôֻ��һ��������,������������ҲӦ����ֻ��һ��
/// ��������λ��, ����, �ٶ�, һ��������ͬ��
class CAudioListener
	: public IAudioListener 
{
public:
	/// ����λ�úͳ���
	void SetTransform(const CVector3f& orientFront, const CVector3f& orientTop, const CVector3f& position);
	/// ����λ��
	const CVector3f& GetPosition() const;
	/// ����λ��
	void SetPosition(const CVector3f& pos);
	/// ���س���(��)
	const CVector3f& GetOrientTop() const;
	/// ���ó���(��)
	void SetOrientTop(const CVector3f& top);
	/// ���س���(ǰ)
	const CVector3f& GetOrientFront() const;
	/// ���ó���(ǰ)
	void SetOrientFront(const CVector3f& front);
	/// �����ٶ�
	void SetVelocity(const CVector3f& velocity);
	/// �����ٶ�
	const CVector3f& GetVelocity() const;

public:
	/// ���캯��
	CAudioListener();
	/// ��������
	~CAudioListener();
};

}// namespace sqr
