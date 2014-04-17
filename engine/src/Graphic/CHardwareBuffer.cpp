#include "stdafx.h"
#include "CHardwareBuffer.h"
#include "CGraphicExp.h"
#include "ExpHelper.h"

CHardwareBuffer::CHardwareBuffer(Usage usage, bool systemMemory, bool useShadowBuffer)
:m_Usage(usage), m_IsLocked(false), m_SystemMemory(systemMemory),
m_UseShadowBuffer(useShadowBuffer), m_pShadowBuffer(NULL), m_ShadowUpdated(false)
{
	m_Size = 0;
	m_sLockStart = 0;
	m_sLockSize = 0;
}

CHardwareBuffer::~CHardwareBuffer()
{}

void* CHardwareBuffer::lock(size_t offset, size_t length, LockOptions options)
{
	Ast(!isLocked() && "Cannot lock this buffer, it is already locked!");
	Ast((offset + length)<=m_Size && "lock too long");


	if( options != HBL_READ_ONLY && (m_Usage&HBU_STATIC)!=0 )
	{
		Ast("HBU_STATIC Buffer�����ڴ���ʱ�������ݣ��Ժ���ͨ��Lock�����޸�");
		return NULL;
	}

	void* ret = 0;
	if (m_UseShadowBuffer)
	{
		if (options != HBL_READ_ONLY)
		{
			// ���ǲ��ò�����һ����/д����������Ӧ��'Ӱ�仺��'
			// �����Ǻ�unlock()��ͬ����
			m_ShadowUpdated = true;
		}

		ret = m_pShadowBuffer->lock(offset, length, options);
	}
	else
	{
		// ������û��Ӧ��'Ӱ�仺��'��ʱ�����Ǿ�������ʵ��������
		ret = _lock(offset, length, options);
		m_IsLocked = true;
	}
	m_lockOp = options;
	m_sLockStart = offset;
	m_sLockSize = length;
	return ret;
}

//���������ȫ����
void* CHardwareBuffer::lock( LockOptions options )
{
	return lock(0,m_Size,options);
}

//������������
void CHardwareBuffer::unlock(void)
{
	Ast(isLocked() && "Cannot unlock this buffer, it is not locked!");

	// �������ǰ������'Ӱ�仺��'
	if (m_UseShadowBuffer&&m_pShadowBuffer->isLocked())
	{
		m_pShadowBuffer->unlock();
		// ��Ӱ�仺���е����ݸ��µ�ʵ�ʵĻ�����
		if (m_ShadowUpdated)
		{
			_updateFromShadow();
			m_ShadowUpdated = false;
		}
	}
	else
	{
		// �������ǽ⿪Ӳ��������
		m_IsLocked = false;
		_unlock();
	}
	m_sLockStart = 0;
	m_sLockSize = m_Size;
}

void CHardwareBuffer::copy(CHardwareBuffer& srcBuffer, size_t srcOffset,
						   size_t dstOffset, size_t length, bool discardBuffer)
{
	const void *srcData = srcBuffer.lock(srcOffset, length, HBL_READ_ONLY);
	this->write(dstOffset, length, srcData, discardBuffer);
	srcBuffer.unlock();
}


size_t CHardwareBuffer::getSize(void)
{
	return m_Size;
}

///���ش���ʱ�Ĵ������Բ���Usage
Usage CHardwareBuffer::getUsage(void)
{
	return m_Usage;
}

/// ���������������ʾ���Ƿ���ϵͳ�ڴ�
bool CHardwareBuffer::isSystemMemory(void)
{
	return m_SystemMemory;
}

/// �������������Ŀǰ�Ƿ�����
bool CHardwareBuffer::isLocked(void)
{
	return m_IsLocked || (m_UseShadowBuffer&&m_pShadowBuffer->isLocked());
}

bool CHardwareBuffer::hasShadowBuffer(void)
{
	return m_UseShadowBuffer;
}

void CHardwareBuffer::updateRes()
{
	m_ShadowUpdated = true;
	_updateFromShadow();
}

void CHardwareBuffer::freeRes()
{
	return;
}

//------------------------------------------------------------------------
void CHardwareBuffer::_updateFromShadow(void)
{
	if (m_UseShadowBuffer && m_ShadowUpdated)
	{
		// �ֶ�������������ʱ������������
		const void *srcData = m_pShadowBuffer->_lock(
			m_sLockStart, m_sLockSize, HBL_READ_ONLY);
		// ���������Χ��ȫ���Ļ�������ʹ��HBL_DISCARD���ԣ�����Ӧ��HBL_NOMAL���� -> ����D3D��Ȼ���Warning
		Ast( (m_sLockStart + m_sLockSize)<=m_Size );
		void *destData = this->_lock(
			m_sLockStart, m_sLockSize, m_lockOp );//(0==m_sLockStart && m_sLockSize==m_Size)?HBL_DISCARD:HBL_NORMAL);
		// �����ݿ�������ʵ��������
		if( NULL != destData && NULL!= srcData )
			memcpy(destData, srcData, m_sLockSize);

		this->_unlock();
		m_pShadowBuffer->_unlock();
		m_ShadowUpdated = false;
	}
}

bool	CHardwareBuffer::IsValid(void)
{
	return true;
}
