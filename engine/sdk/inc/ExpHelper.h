#pragma once 
#include "ModuleDefs.h"
#include "CJmpStack.h"

#ifdef _WIN32
struct _EXCEPTION_POINTERS;
#endif

namespace sqr
{
	MODULE_API void  db();

	MODULE_API void  EnableCatchError(bool bCatchError);
	MODULE_API bool  CatchErrorEnabled();

	MODULE_API void  *GetCurrentContextStack();
	MODULE_API void  SetCurrentContextStack(void* pStack);

	MODULE_API void	 ShowErrorMsg(const char* szMsg, const char* szType = "Error", bool bShowOnce = false);
	MODULE_API void  ShowErrorMsgAndExit(const char* szMsg);

#ifdef _WIN32
	MODULE_API void  CreateCoreDump( _EXCEPTION_POINTERS* pException);
	MODULE_API void  TransESHFun( unsigned int, struct _EXCEPTION_POINTERS* );

	MODULE_API void  EnableESHDump(bool bEnable);
	MODULE_API bool  ESHDumpEnabled();
#endif

	MODULE_API void DbgGenErr(const std::string& sErrorType);
	MODULE_API void DbgGenErr(const std::string& sErrorType, const std::string& sErrorMsg);

	MODULE_API void GenErr(const std::string& sErrorType);
	MODULE_API void GenErr(const std::string& sErrorType, const std::string& sErrorMsg);

	MODULE_API void GenErrnoErr(const std::string& sErrorTitle);
	MODULE_API void GenErrnoErr(const std::string& sErrorTitle, int32 nErrorNum );

////////////////////////////////////////////////////////////////////

	//macro gen cfg err

//�߻�����
#define CFG_ERR_MSG "���ñ���� "

//�������ר��������¼�߻��Ĵ����õģ���Ϊ�߻�������ñ�����ˣ���ô������Ϸ�Ͳ���������ȥ��
//���Բ߻��������������һ����ֱ���׳��쳣
	MODULE_API void CfgErr(const std::string& sErrorType);
	MODULE_API void CfgErr(const std::string& sErrorType, const std::string& sErrorMsg);

///////////////////////////////////////////////////////////////////////


//��������
#define ART_ERR_MSG "��Դȱʧ "

//�������ר��������¼�����Ĵ����õģ���Ϊ�������ȱ����Դ������Ӱ��������Ϸ��������
//��������������ֻ���¼log�����Ҹ��߲��Կͻ����û�

	MODULE_API void ArtErr(const std::string& sErrorType);
	MODULE_API void ArtErr(const std::string& sErrorType, const std::string& sErrorMsg);

#ifndef NDEBUG
#define	Ast(exp) \
	{if(!(exp))GenErr("Assertion failure of expresion '"#exp"'");}
#else
#define	Ast(exp) \
	void (sizeof((exp)));
#endif

#ifndef NDEBUG
#define	AstMsg(exp, msg) \
	{if(!(exp))GenErr(msg);}
#else
#define	AstMsg(exp, msg)
#endif

#ifndef NDEBUG
#define DBreak(exp) {if(exp) db();}
#else
#define DBreak(exp)
#endif // NDEBUG

#ifndef NDEBUG
#define	Ver(exp) \
	{if(!(exp))GenErr("Verification failure of expresion \""#exp"\"");}
#define VerFs(exp) \
	{if(exp)GenErr("Verification failure of expresion \"!"#exp"\"");}
#define VerEq(exp,value) \
	{if((exp) != (value))GenErr("Verification failure of expresion \""#exp"=="#value"\"");}
#define VerNe(exp,value) \
	{if((exp) == (value))GenErr("Verification failure of expresion \""#exp"!="#value"\"");}
#define VerGt(exp,value) \
	{if((exp) <= (value))GenErr("Verification failure of expresion \""#exp">"#value"\"");}
#define VerGe(exp,value) \
	{if((exp) < (value))GenErr("Verification failure of expresion \""#exp">="#value"\"");}
#define VerLs(exp,value) \
	{if((exp) >= (value))GenErr("Verification failure of expresion \""#exp"<"#value"\"");}
#define VerLe(exp,value) \
	{if((exp) > (value))GenErr("Verification failure of expresion \""#exp"<="#value"\"");}
	
#else
#define	Ver(exp) exp
#define	VerFs(exp) exp
#define VerEq(exp,value) exp
#define VerNe(exp,value) exp
#define VerGt(exp,value) exp
#define VerGe(exp,value) exp
#define VerLs(exp,value) exp
#define VerLe(exp,value) exp
#endif

#ifdef _WIN32
#define SQR_TRY { CJmpBuf jmpBuf; \
	if(!setjmp(*(jmpBuf.GetBuf()))) \
	{  \
	try{
#else
#define SQR_TRY {  CJmpBuf jmpBuf; \
	if(!sigsetjmp(*(jmpBuf.GetBuf()), 1)) \
	{ \
	try{
#endif

#ifdef _WIN32
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

#ifdef _WIN32
#define SQR_CATCH_APPEND_ERROR_INFO(error)\
	char str[512];\
	_snprintf_s( str,sizeof(str), _TRUNCATE, "����λ��:%s %s:%d",__FILE__,__FUNCTION__,__LINE__);\
	error.AppendInfo(str)
#else
#define SQR_CATCH_APPEND_ERROR_INFO(error)\
	char str[512];\
	snprintf( str,sizeof(str), "����λ��:%s %s:%d",__FILE__,__FUNCTION__,__LINE__);\
	error.AppendInfo(str)
#endif

	//windows����δ������쳣����ͨ���ṹ���쳣���в������ʹ��...��������鿴��
#ifdef _WIN32
#define SQR_CATCH(sqr_exp)\
	}\
	catch(std::exception& exp) \
	{\
	GenErr(exp.what()); \
	}\
	jmpBuf.Reset(); } \
	else \
	try {\
	jmpBuf.Reset(); \
	CError error((CJmpStack::Inst().GetError())); \
	SQR_CATCH_APPEND_ERROR_INFO(error);\
	throw(error);\
	}\
	catch(CError& sqr_exp) 
#else

#define SQR_CATCH(sqr_exp)\
	}\
	catch(std::exception& exp) \
	{\
	GenErr(exp.what()); \
	}\
	catch(...)\
	{\
	GenErr("������һ��δ֪���쳣����\n"); \
	}\
	jmpBuf.Reset(); } \
	else \
	try {\
	jmpBuf.Reset(); \
	CError error((CJmpStack::Inst().GetError())); \
	SQR_CATCH_APPEND_ERROR_INFO(error);\
	throw(error);\
	}\
	catch(CError& sqr_exp) 

#endif

#define SQR_TRY_END }

#define SQR_THROW CJmpStack::Inst().LongJmp
}

