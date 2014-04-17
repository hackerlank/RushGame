#include "stdafx.h"
#include "CSynAoiDimObj.h"
#include "CSynAoiViewObj.h"
#include "CAoiQuadNode.h"
#include "ExpHelper.h"
#include "CSynAoiDimScene.h"
#include "CRectangle.h"
#include "QuadTreeFun.h"
#include "TSqrAllocator.inl"

CSynAoiDimObj::ListQuadObject_t CSynAoiDimObj::ms_DummyList(1, NULL);

CSynAoiDimObj::CSynAoiDimObj(CSynAoiDimScene* pDimScene, CSynAoiViewObj* pViewObj, 
							 const CFPos& pos, uint32 uDimension)
: m_pViewObj(pViewObj)
, m_pDimScene(pDimScene)
, m_pObjectNode(NULL)
, m_pViewRectNode(NULL)
, m_pSizeRectNode(NULL)
, m_fSize(0)
, m_fEyeSight(0)
, m_fStealth(0)
, m_fKeenness(0)
, m_uViewGroup(0)
, m_Pos(pos)
, m_uDimension(uDimension)
{
	m_ItObject = ms_DummyList.begin();
	m_ItSizeRect = ms_DummyList.begin();
	m_ItViewRect = ms_DummyList.begin();
}

CSynAoiDimObj::~CSynAoiDimObj()
{

}

void CSynAoiDimObj::Release()
{
	delete this;
}

float CSynAoiDimObj::GetSize() const 
{
	return m_fSize;
}

float CSynAoiDimObj::GetEyeSight() const 
{
	return m_fEyeSight;
}

float CSynAoiDimObj::GetStealth() const 
{
	return m_fStealth;
}

float CSynAoiDimObj::GetKeenness() const 
{
	return m_fKeenness;
}

uint32 CSynAoiDimObj::GetViewGroup() const
{
	return m_uViewGroup;
}

const CFPos& CSynAoiDimObj::GetPosition()const
{
	return m_Pos;
}

bool CSynAoiDimObj::CanSee(CSynAoiDimObj *pObj) 
{
	if(!pObj)
		return true;

	CFPos MyPos = GetPosition();
	CFPos OpPos = pObj->GetPosition();

	return CalSeeRelation(MyPos,OpPos,m_fEyeSight,pObj->m_fSize,
		m_fKeenness,pObj->m_fStealth,m_uViewGroup,pObj->m_uViewGroup);
}


void CSynAoiDimObj::OnViewRelationChanged(CSynAoiDimObj* pOtObj, const CFPos& MyAoiPos,
												  const CFPos& OtAoiPos,bool bSee)
{
	m_pViewObj->OnViewRelationChanged(pOtObj->m_pViewObj, MyAoiPos, OtAoiPos, bSee, m_uDimension);
}


void CSynAoiDimObj::OnViewRelationKept(CSynAoiDimObj* pOtObj,
											   const CFPos& MyOldAoiPos,const CFPos& MyAoiPos,
											   const CFPos& OtOldAoiPos,const CFPos& OtAoiPos)
{
	m_pViewObj->OnViewRelationKept(pOtObj->m_pViewObj, MyOldAoiPos, MyAoiPos,
		OtOldAoiPos, OtAoiPos, m_uDimension);
}

void CSynAoiDimObj::OnMsgFromViewer(const void* pContext)
{
	m_pViewObj->OnMsgFromViewer(pContext);
}

void CSynAoiDimObj::OnMsgFromViewee(const void* pContext)
{
	m_pViewObj->OnMsgFromViewee(pContext);
}

void CSynAoiDimObj::OnMsgToVieweeHandled(const void* pContext)
{
	m_pViewObj->OnMsgToVieweeHandled(pContext);
}

void CSynAoiDimObj::OnMsgToViewerHandled(const void* pContext)
{
	m_pViewObj->OnMsgToViewerHandled(pContext);
}

void CSynAoiDimObj::SetAoiPos(const CFPos& NewPos)
{
	m_Pos = NewPos;
}

void CSynAoiDimObj::SetViewRect(const CFPos& NewPos, const CFRect& rcMaxBox)
{
	GetRadiusRect(m_rcViewRect, NewPos, m_fEyeSight, rcMaxBox);
}

const CFRect& CSynAoiDimObj::GetViewRect() const
{
	return m_rcViewRect;
}


void CSynAoiDimObj::SetSizeRect(const CFPos& NewPos, const CFRect& rcMaxBox)
{
	GetRadiusRect(m_rcSizeRect, NewPos, m_fSize, rcMaxBox);
}

