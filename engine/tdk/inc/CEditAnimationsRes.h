#pragma once
#include "CAnimationGroup.h"
#include "CSkeletalFrame.h"
#include "CEditAnimate.h"
#include "CEditSkeletal.h"
#include "CEditSubject.h"
//#include "CAgpCoder.h"
namespace sqr_tools
{
	enum BatchState
	{
		eOpenErr  = 0,//����ת�������ļ���ʧ��
		eTrans    = 1,//����ת���е��ļ���ת������
		eNoTrans  = 2,//����ת���е��ļ�ûת������
	};

	class CEditAnimationGroup;
	class CEditSkeletalFrame 
		: public sqr::CSkeletalFrame
		, public CEditSubject 
	{
	public:
		CEditSkeletalFrame( const CSkeletalFrame& Res );
		CEditSkeletalFrame( const string& szName = "", const wstring& Alias= L"" );

		bool			IsFitForMe( const CEditSkeletalFrame& Skeletal, string& ErrSkeletal );
		int				GetCurSelSkelatal();	/// �õ���ǰ��ѡ����ID
		void			SetCurSelSkelatal(BYTE i);	/// ���õ�ǰ��ѡ����Ϊ��ǰ����ID
		int				GetSkeletalIdBySocketName( sqr::IDNAME SocketName, int* SocketNum );
		bool			BinSave( const string& FileName );
		int			ReLoad( const string& FileName );
		BatchState		SkeResLoad( const string& FileName,CEditAnimationGroup& EditAniGroup,int& filesize);//�������skeר��
		bool			FillSkeletalMask( SKELETAL_MASK& Mask );	//�������й�����Ч
		const CEditSkeletalFrame& operator = ( const CEditSkeletalFrame& fARes );
	};

	class CEditAnimationGroup 
		: public sqr::CAnimationGroup
		, public CEditSubject 
	{
	public:
		//static CAgpFormat Format;
		CEditAnimationGroup( const CAnimationGroup& Res );
		CEditAnimationGroup( const string& szName = "", const wstring& Alias = L"" );
		~CEditAnimationGroup() { Destroy(); }
		void				Destroy();
		CAnimateList&		GetAnimateList(void);
		bool				BinSave( const string& FileName );
		bool				BatBinSave(const string& FileName);//רΪ�������ske����agp��
		bool				ReLoad( const string& FileName );
		BatchState			AgpResLoad(const string& filename,CRefBufFile& GraphicFile);//�������skeר��

		const CEditAnimationGroup& operator = ( const CEditAnimationGroup& fARes );
	};

	inline CAnimationGroup::CAnimateList&	
		CEditAnimationGroup::GetAnimateList(void) 
	{ 
		return m_AnimationList; 
	};
}