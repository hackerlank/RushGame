#include "stdafx.h"
#include "Win32.h"
#include "DirectX9.h"
#include "ConfigFileParser.h"
#include "ImageSplitter.h"
#include "DefaultIndexGenerator.h"

// ---- ʵ�ֲ��� ------------------------------------------------------------------------------

	bool	ProcessFile(CConfigFileParser::Node &node)
	{
		// ����������Ϣ�ļ�
		string szRgnInfo = node.szSrcPath;
		uint32 uWidth	 = 0;
		uint32 uHeight	 = 0;

		locale loc = locale::global(locale(""));
		ifstream file(szRgnInfo.c_str());
		locale::global(loc);

		if ( !file.good() )
		{
			cout << "������Ϣ�ļ� " << szRgnInfo << " ������\n";
			return false;
		}

		// ��ȡ������Ϣ(�����ȡ��ֵҪ����4)
		file.read((char*)&uWidth, sizeof(uint32));
		file.read((char*)&uHeight, sizeof(uint32));
		file.close();
		if ( uWidth<=0 || uHeight<=0 )
			return false;
		uWidth	>>= 2;
		uHeight >>= 2;

		// ���ԭͼƬ�ļ��Ƿ����
		string szOldPicPath = "./source/" + node.szPicName;
		loc = locale::global(locale(""));
		file.open(szOldPicPath.c_str());
		locale::global(loc);
		if ( !file.good() )
		{
			cout << "ԭͼƬ " << node.szPicName << " ������\n";
			return false;
		}
		
		CImageSplitter	cutter(new CDefaultIndexGenerator);
		cutter.SetRgnInfo(uWidth, uHeight);
		cutter.SetSourceFile(szOldPicPath);
		cutter.SetTargetPath(node.szTgtPath);
		cutter.Run();
		
		return true;
	}

	bool	ProcessAllFiles()
	{
		CConfigFileParser::Node node;
		while ( CConfigFileParser::GetInst()->GetNextNode(node) )
		{
			if ( !ProcessFile(node) )
			{
				return false;
			}
		}

		return true;
	}

	int _tmain(int argc, _TCHAR* argv[])
	{
		if ( !CreateMainWindow(0, 0, 256, 256) )
		{
			return ERR_CREATE_WINDOW_FAILED;
		}

		if ( !InitGraphicsDeviceDX9() )
		{
			return ERR_INIT_DIRECT3D9_FAILED;
		}

		bool bRet = false;
		while ( true )
		{
			CConfigFileParser::GetInst()->Reset();
			bRet = CConfigFileParser::GetInst()->SetConfigFilePath("./Config.txt");
			if ( !bRet )
			{
				cout << "�Ƿ����?(Y/N)" << endl;
				string szInput;
				cin >> szInput;
				if ( szInput == "Y" || szInput == "y" )
				{
					continue;
				}
				else
				{
					break;
				}
			}

			bRet = ProcessAllFiles();
			if ( !bRet )
			{
				cout << "�Ƿ����?(Y/N)" << endl;
				string szInput;
				cin >> szInput;
				if ( szInput == "Y" || szInput == "y" )
				{
					continue;
				}
				else
				{
					break;
				}
			}

			cout << "ת���ɹ�" << endl;
			system("pause");
			return 0;
		}
		
		return ERR_PROCESS_FAILED;
	}

