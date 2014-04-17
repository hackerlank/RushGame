#pragma once

#include "CCtrlBase.h"

namespace sqr_tools
{
	class CAmbientAudioCtrlImp :public CCtrlBase
	{
		REG_UI_IMP(CAmbientAudioCtrlImp);

	public:
		CAmbientAudioCtrlImp(void* param);
		~CAmbientAudioCtrlImp(void);

		struct SCueTimePro
		{
			int	nPlayTimeLenMin;	///����ʱ�䳤����Сֵ
			int	nPlayTimeLenMax;	///����ʱ�䳤�����ֵ
			int	nPlayIntervalMin;	///2�β���ʱ����������Сֵ
			int	nPlayIntervalMax;	///2�β���ʱ�����������ֵ

			SCueTimePro()
			{
				nPlayTimeLenMax = nPlayTimeLenMin = nPlayIntervalMax = nPlayIntervalMin = 0;
			}
		};

		typedef vector<SCueTimePro> cueAttributePool;

	private:
		cueAttributePool	m_cueAttributeList;
		vector<int>			m_nAudioRangeList;
		int					m_nAudioRange;

	public:
 		virtual void	Update();
 		virtual bool	_ProcMsg(const CMsg& msg );
		virtual void	SetContext(CEditContext* pContext);

		virtual void	LoadCueTimeConfig();

		virtual size_t	GetCueAttributeCount();
		virtual void	SetCuePlayTimeLenMin(const size_t index, const int time);
		virtual int		GetCuePlayTimeLenMin(const size_t index);
		virtual void	SetCuePlayTimeLenMax(const size_t index, const int time);
		virtual int		GetCuePlayTimeLenMax(const size_t index);
		virtual void	SetCueIntervalTimeLenMin(const size_t index, const int time);
		virtual int		GetCueIntervalTimeLenMin(const size_t index);
		virtual void	SetCueIntervalTimeLenMax(const size_t index, const int time);
		virtual int		GetCueIntervalTimeLenMax(const size_t index);

		virtual size_t	GetAudioRangeCount();
		virtual int		GetAudioRange(const size_t index);
	};

}
