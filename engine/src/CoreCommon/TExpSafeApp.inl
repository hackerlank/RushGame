#pragma once
#include "TExpSafeApp.h"
#include "BaseHelper.h"
#include "TApp.inl"

template<typename Traits>
TExpSafeApp<Traits>::TExpSafeApp()
{
#ifdef _WIN32
	m_PreParamHandler=NULL;
	m_PrePurecallHandler=NULL;
	m_PreSeTransFunction=NULL;
#endif

	SetUpTermSigHandler();

#ifdef _WIN32
	m_PreSeTransFunction = _set_se_translator(TransESHFun);
	m_PreParamHandler = _set_invalid_parameter_handler(InvalidParameterHandler);
	m_PrePurecallHandler = _set_purecall_handler(PurecallHandler);
#endif
	m_PreTerminateFun = std::set_terminate(TerminateHandler);
}

template<typename Traits>
TExpSafeApp<Traits>::~TExpSafeApp()
{
	std::set_terminate(m_PreTerminateFun);
#ifdef _WIN32
	_set_purecall_handler(m_PrePurecallHandler);
	_set_invalid_parameter_handler(m_PreParamHandler);
	_set_se_translator(m_PreSeTransFunction);	
#endif
}


#ifdef _WIN32

template<typename Traits>
void TExpSafeApp<Traits>::InvalidParameterHandler(const wchar_t* expression,
										   const wchar_t* function, 
										   const wchar_t* file, 
										   unsigned int line, 
										   uintptr_t pReserved)
{
#ifdef _DEBUG
	wstringstream strm;
	strm<<L"c����ʱ�⺯����������"<<endl
		<<L"��Ϣ:"<< expression <<endl
		<<L"����:"<< function <<endl
		<<L"�ļ�:"<< file <<endl
		<<L"����:"<< line <<endl;

	wstring strValue = strm.str();
	const wchar_t* wzStr=strValue.c_str();

	size_t stLen=wcstombs(NULL,wzStr,0);

	if( stLen == size_t(-1) )
		GenErr("wcstombs test failed!");

	string str;
	str.resize(stLen);
	
	if( -1 == wcstombs(&str[0],wzStr,stLen) )
		GenErr("wcstombs run failed!");

	GenErr(str);
#else
	GenErr("c����ʱ�⺯����������������������õ�����c����ʱ�⺯���Ĳ�����");
#endif
}

template<typename Traits>
void TExpSafeApp<Traits>::PurecallHandler(void)
{
	GenErr("C++����PurecallHandler!");
}

#endif

template<typename Traits>
void TExpSafeApp<Traits>::TerminateHandler()
{
	GenErr("C++����TerminateHandler!");
}