const CFRect& CSynAoiDimObj::GetSizeRect() const
{
	return m_rcSizeRect;
}


void CSynAoiDimObj::IntSetPos(const CFPos& NewAoiPos)
{
	CFPos MyOldAoiPos = GetPosition();
	
	CFPos MyNewAoiPos = NewAoiPos;

	static VecDimObject_t vecMyObjEnter,vecMyObjLeave,vecOtObjEnter, vecOtObjLeave;
	vecMyObjEnter.clear();
	vecMyObjLeave.clear();
	vecOtObjEnter.clear();
	vecOtObjLeave.clear();

	if (m_pViewRectNode)
		GetScene()->QueryObservee(this, vecMyObjLeave, MyNewAoiPos);
	if (m_pSizeRectNode)
		GetScene()->QueryObserver(this, vecOtObjLeave, MyNewAoiPos);

	GetScene()->SetAoiPos(this, MyNewAoiPos);

	if (m_pViewRectNode)
	{
		GetScene()->QueryObservee(this, vecMyObjEnter, MyOldAoiPos);
		UpdateSeeRelationForMyLeaveObject(vecMyObjLeave, MyOldAoiPos);
		UpdateSeeRelationForMyStayObject(MyOldAoiPos);
		UpdateSeeRelationForMyEnterObject(vecMyObjEnter);
	}
	if (m_pSizeRectNode)
	{
		GetScene()->QueryObserver(this, vecOtObjEnter, MyOldAoiPos);
		UpdateSeeRelationForOtLeaveObject(vecOtObjLeave, MyOldAoiPos);
		UpdateSeeRelationForOtStayObject(MyOldAoiPos);
		UpdateSeeRelationForOtEnterObject(vecOtObjEnter);
	}
	
}

void CSynAoiDimObj::SetPos(const CFPos& PixelPos)
{
	//���첽�Ǳ��Ѿ��ж������Ƿ�һ�£�������ߵ�����һ���ǲ�һ�µ�
	const CFRect& rect = GetScene()->GetSceneBox();
	if(PixelPos.x < rect.left || PixelPos.x > rect.right ||
		PixelPos.y > rect.top || PixelPos.y < rect.bottom )
	{
		ostringstream str;
		str << "Invalid Pos " << PixelPos.x << "  " << PixelPos.y << endl;
		GenErr("CSynAoiDimObj::SetPos Error", str.str().c_str());
	}

	uint32 uMinOff = GetScene()->GetPrecision();
	if(abs((PixelPos.x - m_Pos.x)) >  uMinOff||
		abs((PixelPos.y - m_Pos.y)) > uMinOff )
		IntSetPos(PixelPos);
}

void CSynAoiDimObj::ForceSetPos(const CFPos& PixelPos)
{
	//���첽�Ǳ�û���ж������Ƿ�һ�£�����ǿ�Ƹ��¸ö������Ĳ�����������꣬���Ծ���
	const CFPos& OldPos = GetPosition();	//��������Ѿ������¹���
	if (PixelPos == OldPos)
		return;

	const CFRect& rect = GetScene()->GetSceneBox();
	if(PixelPos.x < rect.left || PixelPos.x > rect.right ||
		PixelPos.y > rect.top || PixelPos.y < rect.bottom )
	{
		ostringstream str;
		str << "Invalid Pos " << PixelPos.x << "  " << PixelPos.y << endl;
		GenErr("CSynAoiDimObj::SetPos Error", str.str().c_str());
	}

	IntSetPos(PixelPos);
}

