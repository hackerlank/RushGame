#pragma once
#include "TRefPointer.h"
#include "CAudioMallocObject.h"

namespace sqr
{

SmartPointer(IAudioEmitter);
/// 3D�ռ������������.
/// λ�������������ʶ
/// ������ЧӦֻ���ٶ��й�
class IAudioEmitter : public CAudioMallocObject
{
public:
	/// �������ü���
	virtual void AddRef() = 0;
	/// �������ü���
	virtual void Release() = 0;

	/// ����λ��
	virtual const CVector3f& GetPosition() const = 0;
	/// ����λ��
	virtual void SetPosition(const CVector3f& pos) = 0;
	/// �����ٶ�
	virtual void SetVelocity(const CVector3f& v) = 0;
	/// �����ٶ�
	virtual const CVector3f& GetVelocity() const = 0;

	/// ��������
	virtual bool Play(const char* cueName) = 0;
	/// ֹͣ
	virtual bool Stop() = 0;

	/// �Ƿ񲥷���
	virtual bool IsPlaying() const = 0;
	/// �Ƿ�ֹͣ
	virtual bool IsStopped() const = 0;

	/// ���ñ���
	virtual void SetVariable(const char* name, float value) = 0;
	/// ȡ�ñ���
	virtual float GetVariable(const char* name) = 0;

	/// �����Զ�����
	virtual void SetAutoStop(bool b) = 0;
	/// �Ƿ��Զ�����
	virtual bool IsAutoStop() const = 0;

protected:
	/// ��������
	virtual ~IAudioEmitter() {};
};

}// namespace sqr
