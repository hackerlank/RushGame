#pragma once 
#include "TBaseTableFileImp.h"
#include "TMemPoolAllocator.h"
#include "CDataFileAllocator.h"


namespace sqr 
{
	class CRowTableFileKey;
	class CRowTableFileField;
	class CRowTableFileNode;
	class CLeafNodeDataMeta;
	class CMemPool;
	class CConstStringPool;

	class CRowTableLoadingData;

	class CRowTableFileImp : public TBaseTableFileImp<CRowTableFileImp>
	{
	public:	
		typedef TBaseTableFileImp<CRowTableFileImp> Parent_t;
		friend class TBaseTableFileImp<CRowTableFileImp>;
		friend class CRowTableFileNode;
		friend class CRowTableFile;
		friend class CRowTableFileKey;
		friend class CRowTableFileTreeKey;
		friend class CRowTableFileLeafKey;
		friend class CRowTableFileField;
		

		CRowTableFileImp();
		~CRowTableFileImp();

		bool LoadXMLFileDerived(const wchar_t* szAliasPath, const char* szFileName);
		void SetFileRowOffsetDerived(const char* szFileName);
		bool LoadDerived();

		typedef vector<const char*, CDataFileAllocator<const char*> > VecFileName_t;

		bool ReloadDerived(VecFileName_t& vecFileName);

		const char* GetIndexByName(const char* szName, CConstStringPool* pStringPool, bool bForceInsert = false );

		const char* GetFileName(uint32 uRow);

	private:
		CRowTableFileNode* ParseFirstCol(int32 nRow, vector<const char*>& vecLastRowData, const char* szFileName);
				
		void EndLoad();

		CMemPool* GetLoadingPool() { return m_pLoadingPool; }

		CConstStringPool* GetXmlIndexPool() { return m_pXmlIndexPool; }
		CConstStringPool* GetTxtIndexPool() { return m_pTxtIndexPool; }

		double* AllocDataNumber(const char* szData);
		char* AllocDataString(const char* szData);
		
	private:
		CRowTableFileNode*			m_pRowTableRootNode;
		CRowTableFileKey*			m_pRowTableFileKey;

		/*
			����ʹ������string pool����������row table file������
			��һ�������ڽ���xml�ļ�����ʹ�õ�index name
			���ڶ�������ʣ�µĽ���txt�ļ������õ�index name

			һ����˵�����ڶ༶��������˵����һ����������txt�ļ������
			���µľ�����xml�ļ�����ģ�Ҳ����root node��Ӧ��Ϊtxt��ʣ�µ����нڵ�
			��Ӧ��Ϊxml
		*/
		CConstStringPool*			m_pXmlIndexPool;
		CConstStringPool*			m_pTxtIndexPool;

		//���loading��ʱ�������������ݵģ�load���֮��ɾ��
		CMemPool*					m_pLoadingPool;

		/*
			ÿ���ļ���ʼ�к�
		*/
		typedef pair<const char*, uint32>	PairFileRowOffset;
		typedef vector<PairFileRowOffset, CDataFileAllocator<PairFileRowOffset> >	VecFileRowOffset_t;
		VecFileRowOffset_t	m_vecFileRowOffset;

		static bool LessFileRow(const PairFileRowOffset& left, const PairFileRowOffset& right);

		CMemPool*					m_pDataBuffer;	//Ҷ������buffer
	
		typedef vector<CLeafNodeDataMeta*, CDataFileAllocator<CLeafNodeDataMeta*> >	VecLeafNodeMeta_t;
		VecLeafNodeMeta_t			m_vecLeafNodeMeta;
		string FileName1;
	};
}