void CSynAoiDimObj::SetSize( float fSize )
{
	if(fSize < 0)
	{
		GenErr("AOI��size����������С��0��ֵ");
	}

	if (fabs(m_fSize - fSize) < 0.01)
	{
		return;
	}

	//��һ��size
	if ( m_fSize == 0 && !m_pSizeRectNode )
	{
		m_fSize = fSize;
		SetSizeRect(m_Pos, GetScene()->GetSceneBox());

		static VecDimObject_t  vecOtObject;
		vecOtObject.clear();

		QueryObserverWithSizeRect(vecOtObject);
		UpdateSeeRelationForOtEnterObject(vecOtObject);
		GetScene()->InsertSizeRectToQuad(this);
		return;
	}

	//���fsizeΪ0�����aoi obj��quad tree��ɾ��
	if ( fSize == 0 && m_pSizeRectNode )
	{
		static VecDimObject_t  vecOtObject;
		vecOtObject.clear();
		GetScene()->RemoveSizeRectFrQuad(this);
		QueryObserverWithSizeRect(vecOtObject);
		UpdateSeeRelationForOtLeaveObject(vecOtObject, GetPosition());
		m_fSize = 0;
		return;
	}
	
	if(fSize > m_fSize)
	{
		//��������õ�size������ǰ��size����ô����һЩ�µĶ��󿴼��Լ�
		static VecDimObject_t vecOtObjEnter;
		vecOtObjEnter.clear();

		float OldSize = m_fSize;
		CFRect OldRect = GetSizeRect();
		
		m_fSize = fSize;
		//��Ϊ�µ�size����ǰ�Ĵ��������µ�size rect nodeһ������ǰ��size rect node�����丸�ڵ�����
		//�������������������Ĳ�����Ȼ���ڴ��µ�size rect node�ڵ�������ܿ����ҵĶ���
		GetScene()->SetAoiSizeRect(this);

		//����size����֮�����ܿ����ҵĶ���
		GetScene()->QueryObserver(this, vecOtObjEnter, m_rcSizeRect, OldRect);

		UpdateSeeRelationForOtStayObject(m_Pos, OldSize, fSize);
		UpdateSeeRelationForOtEnterObject(vecOtObjEnter, fSize);
	}
	else
	{
		CFRect NewRect;
		GetRadiusRect(NewRect, m_Pos, fSize, GetScene()->GetSceneBox());

		static VecDimObject_t vecOtObjLeave;
		vecOtObjLeave.clear();
		
		float OldSize = m_fSize;

		//��Ϊ�µ�size����ǰ��С���������µ�size rect nodeһ������ǰ��size rect node�������ӽڵ�����
		//�������������ȴ�ԭsize rect node������ǰ�ܿ����ҵ������ڲ��ܿ����ҵĶ���Ȼ���������Ĳ���
		//����size��С�Ժ���ǰ�ܿ����ҵ������ڲ��ܿ����ҵĶ���
		GetScene()->QueryObserver(this, vecOtObjLeave, m_rcSizeRect, NewRect);
		
		m_fSize = fSize;
		GetScene()->SetAoiSizeRect(this);

		UpdateSeeRelationForOtLeaveObject(vecOtObjLeave, m_Pos, OldSize);
		UpdateSeeRelationForOtStayObject(m_Pos, OldSize, fSize);
	}
}

void CSynAoiDimObj::SetEyeSight( float fEyeSight )
{
	if(fEyeSight < 0)
	{
		GenErr("AOI��EyeSight����������С��0��ֵ");
	}

	if (fabs(m_fEyeSight - fEyeSight) < 0.01)
	{
		return;
	}

	//��һ��view rect����quad tree
	if (m_fEyeSight == 0 && !m_pViewRectNode)
	{
		m_fEyeSight = fEyeSight;
		//����aoi��view rect
		SetViewRect(m_Pos, GetScene()->GetSceneBox());

		static VecDimObject_t vecMyObject;
		vecMyObject.clear();

		QueryObserveeWithViewRect(vecMyObject);		
		UpdateSeeRelationForMyEnterObject(vecMyObject);	
		GetScene()->InsertViewRectToQuad(this);	
		return;
	}

	//���eye sightΪ0�����view rect��quad tree��ɾ��
	if (fEyeSight == 0 && m_pViewRectNode)
	{
		static VecDimObject_t vecMyObject;
		vecMyObject.clear();

		GetScene()->RemoveViewRectFrQuad(this);
		QueryObserveeWithViewRect(vecMyObject);
		UpdateSeeRelationForMyLeaveObject(vecMyObject,GetPosition());
	
		m_fEyeSight = 0;
		return;
	}

	if(fEyeSight > m_fEyeSight)
	{
		//��������õ�eye sight������ǰ��eye sight����ô�Ҿͻῴ��һЩ�µĶ���
		static VecDimObject_t vecMyObjEnter;
		vecMyObjEnter.clear();

		float OldEyeSight = m_fEyeSight;
		CFRect OldRect = GetViewRect();

		m_fEyeSight = fEyeSight;
		GetScene()->SetAoiViewRect(this);

		//����eye sight����֮�����ܱ��ҿ����Ķ���
		GetScene()->QueryObservee(this, vecMyObjEnter, m_rcViewRect, OldRect);

		UpdateSeeRelationForMyStayObject(m_Pos, OldEyeSight, fEyeSight);
		UpdateSeeRelationForMyEnterObject(vecMyObjEnter, fEyeSight);
	}
	else
	{
		static VecDimObject_t vecMyObjLeave;
		vecMyObjLeave.clear();

		float OldEyeSight = m_fEyeSight;

		CFRect NewRect;
		GetRadiusRect(NewRect, m_Pos, fEyeSight, GetScene()->GetSceneBox());

		//����eye sight��С�Ժ�����ǰ�ܿ����������ڲ��ܿ����Ķ���
		GetScene()->QueryObservee(this, vecMyObjLeave, m_rcViewRect, NewRect);
		
		m_fEyeSight = fEyeSight;
		GetScene()->SetAoiViewRect(this);

		UpdateSeeRelationForMyLeaveObject(vecMyObjLeave, m_Pos, OldEyeSight);
		UpdateSeeRelationForMyStayObject(m_Pos, OldEyeSight, fEyeSight);
	}

}



