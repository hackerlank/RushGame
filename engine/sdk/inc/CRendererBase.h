#pragma once

namespace sqr
{
/// ��Ⱦ��
class CRendererBase
{
public:
	CRendererBase();
	virtual ~CRendererBase();
	virtual void Render() = 0;
};

}// namespace sqr
