#pragma once
#include "CVector3.h"

namespace sqr
{
	class CWireBox;

	class SVertex
	{
	public:
		SVertex(void);
		~SVertex(void);

		DWORD		dwVertexIndex;
		CVector3f	vPosition;
		CVector3f   vNormal;

		D3DCOLOR	oColor;				///original color ����ԭʼ��ɫ������ˢ�Ķ���ɫ��
		D3DCOLOR	bColor;				///baking   color �����Ķ���ɫ
		D3DCOLOR	sColor;				///specular color ����߹�ֵ
		D3DCOLOR	tColor;				///temp baking   color �����Ķ���ɫ����ʱ�洢ֵ

		BYTE		usbUsedCnt;
		DWORD       dwGridsBelongto[4];
		float		fLogicHeight;
		CWireBox   *pWireBoxLogicHeight;
		bool		bHasCalculate;

		CWireBox   *GetWireBoxLogicHeight();
	};
}
