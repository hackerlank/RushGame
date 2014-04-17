#pragma once
#include "CEditorClassPre.h"
#include "CEditAnimate.h"
#include "CEditAnimationsRes.h"
#include "CEditModel.h"
#include "CEditPieceGroup.h"
#include "CEditRenderStyle.h"
#include "CEditSkeletal.h"

namespace sqr_tools
{
	//���߽��õ�һЩ����ģ�ͣ���Ⱦ���͵ȵ�
	//TP_	: Tools Public data
	//EPRS_	: CEditPieceRenderStyle ����
	//RO_	: CRenderOperation		����
	class CEditorCore
	{
	public:
		static bool Initialize(void);
		static bool Destroy(void);
	public:
		static	float					TP_F_GoldenSection;	// �ƽ�ָ����

	public: //����RS
		static CEditRenderStyle	TR_EPRS_SELECT;		// ѡ�������ʾ����_RS
		static CEditRenderStyle	TR_EMRS_SELECT;		// ѡ�������ʾ_RS
		static CEditRenderStyle	TR_EMRS_HOLD;		// ѡ�������ͣ��ʾ_RS
	
	public:
		static CRenderOperation			TP_RO_BONE;			// ��ͷģ��
		static CRenderOperation			TP_RO_CONE;			// ����׶
		static CRenderOperation			TP_RO_SPHERE;		// ��ģ��

	public:
		static ITexture*				TP_TX_BLACKWHITE;	//���̸�ڰ�ͼ
		static ITexture*				TP_TX_VIOLETWHITE;	//���̸��ϰ�ͼ
		static ITexture*				TP_TX_BLACK;		//��ͼ
		static ITexture*				TP_TX_GRAY;			//��ͼ
		static ITexture*				TP_TX_WHITE;		//��ͼ
	};
}