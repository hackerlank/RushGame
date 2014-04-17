#if !defined _STDAFX_H
#define _STDAFX_H

#include "Base.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <vector>
#include <stack>
#include <list>
#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

using namespace CPPUNIT_NS;
using namespace std;
using namespace sqr;

#ifdef _WIN32
#include <hash_map>
#include <hash_set>
using namespace stdext;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dbghelp.h>

#else
#include <ext/hash_map>
#include <ext/hash_set>
using namespace __gnu_cxx;
#endif



#endif

/*
common������ֻ����game��һЩ����ʹ�ã�������Ҫ��ӻ�����⣬�����
��module����runmodule�Լ�game����Ҫ�Ļ����ǾͰ�Ҫ��ӵĴ���ŵ�Base������
common����������ֻ����game���߹��߼��أ�����������ֵ�����ʽ
*/
