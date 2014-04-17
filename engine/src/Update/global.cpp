#include "stdafx.h"
#include <sstream>
#include "global.h"
#include <sys/stat.h>

#define INI_FILE_NAME  L"../../etc/gac/Update.ini"


NAMESPACE_BEGIN(sqr)

void CreatePatchList(PatchList& pl, const string& buf)
{
	int ret = 0;
	// ��ȡ�����İ汾
	FILE *fp = _wfopen(INI_FILE_NAME, L"rb");
	fseek(fp, 0, SEEK_END);
	int iFileLen = ftell(fp);
	string sCusContent;
	sCusContent.resize(iFileLen);
	fseek(fp, 0, SEEK_SET);
	fread(&sCusContent[0], iFileLen, 1, fp);
	fclose(fp);

	int OldHi, OldLo, OldBuild;
	string sVerion(sCusContent.substr(sCusContent.find_first_of("=")+1, iFileLen));
	ret = sscanf(sVerion.c_str(), "%d.%d.%d", &OldHi, &OldLo, &OldBuild);
	//if (ret != 3)
	//	::MessageBox(NULL, "���ذ汾�Ÿ�ʽ����ȷ������Update.ini�����ļ�", "����", MB_ICONSTOP|MB_OK);

	// ѡ�����ڵıȱ����汾�µİ汾����
	string sContent(buf);
	string sTorrentUrl;
	string sVer;
	int hi, lo, build;
	int start = 0;
	int end = 0;
	int count = 0;
	bool bInsert = true;
	bool bSingle = true; // true�����������У���ʾ�汾�ţ�ż��������������ӵ�ַ
	UpdatePacket packet;

	unsigned int i = 0;
	for (i=0; i<sContent.size(); ++i)
	{
		if (sContent.at(i) == '\n')
		{
			bInsert = false;
			end = count;
			if (bSingle) // �汾��
			{
				sVer = sContent.substr(start, end-start);
				ret = sscanf(sVer.c_str(), "%d.%d.%d", &hi, &lo, &build);

				if (ret == 3 && build > OldBuild)
				{
					if (hi != OldHi || lo != OldLo)
					{
						//::MessageBox(NULL, "��Ϸ�ѳ��µ�����Ƭ�������www.linekong.com��ȡ������Ϣ", "����", MB_ICONSTOP|MB_OK);
						break;
					}
					else
					{
						packet.ver.byHiVersion   = hi;
						packet.ver.byLoVersion   = lo;
						packet.ver.wBuildVersion = build;
					}
				}
			}
			else // ���ӵ�ַ
			{
				sTorrentUrl = sContent.substr(start, end-start); // �ô����ܻᵼ�´�����Ϊ��û����\r

				if (ret == 3 && build > OldBuild)
				{
					packet.url = sTorrentUrl;
					pl.push_back(packet);
				}
			}
			start = count + 1;

			if (bSingle)
				bSingle = false;
			else
				bSingle = true;
		}

		++count;
	}
}

NAMESPACE_END
