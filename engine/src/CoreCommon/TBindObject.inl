#pragma once
#include "TBindObject.h"
#include "TCoreObject.h"
#include "TSqrAllocator.inl"
#include "ErrLogHelper.h"
//#include "ExpHelper.h"
//#include "CoreObjectCommon.h"
//#include "CoreCommon.h"
//#include "CLogCoreObject.h"

template<typename ObjectType>
TBindObject<ObjectType>::TBindObject(ObjectType* pObj)
:m_pCoreObj(pObj)
,m_uBindParentID(0)
{
}


template<typename ObjectType>
TBindObject<ObjectType>::~TBindObject()
{
}

template<typename ObjectType>
bool TBindObject<ObjectType>::BindTo(ObjectType* pParent)
{
	if(GetObject()->GetScene() != pParent->GetScene())
	{
		stringstream str;
		str << GetObject()->GetLocalID() << " to " << pParent->GetLocalID() << endl;
		LogErr("�󶨶���˫������ͬһ����", str.str());
		GetObject()->OnBeforeBindTo(pParent->GetLocalID(), false);
		GetObject()->OnBindTo(pParent->GetLocalID(), false);
		return false;
	}

	//ֻ�ܰ󶨵�һ�����ڵ�
	if(IsBindingObj())
	{
		GetObject()->OnBeforeBindTo(pParent->GetLocalID(), false);
		GetObject()->OnBindTo(pParent->GetLocalID(), false);
		return false;
	}

	//��������󶨵ĸ��ڵ�ĸ��ڵ����������������
	ObjectType* pParentRoot = pParent->m_pBindObj->GetRootObject();
	if(pParentRoot == GetObject())
	{
		GetObject()->OnBeforeBindTo(pParent->GetLocalID(), false);
		GetObject()->OnBindTo(pParent->GetLocalID(), false);
		return false;
	}

	GetObject()->OnBeforeBindTo(pParent->GetLocalID(), true);

	//�ⲿ���Ƶ�GetObject()->OnBeforeBindTo(pParent->GetLocalID(), true);
	//if(GetObject()->GetMovState())
	//	GetObject()->IntStopMoving();

	//�ȹ����������ٹ���������
	pParent->m_pBindObj->m_setBindChildID.insert(GetSelfID());
	m_uBindParentID = pParent->GetLocalID();

	//�ⲿ���Ƶ�GetObject()->OnBindTo(m_uBindParentID, true);
	//CFPos fParentPos;
	//pParent->GetPixelPos(fParentPos);
	//GetObject()->IntSetPosition(fParentPos);

	//cout << "	" << GetObject()->GetLocalID()<< " BindTo " << pParent->GetLocalID() << "\n";
		//pParent->m_uID << " At(" << fParentPos.x << ", " << fParentPos.y << ")\n";

	GetObject()->OnBindTo(m_uBindParentID, true);
	return true;
}

template<typename ObjectType>
bool TBindObject<ObjectType>::BindTo(uint32 uParentID)
{
	ObjectType* pParent = static_cast<ObjectType*>(ObjectType::GetObject(uParentID));

	if(!pParent)
	{
		GetObject()->OnBeforeBindTo(m_uBindParentID, false);
		GetObject()->OnBindTo(uParentID, false);
		return false;
	}

	return BindTo(pParent);
}

template<typename ObjectType>
bool TBindObject<ObjectType>::DisbindFromParent()
{
	if(m_uBindParentID == 0)
	{
		GetObject()->OnBeforeDisbindFromParent();
		GetObject()->OnDisbindFromParent(false);
		return false;
	}

	ObjectType* pParent = static_cast<ObjectType*>(ObjectType::GetObject(m_uBindParentID));
	if(!pParent)
	{
		GetObject()->OnBeforeDisbindFromParent();
		//uint32 uBindParentID = m_uBindParentID;
		m_uBindParentID = 0;
		GetObject()->OnDisbindChildSingle();
		GetObject()->OnDisbindFromParent(false);
		return false;
	}

	GetObject()->OnBeforeDisbindFromParent();

	//������������Ȳ��������ٲ�������

	//SetObject_t::iterator itr = pParent->m_setBindChildID.find(m_uId);
	//if(itr == pParent->m_setBindChildID.end())
	//{
	//	m_uBindParentID = 0;
	//	return false;
	//}

	//cout << "[	]" << GetObject()->GetLocalID() << " DisbindFromParent " << m_uBindParentID <<"\n";
	//	m_uBindParentID << " At(" << this->m_Pos.x << ", " << this->m_Pos.y << ")\n";

	pParent->m_pBindObj->m_setBindChildID.erase(GetObject()->GetLocalID());
	//uint32 uBindParentID = m_uBindParentID;
	m_uBindParentID = 0;

	GetObject()->OnDisbindChildSingle();
	static_cast<ObjectType*>(pParent)->OnDisbindParentSingle();

	GetObject()->OnDisbindFromParent(true);
	return true;
}

