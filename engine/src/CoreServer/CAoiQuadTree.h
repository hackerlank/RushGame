#pragma once
#include "TQuadTree.h"
#include "CTraitsAoiQuad.h"

namespace sqr
{
	
	template<class T> struct TRect;
	typedef TRect<float>	CFRect;
	
	class CAoiQuadNode;
	class CSynAoiDimObj;

	//�Ĳ����ڵ�
	class CAoiQuadTree : public TQuadTree<CTraitsAoiQuad>
	{
	public:
		typedef TQuadTree<CTraitsAoiQuad>	Parent_t;
		typedef CSynAoiDimObj				AoiObject;
		friend class CSynAoiDimScene;

		CAoiQuadTree(uint32 uWidth, uint32 uHeight, uint32 uMinSpliteSize);
		~CAoiQuadTree();

		/*void InsertSizeRectToQuad(AoiObject* pObj);
		void InsertViewRectToQuad(AoiObject* pObj);
		void RemoveSizeRectFrQuad(AoiObject* pObj);
		void RemoveViewRectFrQuad(AoiObject* pObj);*/

		void SetAoiPos(AoiObject* pObject, const CFPos& NewPos);
		void SetAoiSizeRect(AoiObject* pObject);
		void SetAoiViewRect(AoiObject* pObject);

		typedef vector<AoiObject*, TQuadAllocator<AoiObject*> > VecObject_t;

		void QueryObservee(VecObject_t& vecObj, const CFPos& pos, 
						 float fAoiRadius);
		void QueryObservee(VecObject_t& vecObj, CFRect& rect);	

		void QueryObservee(AoiObject* pObj, VecObject_t& vecObj);
		void QueryObservee(AoiObject* pObj, VecObject_t& vecObj, const CFPos& NewPos);
		void QueryObservee(AoiObject* pObj, VecObject_t& vecObj, const CFRect& OldRect, const CFRect& NewRect);

		//��ѯ�ܿ����ҵĶ���
		void QueryObserver(VecObject_t& vecObserver, CFRect& rect);
		void QueryObserver(AoiObject* pObj, VecObject_t& vecObserver);
		void QueryObserver(AoiObject* pObj, VecObject_t& vecObserver, const CFPos& NewPos);
		void QueryObserver(AoiObject* pObj, VecObject_t& vecObj, const CFRect& OldRect, const CFRect& NewRect);

	private:	
		void ResetAoiObject(AoiObject* pObj, const CFPos& NewPos);
		
	private:
		//�����Ĳ������ѷ���������aoi�������vector�У�����ǰ�����
		//�������ܿ����Ķ���
		static void TravelQuadForObservee(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& rect);
		static void TravelQuadForObservee(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& MyOldRect, const CFRect& MyNewRect);

		//���ϱ������ܿ����Ķ���
		static void TravelUpQuadForObservee(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& rect);
		static void TravelUpQuadForObservee(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& MyOldRect, const CFRect& MyNewRect);
		
		//���±������ܿ����Ķ���
		static void TravelDownQuadForObservee(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& rect);
		static void TravelDownQuadForObservee(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& MyOldRect, const CFRect& MyNewRect);

		//�����ܿ����ҵĶ���
		static void TravelQuadForObserver(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& rect);
		static void TravelQuadForObserver(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& MyOldRect, const CFRect& MyNewRect);

		//���ϱ����ܿ����ҵĶ���
		static void TravelUpQuadForObserver(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& rect);
		static void TravelUpQuadForObserver(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& MyOldRect, const CFRect& MyNewRect);

		//���±����ܿ����ҵĶ���
		static void TravelDownQuadForObserver(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& rect);
		static void TravelDownQuadForObserver(CAoiQuadNode* pNode, VecObject_t&vecObj, const CFRect& MyOldRect, const CFRect& MyNewRect);
	
		typedef list<CSynAoiDimObj*, TQuadAllocator<CSynAoiDimObj*> > ListQuadObject_t;
	};

}

