#pragma once

#include "PropCtrl\CBasePropCtrlImp.h"

namespace sqr_tools
{

class CLightPropCtrlImp :public CBasePropCtrlImp
{
public:
	CLightPropCtrlImp(void);
	~CLightPropCtrlImp(void);

public:
	//��ȡѡ�еƹ�����(��� �۹�)
	virtual int GetSelLightID();
	virtual int GetSelLightType();
	virtual float GetSelLightPosX();
	virtual float GetSelLightPosY();
	virtual float GetSelLightPosZ();
	virtual int GetSelLightColor();
	virtual float GetSelLightDirX();
	virtual float GetSelLightDirY();
	virtual float GetSelLightDirZ();
	virtual float GetSelLightMaxDis();
	virtual float GetSelLightFallOff(); //�۹������˥��
	virtual float GetSelLightAtten1();
	virtual float GetSelLightAtten2();
	virtual float GetSelLightAtten3();
	virtual float GetSelLightInnerR();
	virtual float GetSelLightOuterR();
	//s����ѡ�еƹ�����(��� �۹�)
	virtual void SetSelLightID(int light_id);
	virtual void SetSelLightType(int light_type);
	virtual void SetSelLightPos(float px,float py,float pz);
	virtual void SetSelLightColor(int color);
	virtual void SetSelLightDir(float dx,float dy,float dz);
	virtual void SetSelLightMaxDis(float mdis);
	virtual void SetSelLightFallOff(float fall); //�۹������˥��
	virtual void SetSelLightAtten1(float atten1);
	virtual void SetSelLightAtten2(float atten2);
	virtual void SetSelLightAtten3(float atten3);
	virtual void SetSelLightInnerR(float innerR);
	virtual void SetSelLightOuterR(float outerR);
};

//!namespace
}