#include "clock.h"
using namespace sqr;
// ��ȡ��ʼʱ��
void StartTimer(uint64 *TimeMarker)
{
	*TimeMarker = GetHDProcessTime();
}

// ���ؿ�ʼ��������ʱ���
static uint64 GetClocks(uint64 TimeMarker)
{
	return GetHDProcessTime() - TimeMarker;
}

// clock_t��һ��������������time.h�ļ��У���������һ������CLOCKS_PER_SEC����������ʾһ���ӻ��ж��ٸ�ʱ�Ӽ�ʱ��Ԫ
double GetSeconds(uint64 TimeMarker)
{
	uint64 clocks = GetClocks(TimeMarker);
	return (double)(double(clocks) / 1000000.0);
}


