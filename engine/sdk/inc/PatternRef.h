#pragma once

#include "PtRefProxy.h"

namespace sqr
{
	template < class HolderType, class PointeeHolderType >
	class TPtRefBase
	{
	protected:
		inline TPtRefBase();
	public:
		inline HolderType*	GetHolder() const;
		inline void SetHolder(HolderType* pHolder);	
	private:
		HolderType*			m_pHolder;
	};

	//���ô����࣬����refer��������refee�ļ���	
	template < class HolderType, class PointeeHolderType >
	class TPtProxy;

	template < class HolderType, class PointeeHolderType >
	class TPtRefee;

	template < class HolderType, class PointeeHolderType >
	class TPtProxyBankMgr;

	//������
	template < class HolderType, class PointeeHolderType >
	class TPtRefer
		:public TPtRefBase< HolderType, PointeeHolderType >
	{
		typedef TPtRefer< HolderType, PointeeHolderType > ReferType;
		typedef TPtRefee< PointeeHolderType, HolderType > RefeeType;
		typedef TPtProxy< PointeeHolderType, HolderType > ProxyType;
		friend class TPtProxyBankMgr<HolderType, PointeeHolderType>;
	public:
		TPtRefer( HolderType* pHolder = NULL );
		TPtRefer( HolderType* pHolder, RefeeType& Refee);
		TPtRefer( const TPtRefer& Other);

		~TPtRefer();

		inline PointeeHolderType*	Get() const;

		void Attach(RefeeType& Refee);

		void Detach();

		const bool operator < ( const ReferType& Other ) const;
	private:
		ProxyType*	m_pProxy;
	}; 

	//��������
	template < class HolderType, class PointerHolderType >
	class TPtRefee
		:public TPtRefBase< HolderType, PointerHolderType >
	{
		typedef TPtProxy< HolderType, PointerHolderType >	ProxyType;
		friend class TPtRefer< PointerHolderType, HolderType >;
		friend class TPtProxy< HolderType, PointerHolderType >;
		friend class TPtProxyBankMgr< HolderType, PointerHolderType >;
	public:
		TPtRefee(HolderType *pHolder = NULL);

		~TPtRefee();

	private:
		ProxyType*	m_pProxy;
	}; 


	//˫�����ã�ֻ������1��1���໥�������
	template < class HolderType, class PointeeHolderType >
	class TPtBiRef
		:public TPtRefBase< HolderType, PointeeHolderType >
	{
		typedef TPtBiRef< HolderType, PointeeHolderType > ThisType;
		typedef TPtBiRef< HolderType, PointeeHolderType > BaseType;
		typedef TPtBiRef< PointeeHolderType, HolderType > PointeeType;
		friend class TPtBiRef< PointeeHolderType, HolderType >;
	public:
		TPtBiRef( HolderType* pHolder = NULL );
		TPtBiRef( HolderType* pHolder, PointeeType& Pointee );
		~TPtBiRef();
		inline PointeeHolderType*	Get() const;
		void Attach(PointeeType& Pointee);
		void Detach();
		const bool operator < ( const ThisType& Other ) const;
	private:
		void Detach(PointeeType& Pointee);

		/* ====================  OPERATIONS    ======================================= */
		void UniAttach(PointeeType& Pointee);
		void UniDetach(PointeeType& Pointee);

		PointeeType* 	m_pPointee;
	};

}
