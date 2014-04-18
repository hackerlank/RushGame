#include "stdafx.h"


#include "CFilePack.h"
#include "ExpHelper.h"
#include "ErrLogHelper.h"
#include "StringHelper.h"


int main(int argc, char* argv[] )
{
	if ( argc != 2 && argc != 4)
	{
		cerr << "usage: " << argv[0] << "[-e yes/no] PkgLogFileName.log\n";
		cerr << "-e �Ƿ����(true/no)\n";
		return EXIT_FAILURE;
	}

	bool IsEncrypt = true;	// ���ļ��Ƿ����
	if ( argc == 4 )
	{
		if ( !strcmp(argv[2], "no") )
		{
			IsEncrypt = false;
		}
	}

	string szPkgLog = argv[argc-1];

	ifstream pkglog_file( szPkgLog.c_str() );
	if ( !pkglog_file.good() )
	{
		cout << "���ļ�" << szPkgLog << "ʧ��" << endl;
	}

	// ȡ�ô��·��
	
	string szTargetPath = szPkgLog;
	sqr::backslash_to_slash(szTargetPath);
	szTargetPath = szTargetPath.substr( 0, szTargetPath.rfind('/') );

	// �Ӵ��log��ȡ�����д���ļ��İ�����Դ�ļ�·��
	
	string szFile;				// szFile ���ڴ洢log�ļ��е���
	string szPkgName;
	string szPkgFile;
	string szSpacer = "###";
	map<string, vector<string> > mapPkgFile;

	while ( std::getline(pkglog_file, szFile) )
	{
		int32  nSpacerPos = szFile.find(szSpacer);
		
		szPkgName = szFile.substr( 0, nSpacerPos );
		szPkgFile = szFile.substr( nSpacerPos + szSpacer.size(), szFile.size() );
		mapPkgFile[szPkgName].push_back( szPkgFile );
	}

	// ��ʼ���
	_chdir( szTargetPath.c_str() );
	map<string, vector<string> >::iterator it = mapPkgFile.begin();

	// ��ʼ��ʱ
	//LARGE_INTEGER nFreq;
	//QueryPerformanceFrequency(&nFreq);
	//LARGE_INTEGER   nBeginTime;
	//LARGE_INTEGER   nEndTime;
	//double useTime = 0.f;
	//double lastTime =  0.f;
	//QueryPerformanceCounter(&nBeginTime);
	//QueryPerformanceCounter(&nEndTime);
	//lastTime = static_cast<double>(nEndTime.QuadPart - nBeginTime.QuadPart) / nFreq.QuadPart;

	for ( ; it != mapPkgFile.end(); ++it )
	{
		string			szPkgName   = it->first;
		vector<string> &vecFileName = it->second;
		vector<string>::iterator file_iter = vecFileName.begin();
		
		CFilePack Pack( szPkgName .c_str(), IsEncrypt );
		for ( ; file_iter!=vecFileName.end(); ++file_iter )
		{
			szPkgFile = *file_iter;
			
			Pack.AddFile( szPkgFile.c_str() );
		}
		Pack.MakePackage();

		//QueryPerformanceCounter(&nEndTime);
		//useTime = static_cast<double>(nEndTime.QuadPart - nBeginTime.QuadPart) / nFreq.QuadPart;
		//printf("%s use time: %f\n", szPkgName.c_str(), useTime);
	}

	cout << "make package success";

	return EXIT_SUCCESS;
};

