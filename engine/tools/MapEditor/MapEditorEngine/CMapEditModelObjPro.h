#pragma once

#include "ExsertStructDB.h"

namespace sqr
{
	struct MoldeSavePropertyOld
	{
		MoldeSavePropertyOld()
		{
			nCenterGridIndex	= 0;
			nRotationCnt		= 0;
			nPitchCnt			= 0;
			nYawCnt				= 0;
			nMirrorType			= 0;

			vRotate				= CVector3f(0.0f, 0.0f, 0.0f);
			vScale				= CVector3f(1.0f, 1.0f, 1.0f);
			vUserOffsetEx		= CVector3f(0.0f, 0.0f, 0.0f);

			byRenderStyle		= 0;
			bReceiveShadow		= false;
			bPlayAnimate		= false;
			bShadow				= false;
			bTranslucent		= false;
			bVisibleByConfigure	= false;
			bBakingColorMark	= false;
			bTransState			= 0;
		}

		///model center grid index;
		int nCenterGridIndex;

		///y����ת����
		int nRotationCnt;

		///z����ת����
		int nPitchCnt;

		///x����ת����
		int nYawCnt;

		//0: �޾��� 1��ǰ���� 2�� ���Ҿ���
		int nMirrorType;

		BYTE byRenderStyle;
		bool bPlayAnimate;///on / off animation
		///�Ƿ���Ӱ��
		bool bShadow;
		///д�Ƿ������Ӱ��Ϣ
		bool bReceiveShadow;
		///�Ƿ��������
		bool bTranslucent;
		///trans state 0 - auto 1 -  trans 2 - untrans
		BYTE bTransState;
		//ģ�Ϳɼ��Ƿ���������ֵ
		bool bVisibleByConfigure;
		//ģ�ͺ�����ɫ������Ӱɫ����,����ǿϣ���決��Ĳݸ��ر��lightmap��ɫһ��, ����color = max(bakedColor, globalShadowColor);
		bool bBakingColorMark;

		///rotate
		CVector3f vRotate;
		//����
		CVector3f vScale;
		///offset
		CVector3f vUserOffsetEx;
	};

	//��ģ�����Գ����һ���ṹ��
	struct MoldeSavePropertyNoPos
	{
		MoldeSavePropertyNoPos()
		{
			nCenterGridIndex	= 0;
			nRotationCnt		= 0;
			nPitchCnt			= 0;
			nYawCnt				= 0;
			nMirrorType			= 0;
			
			vRotate				= CVector3f(0.0f, 0.0f, 0.0f);
			vScale				= CVector3f(1.0f, 1.0f, 1.0f);
			vUserOffsetEx		= CVector3f(0.0f, 0.0f, 0.0f);

			byRenderStyle		= 0;
			bReceiveShadow		= false;
			bPlayAnimate		= false;
			bShadow				= false;
			bTranslucent		= false;
			bVisibleByConfigure	= false;
			bBakingColorMark	= false;
			bUseCameraCollision = false;
			bTransState			= 0;
		}

		///model center grid index;
		int nCenterGridIndex;

		///y����ת����
		int nRotationCnt;

		///z����ת����
		int nPitchCnt;

		///x����ת����
		int nYawCnt;

		//0: �޾��� 1��ǰ���� 2�� ���Ҿ���
		int nMirrorType;

		BYTE byRenderStyle;
		bool bPlayAnimate;///on / off animation
		///�Ƿ���Ӱ��
		bool bShadow;
		///д�Ƿ������Ӱ��Ϣ
		bool bReceiveShadow;
		///�Ƿ��������
		bool bTranslucent;
		///trans state 0 - auto 1 -  trans 2 - untrans
		BYTE bTransState;
		//ģ�Ϳɼ��Ƿ���������ֵ
		bool bVisibleByConfigure;
		//ģ�ͺ�����ɫ������Ӱɫ����,����ǿϣ���決��Ĳݸ��ر��lightmap��ɫһ��, ����color = max(bakedColor, globalShadowColor);
		bool bBakingColorMark;
		///��������������ײ��ǣ���Щ���岻��ʹ��������������ʹ���������ײ��
		bool bUseCameraCollision;

