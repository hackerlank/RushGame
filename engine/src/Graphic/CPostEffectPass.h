#pragma once
//------------------------------------------------------------------------------
/**
	@class CPostEffectPass

	����Ч���Ļ�����ɵ�λ.
	ÿ��passֻ��һ��Ŀ�����, �������ж������.
	���sourceΪ��, ��ȡ��һ��pass��target
	���targetΪ��, �����������һ��pass
 */
#include "CRefObject.h"
#include "CGraphicMallocObject.h"
#include "CShaderConstParams.h"

namespace sqr
{
SmartPointer(CRenderTexture);
SmartPointer(CPostEffectPass);
class CGpuProgram;


class CPostEffectPass
	: public CRefObject
	, public CGraphicMallocObject
{
public:
	CPostEffectPass();
	virtual ~CPostEffectPass();

	/// ������һ��Pass
	void SetPrePass(CPostEffectPass* pass);
	/// ������һ��Pass
	void SetNextPass(CPostEffectPass* pass);
	/// ����Ŀ��
	void SetTarget(const CRenderTexturePtr& texture);
	/// ����Ŀ��
	const CRenderTexturePtr& GetTarget() const;
	/// ������������. ����˳�����
	void AddSource(const CRenderTexturePtr& texture);
	/// ���������������Ŀ
	const size_t GetNumSources() const;
	/// ����ָ����������
	const CRenderTexturePtr& GetSource(index_t sourceIndex) const;
	/// Ŀ���Ƿ���Ҫ���
	bool IsTargetNeedClear() const;
	/// ����Ŀ���Ƿ���Ҫ���
	void SetTargetNeedClear(bool clear);
	/// ����Ч��
	void SetEffect(const SHADER_HANDLE& shader);
	/// �Ƿ����һ��Pass
	bool IsLast() const;

	/// ���д���
	virtual void Apply();
	void UpdateViewSize();

private:
	CPostEffectPass* prePass;
	CPostEffectPass* nextPass;
	CRenderTexturePtr target;
	GVector<CRenderTexturePtr> sources;
	bool needClear;
	CGpuProgram* effect;
	SHADER_HANDLE	m_shader_handle;
	float m_invViewSize[2];
};

//------------------------------------------------------------------------------
inline void
CPostEffectPass::SetPrePass( CPostEffectPass* pass )
{
	this->prePass = pass;
}

//------------------------------------------------------------------------------
inline void
CPostEffectPass::SetNextPass( CPostEffectPass* pass )
{
	this->nextPass = pass;
}

//------------------------------------------------------------------------------
inline void
CPostEffectPass::SetTarget( const CRenderTexturePtr& texture )
{
	this->target = texture;
	this->UpdateViewSize();
}

//------------------------------------------------------------------------------
inline const CRenderTexturePtr&
CPostEffectPass::GetTarget() const
{
	return this->target;
}

//------------------------------------------------------------------------------
inline void
CPostEffectPass::AddSource( const CRenderTexturePtr& texture )
{
	this->sources.push_back(texture);
}

//------------------------------------------------------------------------------
inline const size_t
CPostEffectPass::GetNumSources() const
{
	return this->sources.size();
}

//------------------------------------------------------------------------------
inline const CRenderTexturePtr&
CPostEffectPass::GetSource( index_t sourceIndex ) const
{
	return this->sources[sourceIndex];
}

//------------------------------------------------------------------------------
inline bool
CPostEffectPass::IsTargetNeedClear() const
{
	return this->needClear;
}

//------------------------------------------------------------------------------
inline void
CPostEffectPass::SetTargetNeedClear( bool clear )
{
	this->needClear = clear;
}

//------------------------------------------------------------------------------
inline void
CPostEffectPass::SetEffect( const SHADER_HANDLE& shader )
{
	m_shader_handle = shader;
}

//------------------------------------------------------------------------------
inline bool
CPostEffectPass::IsLast() const
{
	return (NULL == this->nextPass || !this->target.isvalid());
}

}// namespace sqr
