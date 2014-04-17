// ResourceCheckerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CResProcessorMgr.h"
//#include "CAraProcessor.h"
#include "CAreProcessor.h"
#include "CArpProcessor.h"
#include "CMapProcessor.h"
#include "StringHelper.h"
#include <string>
#include <algorithm>


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

CResProcessorMgr::CResProcessorMgr()
: m_bNewResFile(false)
{
}

// ----------------------------------------------------------------------------
// ע�ᴦ���࣬���������ȼ�Ϊ�±����m_ProcessRegist����
void CResProcessorMgr::RegisterProcessor( IResProcessor* pProcessor, int32 nPriority )
{
	if( nPriority >= MAX_PRIORITY_LEVEL )
	{
		GenErr("Priority���");
	}

	pProcessor->Init(this);

	int	nCount = (int)pProcessor->GetCountCanBeProcess();
	if ( nCount == -1 )
	{
		m_ProcessRegist[MAX_PRIORITY_LEVEL].m_pProcessor = pProcessor;
	}
	else
	{
		Ast( !m_ProcessRegist[nPriority].m_pProcessor );
		m_ProcessRegist[nPriority].m_pProcessor = pProcessor;

		for ( int i = 0; i < nCount; ++i )
		{
			int nType = pProcessor->GetTypeCanBeProcess(i);
			Ast( m_mapTypeToRegist.find(nType) == m_mapTypeToRegist.end() );
			m_mapTypeToRegist[nType] = nPriority;
		}
	}
}

void CResProcessorMgr::AddPrcDir( const char* s )
{
	m_vecProcessDir.push_back( s );
}

void CResProcessorMgr::AddSerDir( const char* s )
{
	m_vecSearchDir.push_back( s );
}

void CResProcessorMgr::SetNewSerFile( const char* s )
{
	string str = s;
	if ( str == "t" )
		m_bNewResFile = true;
	else
		m_bNewResFile = false;
}

FTW_RESULT ProcessFile( const wchar_t* szFileName, const struct _stat *, FTW_FLAG eFlag, void* pContext )
{
	if( eFlag == _FTW_DIR && !_wcsicmp( szFileName + wcslen( szFileName ) - 4, L".svn" ) )
		return _FTW_IGNORE;

	CResProcessorMgr* pMgr = (CResProcessorMgr*)pContext;
	string sOldLocale=setlocale(LC_CTYPE,NULL);
	
	setlocale(LC_CTYPE, "");
	if( eFlag == _FTW_DIR )
	{
		pMgr->BuildDir(utf16_to_gbk(szFileName + pMgr->GetSrcDir().size() + 1).c_str());
	}
	else
	{
		pMgr->ProcessWith( utf16_to_gbk(szFileName + pMgr->GetSrcDir().size() + 1).c_str(), "��" );
	}
	setlocale(LC_CTYPE, sOldLocale.c_str());

	return _FTW_CONTINUNE;
}

void CResProcessorMgr::ProcessPath()
{
	AddExType("dir");
	for( size_t i = 0; i < m_vecProcessDir.size(); i++ )
	{
		string szTemp = m_vecProcessDir[i];
		for( size_t j = 0; j < szTemp.size(); j++ )
		{
			if( szTemp[j] == '\\' || szTemp[j] == '/' )
			{
				szTemp[j] = 0;
			}
		}

		szTemp = szTemp.c_str();
		string szIncludeList	 = szTemp + "\\include.lst";
		string szExcludeList	 = szTemp + "\\exclude.lst"; 
		string szAutoExcludeList = szTemp + "\\autoexclude.lst";

		ifstream streamExclude( szExcludeList.c_str() );
		while( streamExclude && !streamExclude.eof() )
		{
			string szFile;
			streamExclude>>szFile;
			if( szFile.empty() )
			{
				continue;
			}

			szFile = szTemp + "\\" + szFile;
			replace( szFile.begin(), szFile.end(), '\\', '/' );
			m_setExcludeFile.insert( szFile );
		}

		ifstream streamAutoExclude( szAutoExcludeList.c_str() );
		while( streamAutoExclude && !streamAutoExclude.eof() )
		{
			string szFile;
			streamAutoExclude>>szFile;
			if( szFile.empty() )
			{			
				continue;
			}

			szFile = szTemp + "\\" + szFile;
			replace( szFile.begin(), szFile.end(), '\\', '/' );
			m_setExcludeFile.insert( szFile );
		}

		ifstream streamInclude( szIncludeList.c_str() );
		while( streamInclude && !streamInclude.eof() )
		{
			string szFile;
			streamInclude>>szFile;
			if( szFile.empty() )
			{
				continue;
			}

			szFile = szTemp + "\\" + szFile;
			replace( szFile.begin(), szFile.end(), '\\', '/' );
			ProcessWith( szFile.c_str(), "��" );
		}
		BuildDir( m_vecProcessDir[i],true );
		FileTreeWalk( gbk_to_utf16(( GetSrcDir() + "\\" + m_vecProcessDir[i] )).c_str(), ProcessFile, this );
	}
}

