#pragma once
#include "IAudioPlayer.h"
#include "TSingleton.h"

namespace sqr
{

/// ����������.
/// Ϊ�˷���ʹ�ö����ķ�װ
/// ����Ϊ����
/// ������ֱ��ʹ��wave, Ҫ�õĻ�����AudioEngine��
/// cue�ĸ������Ծ�����Microsoft Cross-Platform Audio Creation Tool (XACT)�е���
class CAudioPlayer
	: public IAudioPlayer
	, public Singleton<CAudioPlayer>
{
public:
	/// ����һ������
	void PlayCue(const char* cueName);
	/// ����һ��3D����
	void Play3DCue(const char* cueName, const CVector3f& position, const CVector3f& velocity);
	/// ֹͣ����cueName��ʵ��
	void StopCue(const char* cueName);
	/// cue�Ƿ񲥷Ž���
	bool CueIsStop(const char* cueName);
public:
	/// ���캯��
	CAudioPlayer();
	/// ��������
	~CAudioPlayer();
};

}// namespace sqr
