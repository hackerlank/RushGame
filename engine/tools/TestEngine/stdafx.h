// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#include "Base.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Message.h>
#include <cppunit/TestRunner.h>

using namespace CPPUNIT_NS;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <string>


namespace sqr
{
	class CError;
}


using namespace std;
using namespace sqr;

