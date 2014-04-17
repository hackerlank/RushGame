#pragma once
#include "CRefObject.h"
#include "CSceneMallocObject.h"
#include "CShaderConstParams.h"
/**
	@class CWaterEffect

	ˮ��Ч������.
	Ϊ�˷���, ʹ��D3D��Ч�����.
	����Ĳ����������н��й���.
 */
namespace sqr
{
class CGpuProgram;
class CGpuProgramParameters;
SmartPointer(CWaterEffect);

class CWaterEffect 
	: public CRefObject
	, public CSceneMallocObject
{
public:
	CWaterEffect();
	virtual ~CWaterEffect();

	/// ����effect id
	SHADER_HANDLE GetEffectID() const;
	/// ����effect id
	void SetEffectID(SHADER_HANDLE id);

	/// ���ص�ǰʱ��(ms)
	uint GetTime() const;
	/// ��������֡��ʱ���
	uint GetTimeElapsed() const;

	/// ����
	virtual void OnFrameUpdate();
	/// ��Ⱦ��ʼ
	virtual void OnRenderBegin();
	/// ��Ⱦ����
	virtual void OnRenderEnd();

protected:
	SHADER_HANDLE effectId;

	uint absoluteTime;
	uint lastFrameTime;
};

//------------------------------------------------------------------------------
inline SHADER_HANDLE
CWaterEffect::GetEffectID() const
{
	return this->effectId;
}

//------------------------------------------------------------------------------
inline uint
CWaterEffect::GetTime() const
{
	return this->absoluteTime;
}

//------------------------------------------------------------------------------
inline uint
CWaterEffect::GetTimeElapsed() const
{
	return (this->absoluteTime - this->lastFrameTime);
}

}// namespace sqr
