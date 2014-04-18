#pragma once
#include "CRefObject.h"

namespace sqr
{
	SmartPointer(DepathModelInfo);
	class DepathModelInfo : public CRefObject
	{
	public:
		DepathModelInfo() 
			: nModelIndex(0)
			, nGridModelIndex(0)
			, nGroupModelIndex(0)
			, nOriginGridIndex(0)
		{
		}
		string modelName;
		string agpName;
		string skeName;
		string effectName;
		string effectFileName;
		vector<string> modelNames;
		string strItemName;

		float fRotationX;
		float fRotationY;
		float fRotationZ;
		float fScaleX;
		float fScaleY;
		float fScaleZ;
		float fX;
		float fZ;
		float fY;

		float r;
		float g;
		float b;

		bool bShadow;
		bool bSecondShadow;
		bool bTranslucent;
		bool bZTest;
		bool bSelfZTest;
		bool bRecieveShadow;
		bool bPlayAnimate;
		byte byRenderStyle;
		bool bVisibleByConfigure;
		///��������������ײ��ǣ���Щ���岻��ʹ��������������ʹ���������ײ��
		bool bUseCameraCollision;

		int nModelIndex;///���ͻ������ڼ�¼��ars��ģ�͵ڼ���������Ϣ����
		size_t nGridModelIndex;//��ͼ�������ڸø��ӣ�nOriginGridIndex���еĵڼ���modelgroup
		size_t nGroupModelIndex;///��ͼ���ڸ���ͼ�����ǵڼ���λ��
		int nOriginGridIndex;

		///��editor�õı���
		CVector3f vUserOffsetEx; //�����ƶ�
		bool	  bBakingColor;
		int		  nMirrorType;
		///trans state 0 - auto 1 -  trans 2 - untrans
		BYTE	  bTransState;
		int		  nRatio;
	};
}