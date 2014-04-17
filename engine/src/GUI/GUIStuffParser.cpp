#include "stdafx.h"
#include "GUIStuffParser.h"
#include "GUIXmlHandler.h"


namespace sqr
{
	// GUI XML  ����
	StuffParser::StuffParser()
	{
		m_idtfString = "GUI Stuff Parser";
	}

	StuffParser::~StuffParser()
	{
		// ...  ��������
	}

	// All stuff goes here
	void StuffParser::ParseXMLFile(XMLHandler& handler, TiXmlDocument& doc)
	{
		// ���ݴ�����ȡ������
		TiXmlElement* Root = doc.RootElement();
		if (Root)
		{
			// ����
			if ( !ParXMLElement(&handler,Root) )
			{
				// ����ʧ�ܣ��׳��쳣 
				return;
			}
		}
		
	}

	bool StuffParser::ParXMLElement(XMLHandler* handler,TiXmlElement* ElementPtr)
	{
		// ����
		if ( ElementPtr )
		{
			// ��ʼһ��Ԫ�ؽڵ�
			std::string element = ElementPtr->ValueStr();
			StartElement(handler, element.c_str(), ElementPtr);

			{
				// �ݹ����
				TiXmlElement* ChildElement = ElementPtr->FirstChildElement();
				while(ChildElement)
				{
					ParXMLElement(handler, ChildElement);
					ChildElement = ChildElement->NextSiblingElement();
				}
			}

			// ������Ԫ�ؽڵ�
			EndElement(handler, element.c_str());

			return true;
		}
		return false;
	}

	inline void StuffParser::StartElement(XMLHandler* handler, const char* element, TiXmlElement* attr)
	{
		handler->ElementStart(element, attr);
	}

	inline void StuffParser::EndElement(XMLHandler* handler, const char* element)
	{
		handler->ElementEnd(element);
	}

}