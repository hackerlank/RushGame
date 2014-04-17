#include "stdafx.h"
#include "CBaseAoiObj.h"
#include "CPos.h"

void CBaseAoiObj::CalSeeRelation
(
 bool& bASeeB,			bool& bBSeeA,				/*A�ܷ�ҊB��B�ܷ�ҊA*/
 const CFPos& AAoiPos,	const CFPos& BAoiPos,		/*A����λ�ã�B����λ��*/
 float fAEyeSight,		float fBEyeSight,			/*Aҕ�࣬Bҕ��*/
 float fASize,			float fBSize,				/*A�Ĵ�С��B�Ĵ�С*/
 float fAKeenness,		float fBKeenness,			/*A�����J�ȣ�B�����J��*/
 float fAStealth,		float fBStealth,			/*A���[��������B���[������*/
 uint32 uAViewGroup,	uint32 uBViewGroup			/**/
 )
{
	float fASeeBDist = fAEyeSight + fBSize;
	float fBSeeADist = fBEyeSight + fASize;

	if( (uBViewGroup == 0) || (uAViewGroup != uBViewGroup) )
	{
		//��������Է��־����Ӱ��
		float fASeeBDistDec=fBStealth-fAKeenness;
		float fBSeeADistDec=fAStealth-fBKeenness;

		//�����������������������൱����ȫ������������ٸߣ�Ҳ��Ӧ�����Ӷ���ȫ���������Ŀ��Ӿ���

		if(fASeeBDistDec<0)
			fASeeBDistDec=0;
		if(fBSeeADistDec<0)
			fBSeeADistDec=0;

		fASeeBDist-=fASeeBDistDec;
		fBSeeADist-=fBSeeADistDec;
	}

	float fOffSetX = (float)abs(AAoiPos.x - BAoiPos.x);
	float fOffSetY = (float)abs(AAoiPos.y - BAoiPos.y);

	//���a��b����x��y�ϵľ����ASeeBDistҪ��a�϶�������b
	if (fOffSetX > fASeeBDist || fOffSetY > fASeeBDist)
	{
		bASeeB = false;
		//ͬ����bҲ������a
		if (fOffSetX > fBSeeADist || fOffSetY > fBSeeADist)
		{
			bBSeeA = false;
			return;
		}
	}
	if (fOffSetX > fBSeeADist || fOffSetY > fBSeeADist)
	{
		bBSeeA = false;
		if (fOffSetX > fASeeBDist || fOffSetY > fASeeBDist)
		{
			bASeeB = false;
			return;
		}
	}

	float fDist=AAoiPos.Dist(BAoiPos);
	//float fDist = AAoiPos.FastDist(AAoiPos);

	bASeeB= (fBSize>0) && (fAEyeSight>0) ? fASeeBDist>fDist : false;
	bBSeeA= (fASize>0) && (fBEyeSight>0) ? fBSeeADist>fDist : false;
}



bool CBaseAoiObj::CalSeeRelation
(
 const CFPos& AAoiPos,	const CFPos& BAoiPos,
 float fAEyeSight,		float fBSize,
 float fAKeenness,		float fBStealth,
 uint32 uAViewGroup,	uint32 uBViewGroup
 )
{
	if( fBSize <= 0 || fAEyeSight <=0 )
		return false;

	float fASeeBDist = fAEyeSight + fBSize;

	if( (uBViewGroup == 0) || (uAViewGroup != uBViewGroup) )
	{
		//��������Է��־����Ӱ��
		float fASeeBDistDec=fBStealth-fAKeenness;

		//�����������������������൱����ȫ������������ٸߣ�Ҳ��Ӧ�����Ӷ���ȫ���������Ŀ��Ӿ���
		if(fASeeBDistDec<0)
			fASeeBDistDec=0;

		fASeeBDist-=fASeeBDistDec;
	}

	float fOffSetX = (float)abs(AAoiPos.x - BAoiPos.x);
	float fOffSetY = (float)abs(AAoiPos.y - BAoiPos.y);

	if (fOffSetX > fASeeBDist || fOffSetY > fASeeBDist)
	{
		return false;
	}

	float fDist=AAoiPos.Dist(BAoiPos);
	//float fDist = AAoiPos.FastDist(AAoiPos);

	return fASeeBDist>fDist;
}

