#pragma once

#include <vector>
#include "BakerBase.h"

#include "CRenderStyle.h"

#include "CRenderTexture.h"

/********������Ⱦ����*******/

namespace sqr
{
	typedef TVector3<unsigned int> CVector3ud;
	class CRenderTexture;
	SmartPointer(CBakerMgr);
	SmartPointer(CRenderTexture);
	SmartPointer(CBakerBase);
	class CBakerMgr  :public CBakerBase
	{
	public:
		CBakerMgr(void);
		~CBakerMgr(void);
		
		void Setup();
		void Discard();
		void OnProcess(index_t x,index_t z);
		
		bool AddOffLineRenderMethod(CBakerBasePtr p);
		bool DelOffLineRenderMethod(CBakerBasePtr p);
		bool ClearOffLineRenderMethod();

		vector<CBakerBasePtr> MethodList;    //�����б�

		vector<CVector3ud> DisturbTile;      //���ſ�

		void SetTileSize(int width,int depth);
		void SetdistrubSize(int distrubSize);

	private:
		int tileWidth;
		int tileDepth;
		int distrubSize;                    //Ӱ�췶Χ
	};

}
