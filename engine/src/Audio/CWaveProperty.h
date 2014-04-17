#pragma once
#include "CAudioMallocObject.h"

namespace sqr
{

/// wave����.
/// Դ��Ƶ��ʽ����44kHz 16bit PCM .wav�ļ�(CD����), ������ܻ᲻ʶ��
/// ���ʿ�����ѡ��ѹ����ʽʱ����, WMA�Ļ�100%������ѹ����Ҳ�ܸ�
/// @note WaveNameֻ����WaveBankѡ����"Friendly Names"�����
class CWaveProperty : public CAudioMallocObject
{
public:
	enum Format
	{
		FormatPCM,		///< ��ѹ��
		FormatXMA,		///< XBOX���õ�
		FormatADPCM,	///< ��ѹ����
		FormatWMA		///< ��ѹ����(�����)
	};
public:
	/// ���캯��
	CWaveProperty();

	/// wave����
	void SetWaveName(const AString& waveName);
	const AString& GetWaveName() const;

	/// wave��ʽ
	void SetFormat(Format f);
	const Format GetFormat() const;

	/// ������
	void SetNumChannels(const size_t n);
	const size_t GetNumChannels() const;

	/// ������
	void SetSamplesPerSecond(size_t n);
	const size_t GetSamplesPerSecond() const;

	/// ������
	void SetBitsPerSample(size_t n);
	const size_t GetBitsPerSample() const;

private:
	AString waveName;
	Format format;
	size_t numChannels;
	size_t samplesPerSecond;
	size_t bitsPerSample;
};

//------------------------------------------------------------------------------
inline CWaveProperty::CWaveProperty()
: format(FormatPCM)
, numChannels(0)
, samplesPerSecond(0)
, bitsPerSample(0)
{
	// empty
}

//------------------------------------------------------------------------------
inline void CWaveProperty::SetWaveName( const AString& waveName )
{
	this->waveName = waveName;
}

//------------------------------------------------------------------------------
inline const AString& CWaveProperty::GetWaveName() const
{
	return this->waveName;
}

//------------------------------------------------------------------------------
inline void CWaveProperty::SetFormat( Format f )
{
	this->format = f;
}

//------------------------------------------------------------------------------
inline const CWaveProperty::Format CWaveProperty::GetFormat() const
{
	return this->format;
}

//------------------------------------------------------------------------------
inline void CWaveProperty::SetNumChannels( const size_t n )
{
	this->numChannels = n;
}

//------------------------------------------------------------------------------
inline const size_t CWaveProperty::GetNumChannels() const
{
	return this->numChannels;
}

//------------------------------------------------------------------------------
inline void CWaveProperty::SetSamplesPerSecond( size_t n )
{
	this->samplesPerSecond = n;
}

//------------------------------------------------------------------------------
inline const size_t CWaveProperty::GetSamplesPerSecond() const
{
	return this->samplesPerSecond;
}

//------------------------------------------------------------------------------
inline void CWaveProperty::SetBitsPerSample( size_t n )
{
	this->bitsPerSample = n;
}

//------------------------------------------------------------------------------
inline const size_t CWaveProperty::GetBitsPerSample() const
{
	return this->bitsPerSample;
}

}// namespace sqr

