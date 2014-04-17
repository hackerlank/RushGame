#pragma once 
#include "CPos.h"
#include "CBaseAoiObj.h"
#include "CQuadMallocObject.h"
#include "TQuadAllocator.h"

namespace sqr
{

	class CAsyAoiViewObj;
	class CAsyAoiScene;
	class CAsyAoiDimScene;

	class CAsyAoiDimObj : public CBaseAoiObj
						, public CQuadMallocObject
	{
	public:
		CAsyAoiDimObj(CAsyAoiDimScene* pScene, CAsyAoiViewObj* pViewObj, const CFPos& pos);
		~CAsyAoiDimObj();
	
		void Release();

		CAsyAoiDimScene* GetScene() const
		{
			return m_pScene;
		}

		void SetSize( float fSize );
		void SetEyeSight( float fEyeSight );
		void SetStealth( float fStealth );
		void SetKeenness(float Keenness);
		void SetViewGroup(uint32 uGroupID);
		float GetSize() const; 
		float GetEyeSight() const; 
		float GetStealth() const; 
		float GetKeenness() const; 
		uint32 GetViewGroup() const; 

	protected:
		CAsyAoiDimScene*	m_pScene;
		CAsyAoiViewObj*	m_pViewObj;

		//����ÿһ��aoi object��Ӧһ���������Զ����һЩ��������������Ҫ�õ�����aoi ���� 
		float			m_fSize;		//���
		float			m_fEyeSight;	//����(��Ұ��Χ)
		float			m_fStealth;		//���ζ�
		float			m_fKeenness;	//�����
		uint32			m_uViewGroup;	//�۲���	�۲�����ͬ������Object����Stealth��Keenness����
	};
}
