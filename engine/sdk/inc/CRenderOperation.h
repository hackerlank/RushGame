#pragma once
#include "GraphicBase.h"
namespace sqr
{
class CIndexHardwareBuffer;
class CVertexBufferBinding;
class CVertexDeclaration;

class VertexData : public CGraphicMallocObject
{
public:
	VertexData();
	~VertexData(void);

	CVertexDeclaration* vertexDeclaration;
	CVertexBufferBinding* vertexBufferBinding;

	size_t vertexStart;
	size_t vertexCount;

	VertexData* Clone(bool copyData = true) const;
	void Destroy(void);
};

class IndexData : public CGraphicMallocObject
{
public:
	IndexData();
	~IndexData();

	CIndexHardwareBuffer *indexBuffer;

	size_t indexStart;
	size_t indexCount;

	IndexData* Clone(bool copyData = true) const;
	void Destroy(void);
	//void optimiseVertexTriList(void);
};

// ������ѵ����Ǵ�˵�е�Mesh? 
// ��𣺲���˵�����Mesh����Ⱦ�������ʾ��һ����Ⱦ����С��λ��Mesh�����ģ�͵ĸ��߼���װ���������岻ͬ
// �򵥵�˵��3D�����е�������Mesh����Load/Save ����Ϊ��ʽ������������Դ����
// ��������/GUI����û�з�װ��Mesh��������ЩҲҪ��Ⱦ��ÿ�����֣�GUI��Ⱦ��Ҫ����Ⱦ�����

class CRenderOperation : public CGraphicMallocObject
{
public:
	CRenderOperation();
	~CRenderOperation();

	VertexData* vertexData;
	IndexData* indexData;

	EPrimitiveType operationType;
	bool useIndexes;

	bool Create(void);
	void Destroy(void);

	CRenderOperation& operator = (const CRenderOperation& op);
	void  Clone(CRenderOperation& op) const;
};

}