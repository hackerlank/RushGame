#pragma once
#include "TSingleton.h"

namespace sqr
{
	///operator�г���ı���
	class COperatorPropertyCtrl
	{
	public:

		HWND hMainFrame;
		HWND hParamsView;
		HWND hParamsExtraView;
		HWND hResSetView;
		HWND hFXSetView;
		HWND hTextureView;
		HWND hMeshView;
		HWND hVertexView;
		HWND hLoginSetView;
		HWND hRecordView;
		HWND hAudioView;
		HWND hRegionView;
		HWND hTileRegionView;

		bool bEnableRecordCtrl;		// ¼���ݼ������ּ����Ƿ���Ч
		bool bSnapMode;				// �Ƿ���ʾ�����

	public:
		COperatorPropertyCtrl()
		{
			bEnableRecordCtrl = true;
			bSnapMode		  = false;
		}

		~COperatorPropertyCtrl()
		{

		}
	};
}