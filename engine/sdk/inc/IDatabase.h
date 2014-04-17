#pragma once

#include "BaseTypes.h"
#include "Database.h"
#include "CDynamicObject.h"
#include "CDbMallocObject.h"

namespace sqr
{
	/*
	mysql ��ȡ�����ʱ���ѡ���Ƿ񻺳�������������Ҹ������з����ó��Լ���ѡ����
	1����ѯִ��֮������������϶�������mysql�ڲ������Ǹ���mysql�ĵ�:
	(you must retrieve all the rows even if you determine in mid-retrieval
	that you've found the information you were looking for.)
	2��������ڵ���StoreResult����UseResult+ȫ��FetchRow��ǰ��Ҫô����mysql�ڲ���Ҫô�����ⲿ��
	�ڴ�û��ʲô��ʡ,
	3��ʹ��UseResult���������ҪFetchȫ��Row֮�����֪��RowCount,��ܲ�����

	���������Execute����Stmt_Execute,��ѡ��StoreResult�ķ���


	���⣬�ṩ�����ͻ�ȡ���Ӧ��ʹ��Variant��ʽ����ʹ���ṩ���ֲ�ͬ���͵ĺ���Ҳ�ķ�ʽҲ������һ����ϸ�Ŀ��ǡ�
	��������ַ�ʽʵ�ַ�������࣬Variant�����Ƚ����ã�����
	���ܽ�����
	����DateTime��Date�Լ�Time���������Ͷ�ֻ����operator =(time_t&)��ʵ�֣��޷�����
	������ʹ���߲������ʣ�
	����string��binary��ֻ����operator=(const char*)����operator=(const string&);
	������ѡ���ö����͵ĺ�����ʵ��


	Ŀǰ�Ľ����������������id����̬���͵İ��һ���̶�����������ԣ�����Ϊ����õķ�����


	���ǵ���mysql api�ĵط������˻�������������


	2006.1.23�գ����շ��ֶ�mysql statement�ķ�װ������Ͽ��Խ����ĵط���bind��Ӧ����ÿ��execute��ʱ����У�Ӧ��bindһ�Σ�ִ�ж�Σ�������ȱ�����Ҽ��䲻ˬ����ʱ��һ��Ҫ��
	2006.2.23�գ�Statement�ӿڵ��޸�����ɣ����ﲹ��˵��һ��
	*/

	enum Isolation_Level 
	{
		READ_UNCOMMITTED = 1,
		READ_COMMITTED,
		REPEATABLE_READ,
		SERIALIZABLE,
	};

	enum EDbFieldType
	{
		eDFT_Int8 = 0,
		eDFT_Uint8,
		eDFT_Int16,
		eDFT_Uint16,
		eDFT_Int32,
		eDFT_Uint32,
		eDFT_Int64,
		eDFT_Uint64,
		eDFT_Float,
		eDFT_Double,
		eDFT_String,        //�ַ���char/var char
		eDFT_Blob,          //text/blob
	};

	class IDbTextResult
		: public CDbMallocObject
	{
	public:
		virtual size_t GetRowNum() = 0;
		virtual size_t GetColNum() = 0;
		virtual const char * GetColName(size_t uColIndex) const = 0;

		virtual void Locate(size_t uRowIndex) = 0;
		//��ȡ��ǰ��ĳ�е����ݣ��������ַ�������ʽ����
		virtual const char* GetData(size_t uColIndex) = 0;
		virtual unsigned GetDataLength(size_t uColIndex) = 0;

		virtual size_t GetFieldLength(size_t uColIndex) = 0;
		virtual EDbFieldType GetFieldType(size_t uColIndex) = 0;

		virtual void Release()=0;
		virtual void SetTag(void* pTag)=0;
		virtual void* GetTag()const=0;
		virtual ~IDbTextResult(){}
	};

	class IDbConnection;

	class IDbStatement;

	class IDbParamBinder
		: public CDbMallocObject
	{
	public:
		//Text��Binary����������Binary����collate��Ӱ��
		virtual void SetParamNull(const bool* pIsNull,size_t uIndex)=0;

		virtual void SetParamInt8(const int8* pBuffer,size_t uIndex)=0;
		virtual void SetParamUint8(const uint8* pBuffer,size_t uIndex)=0;

		virtual void SetParamInt16(const int16* pBuffer,size_t uIndex)=0;
		virtual void SetParamUint16(const uint16* pBuffer,size_t uIndex)=0;

		virtual void SetParamInt32(const int32* pBuffer,size_t uIndex)=0;
		virtual void SetParamUint32(const uint32* pBuffer,size_t uIndex)=0;

		virtual void SetParamInt64(const int64* pBuffer,size_t uIndex)=0;
		virtual void SetParamUint64(const uint64* pBuffer,size_t uIndex)=0;

		virtual void SetParamFloat(const float* pBuffer,size_t uIndex)=0;
		virtual void SetParamDouble(const double* pBuffer,size_t uIndex)=0;

		virtual void SetParamText(const void* pBuffer,size_t uMaxSize,size_t* pActualSize,size_t uIndex)=0;
		virtual void SetParamBinary(const void* pBuffer,size_t uMaxSize,size_t* pActualSize,size_t uIndex)=0;

		virtual IDbStatement* GetStatement()const=0;

		virtual void Bind()=0;

		virtual void Release()=0;

		virtual ~IDbParamBinder(){}
	};