void CSynAoiDimObj::SetStealth( float fStealth )
{
	if (GetStealth() == fStealth)
	{
		return;
	}

	ModifyVisibilityProperty(m_fSize,fStealth);

	m_fStealth = fStealth;
}

void CSynAoiDimObj::SetKeenness(float fKeenness)
{
	if (GetKeenness() == fKeenness)
	{
		return;
	}

	ModifyObservationProperty(m_fEyeSight,fKeenness);

	m_fKeenness = fKeenness;
}

void CSynAoiDimObj::SetViewGroup(uint32 uViewGroup)
{
	if (GetViewGroup() == uViewGroup)
	{
		return;
	}

	ModifyViewGroupProperty(uViewGroup);

	m_uViewGroup = uViewGroup;
}

void CSynAoiDimObj::InsertToScene()
{
	if (m_fSize != 0)
	{
		SetSizeRect(m_Pos, GetScene()->GetSceneBox());
		static VecDimObject_t  vecOtObject;
		vecOtObject.clear();

		QueryObserverWithSizeRect(vecOtObject);
		UpdateSeeRelationForOtEnterObject(vecOtObject);
		GetScene()->InsertSizeRectToQuad(this);
	}

	if (m_fEyeSight != 0)
	{
		SetViewRect(m_Pos, GetScene()->GetSceneBox());
		static VecDimObject_t vecMyObject;
		vecMyObject.clear();

		QueryObserveeWithViewRect(vecMyObject);		
		UpdateSeeRelationForMyEnterObject(vecMyObject);	
		GetScene()->InsertViewRectToQuad(this);	
	}
}

void CSynAoiDimObj::RemoveFrScene()
{
	CFPos AoiPos = GetPosition();
	static VecDimObject_t	vecMyObject, vecOtObject;
	vecMyObject.clear();
	vecOtObject.clear();
	
	if (m_pViewRectNode)
	{
		GetScene()->RemoveViewRectFrQuad(this);
		QueryObserveeWithViewRect(vecMyObject);
		UpdateSeeRelationForMyLeaveObject(vecMyObject,AoiPos);
	}	
	if (m_pSizeRectNode)
	{
		GetScene()->RemoveSizeRectFrQuad(this);
		QueryObserverWithSizeRect(vecOtObject);
		UpdateSeeRelationForOtLeaveObject(vecOtObject, AoiPos);
	}	
}

void CSynAoiDimObj::QueryObserveeWithViewRect(VecDimObject_t& vecObject)const
{
	CFRect Rect = GetViewRect();
	GetScene()->QueryObservee(vecObject,Rect);
}

void CSynAoiDimObj::QueryObserverWithSizeRect(VecDimObject_t& vecObject) const
{
	CFRect Rect = GetSizeRect();
	GetScene()->QueryObserver(vecObject, Rect);
}


void CSynAoiDimObj::ModifyObservationProperty(float fNewEyeSight,float fNewKeenness)
{
	CFPos AoiPos = GetPosition();

	CFPos OtAoiPos;

	CSynAoiDimObj* pOtObj;

	for(ViewObjSet_t::iterator it=m_setObjInMyViewRect.begin(), it_end = m_setObjInMyViewRect.end();
		it!=it_end;++it)
	{
		pOtObj=*it;
		OtAoiPos = pOtObj->GetPosition();

		bool bISeeOt,bISawOt;

		bISawOt=CalSeeRelation
			(
			AoiPos,			OtAoiPos,
			m_fEyeSight,	pOtObj->m_fSize,
			m_fKeenness,	pOtObj->m_fStealth,
			m_uViewGroup,	pOtObj->m_uViewGroup
			);

		bISeeOt=CalSeeRelation
			(
			AoiPos,			OtAoiPos,
			fNewEyeSight,	pOtObj->m_fSize,
			fNewKeenness,	pOtObj->m_fStealth,
			m_uViewGroup,	pOtObj->m_uViewGroup
			);

		if(bISawOt != bISeeOt)
			pOtObj->OnViewRelationChanged(this,OtAoiPos,AoiPos,bISeeOt);
	}
}		

