// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#include "Base.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#include <windows.h>
#endif

#include <cmath>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <vector>
#include <queue>
#include <list>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

#ifdef _WIN32
#include <hash_map>
using namespace stdext;
#else
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif

using namespace CPPUNIT_NS;
using namespace std;
using namespace sqr;


#define  NORMALSIZE  (sizeof(CNode) << 8)

