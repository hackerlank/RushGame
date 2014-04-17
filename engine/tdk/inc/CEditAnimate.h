#pragma once
#include "CAnimation.h"
#include "CEditSubject.h"

namespace sqr_tools
{
	//ÿ�������ؼ�֡����
	class TEditKeyFrames : public CKeyFrames
	{
	public:
		//void BinSave( ofstream& File ,float fParam);
		void BinSaveFull(ofstream& File, size_t kSize );
		//void _RecordTinyData(GVector<CTinyVector>& tinyVector,GVector<CTinyQuaternion>& tinyQuaternion)	{m_TinyTranData = tinyVector;m_TinyQuatData = tinyQuaternion;};
	};

	class CEditAnimate 
		: public sqr::CAnimate
		, public CEditSubject 
	{
	public:
		CEditAnimate( CAnimationGroup* pAniGp, const string& szFileName, const wstring& Alias, IDNAME& AniName )
			: CAnimate( pAniGp, szFileName,Alias, AniName ){}

		/// �����ɾ������֡�����ַ�����sqr::IDNAME& NameΪ�գ���ɾ������֡�����ַ��� ��
		void	InsertFrameString( WORD Frame, sqr::IDNAME& Name );
		DWORD	GetVersion(void); 
		void	SetName( sqr::IDNAME& Name );
		int		BinSave( string SaveName );
		//CKeyFrames* _NewKeyFrames8(void) { return new TEditKeyFrames<uint8>; }
		//CKeyFrames* _NewKeyFrames16(void) { return new TEditKeyFrames<uint16>; }
		void		SetGroundSpeed(float fGroundSpeed)	{ m_GroundSpeed = fGroundSpeed;}
	};

	inline void 
		CEditAnimate::SetName( sqr::IDNAME& Name )
	{
		m_AnimationeName = Name;
	}

	inline DWORD 
		CEditAnimate::GetVersion(void)
	{
		return 0;
	}
}