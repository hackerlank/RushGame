#include "stdAfx.h"
#include "CFindWindow.h"

CFindWindow::CFindWindow(void)
:m_hWnd(NULL), m_classname(NULL)
{
}

CFindWindow::~CFindWindow(void)
{
}

void CFindWindow::FindWin(HWND hwndParent, LPCSTR classname)
{
	m_hWnd=NULL;
	m_classname=classname;
	FindChildClassHwnd(hwndParent,LPARAM(this));
}

BOOL CALLBACK CFindWindow::FindChildClassHwnd(HWND hwndParent,LPARAM lParam)
{
    CFindWindow *pfw = (CFindWindow*)lParam;
    HWND hwnd = FindWindowEx(hwndParent, NULL, pfw->m_classname, NULL);
    if (hwnd) {
        pfw->m_hWnd = hwnd;
        return FALSE;       //ֹͣö�� ������  
    }
    EnumChildWindows(hwndParent, FindChildClassHwnd, lParam);  //ö�������Ӵ���
	return TRUE;               
};