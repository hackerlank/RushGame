#pragma once
#include "TSingleton.h"
#include "CAudioMallocObject.h"

namespace sqr
{

/**
	@class IAudioSystem

	��Ƶϵͳ.
	Ĭ��ȫ��������"Sound/globals.xgs"
	û��ȫ������Ҳ�ܹ���, ��3DûЧ��
	һ����˵����(�ļ��Ƚϴ�)��������, ��Ч(Ҫ����Ӧ��)ֱ�������ڴ�

	SoundBank���������ͨ������xml������:
	<Audio>
		<SoundBank name="ambient" stream="true"/>
		<SoundBank name="gui" stream="false"/>
		<SoundBank name="item" stream="false"/>
		<SoundBank name="music" stream="true"/>
		<SoundBank name="npc" stream="false"/>
		<SoundBank name="skill" stream="false"/>
		<SoundBank name="speech" stream="true"/>
		<SoundBank name="weapon" stream="false"/>
	</Audio>
 */
class IAudioSystem
	: public Singleton<IAudioSystem>
	, public CAudioMallocObject
{
public:
	virtual ~IAudioSystem(){}
	/// ����ȫ�������ļ�·��
	virtual void SetGlobalSettingsPath(const char* path) = 0;
	/// ����ȫ�������ļ�·��
	virtual const char* GetGlobalSettingsPath() const = 0;

	/// ������Ƶϵͳ
	virtual bool Open() = 0;
	/// �ر���Ƶϵͳ
	virtual void Close() = 0;
	/// ��Ƶϵͳ�Ƿ�����
	virtual bool IsOpen() const = 0;
	/// ����(ÿ֡����)
	virtual void Update() = 0;

	/// ����soundbank
	virtual bool LoadSoundBank(const char* path, bool streaming) = 0;
	/// ж��soundbank
	virtual void UnloadSoundBank(const char* path) = 0;
	/// ��������soundbank
	virtual bool LoadSoundBankFromXml(const char* xmlFile) = 0;

	/// ����ȫ�ֱ���
	virtual void SetGlobalVariable(const char* name, float value) = 0;
	/// ȡ��ȫ�ֱ���
	virtual float GetGlobalVariable(const char* name) = 0;
	
	/// Set SpeedOfSound Variable Value
	virtual void SetSpeedOfSoundVariable(float value) = 0;
	/// Get SpeedOfSound Variable Value
	virtual float GetSpeedOfSoundVariable(const char* name) = 0;

	/// �����õĽӿ�.
	//@{
	virtual size_t GetNumSoundBanks() const = 0;
	virtual const char* GetSoundBankName(index_t index) const = 0;
	virtual size_t GetNumCues(index_t soundBankIndex) const = 0;
	virtual const char* GetCueName(index_t soundBankIndex, index_t cueIndex) const = 0;
	virtual index_t GetSoundBankIndexByName( const char* name ) const = 0;
	//@}

	/// �ж������Դ�Ƿ����
	virtual bool SoundFileIsExist(const char* name) = 0;
};

}// namespace sqr
