#pragma once
#include "CAudioEngine.h"
#include "TSingleton.h"
#include "ThreadTypes.h"

namespace sqr
{
class CXACTAudioEngine : public CAudioEngine ,public Singleton<CXACTAudioEngine>
{
public:
	/// ȡ������������
	size_t GetNumAdapters() const;
	/// ������������Ϣ
	CAudioAdapterInfo GetAdapterInfo(index_t adapterIndex) const;

	/// ����Ƶ����
	bool Open();
	/// ��ʼ����Ƶ�����Լ�ע������¼�
	bool InitializeEngine();
	void CreateAudioThread();
	/// �ر���Ƶ����
	void Close();
	/// �ͷ�globalSettingsBuffer���ֵ
	void ReleaseGlobalSettings();

	/// ����ȫ�ֱ���
	void SetGlobalVariable(const AString& name, float value);
	/// ȡ��ȫ�ֱ���
	float GetGlobalVariable(const AString& name);
	/// �����ӵ�ǰ��Ч�豸�Ƿ����
	bool EstimateCurAudioDeviceIsValid(const bool lastAudioDeviceValid);

	/// ȡ�� XACT engine ��ָ��
	IXACT3Engine* GetXACTEngine() const;
	/// �ȴ�wave bank׼�����
	void WaitForWaveBankPrepared();

public:
	/// ���캯��
	CXACTAudioEngine();
	/// ��������
	virtual ~CXACTAudioEngine();

	void ReleaseEngine();
	bool RestoreXactEngine();
	void RestoreVolume();

private:
	CSoundBank* CreateSoundBank();
	CCue* CreateCue();
	CWave* CreateWave();

	bool OpenXACT();
	void CloseXACT();

	index_t GetDefaultAdapterIndex() const;

	bool LoadGlobalSettings();

private:
	/// �����߳�
	static void ThreadProc(void* pParam);
	static void WINAPI NotificationCallback(const XACT_NOTIFICATION* notification);
	static const size_t MAX_DSP_CHANNELS = 16;
	//audio device invalid��ʱ���ر���engineû��inialize��ʱ��dowork����ʧ�ܣ���ֵΪfalse��ʱ���̲߳���
	static bool isNeedUpdateThread;
	static bool isCloseAudioEngine;
	static bool isDetachXactEngine;
	friend class CXACTCue;
private:
	IXACT3Engine* xactEngine;
	void* globalSettingsBuffer;
	DWORD globalSettingsBufferSize;
	X3DAUDIO_DSP_SETTINGS dspSettings;
	FLOAT32 dspMatrixCoefficients[MAX_DSP_CHANNELS];
	FLOAT32 dspDelayTimes[MAX_DSP_CHANNELS];
	X3DAUDIO_HANDLE x3dAudioHandle;

	HANDLE waveBankPreparedEvent;
	HTHREAD updateThread;

	AMap<AString, float> mapVariableVolumes;
};

//------------------------------------------------------------------------------
/// ��ʱ��Ū������ʱ�Ķ�̬
inline CAudioEngine* GetAudioEngine()
{
	return CXACTAudioEngine::GetInst();
}

//------------------------------------------------------------------------------
inline IXACT3Engine* CXACTAudioEngine::GetXACTEngine() const
{
	Ast(NULL != this->xactEngine);
	return this->xactEngine;
}

}// namespace sqr

