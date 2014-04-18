#pragma once
#include "scene.h"
#include "CMapEditObject.h"

namespace sqr_tools
{
	class CEditModel;
}

namespace sqr
{
	class CPieceGroup;
	class IEffect;
	class IEffectGroup;
	class CWireBox;

	class CMapEditRgnAmbFxObj : public CMapEditObject
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

		///����ֵ��ʾ����Ч�Ƿ�Ϊ�����ͣ״̬
		bool		 IsNeedRender(DWORD dwTime);

	public:

		CMapEditRgnAmbFxObj();

		virtual ~CMapEditRgnAmbFxObj();
		virtual void  Render();
		
		virtual void  SetPlayTimeLenMin( const DWORD dwTimeMin );
		virtual void  SetPlayTimeLenMax( const DWORD dwTimeMax );
		virtual void  SetPlayIntervalMin( const DWORD dwTimeMin );
		virtual void  SetPlayIntervalMax( const DWORD dwTimeMax );
		virtual DWORD GetPlayTimeLenMin();
		virtual DWORD GetPlayTimeLenMax();
		virtual DWORD GetPlayIntervalMin();
		virtual DWORD GetPlayIntervalMax();
		virtual void  SetNeedPlayBeginMark();
	};
}
