#pragma once



// CParamsView ������ͼ

class CParamsView : public CFormView
{
public:
	CParamsView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CParamsView();

public:
	enum { IDD = IDD_PARAMS };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void PostNcDestroy() { }
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float m_fCameraPitch;
	float m_fCameraFOV;

	void SetPitch(float f)
	{
		m_fCameraPitch = f;
	}
	
	void SetFOV(float f)
	{
		m_fCameraFOV = f;
	}

	afx_msg void OnEnChangeEditPitch();
	afx_msg void OnEnChangeEditfov();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	float m_fCameraX;
	float m_fCameraY;
	float m_fCameraZ;
};