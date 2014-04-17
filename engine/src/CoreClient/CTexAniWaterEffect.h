#pragma once
#include "CWaterEffect.h"
/**
	@class CTexAniWaterEffect

	������������ˮ.
	ÿ֡��һ������, �Դ���ʵ�ֲ���Ч��.
	�ԵͶ��Կ�����. ����������������, ������Ҳ��һЩ.(�ο�WOW)
 */
namespace sqr
{
class ITexture;
SmartPointer(CDx9Texture);

class CTexAniWaterEffect : public CWaterEffect
{
public:
	CTexAniWaterEffect();
	virtual ~CTexAniWaterEffect();

	/// ������������
	void SetAnimationTextures(const vector<ITexture*>& t);

	/// ����
	void OnFrameUpdate();
	/// ��Ⱦ��ʼ
	void OnRenderBegin();

private:
	vector<ITexture*> textures;
	index_t currentTextureIndex;
};

}// namespace sqr