		///rotate
		CVector3f vRotate;
		//����
		CVector3f vScale;
		///offset
		CVector3f vUserOffsetEx;
	};

	//��ģ�����Գ����һ���ṹ��
	struct MoldeSaveProperty
	{
		MoldeSaveProperty()
		{
			nCenterGridIndex	= 0;
			nRotationCnt		= 0;
			nPitchCnt			= 0;
			nYawCnt				= 0;
			nMirrorType			= 0;
			
			vPosition			= CVector3f(-1.0f, -1.0f, -1.0f);
			vRotate				= CVector3f(0.0f, 0.0f, 0.0f);
			vScale				= CVector3f(1.0f, 1.0f, 1.0f);
			vUserOffsetEx		= CVector3f(0.0f, 0.0f, 0.0f);

			byRenderStyle		= 0;
			bReceiveShadow		= false;
			bPlayAnimate		= false;
			bShadow				= false;
			bTranslucent		= false;
			bVisibleByConfigure	= false;
			bBakingColorMark	= false;
			bUseCameraCollision = false;
			bTransState			= 0;
		}

		MoldeSaveProperty MoldeSaveProperty::operator = (const MoldeSavePropertyOld& modelProRight)
		{
			this->nCenterGridIndex		= modelProRight.nCenterGridIndex;
			this->nRotationCnt			= modelProRight.nRotationCnt;
			this->nPitchCnt				= modelProRight.nPitchCnt;
			this->nYawCnt				= modelProRight.nYawCnt;
			this->nMirrorType			= modelProRight.nMirrorType;

			this->vRotate				= modelProRight.vRotate;
			this->vScale				= modelProRight.vScale;
			this->vUserOffsetEx			= modelProRight.vUserOffsetEx;

			this->byRenderStyle			= modelProRight.byRenderStyle;
			this->bReceiveShadow		= modelProRight.bReceiveShadow;
			this->bPlayAnimate			= modelProRight.bPlayAnimate;
			this->bShadow				= modelProRight.bShadow;
			this->bTranslucent			= modelProRight.bTranslucent;
			this->bVisibleByConfigure	= modelProRight.bVisibleByConfigure;
			this->bBakingColorMark		= modelProRight.bBakingColorMark;
			this->bTransState			= modelProRight.bTransState;

			this->bUseCameraCollision	= false;

			return *this;
		}

		MoldeSaveProperty MoldeSaveProperty::operator = (const MoldeSavePropertyNoPos& modelProRight)
		{
			this->nCenterGridIndex		= modelProRight.nCenterGridIndex;
			this->nRotationCnt			= modelProRight.nRotationCnt;
			this->nPitchCnt				= modelProRight.nPitchCnt;
			this->nYawCnt				= modelProRight.nYawCnt;
			this->nMirrorType			= modelProRight.nMirrorType;

			this->vRotate				= modelProRight.vRotate;
			this->vScale				= modelProRight.vScale;
			this->vUserOffsetEx			= modelProRight.vUserOffsetEx;

			this->byRenderStyle			= modelProRight.byRenderStyle;
			this->bReceiveShadow		= modelProRight.bReceiveShadow;
			this->bPlayAnimate			= modelProRight.bPlayAnimate;
			this->bShadow				= modelProRight.bShadow;
			this->bTranslucent			= modelProRight.bTranslucent;
			this->bVisibleByConfigure	= modelProRight.bVisibleByConfigure;
			this->bBakingColorMark		= modelProRight.bBakingColorMark;
			this->bTransState			= modelProRight.bTransState;

			this->bUseCameraCollision	= modelProRight.bUseCameraCollision;

			return *this;
		}