void CSynAoiDimObj::ModifyVisibilityProperty(float fNewSize,float fNewStealth)
{
	CFPos AoiPos = GetPosition();

	CFPos OtAoiPos;

	CSynAoiDimObj* pOtObj;

	for( ViewObjSet_t::iterator it=m_setObjInOtViewRect.begin(), it_end = m_setObjInOtViewRect.end() ;
		it!=it_end; ++it )
	{
		pOtObj=*it;

		OtAoiPos = pOtObj->GetPosition();

		bool bOtSeeI,bOtSawI;

		bOtSawI=CalSeeRelation
			(
			OtAoiPos,				AoiPos,
			pOtObj->m_fEyeSight,	m_fSize,
			pOtObj->m_fKeenness,	m_fStealth,
			pOtObj->m_uViewGroup,	m_uViewGroup
			);

		bOtSeeI=CalSeeRelation
			(
			OtAoiPos,				AoiPos,
			pOtObj->m_fEyeSight,	fNewSize,
			pOtObj->m_fKeenness,	fNewStealth,
			pOtObj->m_uViewGroup,	m_uViewGroup
			);

		if(bOtSawI != bOtSeeI)
			OnViewRelationChanged(pOtObj, AoiPos,OtAoiPos,bOtSeeI);
	}
}	

void CSynAoiDimObj::ModifyViewGroupProperty(uint32 uNewViewGroup)
{
	CFPos AoiPos = GetPosition();

	CFPos OtAoiPos;

	CSynAoiDimObj* pOtObj;

	for(ViewObjSet_t::iterator it=m_setObjInMyViewRect.begin(), it_end = m_setObjInMyViewRect.end();
		it != it_end; ++it)
	{
		pOtObj=*it;
		OtAoiPos = pOtObj->GetPosition();

		bool bISeeOt,bISawOt;

		bISawOt=CalSeeRelation
			(
			AoiPos,			OtAoiPos,
			m_fEyeSight,	pOtObj->m_fSize,
			m_fKeenness,	pOtObj->m_fStealth,
			m_uViewGroup,	pOtObj->m_uViewGroup
			);

		bISeeOt=CalSeeRelation
			(
			AoiPos,			OtAoiPos,
			m_fEyeSight,	pOtObj->m_fSize,
			m_fKeenness,	pOtObj->m_fStealth,
			uNewViewGroup,	pOtObj->m_uViewGroup
			);

		if(bISawOt != bISeeOt)
			pOtObj->OnViewRelationChanged(this,OtAoiPos,AoiPos,bISeeOt);
	}

	for( ViewObjSet_t::iterator it=m_setObjInOtViewRect.begin(), it_end = m_setObjInOtViewRect.end(); 
		it!= it_end; ++it )
	{
		pOtObj=*it;

		OtAoiPos = pOtObj->GetPosition();

		bool bOtSeeI,bOtSawI;

		bOtSawI=CalSeeRelation
			(
			OtAoiPos,				AoiPos,
			pOtObj->m_fEyeSight,	m_fSize,
			pOtObj->m_fKeenness,	m_fStealth,
			pOtObj->m_uViewGroup,	m_uViewGroup
			);

		bOtSeeI=CalSeeRelation
			(
			OtAoiPos,				AoiPos,
			pOtObj->m_fEyeSight,	m_fSize,
			pOtObj->m_fKeenness,	m_fStealth,
			pOtObj->m_uViewGroup,	uNewViewGroup
			);

		if(bOtSawI != bOtSeeI)
			OnViewRelationChanged(pOtObj, AoiPos,OtAoiPos,bOtSeeI);
	}
}

static void GenAoiUpdateRelationError(CSynAoiDimObj* pObj1, CSynAoiDimObj* pObj2)
{
	CFPos pos1 = pObj1->GetPosition();
	CFPos pos2 = pObj2->GetPosition();
	stringstream strm;
	strm << "MyObj: pos (" << pos1.x << " " << pos1.y << ") eyesight " << pObj1->GetEyeSight()
		<< " size " << pObj1->GetSize() << endl
		<< "OtObj: pos (" << pos2.x << " " << pos2.y << ") eyesight " << pObj2->GetEyeSight()
		<< " size " << pObj2->GetSize() << endl;
	GenErr("Aoi UpdateSeeRelationError", strm.str().c_str());
}


