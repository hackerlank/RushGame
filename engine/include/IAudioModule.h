#pragma once
#include "IAudioSystem.h"
#include "IAudioPlayer.h"
#include "IAudioListener.h"
#include "IAudioEmitter.h"
#include "CAudioMallocObject.h"

#if defined(_WIN32) && defined(_USRDLL)

#ifdef AUDIO_EXPORTS
#define AUDIO_API extern "C" __declspec(dllexport)
#else
#define AUDIO_API extern "C" __declspec(dllimport)
#endif

#else

#define AUDIO_API

#endif


namespace sqr
{

/// ��Ƶģ��.
/// IAudioSystem, IAudioPlayer, IAudioListenerΪ�ڲ�����, �����ֶ��ͷ�
/// IAudioEmitter��ʹ������ָ��IAudioEmitterPtr�������ü����Ĺ���
class IAudioModule : public CAudioMallocObject
{
public:
	/// ȡ����Ƶ��
	virtual IAudioSystem*    GetAudioSystem() = 0;
	/// ȡ�ò�����
	virtual IAudioPlayer*    GetAudioPlayer() = 0;
	/// ȡ��������
	virtual IAudioListener*  GetAudioListener() = 0;
	/// ����һ������������
	virtual IAudioEmitterPtr CreateAudioEmitter() = 0;
	/// AudioSystem�Ƿ��Ѿ�����
	virtual bool			 HasAudioSystem()	= 0;
};

/// IAudioPlayer�Ŀ�ݵ���.
/// ������IAudioSystem������ſ��Ե���
//@{
AUDIO_API void PlayCue(const char* cueName);
AUDIO_API void Play3DCue(const char* cueName, const CVector3f& position, const CVector3f& velocity);
AUDIO_API void StopCue(const char* cueName);
AUDIO_API bool CueIsStop(const char* cueName);
//@}

AUDIO_API IAudioModule* GetAudioModule();

}// namespace sqr
