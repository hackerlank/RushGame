#pragma once
#include "CVector3.h"
#include "CAudioMallocObject.h"

namespace sqr
{

/// ����������.
/// Ϊ�˷���ʹ�ö����ķ�װ
/// ֻ����һ��ʵ��, ����Ҫ�ͻ����ֶ�����
/// ������ֱ��ʹ��wave, Ҫ�õĻ�����AudioEngine��
/// cue�ĸ������Ծ�����Microsoft Cross-Platform Audio Creation Tool (XACT)�е���
class IAudioPlayer : public CAudioMallocObject
{
public:
	/// ����һ������
	virtual void PlayCue(const char* cueName) = 0;
	/// ����һ��3D����
	virtual void Play3DCue(const char* cueName, const CVector3f& position, const CVector3f& velocity) = 0;
	/// ֹͣ����cueName��ʵ��
	virtual void StopCue(const char* cueName) = 0;
};

}// namespace sqr
