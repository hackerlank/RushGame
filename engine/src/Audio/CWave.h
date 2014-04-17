#pragma once
#include "CRefObject.h"
#include "CWaveProperty.h"
#include "SoundState.h"
#include "CAudioMallocObject.h"

namespace sqr
{

SmartPointer(CWave);
SmartPointer(CSoundBank);

class CWave 
	: public CRefObject
	, public CAudioMallocObject
{
public:
	/// ���캯��
	CWave();
	/// ��������
	virtual ~CWave();

	/// ��ʼ��
	virtual void Setup(const CSoundBankPtr& soundBank, index_t waveIndex, bool startPlayback, size_t loopCount);
	/// �ͷ�
	virtual void Destroy();
	/// �Ƿ�Ϸ�
	bool IsValid() const;

	/// ���ظ�����soundbank
	const CSoundBankPtr& GetSoundBank() const;
	/// ������soundbank�е�����
	index_t GetWaveIndex() const;
	/// ����wave��������Ϣ
	const CWaveProperty& GetWaveProperty() const;

	/// ��������0(����), 1(ԭ����)
	virtual void SetVolume(float v) = 0;
	/// ��������[-1.0, 1.0]
	virtual void SetPitch(float p) = 0;

	/// ����
	virtual void Play() = 0; 
	/// ֹͣ
	virtual void Stop() = 0;
	/// ����ֹͣ
	virtual void StopImmediate() = 0;
	/// ��ͣ
	virtual void Pause() = 0;
	/// ����
	virtual void Resume() = 0;

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

	/// ���ص�ǰ״̬
	virtual SoundState::Mask GetState() const = 0;

protected:
	CSoundBankPtr soundBank;
	index_t waveIndex;
};


//------------------------------------------------------------------------------
inline bool CWave::IsValid() const
{
	return InvalidIndex != this->waveIndex;
}

//------------------------------------------------------------------------------
inline const CSoundBankPtr& CWave::GetSoundBank() const
{
	return this->soundBank;
}

//------------------------------------------------------------------------------
inline index_t CWave::GetWaveIndex() const
{
	return this->waveIndex;
}

//------------------------------------------------------------------------------
inline bool CWave::IsPreparing() const
{
	return 0 != (SoundState::Preparing & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CWave::IsPrepared() const
{
	return 0 != (SoundState::Prepared & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CWave::IsPlaying() const
{
	return 0 != (SoundState::Playing & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CWave::IsStopping() const
{
	return 0 != (SoundState::Stopping & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CWave::IsStopped() const
{
	return 0 != (SoundState::Stopped & this->GetState());
}

//------------------------------------------------------------------------------
inline bool CWave::IsPaused() const
{
	return 0 != (SoundState::Paused & this->GetState());
}

}// namespace sqr
