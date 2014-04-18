#pragma once
#include "CBufFile.h"
#include "atlimage.h"
#include "PlaceElement.h"


enum EUpdateElementState 
{
	eUES_NONE = 0,
	eUES_NPC = 1,
	eUES_OBJ = 1 << 1,
	eUES_TRAP = 1 << 2,
	//�ڴ������ö��,����ʽͬ��, �Ϳ��Բ��޸��κδ���,��֤ԭ���߼�
	eUES_PATH_KEY_POS = 1 << 3,


	eUES_SINGLE_END,
	eUES_ALL = ((eUES_SINGLE_END - 1) << 1) - 1
};




enum EShowState  //��ʾ״̬�Ƚ϶�,�Ժ���ܳ���32,���ԾͲ��� EUpdateElementState �Ĺ���ʽ��
{
	eSS_NPC,
	eSS_OBJ,
	eSS_TRAP,
	eSS_PATH,
	eSS_ARROW,
	eSS_GRID_LINE,
	eSS_SELECT_RECT,//����ѡʱ��ľ���
	eSS_SELECT_MARK,
	eSS_NPC_FULL,//NPC��ȫ, ��ĳ��NPCΪ����,��ѡ״̬, �Ƿ�ȫ������ʾ��Ϣ,(eSS_PATH,eSS_ARROW)
	eSS_GAME_VIEW,//��Ϸ�е��Ӵ�
	eSS_GAME_VIEW_FOLLOW,
	eSS_AREA,
	eSS_BLOCK,
	eSS_IGNORE_BLOCK,

	eSS_COUNT
};

class CMapView : public CStatic
{
	DECLARE_DYNAMIC(CMapView)

public:
	typedef vector<PPlaceNpc>	Npc_Vec;
	typedef vector<PPlaceObj>	Obj_Vec;
	typedef vector<PPlaceTrap>	Trap_Vec;
	typedef vector<CElement*>   Element_Vec;
	typedef vector<FPos>			FPos_Vec;

	typedef Npc_Vec::iterator	Npc_Iter;
	typedef Obj_Vec::iterator	Obj_Iter;
	typedef Trap_Vec::iterator	Trap_Iter;
	typedef Element_Vec::iterator Element_Iter;
	CMapView();
	virtual ~CMapView();
	
	void ClearAllImage();
	bool LoadMap(const CString& strPath, const CString& mapFilePath, OUT CString& errorMessage);
	bool LoadBarrierData(const CString& strPathName);
	bool LoadSceneInfo(const CString& strPathName);
	bool LoadRegionInfo();
	void LoadOneRegion(CBufFile& File, uint32 nChunk,uint32 RegionX, uint32 y);
	void LoadOneChunk(CBufFile& File, uint32 x, uint32 y, uint32 uMagicID);

