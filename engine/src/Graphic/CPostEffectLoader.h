#pragma once
//------------------------------------------------------------------------------
/**
	@class CPostEffectLoader

	��XML�ļ�����PostEffect����
	����RTT Name: default, none, previous
*/
#include "CRefObject.h"
#include "CGraphicMallocObject.h"
class TiXmlNode;
namespace sqr
{
	class CXmlConfig;

	SmartPointer(CPostEffect);
	SmartPointer(CPostEffectLoader);
	SmartPointer(CRenderTexture);

	class CPostEffectLoader
		: public CRefObject
		, public CGraphicMallocObject
	{
	public:
		static const GString DefaultTarget;
		static const GString NoneTarget;
		static const GString PreviousTarget;

	public:
		CPostEffectLoader(const string& fileName);
		virtual ~CPostEffectLoader();

		/// Ԥ����Effect
		void PreLoadEffects(GVector<CPostEffectPtr>& outEffects);
		/// ����PostEffect
		void LoadPostEffect(CPostEffectPtr& outEffect);
		/// ȡ��ָ��RTT
		CRenderTexture* GetRenderTexture(const string& name) const;
		/// ȡ��Ĭ��RTT
		CRenderTexture* CreateDefaultRTT();
		/// �ͷŲ�ʹ�õ�RTT
		void ReleaseUnusedRTT();

	private:
		/// ����Pass���
		void ParsePostEffectPass(TiXmlNode* xmlNode, const CPostEffectPtr& postEffect);
		/// ����ָ����RTT
		const CRenderTexturePtr&  ParseRenderTexture(const string& name);

	private:
		CXmlConfig *pPostEffectCfg;
		TiXmlNode* xmlConfig;
		GMap<GString, CRenderTexturePtr>	renderTargetMap;
	};

	//------------------------------------------------------------------------------
	inline CRenderTexture*
		CPostEffectLoader::CreateDefaultRTT()
	{
		return this->ParseRenderTexture(DefaultTarget.c_str());
	}

}// namespace sqr
