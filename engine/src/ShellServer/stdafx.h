// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#include "Base.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0400
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#undef GetClassName
#undef SendMessage
#undef CreateSemaphore

#include <map>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <climits>

#ifdef _WIN32
#include <hash_map>
using namespace stdext;
#else
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

using namespace std;
using namespace sqr;
using namespace CPPUNIT_NS;
