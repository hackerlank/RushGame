// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#include "Base.h"

#define _WIN32_IE 0x0600
#define _WIN32_WINNT 0x0500
//#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#include <windows.h>

#include <cmath>
#include <sstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <string>

//#pragma warning(disable:4996)

#ifdef _WIN32
#include <hash_map>
using namespace stdext;
#else
#include <ext/hash_map>
#endif

#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>


using namespace std;
using namespace CPPUNIT_NS;
using namespace sqr;

