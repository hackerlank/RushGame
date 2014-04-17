#ifndef _CIndex_Hardware_Buffer_H_
#define _CIndex_Hardware_Buffer_H_

#include "CIndexHardwareBuffer.h"
#include "Dx9Base.h"

namespace sqr
{
/*
����һ��ʹ��D3D�������Դ�����ͣ�ͨ�������Ӧ�þ����ܵ�
ʹ���Դ��<��>���ڴ�(��useSystemMem = false)�����ʹ���Դ�
��Ӧ��ʹ��ShadowBuffer���Ա�֤���ݵİ�ȫ�������ʹ���ڴ���Ϊ
���������Ļ�����Ӧ��ʹ��ShadowBuffer��
*/
class DX9IndexHardwareBuffer : public CIndexHardwareBuffer
{
public:
	DX9IndexHardwareBuffer(size_t numIndexes, Usage usage,
	                       void* pInitData = NULL, 
						   bool useSystemMem = false, bool useShadowBuffer = true);
	~DX9IndexHardwareBuffer();

	LPDIRECT3DINDEXBUFFER9 getD3DIndexBuffer(void) const
	{
		return m_pD3DBuffer;
	}

	void updateRes();
	void freeRes();
	bool IsValid(void);
protected:
	void*	_lock(size_t offset, size_t length, LockOptions options);
	void	_unlock(void);
	bool	_imp_create(void);

protected:
	LPDIRECT3DINDEXBUFFER9 m_pD3DBuffer;
	D3DPOOL m_D3DPool;
};
}

#endif //_CIndex_Hardware_Buffer_H_