void CResProcessorMgr::PostProcessed(const char* szFileName)
{
	m_strCurProcessFile = szFileName;
}

// ----------------------------------------------------------------------------
//  �������ļ��Ĵ���ģʽ

void CResProcessorMgr::ProcessWith( const char* szFileName, const char* szParentFileName )
{
	if( !szFileName || !szFileName[0]  )
		return;

	std::string szTmpFind = szFileName;
	replace( szTmpFind.begin(), szTmpFind.end(), '\\', '/' );
	if( m_setExcludeFile.find( szTmpFind ) != m_setExcludeFile.end() )
	{
		return;
	}

 
	std::string sFile	= szFileName;
	std::string sParent = szParentFileName;
	
	std::locale loc;
	use_facet<ctype<char> >( loc ).tolower( &sFile[0], &sFile[sFile.length()] );
	use_facet<ctype<char> >( loc ).tolower( &sParent[0], &sParent[sParent.length()] );
	replace( sFile.begin(), sFile.end(), '\\', '/' );

	uint32 nType = 0;
	for ( size_t nLength = sFile.length() - 1, i = 0; nLength 
		  && sFile[nLength] != '.'; --nLength, ++i )
	{
		nType = nType|( sFile[nLength]<<(i*8) );
	}

	if( m_setExcludeType.find( nType ) != m_setExcludeType.end() 
		&& !strcmp( szParentFileName, "��" ) )
	{
		return;
	}

	map<uint32,uint32>::iterator it = m_mapTypeToRegist.find( nType );
	if ( it == m_mapTypeToRegist.end() ) 
	{
		// ���þ���������ļ���ֱ�Ӷ���MAX_PRIORITY_LEVEL���������
		m_ProcessRegist[MAX_PRIORITY_LEVEL].m_mapFileRepository[sFile] = sParent;
	}
	else
		m_ProcessRegist[it->second].m_mapFileRepository[sFile] = sParent;

	if( sFile.rfind("/") == -1 )
		sFile = sParent.substr( 0,sParent.rfind( "/" ) + 1 ) + sFile;
	InsertFile(sFile);
}

// ----------------------------------------------------------------------------
// ������

void CResProcessorMgr::Process( IProcessHandler* pHandler )
{
	for( int32 n = 0; n < MAX_PRIORITY_LEVEL + 1; n++ )
	{
		CProcessRegist& ProcessRegist = m_ProcessRegist[n];

		if( !ProcessRegist.m_pProcessor )
			continue;

		int32 nCur = 0;
		int32 nTotal = (int32)ProcessRegist.m_mapFileRepository.size();

		// m_mapFileRepository���ڴ洢��Դ�ļ�·������һ��string�����ļ����ڶ���string�Ǹ��ļ�
		map<string,string>::iterator it = ProcessRegist.m_mapFileRepository.begin();
		for ( ; it != ProcessRegist.m_mapFileRepository.end(); it++, nCur++ )
		{
			// ��Ŀ¼�ı䵽��Դ�ļ���Ŀ¼����:f:/ybtx/artist/resbin
			SureChangeToDir( GetSrcDir().c_str() ); 
			
			// ��ͼ������һ����Դ�ļ�
			const char* szFileName=it->first.c_str();
			FILE* fp = fopen( szFileName, "rb" ); 
			if ( fp )
			{
				fclose( fp );
				bool IsProcessed = ProcessRegist.m_pProcessor->RunProcess( szFileName );
				if( pHandler && IsProcessed )
				{
					pHandler->OnProcessed( m_strCurProcessFile.c_str(), szFileName, false );
				}
			}
			else
			{
				// ȥ����·��Ѱ��
				vector<string>::iterator itr = m_vecSearchDir.begin();
				bool bfond = false;
				while ( itr != m_vecSearchDir.end() )
				{
					string dir = m_strSrcPath + "/" + (*itr);
					string szFile_Name = it->first;
					SureChangeToDir( dir.c_str() ); 
					fp = fopen( szFileName, "rb" ); 
					if ( fp )
					{
						fclose( fp );
						bfond = true;
						// ���
						bool IsProcessed = ProcessRegist.m_pProcessor->RunProcess( szFileName );
						if ( IsProcessed  )
							InsertCollectFile((*itr), szFile_Name);
						break;
					}

					++itr;
				}

				if( pHandler && !bfond )
				{
					pHandler->OnProcessed( szFileName, it->second.c_str(), true );
				}
			}
		}
	}
}

