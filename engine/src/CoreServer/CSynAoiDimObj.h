#pragma once 
#include "CPos.h"
#include "CBaseAoiObj.h"
#include "CRectangle.h"
#include "CQuadMallocObject.h"
#include "TQuadAllocator.h"

namespace sqr
{
	class CSynAoiViewObj;
	class CSynAoiDimScene;
	class CTraitsAoiQuad;
	class CAoiQuadNode;

	template<typename ObjectType>
	class TQuadNode;
	
	template<typename ObjectType>
	class TQuadTree;

	class CSynAoiDimObj : public CBaseAoiObj, public CQuadMallocObject
	{
		friend class CSynAoiDimScene;

		friend class TQuadNode<CTraitsAoiQuad>;
		friend class TQuadTree<CTraitsAoiQuad>;
		friend class CAoiQuadTree;

		typedef vector<CSynAoiDimObj*, TQuadAllocator<CSynAoiDimObj*> >	VecDimObject_t;

	public:
		CSynAoiDimObj(CSynAoiDimScene* pDimScene, CSynAoiViewObj* pViewObj, const CFPos& pos, uint32 uDimension);
		~CSynAoiDimObj();

		void Release();

		CSynAoiDimScene* GetScene() const
		{
			return m_pDimScene;
		}


		void SetSize( float fSize );
		void SetEyeSight( float fEyeSight );
		void SetStealth( float fStealth );
		void SetKeenness(float Keenness);
		void SetViewGroup(uint32 uGroupID);
		
		void SetPos(const CFPos& PixelPos);
		void ForceSetPos(const CFPos& PixelPos);	//�ƶ�������ǿ�Ƹ��������

		float GetSize() const; 
		float GetEyeSight() const; 
		float GetStealth() const; 
		float GetKeenness() const; 
		uint32 GetViewGroup() const; 
		
		void InsertToScene();
		void RemoveFrScene();

		//�����Ƿ��ܿ����Է���true�����ܿ�����false�Ͳ��� 
		bool CanSee(CSynAoiDimObj *pOther); 

		void PostMsgToViewee(const void* pContext);
		void PostMsgToViewer(const void* pContext);

		void PostMsgToSyncee(const void* pContext);
		void PostMsgToSyncer(const void* pContext);

		const CFPos& GetPosition() const;

	private:
		
		void OnViewRelationChanged(CSynAoiDimObj* pViewObj,
			const CFPos& MyAoiPos,const CFPos& OtAoiPos,bool bSee);
		void OnViewRelationKept(CSynAoiDimObj* pOtObj,
			const CFPos& MyOldAoiPos,const CFPos& MyAoiPos,
			const CFPos& OtOldGridPos,const CFPos& OtAoiPos);

		void OnMsgFromViewer(const void* pContext);
		void OnMsgFromViewee(const void* pContext);
	
		void OnMsgToVieweeHandled(const void* pContext);
		void OnMsgToViewerHandled(const void* pContext);

		void OnMsgFromSyncer(const void* pContext);
		void OnMsgFromSyncee(const void* pContext);

		void OnMsgToSynceeHandled(const void* pContext);
		void OnMsgToSyncerHandled(const void* pContext);

		void SetViewRect(const CFPos& NewPos, const CFRect& rcMaxBox);
		const CFRect& GetViewRect() const;

		void SetSizeRect(const CFPos& NewPos, const CFRect& rcMaxBox);
		const CFRect& GetSizeRect() const;
		
		void IntSetPos(const CFPos& GridPos);

		void SetAoiPos(const CFPos& NewPos);
		
	private:
		void ModifyObservationProperty(float fNewEyeSight,float fNewKeenness);
		void ModifyVisibilityProperty(float fNewSize,float fNewStealth);
		void ModifyViewGroupProperty(uint32 uNewViewGroup);

		
		//��ѯ���ҵ�view rect����Ķ���
		void QueryObserveeWithViewRect(VecDimObject_t& vecObject)const;
		void QueryObserveeWithViewRect(CSynAoiDimObj* pObj, VecDimObject_t& vecObject)const;
		//��ѯ���ڶԷ�view rect����Ķ���
		void QueryObserverWithSizeRect(VecDimObject_t& vecObject) const;
		void QueryObserverWithSizeRect(CSynAoiDimObj* pObj, VecDimObject_t& vecObject) const;

