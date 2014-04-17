#pragma once
#include "CSoundBank.h"

namespace sqr
{

class CXACTSoundBank : public CSoundBank
{
public:
	/// ���캯��
	CXACTSoundBank();
	/// ��������
	virtual ~CXACTSoundBank();

	/// ��ʼ��
	bool Setup(const AString& filename, bool streaming);
	/// ����
	void Destroy();

	/// ֹͣ����Cue��ʵ��
	void StopCue(index_t cueIndex, bool immediate);
	/// ֹͣ����Wave��ʵ��
	void StopWave(index_t waveIndex, bool immediate);

	/// ���� XACT sound bank ��ָ��
	IXACT3SoundBank* GetXACTSoundBank() const;
	/// ���� XACT wave bank ��ָ��
	IXACT3WaveBank* GetXACTWaveBank() const;

private:
	bool SetupSoundBank();
	bool SetupWaveBank();
	void DestroySoundBank();
	void DestroyWaveBank();

private:
	IXACT3SoundBank* xactSoundBank;
	IXACT3WaveBank* xactWaveBank;
	void* soundBankData;
	void* waveBankData;
};

}// namespace sqr
