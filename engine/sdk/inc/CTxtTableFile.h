#pragma once
/*---------------------------------------------------------------------------
����Ʊ���ָ��ļ��Ķ�ȡ���Ʊ��ļ���һ�������ʽ���ı��ļ�����Excel�༭���Ʊ�
����Ϊ�ı��ļ����ɣ��Ʊ���ļ�ÿ���ɻس����з��ָ�(0x0d 0x0a)��ÿ����Ԫ֮����
�Ʊ����TAB����0x09���ָ�.
//---------------------------------------------------------------------------*/


#include "CommonDefs.h"
#include "CDataFileMallocObject.h"
#include "TableFileHelper.h"
#include "CDynamicObject.h"
#include "TBaseTableFile.h"

namespace sqr
{
	class CTxtTableFileImp;
	class CVariant;
	class CTableFileValue;

	class COMMON_API CTxtTableFile : virtual public CDynamicObject
								   , public TBaseTableFile<CTxtTableFileImp>
	{
	public:
		CTxtTableFile(void);
		~CTxtTableFile(void);
	
		void Release();

		int32		GetHeight();
		int32		GetWidth();

		ETableFileFieldType GetType( int32 nColumn );
		ETableFileFieldType GetType( const char* szColumnName );

		const char* GetString( int32 nRow, int32 nColumn );
		const char* GetString( int32 nRow, const char* szColumnName );
	
		int32		GetInteger( int32 nRow, int32 nColumn, int32 defaultvalue );
		int32		GetInteger( int32 nRow, const char* szColumnName, int32 defaultvalue );

		float		GetFloat( int32 nRow, int32 nColumn, float defaultvalue );
		float		GetFloat( int32 nRow, const char* szColumnName, float defaultvalue );

		template<typename T>
		T			GetNumber( int32 nRow, int32 nColumn, T defaultvalue);

		template<typename T>
		T			GetNumber( int32 nRow, const char* szColumnName, T defaultvalue );

		CVariant*	GetValue(CTableFileValue* pValue, int32 nRow, int32 nColumn);
		CVariant*	GetValue(CTableFileValue* pValue, int32 nRow, const char* szColumnName);
	};
}

;
