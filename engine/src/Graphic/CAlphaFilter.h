#pragma once
#include "CDefaultFilter.h"

/*������ȷAlpha��ϵ�����*/

namespace sqr
{
	class CAlphaFilter : public CDefaultFilter
	{
	public:
		CAlphaFilter(ERenderObjStyle eRos,CRenderGroup* pRenderGroup);
		virtual ~CAlphaFilter();
	public:
		FilterNode* Filter(FilterNode* fn);
	};
}
