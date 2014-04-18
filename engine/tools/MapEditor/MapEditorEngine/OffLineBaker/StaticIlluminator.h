#pragma once
//------------------------------------------------------------------------------
/**
	@class CStaticIlluminator

	��̬����ģ��.

	����CPU���ж�����մ���
	@note ���й��ռ���һ��Ҫ��CColor4, byte���Ȳ���...

	(C) 2009 ThreeOGCMan
*/
#include "TSingleton.h"
#include "CLight.h"

//------------------------------------------------------------------------------
namespace sqr
{
	class CStaticIlluminator : public TSingleton<CStaticIlluminator>
	{
	public:
		CStaticIlluminator();

		/// ���û�����
		void SetAmbient(const CColor4& color);
		/// ����ȫ�ֹ�(�����)
		void SetGlobalLight(const CLight& global);
		/// ���һ�����Դ
		void AddPointLight(const CLight& point);
		/// ���һ���۹��
		void AddSpotLight(const CLight& spot);

		/// �������й�Դ
		void ClearAllLights();

		/// ���й��ռ���
		CColor4 Illuminate(const sMaterial& materail, const CVector3f& position, const CVector3f& normal);

	private:
		CColor4 DirectLighting(const sMaterial& material, const CVector3f& position, const CVector3f& normal);
		CColor4 PointLighting(const sMaterial& material, const CVector3f& position, const CVector3f& normal);
		CColor4 SpotLighting(const sMaterial& material, const CVector3f& position, const CVector3f& normal);

	private:
		CColor4 ambient;
		CLight globalLight;
		vector<CLight> pointLights;
		vector<CLight> spotLights;
	};

	//------------------------------------------------------------------------------
	inline void
	CStaticIlluminator::SetGlobalLight( const CLight& global )
	{
		Ast(LIGHT_DIRECTIONAL == global.GetType());
		this->globalLight = global;
	}

	//------------------------------------------------------------------------------
	inline void
	CStaticIlluminator::AddPointLight( const CLight& point )
	{
		Ast(LIGHT_POINT == point.GetType());
		this->pointLights.push_back(point);
	}

	//------------------------------------------------------------------------------
	inline void
	CStaticIlluminator::AddSpotLight( const CLight& spot )
	{
		Ast(LIGHT_SPOT == spot.GetType());
		this->spotLights.push_back(spot);
	}

	//------------------------------------------------------------------------------
	inline void
	CStaticIlluminator::SetAmbient( const CColor4& color )
	{
		this->ambient = color;
	}
}