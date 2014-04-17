#pragma once 
#include "CMirrorQuadScene.h"

namespace sqr
{
	template<typename T> struct TRect;
	typedef TRect<float> CFRect;

	class CTrMirrorBuffer;

	class CWBQuadScene : public CMirrorQuadScene
	{
	public:
		friend class CWBQuadSceneCreatedCmd;
		friend class CWBQuadSceneReleasedCmd;
			
		static CWBQuadScene* Create(uint32 uWidth, uint32 uHeight, uint32 uPrecision,
			uint32 uSlot, CTrMirrorBuffer* pMgr);
		void Release(uint32 uSlot, CTrMirrorBuffer* pMgr);

	private:
		CWBQuadScene(uint32 uWidth, uint32 uHeight, uint32 uPrecision);
		~CWBQuadScene();
		
		/*
			��Ϊ������quad scene������buffer���洴���ģ����������ڵ���release��ʱ��
			��Ҫ�ֶ���quad scene���ڴ����
		*/
		void DestroyScene();
	};

}
