#pragma once

/***������Ⱦ����***/

#include "CRefObject.h"
#include "CRenderTexture.h"

namespace sqr
{
	class CRenderTexture;
	SmartPointer(CBakerBase);
	SmartPointer(CRenderTexture);
	class CBakerBase : public CRefObject
	{
	public:
		CBakerBase(void);
		~CBakerBase(void);
		
		//��ʼ��
		virtual void Setup(){};
		//�ͷ���Դ
		virtual void Discard(){};
		//����׼��
		virtual void OnPrepare(){};
		//׼��
		virtual void TilePrepare(index_t x,index_t z){};
		//����
		virtual void OnProcess(index_t x,index_t z){};
	};

}
