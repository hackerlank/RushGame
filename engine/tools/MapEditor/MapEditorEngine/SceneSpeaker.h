#pragma once
#include "CVector3.h"

namespace bly
{
class CSceneSpeaker
{
private:
	CVector3f m_vPos; //����λ��
	std::string m_strEventName; //���������ŵ��¼���
	float m_fMinDistance; //��С��Χ
	float m_fMaxDistance; //���Χ
	float m_fVolume; //����

	std::string m_strInfo; // ��ʶ

// 		ISound* m_pSound; //����

public:
	bool m_bIsSceneSpeaker;

	//���죬����
	CSceneSpeaker( );
	~CSceneSpeaker( );

	void SetPos(CVector3f& vPos);
	CVector3f* GetPos();

	void SetEventName(std::string & strEventName);
	std::string* GetEventName();

	void SetMinDistance(float fMinDistance);
	float GetMinDistance();

	void SetMaxDistance(float fMaxDistance);
	float GetMaxDistance();

	void SetVolume(float fVolume);
	float GetVolume();

	void SetInfo(std::string & strInfo);
	std::string* GetInfo();

	//void PlaySound();
	void PlaySceneSound();
	void StopSound();
};
}