template<typename ObjectType>
bool TBindObject<ObjectType>::DisbindChild(uint32 uChildID)
{

	//������������Ȳ��������ٲ�������
	ObjectType* pChild = static_cast<ObjectType*>(ObjectType::GetObject(uChildID));
	if(!pChild)
	{
		GetObject()->OnBeforeDisbindToChild(uChildID);
	}
	else
	{
		GetObject()->OnBeforeDisbindToChild(uChildID);
	}
	m_setBindChildID.erase(uChildID);
	if(!pChild)
	{
		GetObject()->OnDisbindToChild(uChildID, false);
		GetObject()->OnDisbindParentSingle();
		return false;
	}
	pChild->m_pBindObj->m_uBindParentID = 0;

	//cout << "[	]" << this->m_uID << " DisbindChild " <<
	//	uChildID << " At(" << this->m_Pos.x << ", " << this->m_Pos.y << ")\n";

	pChild->OnDisbindChildSingle();
	GetObject()->OnDisbindParentSingle();

	GetObject()->OnDisbindToChild(uChildID, true);
	return true;
}

template<typename ObjectType>
bool TBindObject<ObjectType>::DisbindAllChildren()
{
	//������������Ȳ��������ٲ�������

	if(m_setBindChildID.empty())
	{
		GetObject()->OnBeforeDisbindToAllChild();
		GetObject()->OnDisbindToAllChild(false);
		GetObject()->OnDisbindParentSingle();
		return false;
	}

	GetObject()->OnBeforeDisbindToAllChild();

	//cout << "[	]" << GetObject()->GetLocalID() << " DisbindAllChildren ";

	for(SetUInt32::iterator itr = m_setBindChildID.begin(); itr != m_setBindChildID.end();)
	{
		uint32 uChildID = (*itr);
		m_setBindChildID.erase(itr++);
		ObjectType* pChild = static_cast<ObjectType*>(ObjectType::GetObject(uChildID));

		//cout << uChildID << " ";

		if(!pChild)
		{
			//������Ҫ�����Ӷ����Ƿ���Ļ᲻���ڣ���������迼�Ƿ���ֵһ����Ϊtrue���߼��߳���ɵ�Ӱ���Լ��Բ�
			continue;
		}
		pChild->m_pBindObj->m_uBindParentID = 0;
		pChild->OnDisbindChildSingle();
	}

	//cout << "\n";
	//cout << "At(" << this->m_Pos.x << ", " << this->m_Pos.y << ")\n";

	GetObject()->OnDisbindParentSingle();
	GetObject()->OnDisbindToAllChild(true);
	return true;
}


template<typename ObjectType>
bool TBindObject<ObjectType>::IsBindingObj() const
{
	return m_uBindParentID != 0;
}

template<typename ObjectType>
bool TBindObject<ObjectType>::IsBoundObj() const
{
	return !m_setBindChildID.empty();
}

template<typename ObjectType>
bool TBindObject<ObjectType>::IsBindingOrBoundObj() const
{
	return m_uBindParentID !=0 || !m_setBindChildID.empty();
}

template<typename ObjectType>
ObjectType*
TBindObject<ObjectType>::GetRootObject()
{
	//BindTo��DisbindTo�������Բ��������������������������ѭ��
	ObjectType*  pRet = GetObject();
	for(;;)
	{
		if(pRet->m_pBindObj->m_uBindParentID == 0)
			break;
		ObjectType* pParent = static_cast<ObjectType*>(ObjectType::GetObject(pRet->m_pBindObj->m_uBindParentID));
		if(!pParent)
			break;
		pRet = pParent;
	}
	return pRet;
}

template<typename ObjectType>
bool TBindObject<ObjectType>::BindObjIntSetPosition(const CFPos& PixelPos)
{
	if(!IsBoundObj())
		return true;
	//���IntSetPositionSingle���ᵼ��ͬ��ɾ�������������ڵ��κ�TCoreObject�������ֱ�ӱ���m_setBindChildID������ҪdeqBindChild����
	//cout << GetObject()->GetLocalID() << " BindObjIntSetPosition(" << PixelPos.x << "," << PixelPos.y << ") To ";
	bool bRet = true;
	deque<uint32> deqBindChild;
	for(SetUInt32::iterator itr = m_setBindChildID.begin(); itr != m_setBindChildID.end(); ++itr)
	{
		deqBindChild.push_back(*itr);
	}
	for(deque<uint32>::iterator itr = deqBindChild.begin(); itr != deqBindChild.end(); ++itr)
	{
		ObjectType* pChild = static_cast<ObjectType*>(ObjectType::GetObject(*itr));
		//cout << (*itr) << "(" << pChild << "), ";
		if(pChild)
		{
			bool bChildRet = pChild->IntSetPositionSingle(PixelPos);
			if(!bChildRet)
			{
				bRet = bChildRet;
			}
		}
	}
	//cout << "\n";
	return bRet;
}

template<typename ObjectType>
void TBindObject<ObjectType>::BindObjNotifyPosChangedIfNeed()
{
	if(!IsBoundObj())
		return;
	//���IntSetPixelPosSingle���ᵼ��ͬ��ɾ�������������ڵ��κ�TCoreObject�������ֱ�ӱ���m_setBindChildID������ҪdeqBindChild����
	deque<uint32> deqBindChild;
	for(SetUInt32::iterator itr = m_setBindChildID.begin(); itr != m_setBindChildID.end(); ++itr)
	{
		deqBindChild.push_back(*itr);
	}
	for(deque<uint32>::iterator itr = deqBindChild.begin(); itr != deqBindChild.end(); ++itr)
	{
		ObjectType* pChild = static_cast<ObjectType*>(ObjectType::GetObject(*itr));
		if(pChild)
		{
			pChild->NotifyPosChangedIfNeedSingle();
			//if(eChildRet != eSPR_Success)
			//{
			//	eRet = eChildRet;
			//}
		}
	}
}

