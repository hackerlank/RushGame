// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#include "Base.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0500
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <climits>
#include <cstring>
#include <stack>

#ifdef _WIN32
#include <hash_set>
#include <hash_map>
using namespace stdext;
#else
#include <ext/hash_set>
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif

#include <queue>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;
using namespace CPPUNIT_NS;
using namespace sqr;

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�

