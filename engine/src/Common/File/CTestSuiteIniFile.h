#pragma once
#include <string>
#include "CIniFile.h"
#include "cppunit/extensions/HelperMacros.h"

namespace sqr
{
	class CTestSuiteIniFile : public TestFixture
	{
	public:
		CTestSuiteIniFile(void);
		~CTestSuiteIniFile(void);
		void setUp();
		void tearDown();

		CPPUNIT_TEST_SUITE(CTestSuiteIniFile);

		CPPUNIT_TEST(TestOpenFile);

		CPPUNIT_TEST(TestGetKeyValue);
		CPPUNIT_TEST(TestGetString);
		CPPUNIT_TEST(TestGetInteger);
		CPPUNIT_TEST(TestGetChar);

		CPPUNIT_TEST(TestWriteString);
		CPPUNIT_TEST(TestWriteInteger);
		CPPUNIT_TEST(TestGetKeyValue);

		CPPUNIT_TEST_SUITE_END();

		void TestOpenFile();//�����ļ��Ƿ��
		//���ݴ򿪵��ļ������Դ���ȡ��ָ�Ƿ���ȷ
		void TestGetKeyValue();
		void TestGetString();
		void TestGetInteger();
		void TestGetChar();
		////���ļ���д����Ϣ��������Ϣ�Ƿ�д��
		void TestWriteString();
		void TestWriteInteger();
	};
}