		//�������к�����ʱ�򣬶����λ���Ѿ����µ�������
		//�����ǲ��ԳƵ�aoi�ˣ��������ܿ����Ķ�����ܿ����ҵĶ���Ҫ�ֱ����
		void UpdateSeeRelationForMyEnterObject(const VecDimObject_t& vecObj, float NewEyeSight);
		void UpdateSeeRelationForMyLeaveObject(const VecDimObject_t& vecObj, const CFPos& pos, float OldEyeSight);
		void UpdateSeeRelationForMyStayObject(const CFPos& pos, float OldEyeSight, float NewEyeSight);
		void UpdateSeeRelationForOtEnterObject(const VecDimObject_t& vecObj, float NewSize);
		void UpdateSeeRelationForOtLeaveObject(const VecDimObject_t& vecObj, const CFPos& pos, float OldSize);
		void UpdateSeeRelationForOtStayObject(const CFPos& pos, float OldSize, float NewSize);
		
		void UpdateSeeRelationForMyEnterObject(const VecDimObject_t& vecObj);
		void UpdateSeeRelationForMyLeaveObject(const VecDimObject_t& vecObj,const CFPos& OldGridPos);
		void UpdateSeeRelationForMyStayObject(const CFPos& OldGridPos);

		void UpdateSeeRelationForOtEnterObject(const VecDimObject_t& vecObj);
		void UpdateSeeRelationForOtLeaveObject(const VecDimObject_t& vecObj,const CFPos& OldGridPos);
		void UpdateSeeRelationForOtStayObject(const CFPos& OldGridPos);

	private:
		CSynAoiViewObj*	m_pViewObj;
		CSynAoiDimScene*	m_pDimScene;

		CFRect m_rcViewRect;
		CFRect m_rcSizeRect;

#ifdef _WIN32
		typedef hash_set<CSynAoiDimObj*, hash_compare<CSynAoiDimObj*, less<CSynAoiDimObj*> >,
			TQuadAllocator<CSynAoiDimObj*> > ViewObjSet_t;
#else
		struct ViewObjHash
		{
			size_t operator() (const CSynAoiDimObj* pObj) const
			{
				size_t stPoint = (size_t)pObj;
				return stPoint;
			};
		};

		struct ViewObjEqual
		{
			bool operator() (const CSynAoiDimObj* pObj1, const CSynAoiDimObj* pObj2) const
			{
				return (size_t)pObj1 == (size_t)pObj2;
			}
		};

		typedef hash_set<CSynAoiDimObj*, ViewObjHash, ViewObjEqual, 
			TQuadAllocator<CSynAoiDimObj*> > ViewObjSet_t;
#endif	
		
		ViewObjSet_t		m_setObjInMyViewRect;		//���ҵ�view rect����Ķ���
		ViewObjSet_t		m_setObjInOtViewRect;		//���ڶԷ�view rect����Ķ���

		CAoiQuadNode*	m_pObjectNode;
		CAoiQuadNode*	m_pViewRectNode;
		CAoiQuadNode*	m_pSizeRectNode;

		typedef list<CSynAoiDimObj*, TQuadAllocator<CSynAoiDimObj*> > ListQuadObject_t;
		ListQuadObject_t::iterator	m_ItObject;
		ListQuadObject_t::iterator	m_ItViewRect;
		ListQuadObject_t::iterator	m_ItSizeRect;
		
		static ListQuadObject_t	ms_DummyList;

		//����ÿһ��aoi object��Ӧһ���������Զ����һЩ��������������Ҫ�õ�����aoi ���� 
		float			m_fSize;		//���
		float			m_fEyeSight;	//����(��Ұ��Χ)
		float			m_fStealth;		//���ζ�
		float			m_fKeenness;	//�����
		uint32			m_uViewGroup;	//�۲���	�۲�����ͬ������Object����Stealth��Keenness����

		CFPos			m_Pos;			
		uint32			m_uDimension;
	};

}
