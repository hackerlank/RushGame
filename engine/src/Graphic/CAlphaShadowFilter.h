#pragma once
#include "CDefaultFilter.h"
namespace sqr
{
	//Ӱ�ӷ�����
	class CAlphaShadowFilter : public CDefaultFilter
	{
	public:
		CAlphaShadowFilter(ERenderObjStyle eRos,CRenderGroup* pRenderGroup)
			:CDefaultFilter(eRos,pRenderGroup) {};
		FilterNode* Filter(FilterNode* fn);
	};
}