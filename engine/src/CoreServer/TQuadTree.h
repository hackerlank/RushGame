#pragma once
#include "CPos.h"
#include "CRectangle.h"
#include "CQuadMallocObject.h"
#include "TQuadAllocator.h"

namespace sqr
{
	template<typename T> struct TRect;
	typedef TRect<float> CFRect;

	template<typename Traits>
	class TQuadNode;

	template<typename Traits>
	class TQuadTree : public CQuadMallocObject
	{
	protected:
		typedef typename Traits::QuadNode		ImpQuadNode_t;
		typedef typename Traits::QuadObject		ImpQuadObject_t;

		friend class TQuadNode<Traits>;

		TQuadTree(uint32 uWidth, uint32 uHeight, uint32 uMinSpliteSize);
		~TQuadTree();

		const CFRect& GetSceneBox() const;  //�õ��Ĳ�����Ӧ�ĳ���ʵ�ʵ�rc box

		void InsertSizeRectToQuad(ImpQuadObject_t* pObj);
		void InsertViewRectToQuad(ImpQuadObject_t* pObj);
		void RemoveSizeRectFrQuad(ImpQuadObject_t* pObj);
		void RemoveViewRectFrQuad(ImpQuadObject_t* pObj);

		void ResetViewRectInQuad(ImpQuadObject_t* pObj, const CFPos& NewPos);
		void ResetSizeRectInQuad(ImpQuadObject_t* pObj, const CFPos& NewPos);
		
		//�������������Ƕ����ƶ���ʱ����õ�	
		void InsertObjectToQuad(ImpQuadObject_t* pObject);
		void RemoveObjectFrQuad(ImpQuadObject_t* pObject);

		//ƽ�滮�ֲ��ԣ����Ƿ���Ҫ����ƽ���з�
		//�Ժ���ܻ����������л��ֲ��ԣ����Կ��Բ���strategyģʽ
		bool BeNeedQuadNodeSplited(ImpQuadNode_t* pNode, ImpQuadObject_t* pObject);

		//����position���ڵ�node
		static ImpQuadNode_t* FindUpQuadNodeWithPos(const CFPos& pos, ImpQuadNode_t* pNode);
		static ImpQuadNode_t* FindDownQuadNodeWithPos(const CFPos& pos, ImpQuadNode_t* pNode);
		
		//������С����rect��node
		static ImpQuadNode_t* FindUpQuadNodeWithRect(const CFRect& rcRect, ImpQuadNode_t* pNode);
		static ImpQuadNode_t* FindDownQuadNodeWithRect(const CFRect& rcRect, ImpQuadNode_t* pNode);

		static ImpQuadNode_t* SelectChildNodeWithPos(const CFPos& pos, ImpQuadNode_t* pNode);
		static ImpQuadNode_t* SelectChildNodeWithRect(const CFRect& rcRect, ImpQuadNode_t* pNode);

	protected:
		ImpQuadNode_t*		m_pRootNode;
		uint32				m_uMinSpliteSize;		//��С����ƽ��
		CFRect				m_rcSceneBox;

		typedef std::list<ImpQuadObject_t*, TQuadAllocator<ImpQuadObject_t*> > ListQuadObject_t;
	};

}

