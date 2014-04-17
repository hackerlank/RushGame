 #pragma once
#include "CFontBase.h"
/**
	@class CDistanceFont

	����distance-field������.
	��ͼ�е��������洢��������ص����������ߵľ���ֵ, �ԻҶȱ�ʾ.
	��Ϊ������в�ֵ��ȵ������徫�ȸ�, ���ԷŴ�������������������.
	����������������ν�ֺ�, �������Ƕ��. ���������С�������ļ�����, һ��Ϊ32
	ȱ������Ҫshader��Ⱦ, ������ҪԤ���ɺ�����ڴ�.
	���KlayGE�ĵ�

	@todo ��ߺ���ӰЧ����Ҫ�Ż�(����Ҫ����Ļ������shader������ƫ��)
 */

namespace sqr
{

class CDistanceFont : public CFontBase
{
public:
	CDistanceFont();
	virtual CVector2f CalculateSize(const wstring& text);
private:
	virtual bool OnCreate();
	virtual void OnResize();
	virtual void OnDestroy();
	virtual void CacheCharacter(wchar_t ch, int x, int y);

	virtual bool LoadFontFile();
	virtual bool UpdateShaderParams();
	
protected:
	virtual const CharInfo* GetCharInfo(wchar_t ch, uint8 idx = 0);
	virtual float GetLineSpacing() const;
	virtual DWORD GetShaderID();
	uint	fontCharSize;						///< �����ļ��е������С

private:
	typedef GHashMap<wchar_t, index_t> IndexMap;
	IndexMap charIndexMap;						///< keyΪ�ַ�, valueΪCharInfo��Index
	GVector<CharInfo> charInfos;				///< �ַ�ʵ�ʴ�С(���ƾ���)
	
	float distRange[2];							///< distance��Χ
	GVector<uint8> distances;					///< �ַ�distance��Ϣ(���Կ��������ؼ�)

	enum
	{
		FontEffectNormal,
		FontEffectBold,

		FontEffectNum
	};
	uint shaderIds[FontEffectNum];
	static uint ShaderIdBase;
	float m_fShowSize;
};

}// namespace sqr
