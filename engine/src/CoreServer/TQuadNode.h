#pragma once 

#include <list>
#include "CRectangle.h"
#include "CPos.h"
#include "CQuadMallocObject.h"
#include "TQuadAllocator.h"

namespace sqr
{
	template<class T> struct TRect;
	typedef TRect<float>	CFRect;

	template<typename Traits>
	class TQuadTree;

	class CMemObjectPool;

	template<typename Traits>
	class TQuadNode : public CQuadMallocObject
	{
	protected:
		typedef typename Traits::QuadTree	ImpQuadTree_t;
		typedef typename Traits::QuadNode	ImpQuadNode_t;
		typedef typename Traits::QuadObject	ImpQuadObject_t;

		friend class TQuadTree<Traits>;
#ifdef _WIN32
		friend ImpQuadTree_t;
#else
		friend class Traits::QuadTree;
#endif

		TQuadNode(ImpQuadNode_t* pParentNode, const CFRect& rcBox);
		~TQuadNode();

		void AddSizeRectToNode(ImpQuadObject_t* pObj);
		void RemoveSizeRectFromNode(ImpQuadObject_t* pObj);

		void AddViewRectToNode(ImpQuadObject_t* pObj);
		void RemoveViewRectFromNode(ImpQuadObject_t* pObj);

		void AddObjectToNode(ImpQuadObject_t* pObj);
		void RemoveObjectFromNode(ImpQuadObject_t* pObj);

		//�����ӽڵ��Լ�ɾ���ӽڵ�
		void CreateChildNodes();
		void DeleteChildNodes();

		//�鲢�ӽڵ㣬���ӽڵ�������Ƶ����ڵ����棬����ɾ���ӽڵ�
		//�������ֻ��ʹ����ֻ��һ���ӽڵ�ĸ��ڵ����棬��Ϊ����ֻ���ܴ�������ײ�Ľڵ�����
		void MergeChildNodes();
		bool BeMergeChildNodesNeeded();

		void MergeChildNodeData(ImpQuadNode_t* pParent,ImpQuadNode_t* pChild);

		//�з�ƽ��󣬸��ڵ��һЩ���ݻ��ƶ���Ҷ�ӽڵ�����
		void SetChildNodesData();	

		float QuadNodeBoxWidth() const;
		float QuadNodeBoxHeight() const;

	protected:
		ImpQuadNode_t*	m_pParentNode;    //���ڵ�

		//�Ĳ����ĸ��ӽڵ�
		/*
		NW  |  NE
		---------
		SW  |  SE
		*/
		ImpQuadNode_t*	m_pNEChildNode;
		ImpQuadNode_t*	m_pNWChildNode;
		ImpQuadNode_t*	m_pSWChildNode;
		ImpQuadNode_t*	m_pSEChildNode;

		typedef std::list<ImpQuadObject_t*, TQuadAllocator<ImpQuadObject_t*> > ListQuadObject_t;

		ListQuadObject_t			m_lstObject;		//���ض��󣬶���ֻ����ײ��ӽڵ���ܹ���
		ListQuadObject_t			m_lstViewRect;		//���ض���view rect��ֻ��view rect��ȫ�ڸ�node��box������ܹ���
		ListQuadObject_t			m_lstSizeRect;		//���ض���size rect��ֻ��size rect��ȫ�ڸ�node��box������ܹ���
		

		CFRect				m_rcQuadNodeBox;		//�ýڵ��Ӧƽ�����
	};
		

}

