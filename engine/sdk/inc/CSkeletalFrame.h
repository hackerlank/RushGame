#pragma once
#include "CSkeletal.h"
#include "CResourceContainer.h "
#include "GraphicRes.h"
#include "GraphicBase.h"

namespace sqr
{
	const int32 maxBoneNum = 256;
	

	class CSkeletalFrame
		: public CBaseGraphicRead
	{
		friend class CSkeCoder;
		friend class CDataSources;
	protected:
		CSkeletal*					m_pSkeletalRoot;			//������
		GVector<CSkeletalUnit*>		m_SkeletalList;			//���й�������SOCKET����Ԫ�б�
		CSkeletalFrame(const string& szName, const wstring& Alias);
	public:
		virtual ~CSkeletalFrame();

		virtual	void	Destroy();

		inline	CSkeletalUnit* operator []( int Num )	const;					//�õ�ָ����������Ԫ
		inline	const CSkeletal& GetRootSkeletal()		const;					//�õ�������
		inline	uint32 GetSkeletalNum()					const; 					//�õ���������

		int		GetSkeletalIDBySocketName( IDNAME SocketName, int& SocketNum );	//ͨ��SOCKET���ֵõ�����ID
		bool	SetSkeletalMask( SKELETAL_MASK& Mask );							//������Ч����
		void	GetIDBySocketName( int& SkeletalID, int& SocketID, IDNAME SocketName );
		int		GetSkeletalIDByName( const char* SkeletalName );;
		int		GetSkeletalIDByIndex( const int Index );
		int		GetSkeletalTotalNum();
		int		GetSkeletalChildNum( uint32 SkeletalID );
		int		GetSkeletalChildID( uint32 SkeletalID, uint32 ChildNum );
		int		GetSocketNum( uint32 SkeletalID );
		const char*	GetSkeletalName( uint32 SkeletalID );
		const char*	GetSocketName( uint32 SkeletalID, uint32 SocketID );

		//�����ļ�
		int		BinLoad(void);
		void	OnRead( CRefBufFile& GraphicFile );
		bool	IsValid();
	protected:
		void	Free(void);
	};

	inline CSkeletalUnit* CSkeletalFrame::operator []( int Num ) const
	{
		return m_SkeletalList[Num];
	}

	inline const CSkeletal& CSkeletalFrame::GetRootSkeletal() const
	{
		return *m_pSkeletalRoot;
	}

	inline uint32 CSkeletalFrame::GetSkeletalNum() const
	{
		return (uint32)m_SkeletalList.size();
	}
}