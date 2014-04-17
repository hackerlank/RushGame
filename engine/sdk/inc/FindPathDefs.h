#pragma once

#include "TCoreSceneAllocator.h"

namespace sqr
{
	//Ѱ·3��ģʽ��1��ֱ�ߣ������ϰ�����NULL����2��a*��3��ֱ�ߣ������ϰ��󷵻�ǰһ�������꣩
	//             4��ֱ�ߺ���Ŀ���ϰ�          5��a* ����Ŀ���ϰ�

	//*********����ע�⣬ĿǰѰ·�������֧��8�֣����Ҫ���ӳ���8�֣���Ҫ�޸�����Э���е�usbFindPathType��λ��************
	enum EFindPathType
	{
		eFPT_Line = 0,       // ֱ��Ѱ·
		eFPT_HypoLine,       // ֱ��Ѱ· -> ����ֱ�ߵ�һ���ϰ���Ѱ·
		eFPT_LineIgnoreEnd,  // ֱ��Ѱ·�����յ�Ϊ�ϰ�

		eFPT_AStar,          // ֱ��Ѱ· -> A*Ѱ·
		eFPT_HypoAStar,      // ֱ��Ѱ· -> A*Ѱ· -> ����ֱ�ߵ�һ���ϰ���Ѱ·(����յ����ϰ���ֱ����HypolineѰ·)
		eFPT_TerrainAStar,   // ���ǵ���Ӱ���A*Ѱ· -> ����ֱ�ߵ�һ���ϰ���Ѱ·(����յ����ϰ���ֱ����HypolineѰ·)
		eFPT_AStarIgnoreEnd, // A*Ѱ·�����յ�Ϊ�ϰ�
		eFPT_HypoAStarOpti,   // Optimization Hypo AStar����Ŀ����޷�����ʱ��ѡ���յ���������ΪѰ·�յ�

		eFPT_AStarUseWeight,	//����Ȩֵ�� A*Ѱ·
		eFPT_HypoAStarOptiUseWeight,	//����Ȩֵ�� eFPT_HypoAStarOptiѰ·, ��ֱ��ʹ��A* ��Ԥ��ʹ��ֱ��Ѱ·(ֱ��Ѱ·�޷�����Ȩֵ, ��ֱ����ͨʱ��һ����Ԥ��Ľ��)

		eFPT_Custom, //�Զ���·��
	};

	//*********����ע�⣬Ŀǰ�ϰ��������֧��64�֣����Ҫ���ӳ���64�֣���Ҫ�޸�����Э���е�usbBarrierType��λ��************
	enum EBarrierType
	{
		eBT_OutRange = -1,
		eBT_NoBarrier = 0,
		eBT_LowBarrier = 1,
		eBT_MidBarrier = 2,
		eBT_HighBarrier = 3,
	};

	class IPath;
	class IMap;

}
