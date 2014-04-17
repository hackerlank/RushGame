#pragma once

#include "GUIXmlParser.h"
#include "CodeCvs.h"

namespace sqr
{
	// UI XML�����ĳ�����
	class GUI_API StuffParser : public XMLParser
	{
	public:
		// ���캯��
		StuffParser(void);
		// ��������
		virtual ~StuffParser(void);
		// ������������XML�ļ� 
		void ParseXMLFile(XMLHandler& handler, TiXmlDocument& doc);

	protected:
		// ��ʼ��������ģ��ĳ�����
		bool InitilaliseImpl(void)	{ return true; }
		// ��ս�����
		void CleanupImpl(void)		{ return;	   }
		// ����DOC
		bool ParXMLElement(XMLHandler* handler,TiXmlElement* ElementPtr);

		// �ص���handlers
		void StartElement(XMLHandler* handler, const char* element, TiXmlElement* attr);
		void EndElement(XMLHandler* handler, const char* element);

	};
}