#pragma once

#include "stdafx.h"
#include "GUIXmlHandler.h"

namespace sqr
{
	// UI XML�����ĳ�����
	class GUI_API XMLParser
	{
	public:
		// ���캯��
		XMLParser(void);
		// ��������
		virtual ~XMLParser(void);
		// ��ʼ��������ģ��
		bool Initialise(void);
		// ��ս�����
		void Cleanup(void);
		// ������������XML�ļ� 
		virtual void ParseXMLFile(XMLHandler& handler, TiXmlDocument& doc) = 0;
		// �ӽ�����Ԫ��ȡ��ǵ�string
		const char* GetIdentifierString() const;

	protected:
		// ��ʼ��������ģ��ĳ�����
		virtual bool InitilaliseImpl(void) = 0;
		// ��ս�����
		virtual void CleanupImpl(void) = 0;
		// ����ģ����ַ�����Ϣ
		UIString m_idtfString;

	private:
		bool m_beInit;			// ����ģ�鱻��ʼ���� =true.
	};
}