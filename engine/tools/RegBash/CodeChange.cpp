
#include "stdafx.h"
#include "CodeChange.h"

// ��Ӣ����ĸ�ĵ��ֽڸ�ʽת��˫�ֽڸ�ʽ
int ANSItoUnicode(char str[], char NewStr[])
{
	int i;
	int flag = 1;
	int len = (int)strlen(str);
	int NewLen = (len+1)*2;

	for (i=0; i<NewLen; i++)
	{
		if (flag==1)
		{
			NewStr[i] = str[i/2];
			flag = 0;
		}
		else
		{
			NewStr[i] = '\0';
			flag = 1;
		}
	}

	return NewLen;
}


// ��Ӣ����ĸ��˫�ֽڸ�ʽת�ɵ��ֽڸ�ʽ
void UnicodetoANSI(char str[], char NewStr[], int len)
{
	int i;
	int flag = 1;
	int NewLen = len/2;
	for (i=0; i<NewLen; i++)
	{
		NewStr[i] = str[i*2];
	}
}