		MoldeSaveProperty MoldeSaveProperty::operator = (const MoldeSaveProperty& modelProRight)
		{
			this->nCenterGridIndex		= modelProRight.nCenterGridIndex;
			this->nRotationCnt			= modelProRight.nRotationCnt;
			this->nPitchCnt				= modelProRight.nPitchCnt;
			this->nYawCnt				= modelProRight.nYawCnt;
			this->nMirrorType			= modelProRight.nMirrorType;

			this->vPosition				= modelProRight.vPosition;
			this->vRotate				= modelProRight.vRotate;
			this->vScale				= modelProRight.vScale;
			this->vUserOffsetEx			= modelProRight.vUserOffsetEx;

			this->byRenderStyle			= modelProRight.byRenderStyle;
			this->bReceiveShadow		= modelProRight.bReceiveShadow;
			this->bPlayAnimate			= modelProRight.bPlayAnimate;
			this->bShadow				= modelProRight.bShadow;
			this->bTranslucent			= modelProRight.bTranslucent;
			this->bVisibleByConfigure	= modelProRight.bVisibleByConfigure;
			this->bBakingColorMark		= modelProRight.bBakingColorMark;
			this->bTransState			= modelProRight.bTransState;

			this->bUseCameraCollision	= modelProRight.bUseCameraCollision;

			return *this;
		}

		bool MoldeSaveProperty::operator == (const MoldeSaveProperty& modelProRight) const
		{
			return this->nCenterGridIndex	== modelProRight.nCenterGridIndex &&
				this->nRotationCnt			== modelProRight.nRotationCnt &&
				this->nPitchCnt				== modelProRight.nPitchCnt &&
				this->nYawCnt				== modelProRight.nYawCnt &&
				this->nMirrorType			== modelProRight.nMirrorType &&
				this->vRotate				== modelProRight.vRotate &&
				this->vScale				== modelProRight.vScale &&
				this->vUserOffsetEx			== modelProRight.vUserOffsetEx &&
				this->byRenderStyle			== modelProRight.byRenderStyle &&
				this->bReceiveShadow		== modelProRight.bReceiveShadow &&
				this->bPlayAnimate			== modelProRight.bPlayAnimate &&
				this->bShadow				== modelProRight.bShadow &&
				this->bTranslucent			== modelProRight.bTranslucent &&
				this->bVisibleByConfigure	== modelProRight.bVisibleByConfigure &&
				this->bBakingColorMark		== modelProRight.bBakingColorMark &&
				this->bTransState			== modelProRight.bTransState &&
				this->bUseCameraCollision	== modelProRight.bUseCameraCollision;
		}

		///model center grid index;
		int nCenterGridIndex;

		///y����ת����
		int nRotationCnt;
		
		///z����ת����
		int nPitchCnt;

		///x����ת����
		int nYawCnt;

		//0: �޾��� 1��ǰ���� 2�� ���Ҿ���
		int nMirrorType;

		BYTE byRenderStyle;
		bool bPlayAnimate;///on / off animation
		///�Ƿ���Ӱ��
		bool bShadow;
		///д�Ƿ������Ӱ��Ϣ
		bool bReceiveShadow;
		///�Ƿ��������
		bool bTranslucent;
		///trans state 0 - auto 1 -  trans 2 - untrans
		BYTE bTransState;
		//ģ�Ϳɼ��Ƿ���������ֵ
		bool bVisibleByConfigure;
		//ģ�ͺ�����ɫ������Ӱɫ����,����ǿϣ���決��Ĳݸ��ر��lightmap��ɫһ��, ����color = max(bakedColor, globalShadowColor);
		bool bBakingColorMark;
		///��������������ײ��ǣ���Щ���岻��ʹ��������������ʹ���������ײ��
		bool bUseCameraCollision;

		///position
		CVector3f vPosition;
		///rotate
		CVector3f vRotate;
		//����
		CVector3f vScale;
		///offset
		CVector3f vUserOffsetEx;
	};

