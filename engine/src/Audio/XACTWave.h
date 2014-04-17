#pragma once
#include "CWave.h"

namespace sqr
{

class CXACTWave : public CWave
{
public:
	/// ���캯��
	CXACTWave();
	/// ��������
	virtual ~CXACTWave();

	/// ��ʼ��
	void Setup(const CSoundBankPtr& soundBank, index_t waveIndex, bool startPlayback, size_t loopCount);
	/// �ͷ�
	void Destroy();

	/// ��������0(����), 1(ԭ����)
	void SetVolume(float v);
	/// ��������[-1.0, 1.0]
	void SetPitch(float p);

	/// ����
	void Play();
	/// ֹͣ
	void Stop();
	/// ����ֹͣ
	void StopImmediate();
	/// ��ͣ
	void Pause();
	/// ����
	void Resume();

	/// ���ص�ǰ״̬
	SoundState::Mask GetState() const;

private:
	IXACT3Wave* xactWave;
	XACTVOLUME xactVolume;
	XACTPITCH xactPitch;
};

}// namespace sqr
