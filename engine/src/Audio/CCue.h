#pragma once
#include "CRefObject.h"
#include "CCueProperty.h"
#include "SoundState.h"
#include "CAudioMallocObject.h"

/**
	@class CCue

	�����������λ.
	Ϊ�˼��ٸ�����, �ѳ����ȥ����. ֻ����λ�ú��ٶ�.
 */

namespace sqr
{

SmartPointer(CCue);
SmartPointer(CSoundBank);

class CCue 
	: public CRefObject
	, public CAudioMallocObject
{
protected:
	/// ��������
	virtual ~CCue();

public:
	/// ���캯��
	CCue();

	/// �ͷ�
	virtual void Destroy();

	/// ��ʼ��
	virtual void Setup(const CSoundBankPtr& soundBank, const AString& strCueName, index_t cueIndex, bool startPlayback, bool enable3D) = 0;
	void Init(const AString& strCueName, index_t cueIndex, bool startPlayback, bool enable3D);

	/// �Ƿ�Ϸ�
	bool IsValid() const;

	/// ������soundbank�е�����
	uint GetCueIndex() const;

	AString GetCueName() const;
	
	/// ����
	virtual void Play() = 0;
	/// ֹͣ
	virtual bool Stop() = 0; ///����ֵ����stop�Ƿ�ɹ���������ֽṹ���쳣��Ҳ��ʾstop���ɹ�������false
	/// ����ֹͣ
	virtual void StopImmediate() = 0;
	/// ��ͣ
	virtual void Pause() = 0;
	/// ����
	virtual void Resume() = 0;
	/// ����3D��Ϣ
	virtual void Update() = 0;
	
	/// �Ƿ�׼����
	bool IsPreparing() const;
	/// �Ƿ�׼�����
	bool IsPrepared() const;
	/// �Ƿ񲥷���
	bool IsPlaying() const;
	/// �Ƿ�ֹͣ��
	bool IsStopping() const;
	/// �Ƿ�ֹͣ��
	bool IsStopped() const;
	/// �Ƿ���ͣ��
	bool IsPaused() const;
	/// �Ƿ�3D cue
	bool Is3DEnabled() const;
	
	/// ���ص�ǰ״̬
	virtual SoundState::Mask GetState() const = 0;

	/// ���ñ���
	virtual void SetVariable(const AString& name, float value) = 0;
	/// ȡ�ñ���
	virtual float GetVariable(const AString& name) = 0;

	/// ����λ��
	const CVector3f& GetPosition() const;
	/// ����λ��
	void SetPosition(const CVector3f& pos);
	/// ��������ռ���ٶ�
	const CVector3f& GetVelocity() const;
	/// ��������ռ���ٶ�
	void SetVelocity(const CVector3f& v);

protected:
	index_t cueIndex;
	CVector3f position;
	CVector3f velocity;
	bool is3DEnabled;
	AString strCueName;
};

//------------------------------------------------------------------------------
inline bool CCue::IsValid() const
{
	return InvalidIndex != this->cueIndex;
}

//------------------------------------------------------------------------------
inline index_t CCue::GetCueIndex() const
{
	return this->cueIndex;
}

//------------------------------------------------------------------------------
inline AString CCue::GetCueName() const
{
	return this->strCueName;
}

//------------------------------------------------------------------------------
inline bool CCue::IsPreparing() const
{
	return 0 != (SoundState::Preparing & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CCue::IsPrepared() const
{
	return 0 != (SoundState::Preparing & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CCue::IsPlaying() const
{
	return 0 != (SoundState::Playing & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CCue::IsStopping() const
{
	return 0 != (SoundState::Stopping & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CCue::IsStopped() const
{
	return 0 != (SoundState::Stopped & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CCue::IsPaused() const
{
	return 0 != (SoundState::Paused & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CCue::Is3DEnabled() const
{
	return this->is3DEnabled;
}

//------------------------------------------------------------------------------
inline const CVector3f& CCue::GetVelocity() const
{
	return this->velocity;
}

//------------------------------------------------------------------------------
inline void CCue::SetVelocity( const CVector3f& v )
{
	this->velocity = v;
}

//------------------------------------------------------------------------------
inline const CVector3f&
CCue::GetPosition() const
{
	return this->position;
}

//------------------------------------------------------------------------------
inline void
CCue::SetPosition( const CVector3f& pos )
{
	this->position = pos;
}

}// namespace sqr