void CSynAoiDimObj::UpdateSeeRelationForMyEnterObject(const VecDimObject_t& vecObj)
{
	UpdateSeeRelationForMyEnterObject(vecObj, m_fEyeSight);
}

struct node
{
	node(CSynAoiDimObj* pObj, CFPos pos)
	{
		m_fDistance = pos.Dist(pObj->GetPosition());
		m_pObj = pObj;
	}
	bool operator>= (const node& right) const
	{
		return m_fDistance >= right.m_fDistance;
	}
	float m_fDistance;
	CSynAoiDimObj* m_pObj;
};

void CSynAoiDimObj::UpdateSeeRelationForMyEnterObject(const VecDimObject_t& vecObj, float NewEyeSight)
{
	CFPos OtAoiPos;
	CFPos AoiPos = GetPosition();

	CSynAoiDimObj* pOtObj;

	static priority_queue<node, vector<node>, greater_equal<node> > priQueue;

	for( VecDimObject_t::const_iterator it=vecObj.begin(), it_end = vecObj.end() ; it!= it_end; ++it )
	{
		pOtObj=*it;
		if( pOtObj == this )
			continue;

		if( !m_setObjInMyViewRect.insert(pOtObj).second )
		{
			GenAoiUpdateRelationError(this, pOtObj);
		}
		if( !pOtObj->m_setObjInOtViewRect.insert(this).second )
		{
			GenAoiUpdateRelationError(pOtObj, this);
		}

		OtAoiPos = pOtObj->GetPosition();

		bool bISeeOt;

		bISeeOt=CalSeeRelation
			(
			AoiPos,			OtAoiPos,
			NewEyeSight,	pOtObj->m_fSize,
			m_fKeenness,	pOtObj->m_fStealth,
			m_uViewGroup,	pOtObj->m_uViewGroup
			);

		if(bISeeOt)
		{
			priQueue.push(node(pOtObj,AoiPos));
		}	
	}

	while( !priQueue.empty() )
	{
		pOtObj = priQueue.top().m_pObj;
		priQueue.pop();
		OtAoiPos = pOtObj->GetPosition();
		pOtObj->OnViewRelationChanged(this, OtAoiPos, AoiPos, true);
	}
}


void CSynAoiDimObj::UpdateSeeRelationForMyLeaveObject(const VecDimObject_t& vecObj, const CFPos& MyOldPos)
{
	UpdateSeeRelationForMyLeaveObject(vecObj, MyOldPos, m_fEyeSight);
}

void CSynAoiDimObj::UpdateSeeRelationForMyLeaveObject(const VecDimObject_t& vecObj,const CFPos& OldGridPos, float OldEyeSight)
{
	CFPos OtAoiPos;
	CSynAoiDimObj* pOtObj;

	for( VecDimObject_t::const_iterator it=vecObj.begin(), it_end = vecObj.end()  ; it!= it_end; ++it )
	{
		pOtObj=*it;
		if( pOtObj == this )
			continue;

		if( !m_setObjInMyViewRect.erase(pOtObj) )
		{
			GenAoiUpdateRelationError(this, pOtObj);
		}
		if( !pOtObj->m_setObjInOtViewRect.erase(this) )
		{
			GenAoiUpdateRelationError(pOtObj, this);
		}

		OtAoiPos = pOtObj->GetPosition();

		bool bISawOt;

		bISawOt=CalSeeRelation
			(
			OldGridPos,		OtAoiPos,
			OldEyeSight,	pOtObj->m_fSize,
			m_fKeenness,	pOtObj->m_fStealth,
			m_uViewGroup,	pOtObj->m_uViewGroup
			);

		if(bISawOt)
			pOtObj->OnViewRelationChanged(this,OtAoiPos,OldGridPos,false);

	}


}


void CSynAoiDimObj::UpdateSeeRelationForMyStayObject(const CFPos& MyOldAoiPos)
{
	UpdateSeeRelationForMyStayObject(MyOldAoiPos, m_fEyeSight, m_fEyeSight);
}

