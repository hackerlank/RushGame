#pragma once
#include <string>
#include "CTxtTableFile.h"
#include "cppunit/extensions/HelperMacros.h"

namespace sqr
{
	class CTestSuiteTableFile : public TestFixture
	{
	public:
		CTestSuiteTableFile(void);
		~CTestSuiteTableFile(void);
		void setUp();
		void tearDown();

		CPPUNIT_TEST_SUITE(CTestSuiteTableFile);

		CPPUNIT_TEST(TestLoadFile);

		CPPUNIT_TEST(TestGetHeight);
		CPPUNIT_TEST(TestGetWidth);

		CPPUNIT_TEST(TestGetString);
		CPPUNIT_TEST(TestGetInteger);
		CPPUNIT_TEST(TestGetFloat);

		CPPUNIT_TEST_SUITE_END();

		// �����Ʊ���ָ��ļ�
		void TestLoadFile();

		// �õ�����
		void TestGetHeight();
		// �õ�����
		void TestGetWidth();

		// �����кŵõ�ĳ��ĳ��
		void TestGetString();
		// �����кŵõ�ĳ��ĳ��
		void TestGetInteger();
		// �����кŵõ�ĳ��ĳ��
		void TestGetFloat();

	};
}

