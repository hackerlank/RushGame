#pragma once

#include "FindPathDefs.h"
#include "CFindPathAllocator.h"
#include <boost/shared_ptr.hpp>

namespace sqr
{
	template<class T> struct TPos;
	typedef TPos<int32> CPos;

	class CMapInfoManager;
	class IDynamicBarrierInfo;


	class CBarrierManager
		:public CFindPathMallocObject
	{
		typedef vector<uint8,CFindPathAllocator<uint8> > VecBarrierMatrix;
	public:
		typedef boost::shared_ptr<VecBarrierMatrix> VecBarrierMatrixSharedPtr;
		CBarrierManager(CMapInfoManager* pMapInfo, const VecBarrierMatrixSharedPtr* pVecBarrierMaxtrixSharedPtr);
		~CBarrierManager();

		void SetJumpBarrier(EBarrierType eBarrier,EFindPathType EFindType,uint32 uBeginIndex,uint32 uEndIndex);
		bool JumpBarrier(uint32 uIndex)const;
		bool JumpBarrierAndGetWeight(uint32 uIndex, float& uBarrierWeight)const;
		bool IsBarrier(int32 x, int32 y)const;
		bool CheckRangeBarrier(const CPos& grid)const;
		void SetDynamicBarrier(const IDynamicBarrierInfo* pDynamicBarrier);
		EBarrierType GetQuarBarrierType(int32 x, int32 y)const;			//ֻ��ȡ��̬�ϰ���Ϣ	
		void SetBarrier(uint32 x, uint32 y, uint8 bVal);
		EBarrierType GetBarrierType(int32 x, int32 y)const;
		const VecBarrierMatrixSharedPtr* GetBarrierMatrix()const;

	private:
		EBarrierType GetDynamicBarrierType(uint32 uIndex)const;
		CMapInfoManager*  m_MapInfo;
		EBarrierType m_eJumpBarrier;         // ��ǰѰ·���ܺ��Ե��ϰ��ȼ�������μ�CNodeState::JumpBarrier
		uint32 m_uBeginIndex, m_uEndIndex;   // ����Ѱ·�Ŀ�ʼ�ͽ�����
		EFindPathType m_eFindType;           // ����Ѱ·ģʽ
		float*		  m_pBarrierWeight;			//���浫ǰѰ·ģʽ��ͬ�ϰ����Ͷ�Ӧ��Ȩֵ����

		VecBarrierMatrixSharedPtr	m_pBarrier;	// �ϰ�����
		const IDynamicBarrierInfo*	m_pDynamicBarrier;      // ��̬�ϰ���Ϣ
	};

}
