#pragma once
#include "GraphicBase.h"
#include "CBufFile.h"
#include "CMatrixUnit.h"
#include "CCoder.h"
namespace sqr
{
	//�����ĵ���SOCKET
	struct CSkeletalSocket : public CGraphicMallocObject
	{
		CSkeletalSocket()
		{
			m_SocketName = 0;
			m_Socket.Identity();
		}

		IDNAME				m_SocketName;			//SOCKET����
		CMatrix				m_Socket;				//SOCKET����
	};

	//ÿ��������SOCKET����

	class CSkeletalUnit  : public CGraphicMallocObject
	{
	public:
		CSkeletal*				m_pSkeletal;
		CMatrixUnit				m_InverseMatrix;//ÿ�����������Ƥ������ȫ�ֿռ䵽�����ֲ��ľ���
		GVector<CSkeletalSocket>	m_SocketList;			//������SOCKET���б�

		int BinLoad( CBufFile& GraphicFile );
		int BinSave( WriteDataInf& wdi );
	};

	//��������
	class CSkeletal  : public CGraphicMallocObject
	{
		friend class CDataSources;
	protected:
		BYTE					m_ParentSkeletalID;		//������ID
		BYTE					m_SkeletalID;           //��ͷID
		GString					m_SkeletalName;			//��ͷ����
		GVector<CSkeletal*>		m_Children;				//�ӹؽ�
		CSkeletal();
	public:
		
		~CSkeletal();

		void	Destroy();
		bool	IsFitForMe( const CSkeletal* Skeletal );				//ƥ�����
		int		GetSkeletalIDByName( const string& Name ) const;		//���ݹ������õ�����ID������������ӹ�����
		const char*	GetSkeletalNameByID( BYTE SkeletalID) const;

		BYTE	GetSkeletalID() const
		{
			return m_SkeletalID;    //�õ���������ID
		}
		BYTE	GetParentSkeletalID() const
		{
			return m_ParentSkeletalID;    //�õ�������ID
		}
		const char* GetName() const
		{
			return m_SkeletalName.c_str();
		}
		void SetName(const char* bonename)
		{
			m_SkeletalName = bonename;
		}
		uint32    GetChildNum() const
		{
			return (uint32)m_Children.size();    //�õ��ӹ�������
		}
		const   CSkeletal* GetChild( uint32 Num )const
		{
			return Num < GetChildNum() ? m_Children[Num] : NULL;    //�õ�ָ�����ӹ���
		}

		//�����������
		void	CalculateMatrix( const CMatrixUnit& matUnit, CRenderMatrix& RenderMatrix,const CAnimationGroup& Animates,
			const CSkeletalFrame& SkeletalFrame, CAnimateSeparation* pParentSep = NULL) const;
		int		BinLoad( CBufFile& GraphicFile, BYTE ParentSkeletalID, CSkeletal** pSkeletal );
		int		BinSave( WriteDataInf& wdi);
	};
};
