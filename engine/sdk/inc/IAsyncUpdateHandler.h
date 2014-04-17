#pragma once


#define  UPDATED_FLAG   L"updated"
#define  UPDATING_FLAG  L"updating"


namespace sqr
{

	class IAsyncUpdateHandler
	{
	public:
		virtual void SetDownState(int percent) = 0;     // ��������progress�ؼ�����
		virtual void SetSetupState(int percent) = 0;    // ���ð�װprogress�ؼ�����
		virtual void ShowMessage(const wchar_t* Msg) = 0; // ��ӡ��ǰ��Ϣ
		virtual void CallBackFinishPatch(const wchar_t* szCmd) = 0; // ������ɻص�����
		virtual void AlertExit(const wchar_t* Msg)=0;     // ���沢�˳�
		virtual void PatchContinue()=0;     // ������ֱ�Ӽ�����Patch
		virtual void UpdateVersion()=0;                 // ���µ�ǰ�汾��
		virtual void VerifyFilesFinish(int bVerifySuccess)=0;                 // У��ͻ����ļ������

	};

}

