#pragma once

namespace sqr
{

	//��������ע���lua�ĺ��������ͼ�飬��Ϊ�п�������ע���lua�Ĳ���д��������
	/*
		����һ������Ϊ int DoFun(int, int)
		����ע��RESIGST_GLOBALFUNCTION(Script, DoFun, "bbb")
		�������п��ܳ������⣬�������Ǳ�����ע���ʱ���ȷ������ƥ��
		����һ��ֻ�ܼ��������ͣ�����������ͺ�ָ�����;Ͳ�����
	*/

namespace FuncTypeCheck
{
	template<typename T>		struct Type2Enum			{	enum {  Value = eFPT_None };	};

	template<>					struct Type2Enum<int8>	{	enum {	Value = eFPT_Int8 };	};
	template<>					struct Type2Enum<int16>	{	enum {	Value = eFPT_Int16 };	};
	template<>					struct Type2Enum<int32>	{	enum {	Value = eFPT_Int32 };	};
	template<>					struct Type2Enum<int64>	{	enum {	Value = eFPT_Int64 };	};

	template<>					struct Type2Enum<uint8>		{	enum {	Value = eFPT_UInt8 };	};
	template<>					struct Type2Enum<uint16>	{	enum {	Value = eFPT_UInt16 };	};
	template<>					struct Type2Enum<uint32>	{	enum {	Value = eFPT_UInt32 };	};
	template<>					struct Type2Enum<uint64>	{	enum {	Value = eFPT_UInt64 };	};

	template<>					struct Type2Enum<float>		{	enum {	Value = eFPT_Float };	};
	template<>					struct Type2Enum<double>	{	enum {	Value = eFPT_Double };	};
	template<>					struct Type2Enum<bool>		{	enum {	Value = eFPT_Bool };	};

	//-------------------------------------------------------------------------------
	template<typename T>		struct Type2Len				{ enum { Value = sizeof(T)}; };
	template<typename T>		struct Type2Len<T&>			{ enum { Value = sizeof(T*)}; };
	template<typename T>		struct Type2Len<const T&>	{ enum { Value = sizeof(T*)}; };
	template<>					struct Type2Len<void>		{ enum { Value = 0}; };
};

typedef vector<size_t, CScriptAllocator<size_t> > ScriptVecParamOffset_t;
typedef vector<int32, CScriptAllocator<int32> >	ScriptVecParamType_t;
typedef vector<size_t, CScriptAllocator<size_t> > ScriptVecParamLen_t;


#ifndef MAX_FUNCTION_PARAMS 
#define MAX_FUNCTION_PARAMS 12
#endif

#define CHECK_RET_TYPE \
	if( FuncTypeCheck::Type2Enum<RT>::Value \
	&& FuncTypeCheck::Type2Enum<RT>::Value != iRetType ) {\
		const char* szError = "����ֵ���Ͳ���ȷ";\
		return szError;}\
	if( FuncTypeCheck::Type2Len<RT>::Value \
	&& FuncTypeCheck::Type2Len<RT>::Value != iRetLen ){\
		const char* szError = "����ֵ��С����ȷ";\
		return szError;}


#define CHECK_GLOBAL_FUNC_TYPE(Z, N, _) \
	if( FuncTypeCheck::Type2Enum<BOOST_PP_CAT(P, N)>::Value \
	&& FuncTypeCheck::Type2Enum<BOOST_PP_CAT(P, N)>::Value != vecParamType[N] ) {\
		const char* szError = "�������Ͳ���ȷ";\
		return szError;}\
	if( FuncTypeCheck::Type2Len<BOOST_PP_CAT(P, N)>::Value != vecParamLen[N] ) {\
		const char* szError = "������С����ȷ";\
		return szError;}


#define CHECK_GLOBAL_FUNC(Z, N, _)\
	template<typename RT BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, typename P)>\
	const char* CheckRegisterFunc(RT (*pFunc)( BOOST_PP_ENUM_PARAMS(N, P) ), const ScriptVecParamType_t& vecParamType, int32 iRetType, \
	const ScriptVecParamLen_t& vecParamLen, size_t iRetLen)\
	{\
		if (N != vecParamType.size()){ return "����������ƥ��"; }\
		BOOST_PP_REPEAT(N, CHECK_GLOBAL_FUNC_TYPE, _)\
		CHECK_RET_TYPE\
		return NULL;\
	}

BOOST_PP_REPEAT(MAX_FUNCTION_PARAMS, CHECK_GLOBAL_FUNC, _)

//--------------------------------------------------------------------------

#define CHECK_MEM_THIS_TYPE if( FuncTypeCheck::Type2Enum<Callee>::Value != vecParamType[0]) return false;

