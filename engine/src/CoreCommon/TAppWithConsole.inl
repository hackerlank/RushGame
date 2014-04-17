#pragma once
#include "TAppWithConsole.h"

template<typename Traits>
TAppWithConsole<Traits>::TAppWithConsole(void)
{
#ifdef _WIN32
	if( ImpAppConfig_t::Inst()->ShowConsole() )
	{
		ShowConsole(true);
		SetConsoleTitle( ImpAppConfig_t::Inst()->GetConsoleTitle().c_str() );
		//��λerrlog�����������̨
		RedirectIOToConsole(true);
	}
#endif
}

template<typename Traits>
TAppWithConsole<Traits>::~TAppWithConsole(void)
{
#ifdef _WIN32
	if( ImpAppConfig_t::Inst()->ShowConsole() )
	{	
		RedirectIOToConsole(false);
		ShowConsole(false);
	}
#endif
}