void CResProcessorMgr::SetCompileLua(bool flag)
{
	m_bCompileLua = flag;
}

bool CResProcessorMgr::GetCompileLua()
{
	return m_bCompileLua;
}

void CResProcessorMgr::BuildDir(const string& path,bool isRoot)
{
	MapDir::iterator it = m_mapDirStream.find(path);
	if(it == m_mapDirStream.end())
	{
		string lPath = path;
		tolower(lPath);
		m_mapDirStream.insert( MapDir::value_type(lPath,DirFile(isRoot) ) );	
		if(!isRoot)
			InsertFile(lPath + ".dir");
	}
}

void CResProcessorMgr::InsertFile(const string& file)
{
	string path = file.substr(0,file.rfind("/"));
	string ofile = file.substr(path.size() + 1,file.size()-path.size()-1);	
	MapDir::iterator it = m_mapDirStream.find(path);
	if(it != m_mapDirStream.end())
		it->second.FilePool.insert(ofile);
}

void CResProcessorMgr::ProcessDir(IProcessHandler* pHandler)
{
	MapDir::iterator eit = m_mapDirStream.end();
	
	string sOldLocale=setlocale(LC_CTYPE,NULL);
	
	setlocale(LC_CTYPE, "");
	for( MapDir::iterator it = m_mapDirStream.begin(); it != eit; ++it )
	{
		string FileName = it->first;
		if(it->second.isRoot)
			FileName += "/root";
		FileName += ".dir";

		ofstream pStream((m_strSrcPath + "/" + FileName).c_str());

		if(pStream.is_open())
		{
			set<string>::iterator fit,efit = it->second.FilePool.end();
			for( fit = it->second.FilePool.begin(); fit!=efit; ++fit )
				pStream<<*fit<<endl;
			pHandler->OnProcessed(FileName.c_str(),FileName.c_str(),false);
		}
		pStream.close();
	}
	setlocale(LC_CTYPE,sOldLocale.c_str());
}

void CResProcessorMgr::InsertCollectFile( string& p, string& s )
{
	MapCltFile::iterator itr = m_CollectFiles.find( p );
	if ( itr == m_CollectFiles.end() )
	{
		SetStr t_setstr;
		m_CollectFiles.insert(make_pair(p, t_setstr));
		itr = m_CollectFiles.find( p );
	}
	itr->second.setStr.insert(s);
}

void CResProcessorMgr::BuildSerDir()
{
	// ͨ��collectfiles�ؽ�ser dir
	MapCltFile::iterator itr = m_CollectFiles.begin();
	while ( itr != m_CollectFiles.end() )
	{
		map<string,MapDirFile>::iterator mitr = m_mapSerDirStream.find( itr->first );
		if ( mitr == m_mapSerDirStream.end() )
		{
			MapDirFile mapdirfile;
			m_mapSerDirStream.insert(make_pair(itr->first, mapdirfile));
			mitr = m_mapSerDirStream.find( itr->first );
		}
		// ��ɸ�path��dir������
		MapDir& mapdir		= mitr->second.mapDir;
		string path			= itr->first;
		set<string> setstr	= itr->second.setStr;
		set<string>::iterator itrstr = setstr.begin();
		while ( itrstr != setstr.end() )
		{
			string str = (*itrstr);
			int i = 0;
			
			if (path.length() > 0)
			{
				bool fst = true;
				while(str[i++] != 0)
				{
					if (str[i-1] == '/')
					{
						string path = str.substr(0, i-1);
						if ( fst )
							BuildSerDir(mapdir, path, true);
						else
						{
							BuildSerDir(mapdir, path, false);
						}
						fst = false;
					}
				}
				InsertSerFile(mapdir, str);
			}
			++itrstr;
		}
		++itr;
	}
}

