#pragma once
#include "CAudioAdapterInfo.h"
#include "CWindowsVersionEnum.h"
#include "CCue.h"
#include "CWave.h"
#include "CSoundBank.h"
#include "CRefObject.h"
#include "CAudioMallocObject.h"

namespace sqr
{

class CAudioEmitter;

/// ��Ƶ����. 
class CAudioEngine 
	: public CRefObject
	, public CAudioMallocObject
{
public:
	/// ���캯��
	CAudioEngine();
	/// ��������
	virtual ~CAudioEngine();

	/// ȡ������������
	virtual size_t GetNumAdapters() const = 0;
	/// ������������Ϣ
	virtual CAudioAdapterInfo GetAdapterInfo(index_t adapterIndex) const = 0;
	/// ����Ĭ�ϵ�������
	void SetDefaultAdapter(index_t adapterIndex);
	/// ����Ĭ��������������
	index_t GetDefaultAdapter() const;
	
	/// ����ȫ�������ļ�·��
	void SetGlobalSettingsPath(const AString& filename);
	/// ����ȫ�������ļ�·��
	const AString& GetGlobalSettingsPath() const;

	/// ����Ƶ����
	virtual bool Open();
	/// �ر���Ƶ����
	virtual void Close();
	/// ������Ƶ����
	virtual void Update();
	/// ��Ƶ�����Ƿ��
	bool IsOpen() const;
	/// ����win7��vista����������
	void SetIsOpen(bool bOpen);
	/// ��Ƶ�豸�Ƿ����
	bool IsAudioDeviceValid() const;
	/// �Ƿ�֧��3DЧ��
	bool IsSupport3D() const;

	/// ����sound bank
	bool LoadSoundBank(const AString& filename, bool streaming);
	/// ж��sound bank
	void UnloadSoundBank(const AString& filename);
	/// ����sound bank������
	size_t GetNumSoundBanks() const;
	/// ����ָ��������sound bank
	const CSoundBankPtr& GetSoundBankByIndex(index_t index);
	/// ����sound bank���ֵõ�sound bank index
	index_t GetSoundBankIndexByName(const AString& name) const;
	/// ����ָ�����ֵ�sound bank
	const CSoundBankPtr& GetSoundBankByName(const AString& name) const;
	/// �Ƿ����ָ��sound bank
	bool HasSoundBank(const AString& filename) const;

	/// ���������ߵ�λ����Ϣ���ٶ�
	void UpdateListener(const CVector3f& pos,
						const CVector3f& top,
						const CVector3f& front,
						const CVector3f& velocity);
	/// ����������λ��
	void SetListenerPosition(const CVector3f& pos);
	/// ���������ߵ�λ��
	const CVector3f& GetListenerPostion() const;
	/// ���������߳���(��)
	void SetListenerOrientTop(const CVector3f& top);
	/// ���������ߵĳ���(��)
	const CVector3f& GetListenerOrientTop() const;
	/// ���������߳���(ǰ)
	void SetListenerOrientFront(const CVector3f& front);
	/// ���������ߵĳ���(ǰ)
	const CVector3f& GetListenerOrientFront() const;
	/// ���������ߵ��ٶ�
	void SetListenerVelocity(const CVector3f& velocity);
	/// ���������ߵ��ٶ�
	const CVector3f& GetListenerVelocity() const;
	/// ��ʼ����ʱ��������Ч���Ƿ���Ч
	void  SetAudioDeviceIsValid(bool isValid);

	/// ���Ż�׼��һ��cue,ʹ��emitter���øýӿڣ��践��һ��cue����
	const CCuePtr& PlayCue(const AString& cueName, bool play, bool enable3D, const CVector3f& position, const CVector3f& velocity);
	/// ���Ż�׼��һ��cue,ȫ�ֲ�����Ч�����米�����֣���ɫ�ܻ��ȣ�����Ҫ����һ��cue����
	void  PlayGloablCue(const AString& cueName, bool play, bool enable3D, const CVector3f& position, const CVector3f& velocity);
	/// ֹͣ����cue��ʵ��
	void  StopCue(const AString& cueName, bool immediate);
	bool  CueIsStop(const AString& cueName);

	/// ���Ż�׼��һ��wave
	const CWavePtr& PlayWave(const AString& waveName, bool play, size_t loopCount, float volume, float pitch);
	/// ֹͣ����wave��ʵ��
	void StopWave(const AString& waveName, bool immediate);

	/// ����cue��soundbank
	index_t FindCue(const AString& cueName, CSoundBankPtr& outSoundBank);
	/// ����wave��soundbank
	index_t FindWave(const AString& waveName, CSoundBankPtr& outSoundBank);

	/// ����ȫ�ֱ���
	virtual void SetGlobalVariable(const AString& name, float value) = 0;
	/// ȡ��ȫ�ֱ���
	virtual float GetGlobalVariable(const AString& name) = 0;

	/// �����ӵ�os version
	void EstimateSystemVersion();

	void InsertAudioEmitter(CAudioEmitter *pAudioEmitter);
	void DeleteAudioEmitter(CAudioEmitter *pAudioEmitter);

protected:
	void DestroyStoppedCues();
	void DestroyStoppedWaves();
	void DestroyWavesResource();
	void DestroySoundBankResource();
	void DestroySoundResource();
	void NodityAmitterDestroySoundResource();

	virtual CSoundBank* CreateSoundBank() = 0;
	virtual CCue* CreateCue() = 0;
	virtual CWave* CreateWave() = 0;
protected:
	index_t defaultAdapterIndex;
	AString  globalSettingsPath;
	bool isOpen;
	//////////////////////////////////////////////////////////////////////////
	///win7 deal with
	bool isAudioDeviceValid;
	bool isLastAudioDeviceValid;
	static bool isDestroyResourceInGame;
	
	//////////////////////////////////////////////////////////////////////////
	bool isSupport3D;
	AMap<AString, CSoundBankPtr> soundBanks;
	AList<CCuePtr> activeNormalCues;		///normal cue, not need emitter to play 
	AList<CCuePtr> activeEmitterCues;///emitter to play cue
	AList<CWavePtr> activeWaves;

	set<CAudioEmitter*> setAudioEmitters;

	CVector3f listenerPosition;
	CVector3f listenerOrientTop;
	CVector3f listenerOrientFront;
	CVector3f listenerVelocity;
	HANDLE updateEvent;
	HANDLE detachEvent;
	HANDLE attachEvent;

public:
	static WindowsVersion eWindowsType;
};

extern CAudioEngine* GetAudioEngine();

//------------------------------------------------------------------------------
inline void CAudioEngine::SetDefaultAdapter( index_t adapterIndex )
{
	this->defaultAdapterIndex = adapterIndex;
}

//------------------------------------------------------------------------------
inline index_t CAudioEngine::GetDefaultAdapter() const
{
	return this->defaultAdapterIndex;
}

//------------------------------------------------------------------------------
inline void CAudioEngine::SetGlobalSettingsPath( const AString& filename )
{
	this->globalSettingsPath = filename;
}

//------------------------------------------------------------------------------
inline const AString& CAudioEngine::GetGlobalSettingsPath() const
{
	return this->globalSettingsPath;
}

//------------------------------------------------------------------------------
inline bool CAudioEngine::IsOpen() const
{
	return this->isOpen;
}

//------------------------------------------------------------------------------
inline void CAudioEngine::SetIsOpen(bool bOpen)
{
	this->isOpen = bOpen;
}

//------------------------------------------------------------------------------
inline bool CAudioEngine::IsAudioDeviceValid() const
{
	return this->isAudioDeviceValid && this->isOpen;
}

//------------------------------------------------------------------------------
inline bool CAudioEngine::IsSupport3D() const
{
	return this->isSupport3D;
}

//------------------------------------------------------------------------------
inline size_t CAudioEngine::GetNumSoundBanks() const
{
	return this->soundBanks.size();
}

//------------------------------------------------------------------------------
inline bool CAudioEngine::HasSoundBank( const AString& filename ) const
{
	return (this->soundBanks.end() != this->soundBanks.find(filename));
}

//------------------------------------------------------------------------------
inline void CAudioEngine::SetListenerVelocity( const CVector3f& velocity )
{
	this->listenerVelocity = velocity;
}

//------------------------------------------------------------------------------
inline const CVector3f& CAudioEngine::GetListenerVelocity() const
{
	return this->listenerVelocity;
}

//------------------------------------------------------------------------------
inline void
CAudioEngine::SetListenerPosition( const CVector3f& pos )
{
	this->listenerPosition = pos;
}

//------------------------------------------------------------------------------
inline const CVector3f&
CAudioEngine::GetListenerPostion() const
{
	return this->listenerPosition;
}

//------------------------------------------------------------------------------
inline void
CAudioEngine::SetListenerOrientTop( const CVector3f& top )
{
	this->listenerOrientTop = top;
}

//------------------------------------------------------------------------------
inline const CVector3f&
CAudioEngine::GetListenerOrientTop() const
{
	return this->listenerOrientTop;
}

//------------------------------------------------------------------------------
inline void
CAudioEngine::SetListenerOrientFront( const CVector3f& front )
{
	this->listenerOrientFront = front;
}

//------------------------------------------------------------------------------
inline const CVector3f&
CAudioEngine::GetListenerOrientFront() const
{
	return this->listenerOrientFront;
}

//------------------------------------------------------------------------------
inline void
CAudioEngine::SetAudioDeviceIsValid(bool isValid)
{
	this->isAudioDeviceValid = isValid;
}

}// namespace sqr
