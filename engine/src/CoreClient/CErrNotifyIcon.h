#pragma once

namespace sqr
{
	template<typename HolderType, void(HolderType::*pFun)() >
	class TTickFunctionHandler;

	/*
		������������ڲ������õģ���������Ϊtesting client֮��
		�����������½����һ���������е�ͼ��
		Ȼ���д�������ʱ�����ͼ��ͻ��ɴ��󾯸�״̬������ʮ��
		�ڻظ�ԭ״̬�����û������������ͼ������ʱ��
		����ʾ�ϴδ�������ʱ��
	*/
	class CErrNotifyIcon
	{
	public:
		static void Init(HWND hWnd, HINSTANCE hInstance);
		static void Unit();
		static CErrNotifyIcon*& Inst();
		
		void ShowErrIcon(const char* szMsg);
		void ShowAppIcon();

	private:
		CErrNotifyIcon(HWND hWnd, HINSTANCE hInstance);
		~CErrNotifyIcon();
		
		void AddNotifyIcon();
		void DelNotifyIcon();

		void ModifyNotifyIcon();

	private:
		NOTIFYICONDATA*	m_pData;
		HWND			m_hWnd;
		HINSTANCE		m_hInstance;

		HICON			m_hErrIcon;  //���ִ���ʱ���icon
		HICON			m_hAppIcon;  //��Ϸ��������ʱ���icon
		
		typedef TTickFunctionHandler<CErrNotifyIcon,&CErrNotifyIcon::ShowAppIcon> TickFuncHandler_t;
		TickFuncHandler_t* m_pTickShowErrIcon;

	};

}