void CResProcessorMgr::BuildSerDir(MapDir& mapdir, const string& path, bool isRoot)
{
	MapDir::iterator it = mapdir.find(path);
	if(it == mapdir.end())
	{
		string lPath = path;
		tolower(lPath);
		mapdir.insert( MapDir::value_type(lPath,DirFile(isRoot) ) );	
		if(!isRoot)
			InsertSerFile(mapdir, lPath + ".dir");
	}
}

void CResProcessorMgr::InsertSerFile(MapDir& mapdir, const string& file)
{
	string path = file.substr(0,file.rfind("/"));
	string ofile = file.substr(path.size() + 1,file.size()-path.size()-1);	
	MapDir::iterator it = mapdir.find(path);
	if(it != mapdir.end())
		it->second.FilePool.insert(ofile);
}

void CResProcessorMgr::BuildSerLogFile()
{
	string		szSpacer	 = "###"; 
	string		szPkgLogFile = GetDstDir() + "/PkgSerLog.log";

	string sOldLocale=setlocale(LC_CTYPE,NULL);
	setlocale(LC_CTYPE, "");

	set<string> linestr;
	ifstream pkglog_file( szPkgLogFile.c_str() );
	if ( pkglog_file.good() && !m_bNewResFile)
	{
		string szFile;
		while ( std::getline(pkglog_file, szFile) )
			linestr.insert(szFile);
	}

	// �Ӵ��log��ȡ�����д���ļ��İ�����Դ�ļ�·��
	ofstream	pkg_file( szPkgLogFile.c_str() );
	if(!pkg_file.is_open())
		return;

	// ��������� 1 �ļ� m_CollectFiles�Ĵ���
	MapCltFile::iterator itr = m_CollectFiles.begin();
	while ( itr != m_CollectFiles.end() )
	{
		string root_path = GetSrcDir();
		string path = itr->first;
		string temp = path.substr(0, 3);
		
		while ( temp == "../" )
		{
			path = path.substr( 3, path.length() - 3);
			root_path = root_path.substr(0, root_path.find_last_of("/"));
			temp = path.substr(0, 3);
		}
		root_path = root_path + "/" + path;
		set<string>::iterator str_itr = itr->second.setStr.begin();
		while ( str_itr != itr->second.setStr.end() )
		{
			string file_name = *str_itr;
			string pkg_name = file_name.substr(0, file_name.find_first_of('/'));
			file_name = root_path + "/" + file_name;
			string sstr = pkg_name + szSpacer + file_name;
			linestr.insert(sstr);
			++str_itr;
		}
		++itr;
	}

	// ���dir
	map<string,MapDirFile>::iterator it = m_mapSerDirStream.begin();
	while ( it != m_mapSerDirStream.end() )
	{
		string root_path = GetSrcDir();
		string path = it->first;
		string temp = path.substr(0, 3);
		while ( temp == "../" )
		{
			path = path.substr( 3, path.length() - 3);
			root_path = root_path.substr(0, root_path.find_last_of("/"));
			temp = path.substr(0, 3);
		}
		root_path = root_path + "/" + path;

		MapDir::iterator map_eit = it->second.mapDir.end();

		
		for( MapDir::iterator map_it = it->second.mapDir.begin(); map_it != map_eit; ++map_it )
		{
			string FileName = map_it->first;
			if(map_it->second.isRoot)
				FileName += "/root";
			FileName += ".dir";

			ofstream pStream((root_path + "/" + FileName).c_str());

			if(pStream.is_open())
			{
				set<string>::iterator fit,efit = map_it->second.FilePool.end();
				for( fit = map_it->second.FilePool.begin(); fit!=efit; ++fit )
					pStream<<*fit<<endl;

				string pkg_name = FileName.substr(0, FileName.find_first_of('/'));
				FileName = root_path + "/" + FileName;
				string sstr = pkg_name + szSpacer + FileName;
				linestr.insert(sstr);
			}
			pStream.close();
		}
		++it;
	}

	set<string>::iterator litr = linestr.begin();
	while ( litr != linestr.end() )
	{
		pkg_file << *litr << endl;
		++litr;
	}

	pkg_file.close();
	setlocale(LC_CTYPE,sOldLocale.c_str());
}


