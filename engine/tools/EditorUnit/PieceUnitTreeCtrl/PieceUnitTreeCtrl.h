#pragma once
#include "CEditorCore.h"

namespace sqr
{
	// CPieceUnitTreeCtrl
	class CPieceUnitTreeCtrl : public CTreeCtrl
	{
		DECLARE_DYNAMIC(CPieceUnitTreeCtrl)

		CImageList m_Image;

	public:
		CEdit m_zBais;
		int m_nCount;
		list<HTREEITEM> m_Node;//������ͬ��ʽRS���ֵĲ�ͬpiece
		CString m_NodeText;
		CString m_CheckNodeInfo;
	public:
		int		AddPieceUnit( CEditPiece* Piece );
		int		AddPiece( HTREEITEM hPieceItem, CEditPiece* Piece );
		int		AddPieceRS( HTREEITEM hRSItem, CEditPiece* Piece );
		int		AddPieceVerAni( HTREEITEM hVAItem, CEditPiece* Piece );
		int		DeletePieceUnit( CEditPiece* Piece );
		int		CreateRS( CString FileName = "" );
		void	CreateLevel( float Dist, float FacePercent, float DeleteMaxLen, int MinTris );
		void	AddVerAni( CString VerAniName, CEditPieceGroup* PieceGroup );
		bool	CheckPieceGroup();
		void    FindNodeName(HTREEITEM hItem);

		void	TreeNodeVisit(HTREEITEM hItem);//��������ÿ���ڵ�

		//CSoftLevel* GetSelectSoftLevel();
		CMesh*		GetSelectMesh();
		//CSoftClothLevel* GetSelectSoftClothLevel();

	protected:
		DECLARE_MESSAGE_MAP()
		virtual void PreSubclassWindow();
	public:
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);

		afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnTimer(UINT nIDEvent);
	protected:
		UINT          m_TimerTicks;      //��������Ķ�ʱ����������ʱ��
		UINT          m_nScrollTimerID;  //��������Ķ�ʱ��
		CPoint        m_HoverPoint;      //���λ��
		UINT          m_nHoverTimerID;   //������ж�ʱ��
		DWORD         m_dwDragStart;     //������������һ�̵�ʱ��
		BOOL          m_bDragging;       //��ʶ�Ƿ������϶�������
		CImageList*   m_pDragImage;      //�϶�ʱ��ʾ��ͼ���б�
		HTREEITEM     m_hItemDragS;      //���϶��ı�ǩ
		HTREEITEM     m_hItemDragD;      //�����϶��ı�ǩ
		int			  m_nImage;
	private:
		HTREEITEM CopyBranch(HTREEITEM htiBranch,HTREEITEM htiNewParent,HTREEITEM htiAfter);
		HTREEITEM CopyItem(HTREEITEM hItem,HTREEITEM htiNewParent,HTREEITEM htiAfter);
	};
}