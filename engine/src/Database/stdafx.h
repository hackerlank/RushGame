// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#if !defined _MYSQL_STDAFX_H
#define _MYSQL_STDAFX_H

#include "Base.h"

#ifdef _WIN32
#include <WinSock2.h>
#endif


#include "mysql/mysql.h"

#include <iostream>
#include <list>
#include <stdexcept>
using namespace std;
using namespace sqr;

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

using namespace CPPUNIT_NS;

#endif


