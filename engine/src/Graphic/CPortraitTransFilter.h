#pragma once
#include "CDefaultFilter.h"
/**
@class CPortraitTransFilter

ͷ������vs������ģ��
*/

namespace sqr
{
	class CPortraitTransFilter : public CDefaultFilter
	{
	public:
		CPortraitTransFilter(ERenderObjStyle eRos,CRenderGroup* pRenderGroup);
		virtual ~CPortraitTransFilter();
	public:
		FilterNode* Filter(FilterNode* fn);
	};

	class CNormalTransFilter : public CDefaultFilter
	{
	public:
		CNormalTransFilter(ERenderObjStyle eRos,CRenderGroup* pRenderGroup);
		virtual ~CNormalTransFilter();
	public:
		void		Render(void);
		FilterNode* Filter(FilterNode* fn);
	};
}
