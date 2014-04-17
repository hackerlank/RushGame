#ifndef _CShadowBuffer_H_
#define _CShadowBuffer_H_

#include "CHardwareBuffer.h"

namespace sqr
{
/*
���������Ҫ�����������ڴ棬Ϊ�Կ��ϵ�Ӳ�������ṩ�ڴ��ϵ�ӳ��ռ�

<��ע> ��Ϊ��������һ�����⣺��������Կ�ʧȥ��ռȨ��ʱ��
�Կ����Զ��ͷ��Կ��ϵ���Դ�������ڶ�ռȨʧ�����õ�ʱ������Ҫ���ľ���
��CShadowBuffer�е����ݿ������Կ���ʹ����Լ�������������
<��ע> ��ʵ��Ӧ���У������Ҫ�����Ի����е����ݽ����޸ģ����޸��Դ���
���ݵ�Ч��Ҫ�����޸��ڴ��е����ݣ�����������£�CShadowBuffer������Ϊ
һ����������ʹ�á��ӿ������޸ĺͿ������ٶȡ�
*/
class ShadowBuffer : public CHardwareBuffer
{
public:
	ShadowBuffer(size_t fSize, Usage usage = HBU_DYNAMIC);
	~ShadowBuffer();
public:
	void read(size_t offset, size_t length, void* pDest);
	void write(size_t offset, size_t length, const void* pSource,
	           bool discardBuffer = false);
protected:
	void* _lock(size_t offset, size_t length, LockOptions options);
	void _unlock(void);
	TBYTE* m_pData;
};
}


#endif //_CShadowBuffer_H_