#pragma once


namespace sqr
{
class IUpdateObjectBTHandler;

class IBTDownloaderHandler
{
public:
	// �������ؽ���
	virtual void UpdateDownState()=0;
	// ���°�װ����
	virtual void UpdateSetupState()=0;
};
};
