#pragma once
#include "CGraphicMallocObject.h"
#include "GraphicClassDefine.h"
#include "TSafeBuffer.h"

namespace sqr
{
	/// ��������ѡ��
	enum LockOptions
	{
		/** �������������������д�������������ܵ�����*/
		HBL_NORMAL,
		/** ��������ʱ���������ԭ���Ļ���ռ�;ֻ���� HBU_DYNAMIC
		���Դ����Ļ��������ܹ�ʹ��.
		*/
		HBL_DISCARD,
		/** ��ֻ��ģʽ����һ����������ʹ������HBU_WRITE_ONLY���Դ����Ļ������С�
		���侲̬�й�, ����HBU_DYNAMIC�����ڴ˲������á�.
		*/
		HBL_READ_ONLY,
		/** �����Ϻ�HBL_NORMALѡ����ͬ, �������Ӧ�ÿ��Ա�֤�������Ѿ�д������
		������д������,������API����ֱ�ӽ����Ż�. */
		HBL_NO_OVERWRITE

	};

	class CHardwareBuffer : public CGraphicMallocObject
	{
	protected:
		size_t	m_Size;
		Usage	m_Usage;
		bool	m_IsLocked;
		size_t	m_sLockStart;
		size_t	m_sLockSize;
		bool	m_SystemMemory;
		bool	m_UseShadowBuffer;
		bool	m_ShadowUpdated;
		LockOptions			m_lockOp;
		CHardwareBuffer*	m_pShadowBuffer;

	public:
		CHardwareBuffer(Usage usage, bool systemMemory, bool useShadowBuffer = true);
		virtual ~CHardwareBuffer();

		//����ĳһ����׼����д
		void* lock(size_t offset, size_t length, LockOptions options);
		//���������ȫ������
		void* lock( LockOptions options );

		//����ȫ����������
		void unlock(void);


		virtual void read(size_t offset, size_t length, void* pDest) = 0;
		virtual void write(size_t offset, size_t length, const void* pSource,
			bool discardBuffer = false) = 0;
		virtual	void copy(CHardwareBuffer& srcBuffer, size_t srcOffset,
			size_t dstOffset, size_t length, bool discardBuffer = false);

		size_t			getSize(void);
		Usage			getUsage(void);

		bool			isSystemMemory(void);
		bool			isLocked(void);
		bool			hasShadowBuffer(void);
		virtual bool	IsValid(void);
		virtual void	updateRes(void);
		virtual void	freeRes(void);
	protected:
		virtual void*	_lock(size_t offset, size_t length, LockOptions options) = 0;
		virtual void	_unlock(void) = 0;
		virtual void	_updateFromShadow(void);
	};

	template <class T>
	class THardwareSafeBuffer :public TSafeBuffer<T>
	{
	public:
		THardwareSafeBuffer(CHardwareBuffer * pHBuffer, LockOptions options)
			: TSafeBuffer(pHBuffer->lock(options),pHBuffer->getSize())
			,m_pHBuffer(pHBuffer)
		{
		}

		~THardwareSafeBuffer()
		{
			m_pHBuffer->unlock();
		}
	protected:
		CHardwareBuffer* m_pHBuffer;
	};
}
