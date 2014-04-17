#pragma once
#include "tinyxml/tinyxml.h"

namespace sqr_tools
{
	class CEditFormatBase;

	class CEditInfo
	{
		friend class CEditFormatBase;

	public:
		CEditInfo(){}
		CEditInfo(CEditFormatBase* edit_format);
		virtual ~CEditInfo();

		void			RefersTo(CEditInfo* edit_info);
		const string&	GetDeadPath()	const;
		const string&	GetLivePath()	const;
		string			GetSavePath()	const;

	protected:
		TiXmlDocument*	getXmlDoc();

		/**
		* ��after_this�ڵ�(���������ϲ�ڵ�)֮�����"<node>text</node>"��ʽ�Ľڵ�,�����ر�����Ľڵ�ָ��
		*/
		TiXmlNode*		xmlInsertNodeTextAfter(TiXmlNode* after_this, const string& node_name, const string& node_text);

	private:
		/**
		* �̳��������ȥ��˽�б༭��Ϣ
		*/
		virtual void	saveCustomInfo() {};

	private:
		void			addReferenceParent(const string& ref_parent);
		void			addXmlDecl();
		void			setDeadPath(const string& dead_path);
		void			setLivePath(const string& live_path);
		void			writeXmlData(FILE* file);
		void			saveReference();
		void			saveEditList();

		typedef list<CEditInfo*>	EditInfoList;
		typedef set<CEditInfo*>		EditInfoSet;
		typedef set<string>			ReferenceSet;

		CEditFormatBase*	m_edit_format;
		TiXmlDocument		m_xml_doc;
		EditInfoList		m_edit_info_list;
		EditInfoSet			m_edit_info_set;	// ����ظ�ע��
		string				m_dead_path;		// tgt֮ǰ�ľ���·��(������tgt)
		string				m_live_path;		// tgt֮������·��(����charactor\tex\ar001.dds)
		ReferenceSet		m_ref_parent_set;	// ���õ������ļ��б�

	public:
		static const string	REF_ROOT_NODE;
		static const string	REF_ELEMENT_NODE;
	};

}
