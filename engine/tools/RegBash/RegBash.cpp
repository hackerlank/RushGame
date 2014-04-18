
#include "stdafx.h"

#include "RegBash.h"
#include "CodeChange.h"


const int BufLen = 500;

void RegBash(void)
{
	char chbuf[BufLen]; //��ȡ����
	char NewStr[BufLen]; //ת���뻺��
	char RootPath[BufLen]; //cygwin��װ·��
	char BinPath[BufLen]; //cygwin��װ��binĿ¼
	char IcoPath[BufLen]; //cygwin��ͼ��λ��
	int NewLen = 0; //ת�����ľ��峤��
	HKEY hkey;
	DWORD type = 0;
	DWORD len = BufLen; //ÿ�ζ�����ʱlen��Ҫ����ΪBufLenһ��
	memset(chbuf, '\0', BufLen);

	// ��ȡcygwin��װ·��
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2\\/"), 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		len = BufLen;
		if (RegQueryValueEx(hkey, _T("native"), NULL, &type, (LPBYTE)chbuf, &len))
		{
			printf("have error!");
		}

		memset(RootPath, '\0', BufLen);
		UnicodetoANSI(chbuf, RootPath, len);
		strncpy_s(BinPath ,RootPath, BufLen);
		strncpy_s(IcoPath ,RootPath, BufLen);
		strcat_s(IcoPath, "\\Cygwin.ico,0");
		RegCloseKey(hkey);
	}


	// д��ע�����Ϣ
	DWORD Disposition;
	len = BufLen;
	// ������
	try
	{
		RegCreateKeyEx(HKEY_CLASSES_ROOT, _T(".sh"), NULL, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hkey, &Disposition);
		RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("ybtx_bash\\DefaultIcon"), NULL, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hkey, &Disposition);
		RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("ybtx_bash\\shell"), NULL, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hkey, &Disposition);
		RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("ybtx_bash\\shell\\open"), NULL, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hkey, &Disposition);
		RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("ybtx_bash\\shell\\open\\command"), NULL, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hkey, &Disposition);
	}
	catch(...)
	{
		printf("Create new key error!");
	}

	memset(NewStr, '\0', BufLen);
	NewLen = ANSItoUnicode("ybtx_bash", NewStr);
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, _T(".sh"), 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		RegSetValueEx(hkey, NULL, NULL, REG_SZ, (LPBYTE)NewStr, NewLen);
		RegCloseKey(hkey);
	}

	memset(NewStr, '\0', BufLen);
	NewLen = ANSItoUnicode(IcoPath, NewStr);
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("ybtx_bash\\DefaultIcon"), 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		RegSetValueEx(hkey, NULL, NULL, REG_SZ, (LPBYTE)NewStr, NewLen);
		RegCloseKey(hkey);
	}

	memset(NewStr, '\0', BufLen);
	NewLen = ANSItoUnicode("open", NewStr);
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("ybtx_bash\\shell"), 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		RegSetValueEx(hkey, NULL, NULL, REG_SZ, (LPBYTE)NewStr, NewLen);
		RegCloseKey(hkey);
	}

	memset(NewStr, '\0', BufLen);
	char* command = "\\bin\\bash.exe --login -c \"f='%1';f=${f//\\\\//};cd \\\"$(dirname \\\"$f\\\")\\\";bash \\\"$(basename \\\"$f\\\")\\\";unset f\"";
	strcat_s(BinPath, command);
	NewLen = ANSItoUnicode(BinPath, NewStr);
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("ybtx_bash\\shell\\open\\command"), 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
    {
		RegSetValueEx(hkey, NULL, NULL, REG_SZ, (LPBYTE)NewStr, NewLen);
		RegCloseKey(hkey);
    }

	printf("�ļ�������ϣ�");
}

