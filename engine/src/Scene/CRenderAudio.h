#pragma once
#include "CRenderNode.h"
#include "CRenderSpaceNode.h"
#include "IAudioEmitter.h"

namespace sqr
{
class CMetaRegion;
struct CRegionAI;

class CRenderAudio 
	: public CRenderNode
{
	uint16			m_uUnitSetIndex;
	uint32			m_uPlayTimeLenMin;		///����ʱ�䳤����Сֵ
	uint32			m_uPlayTimeLenMax;		///����ʱ�䳤�����ֵ
	uint32			m_uPlayIntervalMin;		///2�β���ʱ����������Сֵ
	uint32			m_uPlayIntervalMax;		///2�β���ʱ�����������ֵ
	uint32			m_uCurRandomPlayTime;	///��ǰ�������ʱ�䳤��
	uint32			m_uCurRandomInterval;	///��ǰ���2�β���ʱ��������
	uint32			m_uPlayTime;			///��ǰ�Ѿ����Ŷ��ʱ��
	uint32			m_uIntervalTime;		///��ǰ������ʱ��
	bool			m_bPlayNotRepeat;
	bool			m_bNeedPlayBegin;
	bool			m_bNeedIntervalBegin;

	IAudioEmitterPtr m_pAEmitter;
	SString			 m_strAudioName;
	bool			 m_bPlayError;
	bool	IsNeedPlay();
	uint32	GetRandomValue(uint32 nLowBound, uint32 nHighBound);
public:
	CRenderAudio(const CRegionAI& AudioInfo, const SString& AudioName,ILogicHandler* pHandler = NULL );
	~CRenderAudio();

	void	OnPreRender( uint32 uCurTime,CCamera* pCamera );
	void	Render( void );
	bool	UpdateTime(uint32 uCurTime){ return false; };


	const AniResult		Render( uint32 uCurTime, RenderObjStyle* pRORS = NULL );
	void				SetVisible( bool isVisible );
	ERenderObjType		GetType(void)	{ return ROT_AUDIO; };
};

}