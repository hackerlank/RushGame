#pragma once
#include "tinyxml/tinyxml.h"
#include "SQRWnd.h"
#include "GUIBase.h"
#include "SQRGuiMallocObject.h"

namespace sqr
{
	// ���ܣ�XMLhandler������
	class GUI_API XMLHandler : public SQRGuiMallocObject
	{
	public:
		// ���캯��
		XMLHandler(void)			{};
		// ��������
		virtual ~XMLHandler(void)	{};

		// �������
		virtual void ElementStart(const char* element, TiXmlElement* ElementPtr)	{};
		virtual void ElementEnd	 (const char* element)							{};
	};
	
}