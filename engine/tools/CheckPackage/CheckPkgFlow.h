/*	checkpkgflow  ���ɴ���ļ�������������
	�� checkpackage �г������
*/
#pragma once
#include "CResProcessorMgr.h"
#include "CAraProcessor.h"
#include "CAreProcessor.h"
#include "CArpProcessor.h"
#include "CMapProcessor.h"
#include "CArsProcessor.h"
#include "CLuaProcessor.h"
#include "CDftProcessor.h"
#include "CAgpProcessor.h"
#include "CSkeletalsMgr.h"
#include "CGuiProcessor.h"
#include "SMapProcessor.h"
#include "ExpHelper.h"
#include "ErrLogHelper.h"
#include "CDataSources.h"
#include "CodeCvs.h"

struct CProcessHandler : public IProcessHandler
{
	map< string, list<string> > m_mapProcessFiles;
	map< string, string >		m_mapMissFile;

	void OnProcessed( const char* szFileName, const char* szParentFileName, bool bMiss )
	{
		if( !bMiss )
		{
			string sName = szParentFileName;
			size_t i = 0;
			while( i < sName.size() && sName[i] != '/' )
			{
				i++;
			}
			sName[i] = 0;

			m_mapProcessFiles[sName.c_str()].push_back( szFileName );
		}
		else
		{
			// ����·��Ҳû��
			m_mapMissFile[szFileName] = szParentFileName;
		}
	}
};

class CheckPkgFlow
{
public:
	CheckPkgFlow();
	~CheckPkgFlow();

	// �������в�������
	void	ProcessFlow( int argc, char* argv[] );
private:
	void	_ProcessParameter( int argc, char* argv[] );	// �����������ʼ��ϵͳ
	void	_RegisterPrcessor();							// ע��Processor
	void	_ProcessPath();									// ɨ�账��·��������dir
	void	_Process();										// �����ļ���ɨ���ļ�
	void	_UnRegisterPrcessor();							// ע��Processor
	void	_BuildErrorFile();								// ����ȷʵ�ļ�·��
	void	_BuildPkgFile();								// ���ɴ���ļ�·��
	void	_BuildSerPkgFile();								// ����ser�ļ�·��
private:
	CResProcessorMgr	m_ResMgr;
	CProcessHandler		m_Handler;
	CPathMgr			m_PathMgr;
	IResProcessor*		m_pProcessor[MAX_PRIORITY_LEVEL];
	int					m_PackType;
	bool				m_beProcessOk;
};