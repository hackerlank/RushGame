// CrcCheck.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CrcCheck.h"

using namespace sqr;

int _tmain(int argc, _TCHAR* argv[])
{
	CCrcChecking *pInst = new CCrcChecking;
	bool bCheckResult = pInst->Check();
	if (bCheckResult)
	{
		cout << "У������û�д���" << endl;
	}
	cout << "�밴���������";
	getchar();
	return 0;
}

