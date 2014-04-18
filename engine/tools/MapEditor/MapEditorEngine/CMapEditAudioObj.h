#pragma once
#include "scene.h"
#include "CMapEditObject.h"

namespace sqr
{
	class CMapEditAudioObj : public CMapEditObject
	{
		DWORD	m_dwPlayTimeLenMin;		///����ʱ�䳤����Сֵ
		DWORD	m_dwPlayTimeLenMax;		///����ʱ�䳤�����ֵ
		DWORD	m_dwPlayIntervalMin;	///2�β���ʱ����������Сֵ
		DWORD	m_dwPlayIntervalMax;	///2�β���ʱ�����������ֵ
		DWORD	m_dwCurRandomPlayTime;	///��ǰ�������ʱ�䳤��
		DWORD	m_dwCurRandomInterval;	///��ǰ���2�β���ʱ��������
		DWORD	m_dwPlayTime;			///��ǰ�Ѿ����Ŷ��ʱ��
		DWORD	m_dwIntervalTime;		///��ǰ������ʱ��
		bool	m_bNeedPlayBegin;
		bool	m_bNeedIntervalBegin;
		
	public:

		CMapEditAudioObj();

		virtual ~CMapEditAudioObj();
		virtual void Render();

		virtual void SetSceneCueName(const string& name);	

		virtual string GetSceneCueName() const
		{
			return m_strSceneCueName;
		}

		virtual void StopSceneCue( const bool bStop, const bool bAuto = true );
		virtual bool IsNeedPlay();
		virtual bool PlaySceneCue(DWORD dwTime);
		virtual bool GetIsStopSound();

		virtual void	SetPlayTimeLenMin( const DWORD dwTimeMin );
		virtual void	SetPlayTimeLenMax( const DWORD dwTimeMax );
		virtual void	SetPlayIntervalMin( const DWORD dwTimeMin );
		virtual void	SetPlayIntervalMax( const DWORD dwTimeMax );
		virtual DWORD	GetPlayTimeLenMin();
		virtual DWORD	GetPlayTimeLenMax();
		virtual DWORD	GetPlayIntervalMin();
		virtual DWORD	GetPlayIntervalMax();
		virtual DWORD	GetPlayTimeLen()
		{
			return m_dwCurRandomPlayTime;
		}
		virtual DWORD	GetCurPlayIntervalTime()
		{
			return m_dwCurRandomInterval;
		}

		virtual void	SetNeedPlayBeginMark();
		
		virtual void	AddObjectRenderDialog(CTObject *pObject);
		virtual void	SetVisible(bool b);
		virtual void	HideSelf();
		virtual void	ShowSelf();
		virtual void    SetLocked(bool b);
		
	};
}
