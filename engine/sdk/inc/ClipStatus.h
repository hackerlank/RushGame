#pragma once
//------------------------------------------------------------------------------
/**
    @struct ClipStatus 

	�ü�״̬.
	CLR����ö�ٶ���, ��ֹȫ�����ֳ�ͻ.

    (C) 2009 ThreeOGCMan
*/

//------------------------------------------------------------------------------
namespace sqr
{

struct ClipStatus
{
	enum Type
	{
		Outside,	///< �ⲿ
		Intersect,	///< �ཻ
		Inside,		///< �ڲ�

		Invalid
	};
};

}// namespace sqr