	void Init();
	void RePaint();
	void DrawArea(CDC* pDC);
	void DrawBlock(CDC* pDC);
	void DrawGridLine(CDC* pDC);
	void DrawMap(CDC* pDC);
	void DrawNpc(CDC* pDC);
	void DrawAllDirection(CDC* pDC);
	void DrawNpcDirection(CDC* pDC);
	void DrawObjDirection(CDC* pDC);
	void DrawTrapDirection(CDC* pDC);
	void DrawPasteElementDirection(CDC* pDC);
	void DrawObj(CDC* pDC);
	void DrawTrap(CDC* pDC);
	void DrawAllPath(CDC* pDC);
	void DrawSingleNpcPath(CDC* pDC, PPlaceNpc pNpc);
	void DrawDirection(CDC* pDC,PPlaceNpc pNpc);
	void DrawSelectRect(CDC* pDC);
	void DrawSelectMark();
	void DrawFocus(CDC* pDC);
	void DrawSelectElement(CDC* pDC);
	void DrawGameView(CDC* pDC);
	void DrawPenRect(CDC* pDC);
	void DrawPasteElement(CDC* pDC);
	void DrawRotateRound(CDC* pDC);
	void DrawMeasure(CDC* pDC);

	
	inline void GetGridRectOfView(OUT FRect& gridFRect) const;//��ȡ��ǰ��ͼ�ľ��������Ӧ�ĳ�����������,
	bool GetCurCursorViewPos(OUT Pos& pos) const; //��ȡ��ǰ����λ��,����Ӵ�������. �����Ƿ����Ӵ���
	int CurCursorX()const;//��ȡ��ǰ����λ��,����Ӵ���X����.
	int CurCursorY()const;//��ȡ��ǰ����λ��,����Ӵ���Y����.
	bool GetCurCursorGridPos(OUT FPos& pos) const;//������ĵ�ͼ�ϵĸ���λ��  (��ͼ���½�Ϊ (0,0)���� ) �����Ƿ����Ӵ��ڵĵ�ͼ��,
	inline void GridPosToViewPos(OUT Pos& viewPos, const FPos& gridFPos) const;//�����ĸ�������ת��Ϊ��ͼ������
	inline void ViewPosToGridPos(OUT FPos& gridFPos, const Pos& viewPos) const;//��ͼ������ת��Ϊ�����ĸ�������
	inline void GridRectToViewRect(OUT Rect& viewRect, const FRect& gridFRect) const;
	inline void ViewRectToGridRect(OUT FRect& gridFRect, const Rect& viewRect) const;
	inline float GetViewDistance(float gridDis) const;//���Ӿ���ת�����Ӵ�����
	inline float GetGridDistance(float viewDis) const;//��ͼ����ת���ɸ��Ӿ��� 
	//inline double GetRadianDir(int dir) const;//��Ϸ�еķ���ת�ɻ���
	
	bool IsPosNearLine(OUT float& dis ,const FPos& fPosLineStart, const FPos& fPosLineEnd, const FPos& fPosTest, float fRange) const;//�жϵ��Ƿ����߸���
	bool IsCursorInView();
	
	int GetMapGridWidth(){return m_nMapGridX;};
	int GetMapGridHeight(){return m_nMapGridY;};

	void MoveMap(int xOffset, int yOffset);//���֮ǰƫ��
	void SetMapPos(int posX, int posY);//ֱ������ƫ��, ����б߽紦��
	void MoveMapToCenter(float fGridX, float fGridy);
	void MoveMapToCenter(const FPos& fPos);
	void MoveMapToSelectCenter();
	bool IsMapOk();
	//bool IsMouseInView();
	void ReSetView();
	void ReSetShowState();
	bool SetScaling(float fScaling);
	void UpdateAll();
	void UpdateObj();
	void UpdateNpc();
	void UpdateTrap();
	void SetUpdateMark(EUpdateElementState eState);//���ø���״̬
	void RemoveUpdateMark(EUpdateElementState eState);//�Ƴ�����״̬
	bool IsUpdate(EUpdateElementState eState);//����״̬���
	
	void SetMyFocus(CElement* pElement);
	bool IsSelectElement(CElement* pElement);
	bool AddSelectElement(CElement* pElement);
	void SelectColony(int iColonyId);
	int RemoveSelectElement(int eType);
	bool RemoveSelectElement(CElement* pElement);
	void ClearSelectElement();
	bool GetVecElementCenter(OUT FPos& fPosCenter, const Element_Vec& srcVec, int eType);
	bool GetVecElementRect(OUT FRect& fRect, const Element_Vec& srcVec, int eType);

	bool UserCopyElement(Element_Vec* pVecSrc = NULL);
	bool CheckUserCopy(Element_Vec& vecSrc, OUT CString& msg);
	void CutElement();
	void ClearCopy();
	void PasteElement();
	void ClearRotateBasic();

	void CopeyVecElement(OUT Element_Vec& destVec, const Element_Vec& srcVec, int type);
	
