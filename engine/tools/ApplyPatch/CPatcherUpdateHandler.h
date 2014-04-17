
#include "IAsyncUpdateHandler.h"



class CPatcherUpdateHandler
	: public sqr::IAsyncUpdateHandler
{
public:
	CPatcherUpdateHandler();
	~CPatcherUpdateHandler();

	// ʵ�ֳ������麯��
	virtual void SetDownState(int percent);
	virtual void SetSetupState(int percent);
	virtual void ShowMessage(const wchar_t* Msg);
	virtual void CallBackFinishPatch(const wchar_t* szCmd);
	virtual void AlertExit(const wchar_t* Msg);
	virtual void PatchContinue();
	virtual void UpdateVersion();
	virtual void VerifyFilesFinish(int);
};

