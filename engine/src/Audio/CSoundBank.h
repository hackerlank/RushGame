#pragma once
#include "CRefObject.h"
#include "CCueProperty.h"
#include "CWaveProperty.h"
#include "CAudioMallocObject.h"

namespace sqr
{

SmartPointer(CSoundBank);

/// ����"����".
/// wave bank + sound bank��װ����һ��, ���Ҫ��������������ͬ��
/// @note ��ʽ���ص���Ҫ��wavebank��type����ΪStreaming
class CSoundBank 
	: public CRefObject
	, public CAudioMallocObject
{
public:
	/// ���캯��
	CSoundBank();
	/// ��������
	virtual ~CSoundBank();

	/// ��ʼ��
	virtual bool Setup(const AString& filename, bool streaming);
	/// ����
	virtual void Destroy();
	/// �Ƿ��ʼ����
	bool IsValid() const;

	/// ����sound bank����
	const AString& GetFileName() const;
	/// �Ƿ���ʽ����
	bool IsStreaming() const;

	/// ����Cue��Ŀ
	size_t GetNumCues() const;
	/// ȡ��Cue����
	const CCueProperty& GetCueProperty(index_t cueIndex) const;
	/// �Ƿ����ָ��Cue
	bool HasCue(const AString& cueName) const;
	/// ����Cue���ֲ�������
	index_t FindCueIndexByName(const AString& cueName) const;

	/// ����Wave��Ŀ
	size_t GetNumWaves();
	/// ȡ��Wave����
	const CWaveProperty& GetWaveProperty(index_t waveIndex) const;
	/// �Ƿ����ָ��Wave
	bool HasWave(const AString& waveName) const;
	/// ����Wave���ֲ�������
	index_t FindWaveIndexByName(const AString& waveName) const;

	/// ֹͣ����Cue��ʵ��
	virtual void StopCue(index_t cueIndex, bool immediate) = 0;
	/// ֹͣ����Wave��ʵ��
	virtual void StopWave(index_t waveIndex, bool immediate) = 0;

	/// ���� XACT sound bank ��ָ��
	virtual IXACT3SoundBank* GetXACTSoundBank() const = 0;
	/// ���� XACT wave bank ��ָ��
	virtual IXACT3WaveBank* GetXACTWaveBank() const = 0;

	AString GetSoundBankFileName() const;

protected:
	AString filename;
	bool isValid;
	bool isStreaming;
	AVector<CCueProperty> cueProperties;
	AVector<CWaveProperty> waveProperties;
	AMap<AString, index_t> cueIndexMap;
	AMap<AString, index_t> waveIndexMap;
};



//------------------------------------------------------------------------------
inline bool CSoundBank::IsValid() const
{
	return this->isValid;
}

//------------------------------------------------------------------------------
inline const AString& CSoundBank::GetFileName() const
{
	return this->filename;
}

//------------------------------------------------------------------------------
inline bool CSoundBank::IsStreaming() const
{
	return this->isStreaming;
}

//------------------------------------------------------------------------------
inline size_t CSoundBank::GetNumCues() const
{
	return this->cueIndexMap.size();
}

//------------------------------------------------------------------------------
inline size_t CSoundBank::GetNumWaves()
{
	return this->waveIndexMap.size();
}

//------------------------------------------------------------------------------
inline const CCueProperty& CSoundBank::GetCueProperty( index_t cueIndex ) const
{
	return this->cueProperties[cueIndex];
}

//------------------------------------------------------------------------------
inline bool CSoundBank::HasCue( const AString& cueName ) const
{
	return (this->cueIndexMap.end() != this->cueIndexMap.find(cueName));
}

//------------------------------------------------------------------------------
inline const CWaveProperty& CSoundBank::GetWaveProperty( index_t waveIndex ) const
{
	return this->waveProperties[waveIndex];
}

//------------------------------------------------------------------------------
inline bool CSoundBank::HasWave( const AString& waveName ) const
{
	return (this->waveIndexMap.end() != this->waveIndexMap.find(waveName));
}

//------------------------------------------------------------------------------
inline index_t CSoundBank::FindWaveIndexByName( const AString& waveName ) const
{
	AMap<AString, index_t>::const_iterator it = this->waveIndexMap.find(waveName);
	if (this->waveIndexMap.end() != it)
		return it->second;
	else
		return InvalidIndex;
}

}// namespace sqr