	CElement* PutElement(CElement* pElement, const FPos& gridPos);
	CElement* PutNpc(PPlaceNpc pNpc, const FPos& gridPos);
	CElement* PutObj(PPlaceObj pObj, const FPos& gridPos);
	CElement* PutTrap(PPlaceTrap pTrap, const FPos& gridPos);
	bool PutNpcCheck(PPlaceNpc pNpc, const FPos& gridPos, OUT CString* msg = NULL);
	bool PutObjCheck(PPlaceObj pObj, const FPos& gridPos, OUT CString* msg = NULL);
	bool PutTrapCheck(PPlaceTrap pTrap, const FPos& gridPos, OUT CString* msg = NULL);
	void PutPasteElement();


	CElement* FindNearestElement(const FPos& fPos, float fRange);
	int GetShowElementMark(int rangeMark = eET_ALL) const;
	int FindElement(OUT Element_Vec& vec, const FRect& fRect, int mark)const;//Element ����Npc,obj,trap,pathKeyPos
	CArrow* FindArrow(const FPos& fPos) const;

	CPathLine* FindPathLine(const FPos& fPos) const;

	void UpdateFocus();
	void OnMoveElement();
	void MoveElement(CElement* pElement, const FPos& fPos);
	void RotateElement();
	void BeginRotateElement();
	bool IsCursorOnRotateRound();
	void UpdateRotateElement();

	bool CheckInsertPathKeyPos(PPathKeyPos pPathKeyPos, const FPos& fPos);
	bool CheckInsertPathKeyPos(PPlaceNpc pNpc, const FPos& fPos);
	bool CheckInsertPathKeyPos(CPathLine* pPathLine, const FPos& fPos);
	bool CheckMovePathKeyPos(PPathKeyPos pPathKeyPos, const FPos& fPos);
	bool CheckMoveNpc(PPlaceNpc pNpc, const FPos& fPos);

	void MoveAllSelectElement(const FPos& offset);
	void MoveAllSelectElementToGridMiddle();
	void DeleteElement(CElement* pElement);
	bool DeleteElemetVec(Element_Vec& vecElement);	//���е�ɾ����������
	bool DeleteAllSelectElement();

	void EditElement(CElement* pElement);


	CElement* GetMyFocus(){return m_pFocusElement;}
	
	bool CheckPos(const FPos& fPos, int mark, CElement* pExclude = NULL);//���õ��Ƿ��ص�
	
	void SetSelectStart();
	void SetSelecting();
	
	bool IsShowSate(EShowState eShowState){return m_bShowStates[eShowState];}
	void SetShowState(EShowState eShowState, bool isShow = true);
	
	void SetAreaImageAlpha(int alpha);
	void SetAreaImageColor(float centerX, float centerY, int size,int color);//color Ϊ-1 �������
	void FillArea(int x, int y, int color);
	void ClearArea();
	void ReplaceColor(int oldColor, int newColor);
	void DelColor(int color);
	void SetContourColor(int gridX, int gridY, int color);
	void DrawAreaLine(int gridX, int gridY1, int gridY2, int color);
	int GetAreaColor(int gridX, int gridY);
	bool IsAreaEdge(int ix, int iy);//��� ix,iy ���Ƿ������Ե

	void ComputeContour();//��������
	void FillContour();//�������
	bool IsInsidePos(int x, int y);
	bool IsContourPos(int gridX, int gridY);
	void ClearContour(){ memset(m_pAreaContour, false, sizeof(bool) * m_nMapGridX * m_nMapGridY);}
	
	void AddBarrierObj(int x, int y);
	void AddBarrierObj(const Pos& gridFPos);
	bool IsBarrierObj(int gridX, int gridY);

	void SetBlockImageColor(int gridX, int gridY, int color);
	bool IsBlock(int gridX, int gridY);
	bool IsBlock(const FPos& gridFPos);
	bool Line(const FPos& srcPos, const FPos& destPos);
	
