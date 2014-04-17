#pragma once 
#include "CDataFileAllocator.h"
#include "TableFileHelper.h"

namespace sqr 
{
	/*
	����ÿһ��leaf node��˵������ʵ����һ��ԭ�͵ģ���Ϊ����ͨ��xml����һ��leaf node֮��
	�Ժ����и���leaf node��������������leaf node������ʵ�����ݲ�һ��֮�⣬�������в���
	����һ����
	*/

	class CLeafFieldInfo
	{
	public:
		CLeafFieldInfo()
			: nDataOffset(-1)
			, eFieldType(eTFFT_Empty) {}
		CLeafFieldInfo(int32 nOffset, ETableFileFieldType eType) 
			: nDataOffset(nOffset)
			, eFieldType(eType){}

		int32				nDataOffset;
		ETableFileFieldType eFieldType;
	};

	class CRowTableFileField;
	class CLeafNodeDataMeta : public CDataFileMallocObject
	{
		friend class CRowTableFileNode;

	public:
		CLeafNodeDataMeta(CRowTableFileField& RowField);
		~CLeafNodeDataMeta();

		int32 DataNum() { return (int32)m_vecDataFieldInfo.size(); }
		CLeafFieldInfo& GetFieldInfo(const char* szKey);
		
		typedef pair<const char*, CLeafFieldInfo>	PairKey2Field_t;
		typedef vector<PairKey2Field_t,
			CDataFileAllocator<PairKey2Field_t> >	VecKey2Field_t;
	
	private:
		static bool LessField(const PairKey2Field_t& Left, const PairKey2Field_t& Right);

		VecKey2Field_t	m_vecDataFieldInfo;
	};
}

