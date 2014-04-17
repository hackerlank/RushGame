#pragma once
#include "IAudioModule.h"
#include "TSingleton.h"
#include "TRefPointer.h"

namespace sqr
{

class IAudioSystem;
class IAudioPlayer;
class IAudioListener;
SmartPointer(IAudioEmitter);

/// ��Ƶģ��.
class CAudioModule
	: public IAudioModule
	, public Singleton<CAudioModule>
{
public:
	/// ȡ����Ƶ��
	IAudioSystem* GetAudioSystem();
	/// ȡ�ò�����
	IAudioPlayer* GetAudioPlayer();
	/// ȡ��������
	IAudioListener* GetAudioListener();
	/// ����һ������������
	IAudioEmitterPtr CreateAudioEmitter();
	bool			 HasAudioSystem();

public:
	CAudioModule();
	~CAudioModule();
};

}// namespace sqr
