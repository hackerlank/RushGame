#pragma once
/**
	@class CVertexShadowJudge

	������Ӱ����.
	�ж�һ�������Ƿ�����Ӱ֮��, ShadowMap��CPUʵ��

	(C) 2009 ThreeOGCMan
*/
#include "CRefObject.h"
#include "GraphicBase.h"
#include "MathAlgorithm.h"

namespace sqr
{
	class CRenderTexture;
	SmartPointer(CVertexShadowJudge);

	class CVertexShadowJudge : public CRefObject
	{
	public:
		CVertexShadowJudge();
		virtual ~CVertexShadowJudge();

		/// ��ʼ�Ƚ�
		void Begin(CRenderTexture* target, const CMatrix& shadowMatrix);
		/// �ж�һ�����Ƿ�����Ӱ֮��
		bool IsInShadow(const CVector3f& point);
		/// �����Ƚ�
		void End();

	private:
		/// the same to HLSL
		float tex2Dproj(const CVector4f& texCoord);
	private:
		ITexture*	m_pDepthTarget;
		CMatrix		m_ShadowMatrix;
		bool		m_isLocking;
		float*		m_pDepthData;
	};
}
