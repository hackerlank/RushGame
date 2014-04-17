#pragma once
#include "PatternRef.h"
#include "ExpHelper.h"
#include "PtRefProxy.inl"

/*
����һ������(refee)�����Ա��������(refer)����
refer��refee֮�䣬ͨ��һ�����ô�����proxy��ϵ��
referͨ��proxy����refee
��referÿattach refee��ʱ��proxy��������ü�����һ
��referÿdetach refee��ʱ��proxy��������ü�����һ
��proxy�����ü���Ϊ0��ʱ�򣬱���û��referȥ����refee��
��refee����ɾ����ʱ������ж�Ӧ��proxy����ô��proxy����ָ���Լ���ָ�����
����refer����ͨ��proxy�õ���ָ���Ϊnull����ֹҰָ��
*/

template < class HolderType, class PointeeHolderType >
TPtRefBase<HolderType,PointeeHolderType>::TPtRefBase()
{
	m_pHolder = NULL;    
}

template < class HolderType, class PointeeHolderType >
inline HolderType* TPtRefBase<HolderType,PointeeHolderType>::GetHolder() const
{
	return m_pHolder;
}

template < class HolderType, class PointeeHolderType >
void TPtRefBase<HolderType,PointeeHolderType>::SetHolder(HolderType* pHolder)
{
	m_pHolder = pHolder;
}



template < class HolderType, class PointeeHolderType >
TPtRefer<HolderType,PointeeHolderType>::TPtRefer( HolderType* pHolder )
:m_pProxy(NULL)
{
	this->SetHolder(pHolder);
}

template < class HolderType, class PointeeHolderType >
TPtRefer<HolderType,PointeeHolderType>::TPtRefer( HolderType* pHolder, RefeeType& Refee)
:m_pProxy(NULL)  
{
	this->SetHolder(pHolder);
	this->Attach(Refee);
}

template < class HolderType, class PointeeHolderType >
TPtRefer<HolderType,PointeeHolderType>::TPtRefer( const TPtRefer& Other)
:m_pProxy(Other.m_pProxy)  
{
	if(m_pProxy)m_pProxy->m_uRefNum++;
}

template < class HolderType, class PointeeHolderType >
TPtRefer<HolderType,PointeeHolderType>::~TPtRefer()
{
	Detach();
}

template < class HolderType, class PointeeHolderType >
inline PointeeHolderType* TPtRefer<HolderType,PointeeHolderType>::Get() const
{
	return 	m_pProxy ? m_pProxy->GetHolder() : NULL;
}

template < class HolderType, class PointeeHolderType >
void TPtRefer<HolderType,PointeeHolderType>::Attach(RefeeType& Refee)
{
	//�����ж��ǲ���ȥ��������һ������
	if(m_pProxy)
	{
		if(m_pProxy == Refee.m_pProxy)
		{
			return;
		}
		//����ǣ�����detachԭ�ȵ����ö���
		if(!Refee.m_pProxy || m_pProxy != Refee.m_pProxy)
			Detach();
	}

	if(!Refee.m_pProxy)
	{
		Refee.m_pProxy = TPtProxyBankMgr<PointeeHolderType,HolderType>::Inst()->PopOrNew();
		Refee.m_pProxy->SetHolder(&Refee);
	}

	m_pProxy = Refee.m_pProxy;
	m_pProxy->m_uRefNum++;
}	

template < class HolderType, class PointeeHolderType >
void TPtRefer<HolderType,PointeeHolderType>::Detach()
{
	if(!m_pProxy)
		return;

	//���ref numΪ0������û��refeeû�б�������
	if( --m_pProxy->m_uRefNum == 0)
		TPtProxyBankMgr<PointeeHolderType,HolderType>::Inst()->Push(m_pProxy);

	m_pProxy = NULL;
}

template < class HolderType, class PointeeHolderType >
const bool TPtRefer<HolderType,PointeeHolderType>::operator < ( const ReferType& Other ) const
{
	return Get() < Other.Get();
}

template < class HolderType, class PointerHolderType >
TPtRefee<HolderType,PointerHolderType>::TPtRefee(HolderType *pHolder)
: m_pProxy(NULL)              
{
	this->SetHolder(pHolder);
}

template < class HolderType, class PointerHolderType >
TPtRefee<HolderType,PointerHolderType>::~TPtRefee()
{
	if(m_pProxy)
		m_pProxy->m_pRefee = NULL;
	m_pProxy = NULL;
}



template < class HolderType, class PointeeHolderType >
TPtBiRef<HolderType,PointeeHolderType>::TPtBiRef( HolderType* pHolder )
:m_pPointee(NULL)      
{
	this->SetHolder(pHolder);
}

template < class HolderType, class PointeeHolderType >
TPtBiRef<HolderType,PointeeHolderType>::TPtBiRef( HolderType* pHolder, PointeeType& Pointee )
:m_pPointee(NULL)
{
	this->SetHolder(pHolder);
	this->Attach(Pointee);
}

template < class HolderType, class PointeeHolderType >
TPtBiRef<HolderType,PointeeHolderType>::~TPtBiRef()
{
	Detach();
}

template < class HolderType, class PointeeHolderType >
inline PointeeHolderType* TPtBiRef<HolderType,PointeeHolderType>::Get() const
{
	return 	m_pPointee ? m_pPointee->GetHolder() : NULL;
}

template < class HolderType, class PointeeHolderType >
void TPtBiRef<HolderType,PointeeHolderType>::Attach(PointeeType& Pointee)
{
	Ast((void*)&Pointee != (void*)this);
	this->UniAttach(Pointee);
	Pointee.UniAttach(*this);
}

template < class HolderType, class PointeeHolderType >
void TPtBiRef<HolderType,PointeeHolderType>::Detach()
{
	if (m_pPointee != NULL)
		m_pPointee->Detach(*this);
}

template < class HolderType, class PointeeHolderType >
const bool TPtBiRef<HolderType,PointeeHolderType>::operator < ( const ThisType& Other ) const
{
	return Get() < Other.Get();
}

template < class HolderType, class PointeeHolderType >
void TPtBiRef<HolderType,PointeeHolderType>::Detach(PointeeType& Pointee)
{
	this->UniDetach(Pointee);
	Pointee.UniDetach(*this);
}

template < class HolderType, class PointeeHolderType >
void TPtBiRef<HolderType,PointeeHolderType>::UniAttach(PointeeType& Pointee)
{
	if (m_pPointee)
		Detach(*m_pPointee);
	m_pPointee = &Pointee;
}

template < class HolderType, class PointeeHolderType >
void TPtBiRef<HolderType,PointeeHolderType>::UniDetach(PointeeType& Pointee)
{
	if (&Pointee == m_pPointee)
		m_pPointee = NULL;
}
