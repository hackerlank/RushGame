// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#include "Base.h"
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <iostream>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Network.h"

#ifdef _WIN32
#include <hash_map>
using namespace stdext;
#else
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif

using namespace std;
using namespace sqr;
using namespace CPPUNIT_NS;