	bool IsPosDraw(int gridX, int gridY);
	void EmptyFillRecord();

	void PauseRender();
	
	void Measure();
	void AddMeasurePos(const FPos& fPos);
	void ClearMeasurePos();
	bool IsOverArea;
protected:
	DECLARE_MESSAGE_MAP()
private:
	CImage		m_ImageMap;//��ŵ�ͼͼƬ
	CImage		m_ImageBuffer;//����ͼƬ.�Ȱ����Զ���������,Ȼ��һ���Ի����ؼ���
	CImage		m_ImageArea;//��͸��������Ƶ�����, 
	CImage		m_ImageBlock;//�ϰ���͸��ͼƬ

	bool*		m_pAreaContour;//��������, ͨ��ComputeContour ����������������������

	byte		m_AlphaValue;
	COLORREF	m_curAreaColor;
	
	float		m_fScaling;//��ͼ���ű���
	
	int			m_iCurCursorPosX;//��ǰ�������ͼ�е�X����, ���ǻ�ȡ��ǰ���λ�ú������������ǵõ�, ���������������������2֡���ƶ����پ���ʱ��������
	int			m_iCurCursorPosY;//��ǰ�������ͼ�е�Y����  ͬ��

	int			m_nViewWidth;//��ͼ�Ӵ��Ŀ�
	int			m_nViewHeight;//��ͼ�Ӵ��ĸ�
	int			m_nMapWidth;//��ͼͼƬ�Ŀ�
	int			m_nMapHeight;//��ͼͼƬ�ĸ�
	int			m_nMapGridX; //��ͼ��ĸ����� = m_nMapWidth/m_nGridPixel
	int			m_nMapGridY; //��ͼ�ߵĸ����� = m_nMapHeight/m_nGridPixel


	int			m_nCurMapWidth;//����֮��ĵ�ͼ�Ŀ�
	int			m_nCurMapHeight;//����֮��ĵ�ͼ�ĸ�
	
	int			m_iOffsetX;//��ͼ��Xƫ��(��ͼ����Ӵ���ƫ��Ϊ��)
	int			m_iOffsetY;//��ͼ��Yƫ��(��ͼ����Ӵ���ƫ��Ϊ��)
	int			m_iViewOffsetX;//�Ӵ���� �Ի����Xƫ��
	int			m_iViewOffsetY;//�Ӵ���� �Ի����Yƫ��
	int			m_nGridPixel;//���ӵ�������

	
	int			m_iUpdateViewElementMark;//�Ƿ���Ҫ�����Ӵ��ڵ�Ԫ��(����ͼ�ƶ�,Npc,Obj,Trap�޸�ʱ����Ҫ)
	
	Npc_Vec		m_vecCurViewNpc;//���浱ǰ�Ӵ��е�npc
	Obj_Vec		m_vecCurViewObj;//���浱ǰ�Ӵ��е�Obj
	Trap_Vec	m_vecCurViewTrap;//���浱ǰ�Ӵ��е�Trap
	
	CElement*	m_pFocusElement;
	Element_Vec	m_vecSelectElement;//ֻ����NPC,OBJ,TRAP, PathKeyPos
	Element_Vec m_vecSelectSpare;
	mutable CPathLine	m_PathLine;//��¼����Ѳ����

	Element_Vec m_vecCopyElement;//���Ƶļ�¼

	Rect		m_rectSelect;//��������ѡ�����
	long		m_lFrameCount;//֡��¼
	FPos		m_fPosGameViewCenter;
	bool		m_bShowStates[eSS_COUNT];
	
	FPos_Vec		m_vecMeasurePos;

	FRect		m_fRectRotate;
	FPos		m_fPosRotateStart;
	Element_Vec m_vecRotateBasic;	//Ϊ����ת��׼, ������תǰ�Ķ���

	bool*	 m_pFillRecord; //����¼

	int	 m_iPauseTimes;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
