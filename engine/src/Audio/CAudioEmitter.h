#pragma once
#include "IAudioEmitter.h"
#include "TRefPointer.h"

namespace sqr
{

SmartPointer(CCue);
SmartPointer(CAudioEmitter);
/// 3D�ռ������������.
/// λ�������������ʶ
/// ������ЧӦֻ���ٶ��й�
class CAudioEmitter : public IAudioEmitter
{
public:
	/// ���캯��
	CAudioEmitter();
	/// �������ü���
	void AddRef();
	/// �������ü���
	void Release();

	/// ����λ��
	const CVector3f& GetPosition() const;
	/// ����λ��
	void SetPosition(const CVector3f& pos);
	/// �����ٶ�
	void SetVelocity(const CVector3f& v);
	/// �����ٶ�
	const CVector3f& GetVelocity() const;

	/// ��������
	bool Play(const char* cueName);
	/// ֹͣ
	bool Stop();

	/// �Ƿ񲥷���
	bool IsPlaying() const;
	/// �Ƿ�ֹͣ
	bool IsStopped() const;

	/// ���ñ���
	void SetVariable(const char* name, float value);
	/// ȡ�ñ���
	float GetVariable(const char* name);

	/// �����Զ�����
	void SetAutoStop(bool b);
	/// �Ƿ��Զ�����
	bool IsAutoStop() const;

	void Noticfy();
private:
	/// ��������
	virtual ~CAudioEmitter();

	void	InValid();

private:
	volatile int refCount;

	CCuePtr cue;
	CVector3f position;
	CVector3f velocity;
	AMap<AString, float> variableMap;
	bool isAutoStop;
	AString strCueName;
	bool   isValid;
};


//------------------------------------------------------------------------------
inline void CAudioEmitter::AddRef()
{
	InterlockedIncrement((volatile long*)&this->refCount);
}

//------------------------------------------------------------------------------
inline void CAudioEmitter::Release()
{
	Ast(this->refCount > 0);
	InterlockedDecrement((volatile long*)&this->refCount);
	if (0 == this->refCount)
	{
		delete this;
	}
}

//------------------------------------------------------------------------------
inline const CVector3f& CAudioEmitter::GetVelocity() const
{
	return this->velocity;
}

//------------------------------------------------------------------------------
inline const CVector3f&
CAudioEmitter::GetPosition() const
{
	return this->position;
}

//------------------------------------------------------------------------------
inline float
CAudioEmitter::GetVariable( const char* name )
{
	return this->variableMap[name];
}

}// namespace sqr