void CSynAoiDimObj::UpdateSeeRelationForMyStayObject(const CFPos& MyOldAoiPos, float OldEyeSight, float NewEyeSight)
{
	CFPos NewAoiPos = GetPosition();

	CFPos OtAoiPos;

	CSynAoiDimObj* pOtObj;

	for(ViewObjSet_t::iterator it=m_setObjInMyViewRect.begin(), it_end = m_setObjInMyViewRect.end();
		it!=it_end;++it)
	{
		pOtObj=*it;
		OtAoiPos = pOtObj->GetPosition();

		bool bISeeOt,bISawOt;

		bISawOt=CalSeeRelation
			(
			MyOldAoiPos,	OtAoiPos,
			OldEyeSight,	pOtObj->m_fSize,
			m_fKeenness,	pOtObj->m_fStealth,
			m_uViewGroup,	pOtObj->m_uViewGroup
			);

		bISeeOt=CalSeeRelation
			(
			NewAoiPos,		OtAoiPos,
			NewEyeSight,	pOtObj->m_fSize,
			m_fKeenness,	pOtObj->m_fStealth,
			m_uViewGroup,	pOtObj->m_uViewGroup
			);

		if(bISawOt)
		{
			if(!bISeeOt)
			{
				pOtObj->OnViewRelationChanged(this,OtAoiPos,MyOldAoiPos,false);
			}
			else
			{
				OnViewRelationKept( pOtObj, MyOldAoiPos, NewAoiPos, OtAoiPos, OtAoiPos );
			}
		}
		else
		{
			if(bISeeOt)
			{
				pOtObj->OnViewRelationChanged(this,OtAoiPos,NewAoiPos,true);
			}
		}
	}

}

void CSynAoiDimObj::UpdateSeeRelationForOtEnterObject(const VecDimObject_t& vecObj)
{
	UpdateSeeRelationForOtEnterObject(vecObj, m_fSize);
}

void CSynAoiDimObj::UpdateSeeRelationForOtEnterObject(const VecDimObject_t& vecObj, float NewSize)
{
	CFPos OtAoiPos;
	CFPos AoiPos = GetPosition();

	CSynAoiDimObj* pOtObj;

	for( VecDimObject_t::const_iterator it=vecObj.begin(), it_end = vecObj.end() ; it!= it_end; ++it )
	{
		pOtObj=*it;
		if( pOtObj == this )
			continue;

		if( !m_setObjInOtViewRect.insert(pOtObj).second )
		{
			GenAoiUpdateRelationError(this, pOtObj);
		}
		if( !pOtObj->m_setObjInMyViewRect.insert(this).second )
		{
			GenAoiUpdateRelationError(pOtObj, this);
		}

		OtAoiPos = pOtObj->GetPosition();

		bool bOtSeeI;

		bOtSeeI=CalSeeRelation
			(
			OtAoiPos,				AoiPos,
			pOtObj->m_fEyeSight,	NewSize,
			pOtObj->m_fKeenness,	m_fStealth,
			pOtObj->m_uViewGroup,	m_uViewGroup
			);

		if(bOtSeeI)
			OnViewRelationChanged(pOtObj, AoiPos,OtAoiPos,bOtSeeI);
	}
}

void CSynAoiDimObj::UpdateSeeRelationForOtLeaveObject(const VecDimObject_t& vecObj, const CFPos& OldPos)
{
	UpdateSeeRelationForOtLeaveObject(vecObj, OldPos, m_fSize);
}

void CSynAoiDimObj::UpdateSeeRelationForOtLeaveObject(const VecDimObject_t& vecObj,const CFPos& OldGridPos, float OldSize)
{
	CFPos OtAoiPos;
	CSynAoiDimObj* pOtObj;

	for( VecDimObject_t::const_iterator it=vecObj.begin(), it_end = vecObj.end() ; it!= it_end; ++it )
	{
		pOtObj=*it;
		if( pOtObj == this )
			continue;

		if( !m_setObjInOtViewRect.erase(pOtObj) )
		{
			GenAoiUpdateRelationError(this, pOtObj);
		}
		if( !pOtObj->m_setObjInMyViewRect.erase(this) )
		{
			GenAoiUpdateRelationError(pOtObj, this);
		}

		OtAoiPos = pOtObj->GetPosition();

		bool bOtSawI;

		bOtSawI=CalSeeRelation
			(
			OtAoiPos,				OldGridPos,
			pOtObj->m_fEyeSight,	OldSize,
			pOtObj->m_fKeenness,	m_fStealth,
			pOtObj->m_uViewGroup,	m_uViewGroup
			);

		if(bOtSawI)
			OnViewRelationChanged(pOtObj, OldGridPos,OtAoiPos,false);

	}
}

void CSynAoiDimObj::UpdateSeeRelationForOtStayObject(const CFPos& MyOldAoiPos)
{
	UpdateSeeRelationForOtStayObject(MyOldAoiPos, m_fSize, m_fSize);
}

