// CatLog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CatHelper.h"
#include "CMapFileMgr.h"
#include "CFetchMapFile.h"
#include "ErrLogHelper.h"
#include "bzip2/bzlib.h"

#include <sstream>


int main(int argc, char* argv[])
{
	//���ȿ��Ƿ���helpѡ�����У�ֱ�Ӵ�ӡhelp��Ϣ���˳�
	for(int i = 1; i < argc; i++)
	{
		char* szCmd = argv[i];
		if(!strcmp(szCmd, "-help") || !strcmp(szCmd, "-h"))
		{
			PrintUsage();
			return -1;
		}
	}

	if(argc != 2 && argc != 4  && argc != 6) 
	{
		PrintMessage("Error ������������ȷ");
		return -1;
	}

	bool bHasMapFile = false;
	const char* szOutFile = NULL;

	if(argc != 2)
	{
		bool bPath = false;
		bool bOut = false;
		for(int i = 2; i < argc;)
		{
			char* szCmd = argv[i];
			if(strcmp(szCmd, "-path") == 0 || strcmp(szCmd, "-p") == 0)
			{
				if(bPath)
				{
					PrintMessage("Error ���ܶ������-path/-pѡ��");
					return -1;
				}
				bPath = true;

				if(i + 1 < argc)
				{
					const char* szMapFilePath = argv[i + 1];

					if(IsDir(szMapFilePath))
					{	
						bHasMapFile = true;
						CFetchMapFile::Inst().SetPath(szMapFilePath);
					}
					else
					{
						PrintMessage("Error MapFileָ���Ĳ���·��");
						return -1;
					}
				}
				else
				{
					PrintMessage("Error �����������");
					return -1;
				}

				i += 2;
				continue;
			}
			if(strcmp(szCmd, "-o") == 0)
			{
				if(bOut)
				{
					PrintMessage("Error ���ܶ������-oѡ��");
					return -1;
				}
				bOut = true;

				if(i + 1 < argc)
				{
					szOutFile = argv[ i + 1];
				}
				else
				{
					PrintMessage("Error �����������");
					return -1;				
				}

				i += 2;
				continue;
			}

			
			i++;
		}

		
	}

	if(!bHasMapFile)
	{
		CFetchMapFile::Inst().SetPath("");
	}

	//����log��ѹ�����ѹ������
	SetCompressFun(&BZ2_bzBuffToBuffCompress);
	SetDeCompressFun(&BZ2_bzBuffToBuffDecompress);

	bool bFile = IsFile(argv[1]);
	bool bDir = IsDir(argv[1]);

	if(bDir)
	{
		if(!szOutFile || !IsDir(szOutFile))
		{
			PrintMessage("Error ����ָ�����·��");
			return -1;
		}

		HandleDir(argv[1], szOutFile);
	}
	else if(bFile)
	{
		HandleFile(argv[1], szOutFile);
	}

	
	
	return 0;
}

