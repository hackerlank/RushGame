#pragma once
#include "Resource.h"
#include "PlaceElement.h"


// CColonyEditDlg dialog

class CColonyEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CColonyEditDlg)

public:
	typedef vector<PPlaceNpc>	Npc_Vec;

	CColonyEditDlg(int iColonyId, CWnd* pParent = NULL);   // standard constructor
	virtual ~CColonyEditDlg();

// Dialog Data
	enum { IDD = IDD_COLONY_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void	SetDummyNpc(PPlaceNpc pNpc);
	void	AddNpc(PPlaceNpc pNpc);

	void ClearNewNpc();
	void RemoveNewNpc(int nIndex);

	bool GetAutoPos(OUT FPos& fPos );//���һ���Զ������ ����npc ������λ��, ֻ���� m_bIsNewColony == false ��ʱ�����
private:
	int			m_iColonyId;
	CListBox m_listboxMember;
	Npc_Vec		m_vecNewNpc;

	bool		m_bIsNewColony;
public:
	afx_msg void OnBnClickedSetBoss();
	afx_msg void OnBnClickedAddNpc();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedCopyAndPut();
	afx_msg void OnClose();
	afx_msg void OnLbnDblclkListMember();
};
