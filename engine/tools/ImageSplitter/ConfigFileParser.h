#pragma once
#include "BaseDefs.h"

	class CConfigFileParser
	{
	public:
		struct	Node
		{
			string szPicName;	// ���з�ͼƬ������
			string szSrcPath;	// ��ͼ������Ϣ�ļ�·��
			string szTgtPath;	// �洢Ŀ��·��	
		};

	private:
		typedef vector<string> SourcePathVector;
		typedef vector<string> PictureNameVector;
		typedef vector<string> TargetPathVector;
		
		PictureNameVector	m_vecPicName;
		SourcePathVector	m_vecSourcePath;
		TargetPathVector	m_vecTargetPath;
		
		uint32				m_uCurrIndex;
		uint32				m_uNodeCount;
		string				m_szConfigFile;

	private:
		static CConfigFileParser *ms_Inst;
		CConfigFileParser();
		CConfigFileParser( const CConfigFileParser & );
		CConfigFileParser& operator = (const CConfigFileParser &);

		// ---- ɾ���ַ������˵Ŀո�
		string	_Trim(string &str);

		// ---- �����ļ�����һ���ڵ�
		bool	_ParseNode( ifstream &file );

	public:
		~CConfigFileParser();

		static CConfigFileParser* GetInst();

		// ---- ���������ļ�·��, ���������ļ�
		bool	SetConfigFilePath( const string &szConfigFile );

		// ---- ����·����Ϣ���Ա��´�ʹ��
		void	Reset();

		// ---- ȡ����һ���з������ļ��ڵ�, ������ʧ���򷵻�false
		bool	GetNextNode( Node &sNextNode );
	};