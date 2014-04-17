#pragma once
#include "IAudioSystem.h"
#include "CAudioAdapterInfo.h"

namespace sqr
{

typedef AVector<CAudioAdapterInfo> CAudioAdapterInfoList;

/// ��Ƶϵͳ���ķ���.
class CAudioSystem
	: public IAudioSystem
{
public:
	/// ����ȫ�������ļ�·��
	void SetGlobalSettingsPath(const char* path);
	/// ����ȫ�������ļ�·��
	const char* GetGlobalSettingsPath() const;

	/// ������Ƶϵͳ
	bool Open();
	/// �ر���Ƶϵͳ
	void Close();
	/// ��Ƶϵͳ�Ƿ�����
	bool IsOpen() const;
	/// ����(ÿ֡����)
	void Update();

	/// ������Ƶ�������б�
	const CAudioAdapterInfoList& GetAudioAdapterList();
	/// ѡ��������
	void SetAdapterIndex(index_t adapterIndex);
	/// ��������ʹ�õ�����������
	index_t GetAdapterIndex() const;

	/// ����soundbank
	bool LoadSoundBank(const char* path, bool streaming);
	/// ж��soundbank
	void UnloadSoundBank(const char* path);
	/// ��������soundbank
	bool LoadSoundBankFromXml(const char* xmlFile);

	/// ����ȫ�ֱ���
	void SetGlobalVariable(const char* name, float value);
	/// ȡ��ȫ�ֱ���
	float GetGlobalVariable(const char* name);
	
	/// Set SpeedOfSound Variable Value
	virtual void SetSpeedOfSoundVariable(float value);
	/// Get SpeedOfSound Variable Value
	virtual float GetSpeedOfSoundVariable(const char* name);

	/// �����õĽӿ�.
	//@{
	/// ���ؼ��ص�sound bank��Ŀ
	size_t GetNumSoundBanks() const;
	/// ����sound bank������
	const char* GetSoundBankName(index_t index) const;
	/// ����sound bank�е�cue��Ŀ
	size_t GetNumCues(index_t soundBankIndex) const;
	/// ����sound bank��ָ��cue������
	const char* GetCueName(index_t soundBankIndex, index_t cueIndex) const;
	/// ����nameָ����sound bank index
	index_t GetSoundBankIndexByName( const char* name ) const;
	//@}

	void CollectSoundDirFileInfo();

	bool SoundFileIsExist(const char* name);
public:
	friend class TSingleton<CAudioSystem>;
	CAudioSystem();
	~CAudioSystem();

private:
	bool isOpen;
	CAudioAdapterInfoList adapterList;
	uint64 lastUpdateTime;
	int  nLoadXmlCount;

	static const uint UPDATE_INTERVAL;	///< ���¼��(Ƶ��)

	ASet<AString> listSoundDirFileName;
};



//------------------------------------------------------------------------------
inline bool CAudioSystem::IsOpen() const
{
	return this->isOpen;
}

//------------------------------------------------------------------------------
inline const CAudioAdapterInfoList& CAudioSystem::GetAudioAdapterList()
{
	return this->adapterList;
}

}// namespace sqr