#define CHECK_MEM_FUNC_TYPE(Z, N, _) \
	if( FuncTypeCheck::Type2Enum<BOOST_PP_CAT(P, N)>::Value \
	&& FuncTypeCheck::Type2Enum<BOOST_PP_CAT(P, N)>::Value \
	!= vecParamType[BOOST_PP_INC(N)] ) \
	{\
		const char* szError = "�������Ͳ���ȷ";\
		return szError;}\
	if( FuncTypeCheck::Type2Len<BOOST_PP_CAT(P, N)>::Value \
	!= vecParamLen[BOOST_PP_INC(N)] ) \
	{\
		const char* szError = "������С����ȷ";\
		return szError;}\


#define CHECK_MEM_FUNC(Z, N, _)\
	template<typename Callee, typename RT BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, typename P)>\
	const char* CheckRegisterFunc(RT (Callee::*pFunc)( BOOST_PP_ENUM_PARAMS(N, P) ), const ScriptVecParamType_t& vecParamType, int32 iRetType, \
	const ScriptVecParamLen_t& vecParamLen, size_t iRetLen)\
	{\
		if (N != vecParamType.size() - 1){ return "����������ƥ��"; }\
		BOOST_PP_REPEAT(N, CHECK_MEM_FUNC_TYPE, _)\
		CHECK_RET_TYPE\
		return NULL; \
	}

BOOST_PP_REPEAT(MAX_FUNCTION_PARAMS, CHECK_MEM_FUNC, _)


#define CHECK_MEM_CONST_FUNC(Z, N, _)\
	template<typename Callee, typename RT BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, typename P)>\
	const char* CheckRegisterFunc(RT (Callee::*pFunc)( BOOST_PP_ENUM_PARAMS(N, P) ) const, const ScriptVecParamType_t& vecParamType, int32 iRetType, \
	const ScriptVecParamLen_t& vecParamLen, size_t iRetLen)\
	{\
		if (N != vecParamType.size() - 1){ return "����������ƥ��"; }\
		BOOST_PP_REPEAT(N, CHECK_MEM_FUNC_TYPE, _)\
		CHECK_RET_TYPE\
		return NULL; \
	}

BOOST_PP_REPEAT(MAX_FUNCTION_PARAMS, CHECK_MEM_CONST_FUNC, _)


#define CHECK_MEM_CALLBACK_TYPE(Z, N, _) \
	if( FuncTypeCheck::Type2Enum<BOOST_PP_CAT(P, N)>::Value \
	&& FuncTypeCheck::Type2Enum<BOOST_PP_CAT(P, N)>::Value \
	!= vecParamType[N] ) \
	{\
		const char* szError = "�������Ͳ���ȷ";\
		return szError;}\
	if( FuncTypeCheck::Type2Len<BOOST_PP_CAT(P, N)>::Value \
	!= vecParamLen[N] ) \
	{\
		const char* szError = "������С����ȷ";\
		return szError;}


#define CHECK_MEM_CALLBACK(Z, N, _)\
	template<typename Callee, typename RT BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, typename P)>\
	const char* CheckRegisterCallBack(RT (Callee::*pFunc)( BOOST_PP_ENUM_PARAMS(N, P) ), const ScriptVecParamType_t& vecParamType, int32 iRetType, \
	const ScriptVecParamLen_t& vecParamLen, size_t iRetLen)\
	{\
		if (N != vecParamType.size()){ return "����������ƥ��"; }\
		BOOST_PP_REPEAT(N, CHECK_MEM_CALLBACK_TYPE, _)\
		CHECK_RET_TYPE\
		return NULL; \
	}

BOOST_PP_REPEAT(MAX_FUNCTION_PARAMS, CHECK_MEM_CALLBACK, _)


#define CHECK_MEM_CONST_CALLBACK(Z, N, _)\
	template<typename Callee, typename RT BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, typename P)>\
	const char* CheckRegisterCallBack(RT (Callee::*pFunc)( BOOST_PP_ENUM_PARAMS(N, P)) const, const ScriptVecParamType_t& vecParamType, int32 iRetType, \
	const ScriptVecParamLen_t& vecParamLen, size_t iRetLen)\
	{\
		if (N != vecParamType.size()){ return "����������ƥ��"; }\
		BOOST_PP_REPEAT(N, CHECK_MEM_CALLBACK_TYPE, _)\
		CHECK_RET_TYPE\
		return NULL; \
	}

BOOST_PP_REPEAT(MAX_FUNCTION_PARAMS, CHECK_MEM_CONST_CALLBACK, _)


#undef CHECK_RET_TYPE
#undef CHECK_GLOBAL_FUNC_TYPE
#undef CHECK_GLOBAL_FUNC
#undef CHECK_MEM_THIS_TYPE
#undef CHECK_MEM_FUNC_TYPE
#undef CHECK_MEM_FUNC
#undef CHECK_MEM_CONST_FUNC
#undef CHECK_MEM_CALLBACK
#undef CHECK_MEM_CONST_CALLBACK

}
