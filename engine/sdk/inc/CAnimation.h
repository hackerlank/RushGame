#pragma once
#include "GraphicBase.h"
#include "CBufFile.h"
#include "CResourceContainer.h"
#include "CAnimationGroup.h"
#include "TraceHelper.h"
#include "CCoder.h"
namespace sqr
{
	struct CKeyFrames : public CGraphicMallocObject
	{
		virtual ~CKeyFrames() {}
	protected:
		GVector<uint16>				m_TranFrame;
		GVector<CVector3f>			m_TranData;
		GVector<uint16>				m_QuatFrame;
		GVector<CQuaternion>		m_QuatData;
	public:
		bool IsFrameExist() const
		{
			return !m_TranData.empty() && !m_QuatData.empty();
		};
		WORD GetFrameNum() const
		{
			return *m_TranFrame.rbegin();
		}
		void GetFirst( CQuaternion& Quat, CVector3f& Tran, float fParam )const
		{
			Quat = (CQuaternion)(*m_QuatData.begin());
			Tran = m_TranData[0]/*m_TranData.begin()->Get( fParam )*/;
		}

		void GetLast( CQuaternion& Quat, CVector3f& Tran, float fParam )const
		{
			Quat = (CQuaternion)(*m_QuatData.rbegin());
			Tran =m_TranData[m_TranData.size()-1]/* m_TranData.rbegin()->Get( fParam )*/;
		}

		void GetFrameData( CQuaternion& Quate, CVector3f& Trans, float fFrame, float fParam )const;

		void BinLoad( CBufFile& GraphicFile ,float fParam, size_t kSize);
		void BinLoadFull(CBufFile& GraphicFile, size_t kSize );
		//void BinSaveFull(WriteDataInf& wdi );
		virtual void _RecordTinyData(GVector<CTinyVector>& tinyVector,GVector<CTinyQuaternion>& tinyQuaternion)	{};

	};

	//������
	class CAnimate 
		: public CBaseGraphicRead
		, public CResUintNode
	{
		friend class CAniCoder;
	protected:
		//CAnimationGroup*		m_pAnimationGroup;
		int						m_MaxFrame;				//�������֡��
		float					m_FrameRatio;			//֡��
		float					m_fScale;				//�Ŵ���
		IDNAME					m_AnimationeName;		//������
		GVector<CKeyFrames>		m_SkeletalList;			//�����ļ���
		GMap<WORD,IDNAME>		m_FrameString;			//����֡�����ַ���

		bool					m_IsCpAni;				//�����Ƿ�ѹ��

		float					m_fTransParam;
		float					m_GroundSpeed;			//ÿ�������Եص��ٶ�

		bool					m_bLoad;

		void OnRead( CRefBufFile& GraphicFile );
		//void BinLoad();

	public:
		CAnimate(CAnimationGroup* pAniGp, const string& szFileName, const wstring& Alias, IDNAME& AniName );
		
		virtual ~CAnimate(){}

	public:
		int32	CreateResFile( void );
		float	GetFrameRatio();
		void    SetFrameRatio( float Ratio );

		//���ع�����Ŀ
		uint32	GetSkeletalNum();

		//���ض�������
		IDNAME	GetName();

		//�õ�����֡��
		int		GetFrameNum();

		//����ָ�������Ķ����ؼ�֡�б�
		const CKeyFrames* GetKeyFrame( uint32 SkeletalID );

		//���ر��������ж���֡�����ַ���
		const GMap<WORD,IDNAME>* GetFrameString();

		//���ر����������нŲ����Ĺؼ�֡���ؼ�֡��
		const GMap<WORD, GString> GetFootStepFrames();
		//�õ�ָ��֡����֡�����ַ���
		const IDNAME* GetFrameString( WORD CurFrameNum );

		//�õ������Եص��ٶ�
		float	GetGroundSpeed();
		float GetScale();
		float	GetTransParam();
		CAnimationGroup* GetRootGroup(void);	

		bool IsLoaded();
	};

	inline float	CAnimate::GetFrameRatio()
	{
		return m_FrameRatio;
	}

	inline void    CAnimate::SetFrameRatio( float Ratio )
	{
		m_FrameRatio = Ratio;
	}

	//���ع�����Ŀ
	inline uint32	CAnimate::GetSkeletalNum()
	{
		if ( IsValid() )
			return (uint32)m_SkeletalList.size();
		else
			return 0;
	}

	//���ض�������
	inline IDNAME	CAnimate::GetName()
	{
		return m_AnimationeName;
	};

	//�õ�����֡��
	inline int		CAnimate::GetFrameNum()
	{
		//if ( !m_szName.empty() )BinLoad();
		if(IsValid())
			return m_MaxFrame;
		else
			return 0;
	}

	//����ָ�������Ķ����ؼ�֡�б�
	inline const CKeyFrames* CAnimate::GetKeyFrame( uint32 SkeletalID )
	{
		//if ( !m_szName.empty() )BinLoad();
		if( IsValid() && SkeletalID < m_SkeletalList.size() )
		{
			return &m_SkeletalList[SkeletalID];
		}
		else 
			return NULL;
	}

	//���ر��������ж���֡�����ַ���
	inline const GMap<WORD,IDNAME>* CAnimate::GetFrameString()
	{
		//if ( !m_szName.empty() )BinLoad();
		return &m_FrameString;
	}

	//�õ������Եص��ٶ�
	inline float	CAnimate::GetGroundSpeed()
	{
		return m_GroundSpeed;
	}

	inline float CAnimate::GetScale()
	{
		return m_fScale;
	}

	inline float	CAnimate::GetTransParam()
	{
		return m_fTransParam;
	}

	inline CAnimationGroup* CAnimate::GetRootGroup(void)
	{
		return (CAnimationGroup*)m_pGroup;
	}
}