	class IDbResultBinder
		: public CDbMallocObject
	{
	public:
		virtual void SetResultInt8(int8* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;
		virtual void SetResultUint8(uint8* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;

		virtual void SetResultInt16(int16* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;
		virtual void SetResultUint16(uint16* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;

		virtual void SetResultInt32(int32* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;
		virtual void SetResultUint32(uint32* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;

		virtual void SetResultInt64(int64* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;
		virtual void SetResultUint64(uint64* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;

		virtual void SetResultFloat(float* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;
		virtual void SetResultDouble(double* pBuffer,bool* pIsNull,bool* pError,size_t uColIndex)=0;

		virtual void SetResultText(void* pBuffer,size_t uBufferSize,size_t* uDataSize,bool* pIsNull,bool* pError,size_t uColIndex)=0;
		virtual void SetResultBinary(void* pBuffer,size_t uBufferSize,size_t* uDataSize,bool* pIsNull,bool* pError,size_t uColIndex)=0;

		virtual IDbStatement* GetStatement()const=0;

		virtual void Bind()=0;

		virtual void Release()=0;
		virtual void Reset() = 0;

		virtual ~IDbResultBinder(){}
	};


	class IDbStatement
		:public virtual CDynamicObject
		,public CDbMallocObject
	{
	public:
		virtual IDbParamBinder* CreateParamBinder()const=0;
		virtual IDbResultBinder* CreateResultBinder()const=0;

		virtual void FetchResultRow(size_t uRowIndex)=0;

		virtual size_t GetParamNum()const=0;
		virtual size_t GetResultColNum()const=0;
		virtual size_t GetResultRowNum()const=0;
		virtual const char* GetResultColName(size_t uColIndex) const = 0;

		virtual IDbConnection* GetConnection()const=0;

		virtual size_t GetFieldLength(size_t uColIndex) = 0;
		virtual EDbFieldType GetFieldType(size_t uColIndex) = 0;

		virtual void Execute()=0;
		virtual void ClearResult()=0;
		virtual void Release()=0;
		
		virtual void SetTag(void* pTag)=0;
		virtual void* GetTag()const=0;

		virtual string GetSql() const = 0;

		virtual ~IDbStatement(){}
	};

	class CError;
	class IDbConnection
		:public virtual CDynamicObject
		,public CDbMallocObject
	{
	public:
		virtual IDbTextResult* Execute(const char* szSqlBuffer,size_t uSize)=0;
		virtual IDbTextResult* Execute(const char* szSqlBuffer)=0;

		virtual IDbStatement* CreateStatement(const char* szSqlBuffer,size_t uSqlLength)=0;
		virtual IDbStatement* CreateStatement(const char* szSqlBuffer)=0;

		virtual size_t EscapeString(char* szOut,const char* szIn,size_t uInSize)=0;
		virtual uint64 LastInsertId()const=0;
		virtual uint64 LastAffectedRowNum()const=0;
		virtual bool Ping()=0;

		virtual void SetTag(void* pTag)=0;
		virtual void* GetTag()const=0;

		virtual void Release()=0;

		virtual void SetAutoCommit(bool bEnabled) = 0;
		virtual void SetIsolationLevel(Isolation_Level eIsolationLevel) = 0;
		virtual void SetCharacterSet(const char* szCharSet) = 0;
		virtual void CommitTrans() = 0;
		virtual void RollbackTrans() = 0;
		virtual void SetSavepoint(const char* szName) = 0;
		virtual void RollbackTransTo(const char* szName) = 0;
		virtual ~IDbConnection(){}
	};


	class IDatabase : public CDbMallocObject
	{
	public:
		virtual IDbConnection* CreateConnection(const char* szHost,uint16 uPort,
			const char* szUser,const char* szPassword,
			const char* szDatabase)=0;

		virtual IDbConnection* CreateConnection(const char* szPipe,//under unix is this unixsocket
			const char* szUser,const char* szPassword,
			const char* szDatabase)=0;

		virtual void BeginDbThread()=0;
		virtual void EndDbThread()=0;

		virtual void Release()=0;
		virtual ~IDatabase(){}
	};

	DATABASE_API IDatabase* GetDatabase();
}
