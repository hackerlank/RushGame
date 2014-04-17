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
	//const SVector<ITexture*>& GetAnimationTextures() const;
	/// ������������
	void SetAnimationTextures(const SVector<ITexture*>& t);

	/// ����
	void OnFrameUpdate();
	/// ��Ⱦ��ʼ
	void OnRenderBegin();

private:
	SVector<ITexture*> textures;
	index_t currentTextureIndex;
};

}// namespace sqr
