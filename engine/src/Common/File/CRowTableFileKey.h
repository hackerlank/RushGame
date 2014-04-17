#pragma once 
#include "TableFileHelper.h"
#include "CDataFileAllocator.h"
#include "TMemPoolAllocator.h"

class TiXmlNode;

namespace sqr 
{
	/*
		���漸��������������row table file��Ӧ��xml�ļ�ʹ�õ�
		ÿһ��row table file����һ��xml�ļ������������row table file��
		�ؼ����б��������Ƚ�������ؼ����б�Ȼ���ڽ���txt�ļ���ʱ��
		ͨ������ؼ����б������һ���Ĵ���
	*/

	class CRowTableFileImp;
	class CLeafNodeDataMeta;
	class CMemPool;

	class CRowTableFileField
	{
	public:
		friend class CRowTableFileImp;

		CRowTableFileField(TiXmlNode* pNode, CRowTableFileImp* pImp, CMemPool* pPool);
		ETableFileFieldType GetFieldType(int32 nColIndex) { return m_vecFieldType[nColIndex]; }
		const char* GetFieldKey(int32 nColIndex) { return m_vecFieldKey[nColIndex]; }

		int32 FieldNum() { return (int32)m_vecFieldType.size(); }
		int32 DataNum() { return m_nDataNum; }

		bool Equal(const CRowTableFileField& Other);

		ERowTableNodeType LeafNodeType() { return m_LeafNodeType; }

	private:
		typedef TMemPoolAllocator<const char*>				MemIndexAlloc_t;
		typedef TMemPoolAllocator<ETableFileFieldType>		MemTypeAlloc_t;

		typedef vector<const char*, MemIndexAlloc_t>			VecKey_t;
		typedef vector<ETableFileFieldType, MemTypeAlloc_t>		VecType_t;

		VecKey_t				m_vecFieldKey;
		VecType_t				m_vecFieldType;
		CRowTableFileImp*		m_pImp;

		ERowTableNodeType		m_LeafNodeType;
		int32					m_nDataNum;
	};

	enum ETableFileKeyType
	{
		eTFKT_Tree = 0,	//������һ��key��tree���͵ģ��������滹�����ӽڵ�
		eTFKT_Leaf,			//���key��leaf���͵ģ����������������field�Ķ�����
	};

	class CRowTableFileKey
	{
	public:
		friend class CRowTableFileImp;;

		virtual ~CRowTableFileKey(){}
		virtual ETableFileKeyType GetType() = 0;

		virtual bool Equal(const CRowTableFileKey* pOther) = 0;

		static void* operator new(size_t stSize, CMemPool* pPool);
		static void operator delete(void* pMem, CMemPool* pPool){ (void)pMem; (void)pPool; }
		static void operator delete(void* pMem) { (void)pMem; }
	};

	enum ETableFileIndexKeyType
	{
		eTFIKT_Name = 0,   //��������
		eTFIKT_Default,    //Ĭ������
		eTFIKT_Random,	   //��������
	};

	class CRowTableFileTreeKey : public CRowTableFileKey
	{
	public:
		friend class CRowTableFileKey;
		friend class CRowTableFileImp;

		CRowTableFileTreeKey (TiXmlNode* pRootNode, CRowTableFileImp* pImp, CMemPool* pPool); 
		~CRowTableFileTreeKey();

		ETableFileKeyType GetType() { return eTFKT_Tree; }					

		CRowTableFileKey* GetSubKey(const char* szKeyName);

		//�Ƿ������ֹؼ��ֻ���Ĭ�Ϲؼ���
		ETableFileIndexKeyType GetIndexKeyType() { return m_eIndexKeyType; }

		bool Equal(const CRowTableFileKey* pOther);

	private:
		CRowTableFileKey* CreateSubKey(TiXmlNode* pNode, CMemPool* pPool);

	private:
		typedef pair<const char*, CRowTableFileKey*>	PairIndex2Key_t;
		typedef TMemPoolAllocator<PairIndex2Key_t>	MemIndexKeyAlloc_t;

		typedef vector<PairIndex2Key_t, MemIndexKeyAlloc_t> VecIndex2Key_t;

		VecIndex2Key_t*					m_pVecIndex2Key;
		ETableFileIndexKeyType			m_eIndexKeyType;
		CRowTableFileImp*				m_pImp;
	};

	class CRowTableFileLeafKey : public CRowTableFileKey
	{
	public:
		friend class CRowTableFileKey;
		friend class CRowTableFileImp;

		CRowTableFileLeafKey(TiXmlNode* pRootNode, CRowTableFileImp* pImp, CMemPool* pPool);
		~CRowTableFileLeafKey();

		ETableFileKeyType GetType() { return eTFKT_Leaf; }

		CRowTableFileField& GetField() { return m_Field; }		

		bool Equal(const CRowTableFileKey* pOther);

	private:
		CRowTableFileField		m_Field;
	};

}

