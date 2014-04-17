#pragma once
#include "CDynamicObject.h"
#include "TMtDbAllocator.h"

namespace sqr
{
	class CValueTable;

	class CDbContext
		:public virtual CDynamicObject
		,public CMtDbMallocObject
	{
		typedef basic_string<char,std::char_traits<char>,TMtDbAllocator<char> >	ReportString_t;
	public:		
		//������������Ӧ����protected���壬������lua�������Ʋ�֧��protected,���Բ��ò�д��public
		CDbContext();
		~CDbContext();
		void Clear();

		void SetValueTable(CValueTable* pValueTable);
		CValueTable* GetValueTable();

		void SetReport(const ReportString_t& strReport);
		const char* GetReport();

	private:
		CValueTable*		m_pValueTable;
		ReportString_t	m_strReport;
	};

}
