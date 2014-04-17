#pragma once
//------------------------------------------------------------------------------
/**
	@class CPostEffect

	����Ч��.
	ÿ��Ч���ɶ��Pass���, һ���ϸ�Pass�������Ϊ�¸�Pass������, �Դ��γ�һ��������ˮ��
 */
#include "CRefObject.h"
#include "CGraphicMallocObject.h"
namespace sqr
{
SmartPointer(CPostEffectPass);
SmartPointer(CPostEffect);

class CPostEffect
	: public CRefObject
	, public CGraphicMallocObject
{
public:
	CPostEffect();
	virtual ~CPostEffect();
	/// ��������
	const char* GetName() const;
	/// ��������
	void SetName(const string& sName);
	/// ���һ��pass
	void AddPass(const CPostEffectPassPtr& pass);
	/// ����pass��Ŀ
	size_t GetNumPasses() const;
	/// ����ָ��pass
	const CPostEffectPassPtr& GetPass(index_t passIndex) const;
	/// �������pass
	void Clear();
	/// �Ƿ�Ϊ��(��Ч״̬)
	bool IsEmpty() const;
	void SetReset(bool iReset);
	bool IsReset(void);
	/// ���д���
	void Apply();
private:
	GString m_PostName;
	bool	m_isReset;
	GVector<CPostEffectPassPtr> passes;
};

inline bool 
CPostEffect::IsReset(void)
{
	return m_isReset;
}

inline void 
CPostEffect::SetReset(bool iReset)
{
	m_isReset = iReset;
}

//------------------------------------------------------------------------------
inline const char*
CPostEffect::GetName() const
{
	return m_PostName.c_str();
}

//------------------------------------------------------------------------------
inline void
CPostEffect::SetName( const string& sName )
{
	m_PostName = sName.c_str();
}

//------------------------------------------------------------------------------
inline size_t
CPostEffect::GetNumPasses() const
{
	return this->passes.size();
}

//------------------------------------------------------------------------------
inline const CPostEffectPassPtr&
CPostEffect::GetPass( index_t passIndex ) const
{
	Ast(passIndex < this->GetNumPasses());

	return this->passes[passIndex];
}

//------------------------------------------------------------------------------
inline bool
CPostEffect::IsEmpty() const
{
	return this->passes.empty();
}

//------------------------------------------------------------------------------
inline void
CPostEffect::Clear()
{
	this->passes.clear();
}

}// namespace sqr