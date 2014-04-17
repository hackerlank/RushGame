#ifndef _CVertex_Hardware_Buffer_H_
#define _CVertex_Hardware_Buffer_H_

#include "CVertexHardwareBuffer.h"
#include "Dx9Base.h"

namespace sqr
{
/*
����һ��ʹ��D3D�������ִ�����ͣ�ͨ�������Ӧ�þ����ܵ�
ʹ���Դ��<��>���ڴ�(��useSystemMem = false)�����ʹ���Դ�
��Ӧ��ʹ��ShadowBuffer���Ա�֤���ݵİ�ȫ�������ʹ���ڴ���Ϊ
���������Ļ�����Ӧ��ʹ��ShadowBuffer��
*/
class DX9VertexHardwareBuffer : public CVertexHardwareBuffer
{
public:
	DX9VertexHardwareBuffer(size_t vertexSize, size_t numVertices,
	                        Usage usage, void* pInitData = NULL,
							bool useSystemMem = false, bool useShadowBuffer = true);
	~DX9VertexHardwareBuffer();

	LPDIRECT3DVERTEXBUFFER9 getD3D9VertexBuffer(void) const
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
	LPDIRECT3DVERTEXBUFFER9 m_pD3DBuffer;
	D3DPOOL					m_D3DPool;
};
}

#endif //_CVertex_Hardware_Buffer_H_