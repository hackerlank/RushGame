#pragma once
#include "CHardwareBufferMgr.h"
#include "CRefObject.h"

namespace sqr
{
	// ---- VB�й�, ����Ұָ��
	
	class CVertexHardwareBuffer;
	
	class VBRef : public CRefObject
	{
	public:
		VBRef()
			: vertexBuffer(NULL) {};
		virtual ~VBRef()
		{
			if (NULL != this->vertexBuffer)
				CHardwareBufferMgr::GetInst()->DestroyBuffer(this->vertexBuffer);
		}

		void SetVB(CVertexHardwareBuffer* vb)
		{
			Ast(NULL == this->vertexBuffer);
			this->vertexBuffer = vb;
		}
		CVertexHardwareBuffer* GetVB() const
		{
			return this->vertexBuffer;
		}

	private:
		CVertexHardwareBuffer* vertexBuffer;
	};

	SmartPointer(VBRef);
}