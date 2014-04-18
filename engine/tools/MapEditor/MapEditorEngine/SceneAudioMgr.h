#pragma once
#include "TSingleton.h"

namespace sqr
{
	class CTObject;
	class CSceneAudioMgr: public Singleton<CSceneAudioMgr>
	{
	private:
		///��Ч
		int						m_nCueRange;
		map<string,int8>		m_mapTerMaterialsIndexs;   //first: texname name second: material index
		map<string,int8>		m_mapMaterialIndexs;	   //first glass, sold, sand... second: material index
		map<int8,string>		m_mapIndexMaterials;	   //second glass, sold, sand... first: material index
		string					m_strPhyleName;			   //��������
		int8					m_nCurSeleteMaterialIndex; ///��ǰ��ͼ�ذ���ѡ�еĲ�������

		vector< CTObject *>		m_vecIndividualSoundObjects;
	public:
		CSceneAudioMgr();
		~CSceneAudioMgr();

		void	createAudioSystem();
		void	UpdateAudio();

		void	SetPhyleName( const string& strPhyleName, const string& strDefaultMatName );
		string  GetPhyleMaterialName(int8 index);	

		void	SetMaterialIndex( string& name, int8 index );
		int8	GetMaterialIndex( string& name );
		string	GetMaterialName(const int8 index) const;

		void	SetTerMaterialIndex( vector<string>& names, int8 index );
		int8	GetTerMaterialIndex( string& name );

		void	CalTerrainMaterialIndex();
		
		///������Чģ��show/hide
		void SwitchSoundObjectsShow(bool bRenderSmallMap = false);
		///������Чplay/stop
		void SwitchSoundPlay();
		///�޸���Ч��Χ
		void ChangeSoundRange();

		void SetSpeedOfSoundVariable(float fValue);
		void SetAmbientVolume(const float fValue);

		inline void SetSceneCueBoxRange(const int nRange)
		{
			m_nCueRange = nRange;
		}

		inline int GetSceneCueBoxRange() const
		{
			return m_nCueRange;
		}

		// -----------------------------------------------------------------------------
		inline void SetCurSeleteMaterialIndex( const int8 nMatIndex )
		{
			m_nCurSeleteMaterialIndex = nMatIndex;
		}

		// -----------------------------------------------------------------------------
		inline int8 GetCurSeleteMaterialIndex( )
		{
			return m_nCurSeleteMaterialIndex;
		}

		inline void AddIndividualSoundObject( CTObject * p )
		{
			m_vecIndividualSoundObjects.push_back(p);
		}

		inline void RemoveIndividualSoundObject(CTObject * p)
		{
			m_vecIndividualSoundObjects.erase(
				remove(m_vecIndividualSoundObjects.begin(), m_vecIndividualSoundObjects.end(), p), m_vecIndividualSoundObjects.end() );
		}
	};
}