#pragma once
//��shell��¶��ͷ�ļ�����Ҫ��������ר�õĴ���

namespace sqr
{
	enum EServerObjectType
	{
		eSOT_Dictator,
		eSOT_Mediator,
		eSOT_Calculator
	};

	enum EServerObjectFlag
	{
		eSOF_None,
		eSOF_GridPosChangedNotifycation	=	1
	};
}

