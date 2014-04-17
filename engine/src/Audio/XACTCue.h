#pragma once
#include "CCue.h"

namespace sqr
{

class CXACTCue : public CCue
{
private:
	/// ��������
	virtual ~CXACTCue();

public:
	/// ���캯��
	CXACTCue();

	/// �ͷ�
	void Destroy();

	/// ��ʼ��
	void Setup(const CSoundBankPtr& soundBank, const AString& strCueName, index_t cueIndex, bool startPlayback, bool enable3D);

	/// ����
	void Play();
	/// ֹͣ
	bool Stop();
	/// ����ֹͣ
	void StopImmediate();
	/// ��ͣ
	void Pause();
	/// ����
	void Resume();
	/// ����3D��Ϣ
	void Update();

	/// ���ص�ǰ״̬
	SoundState::Mask GetState() const;

	/// ���ñ���
	void SetVariable(const AString& name, float value);
	/// ȡ�ñ���
	float GetVariable(const AString& name);

private:
	IXACT3Cue* xactCue;
};

}// namespace sqr

