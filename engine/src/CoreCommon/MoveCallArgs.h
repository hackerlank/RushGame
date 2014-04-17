#pragma once

namespace sqr
{
	template<class T> struct TPos;
	typedef TPos<int32> CPos;
	typedef TPos<float> CFPos;

	//��ΪMoveBeganDerived�����Ĳ������޸��������飬���Ի�������һ���ṹ��ʹ���Ժ����������һЩ
	struct CMoveBeganDerivedArgs
	{
		const CFPos&		EndPixelPos;
		float			fSpeed;
		EFindPathType	eFindPathType;
		EBarrierType	eBarrierType;
		float			fEndDist;
		const CPixelPath* pPixelPath;
	};

	struct CMovePathChangedDerivedArgs
	{
		const			CFPos& EndPixelPos;
		float			fSpeed;
		EFindPathType	eFindPathType;
		EBarrierType	eBarrierType;
		float			fEndDist;
		float			fMovedDist;
		const CPixelPath* pPixelPath;
	};

	struct CMoveSpeedChangedDerivedArgs
	{
		float			fSpeed;
	};
}