	struct ModelCommonProperty
	{
		ModelCommonProperty()
		{
			eEditState = EES_DANGLING;
			eEditStatePlaced = EESP_NONE;
			matUserOffset.Identity();
			matSelfOff.Identity();
			matSelfOffXMirror.Identity();
			matSelfOffZMirror.Identity();
			matWorldOff.Identity();
			mat.Identity();
			matPieceOff.Identity();

			bSelected = false;
	
			vBlockCenter = CVector3f(0.0f,0.0f,0.0f);
			vUserOffset  = CVector3f(0.0f,0.0f,0.0f);
			bLocked = false;
			fRotationCurrent = 0.0f;
			fLift = 0.0f;
			fShiftX = 0.0f;
			fShiftZ = 0.0f;

			bSubUnit = false;
			bIsChanged = true;
			bFixedHeight = false;
	
			bCameraOriented = false;
			bEffectBase = false;
			bResGroupMark = 0;

			bHideModelVertexColor = false;
			bEffectLight = false;
			bStopCue = false;

			fRedOverallAmbientRatio = 1.0f;
			fBlueOverallAmbientRatio = 1.0f;
			fGreenOverallAmbientRatio = 1.0f;

			bNotGroup = TRUE;//%%%%%
			nCenterGridIndexCurrent = INVALID;//%%%%%

			strFileName		= "";
			strModelAgpName = "";
			strModelSkeName = "";
		}

		//�Ƿ��Ǹ���ͼ��
		bool		bSubUnit;
		// �Ƿ����ض���ɫ��ģ�͵�Ӱ��
		bool		bHideModelVertexColor;
		// �Ƿ�Ϊnpc
		BYTE		bResGroupMark;
		bool		bStopCue;
		bool		bEffectLight;
		bool		bLocked;
		bool		bFixedHeight;
		bool		bCameraOriented;
		bool		bEffectBase;
		bool		bIsChanged;//�Ƿ���Ҫ����ʵʱ�������
		bool		bSelected;
		// �Ƿ���Ⱥ��
		BOOL		bNotGroup;

		float		fLift;
		float		fShiftX;
		float		fShiftZ;
		float		fHeight;
		float		fWidth;
		float		fDepth;
		float		fRedOverallAmbientRatio;
		float		fBlueOverallAmbientRatio;
		float		fGreenOverallAmbientRatio;
		float		fRotationCurrent;// ����ʵʱ���㵱ǰ���ĸ�������

		int			nWidthDimInGrids;
		int			nDepthDimInGrids;
		int			nXGroupOffset;
		int			nZGroupOffset;
		// center grid
		int			nPreCenterGridIndex;
		// derived from "scale and offset"
		int			nWidthRadius;
		int			nDepthRadius;
		int			nCenterGridIndexCurrent;// ʵʱ����õ��ĵ�ǰ���ĸ�������������Ⱥ����ת��

		EEdit_State eEditState;
		sqr::EEdit_State_Placed eEditStatePlaced;
		sqr::ECenter_Type eCenterType;

		string		strFileName;///model name
		string		strModelAgpName;
		string		strModelSkeName;

		CVector3f	m_vCorners[8];
		CVector3f	vCorners[8];
// 		CVector3f	vScale; ///ģ�ͱ�����ݴ�С��scale
// 		CVector3f	vOffset;
		CVector3f	vOffsetOverall;
		CVector3f	vBlockCenter;
		CVector3f	vImageCenter;
		CVector3f	vGroupOffset;
		CVector3f   vUserOffset;///�ڱ������ƫ����Ϣ�󣬸�ֵ��Ϊ�򿪵�ͼ�������ƶ���ƫ�����������ֵ��Ϊ�˱���vUserOffsetEx�������������ڵ�ͼ����е�ƫ��
		// ���Ⱥ�������
		CVector3f vGroupCenter;

		CMatrix		matSelfOff;///SetOffsetOverall
		CMatrix		matSelfOffXMirror;
		CMatrix		matSelfOffZMirror;
		CMatrix		matUserOffset;
		CMatrix		matWorldOff;///������������ƫ��
		CMatrix		matRotY;	///xyz��ת��ľ���
		CMatrix		matScale;	///����
		CMatrix		mat;
		CMatrix		WireBoxMat;
		CMatrix		matPieceOff;
	};
}