void CSynAoiDimObj::UpdateSeeRelationForOtStayObject(const CFPos& MyOldAoiPos, float OldSize, float NewSize)
{
	CFPos NewAoiPos = GetPosition();

	CFPos OtAoiPos;

	CSynAoiDimObj* pOtObj;


	for( ViewObjSet_t::iterator it=m_setObjInOtViewRect.begin(), it_end = m_setObjInOtViewRect.end() ; 
		it!= it_end; ++it )
	{
		pOtObj=*it;

		OtAoiPos = pOtObj->GetPosition();

		bool bOtSeeI,bOtSawI;

		bOtSawI=CalSeeRelation
			(
			OtAoiPos,				MyOldAoiPos,
			pOtObj->m_fEyeSight,	OldSize,
			pOtObj->m_fKeenness,	m_fStealth,
			pOtObj->m_uViewGroup,	m_uViewGroup
			);

		bOtSeeI=CalSeeRelation
			(
			OtAoiPos,				NewAoiPos,
			pOtObj->m_fEyeSight,	NewSize,
			pOtObj->m_fKeenness,	m_fStealth,
			pOtObj->m_uViewGroup,	m_uViewGroup
			);

		if(bOtSawI)
		{
			if(!bOtSeeI)
			{
				OnViewRelationChanged(pOtObj,MyOldAoiPos,OtAoiPos,false);
			}
			else
			{
				pOtObj->OnViewRelationKept( this, OtAoiPos, OtAoiPos, MyOldAoiPos, NewAoiPos );
			}
		}
		else
		{
			if(bOtSeeI)
			{
				OnViewRelationChanged(pOtObj,NewAoiPos,OtAoiPos,true);
			}
		}
	}
}

void CSynAoiDimObj::PostMsgToViewee(const void* pContext)
{
	CSynAoiDimObj* pObject;
	ViewObjSet_t::const_iterator iter = m_setObjInMyViewRect.begin();
	ViewObjSet_t::const_iterator iter_end = m_setObjInMyViewRect.end();
	while (iter != iter_end)
	{
		pObject = (*iter);
		if (CanSee(pObject))
		{
			pObject->OnMsgFromViewer(pContext);
		}
		++iter;
	}

	OnMsgToVieweeHandled(pContext);
}

void CSynAoiDimObj::PostMsgToViewer(const void* pContext)
{
	CSynAoiDimObj* pObject;
	ViewObjSet_t::const_iterator iter = m_setObjInOtViewRect.begin();
	ViewObjSet_t::const_iterator iter_end = m_setObjInOtViewRect.end();

	while (iter != iter_end)
	{
		pObject = (*iter);
		if (pObject->CanSee(this))
		{
			pObject->OnMsgFromViewee(pContext);
		}
		++iter;
	}

	OnMsgToViewerHandled(pContext);
}


void CSynAoiDimObj::PostMsgToSyncee(const void *pContext)
{
	CSynAoiDimObj* pObject;
	ViewObjSet_t::const_iterator iter = m_setObjInOtViewRect.begin();
	ViewObjSet_t::const_iterator iter_end = m_setObjInOtViewRect.end();

	while (iter != iter_end)
	{
		pObject = (*iter);
		if (pObject->CanSee(this))
		{
			CFPos OtPos = pObject->GetPosition();
			if(m_pDimScene->IsInSyncAoi(m_Pos - OtPos))
				pObject->OnMsgFromSyncer(pContext);
		}
		++iter;
	}

	OnMsgToSynceeHandled(pContext);
}

void CSynAoiDimObj::PostMsgToSyncer(const void *pContext)
{
	CSynAoiDimObj* pObject;
	ViewObjSet_t::const_iterator iter = m_setObjInMyViewRect.begin();
	ViewObjSet_t::const_iterator iter_end = m_setObjInMyViewRect.end();
	while (iter != iter_end)
	{
		pObject = (*iter);
		if (CanSee(pObject))
		{
			CFPos OtPos = pObject->GetPosition();
			if(m_pDimScene->IsInSyncAoi(OtPos - m_Pos))
				pObject->OnMsgFromSyncee(pContext);
		}
		++iter;
	}

	OnMsgToSyncerHandled(pContext);
}

void CSynAoiDimObj::OnMsgFromSyncer(const void* pContext)
{
	m_pViewObj->OnMsgFromSyncer(pContext);
}

void CSynAoiDimObj::OnMsgFromSyncee(const void* pContext)
{
	m_pViewObj->OnMsgFromSyncee(pContext);
}

void CSynAoiDimObj::OnMsgToSyncerHandled(const void* pContext)
{
	m_pViewObj->OnMsgToSyncerHandled(pContext);
}

void CSynAoiDimObj::OnMsgToSynceeHandled(const void* pContext)
{
	m_pViewObj->OnMsgToSynceeHandled(pContext);
}
