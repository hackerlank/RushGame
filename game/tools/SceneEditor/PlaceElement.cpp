#include "stdafx.h"
#include <math.h>
#include "BaseTypes.h"
#include "PlaceElement.h"
#include "DataType.h"
#include "ScriptMgr.h"
#include "EnumMgr.h"
#include "StateMgr.h"

int TransDir(int gameDir)
{
	gameDir %= 256;
	return gameDir < 0 ? 256 + gameDir: gameDir;
}

void CElement::SetDir(int gameDir)
{
	this->direction = TransDir(gameDir);
}

CPlaceNpc::~CPlaceNpc()
{
	DeletePath();
}

//��һ���ؼ��� Ϊ�Լ�λ��, ��lua�������Ѿ�����
bool CPlaceNpc::Exp_AddPathKeyPos(float _x, float _y)
{
	_x += g_pCache->GetOffsetPos().x;
	_y += g_pCache->GetOffsetPos().y;
	PPathKeyPos pPos = new PathKeyPos();
	if (g_eModeType == eMT_DYNAMIC || g_eModeType == eMT_PATH_EDIT)
	{
		FPos centerPos = g_pCache->GetCenterPos();
		_x += centerPos.x;
		_y += centerPos.y;
	}
	pPos->fPos.x = _x;
	pPos->fPos.y = _y;
	pPos->direction = 0;
	pPos->speed = 0;
	pPos->delayTime = 0;
	pPos->pOwner = this;
	if (g_pCache->AddPathKeyPos(pPos))
	{
		pathList.push_back(pPos);
		if (g_pMapView->IsBlock((int)_x, (int) _y))
		{
			CString s;
			s.Format("%s��Ѳ�ߵ� [%0.2f,%0.2f] ���ϰ�����!", name, _x, _y);
			g_pMapView->MoveMapToCenter(_x,_y);
			g_pMainDlg->MessageBox(s);
		}
		return true;
	}
	else
	{
		delete pPos;
		return false;
	}
}

void CPlaceNpc::Exp_SetData(int _moveType, int _moveRange, int _direction, int _minDelayTime, int _maxDelayTime, int _speed, int _idleType, int _colonyID, int _colonyType, int _isIgnoreClog)
{
	moveType = _moveType;
	moveRange = _moveRange;
	direction = _direction;
	minDelayTime = _minDelayTime;
	maxDelayTime = _maxDelayTime;
	speed = _speed;
	idleType = _idleType;
	colonyId = _colonyID;
	colonyType = _colonyType;
	isIgnoreClog = _isIgnoreClog;
	Arrow.Reset();
	if (colonyId != 0 && colonyType == CEnumMgr::Inst().GetColonyTypeID("BOSS"))
	{
		g_pMainDlg->OnAddElement(this, eTPT_COLONY);
	}
}

void CPlaceNpc::SetPos(const FPos& fPos)
{
	this->fPos = fPos;
	Arrow.Reset();
}

void CPlaceNpc::SetDir(int gameDir)
{
	Arrow.SetDir(gameDir);
}

PPathKeyPos CPlaceNpc::InsetPathKeyPos(PPathKeyPos pBackKeyPos, const FPos& fPos)
{
	if (moveType == CEnumMgr::Inst().GetMoveTypeID("�̶�") || moveType == CEnumMgr::Inst().GetMoveTypeID("���") )
	{
		g_pMainDlg->MessageBox("���޸Ĵ� NPC ���ƶ�����Ϊ 'Ѳ������' �������Ѳ��·��","", MB_OK);
		CStateMgr::Inst().DoEvent(eEVENT_SHIFT_UP);
		CStateMgr::Inst().DoEvent(eEVENT_BACK_NORMAL);
		return NULL;
	}
	Path_List::iterator iter = pathList.begin();
	if (pBackKeyPos != NULL)
	{
		for (; iter != pathList.end(); iter++)
		{
			if (*iter == pBackKeyPos)
			{
				break;
			}
		}
		if (iter == pathList.end())
		{
			cout << " CPlaceNpc::InsetPathKeyPos ���ʧ��, pBackKeyPos ���Ǵ�npc��Ѳ�ߵ�" << endl;
			return NULL;
		}
	}
	PPathKeyPos pNewKeyPos = new PathKeyPos();
	pNewKeyPos->fPos = fPos;
	pNewKeyPos->direction =direction;
	pNewKeyPos->speed = speed;
	pNewKeyPos->delayTime = minDelayTime;
	pNewKeyPos->pOwner = this;
	if (g_pCache->AddPathKeyPos(pNewKeyPos))
	{
		if (pBackKeyPos == NULL)
		{
			pathList.push_back(pNewKeyPos);
		}
		else
		{
			pathList.insert(iter, pNewKeyPos);
		}
		return pNewKeyPos;
	}
	else
	{
		delete pNewKeyPos;
		return NULL;
	}
}

bool CPlaceNpc::DeletePath()
{
	Path_Iter iter = pathList.begin();
	while (iter != pathList.end())
	{
		if(!g_pCache->DeletePathKeyPos(*iter))
		{
			cout<< "����CPlaceNpc()ʱ ��ͼɾ������CCahce�����е�PathKeyPos" << endl;
			return false;
		}
		delete *iter;
		iter = pathList.erase(iter);
	}
	return true;
}

bool CPlaceNpc::DeletePathKeyPos(PPathKeyPos pKeyPos)
{
	if(!g_pCache->DeletePathKeyPos(pKeyPos))
	{
		cout<< "CPlaceNpc::DeletePathKeyPos ɾ��ʧ��" << endl;
		return false;
	}
	Path_List::iterator iter = pathList.begin();
	for ( ; iter != pathList.end(); iter++)
	{
		if (*iter == pKeyPos)
		{
			delete pKeyPos;
			pathList.erase(iter);
			return true;
		}
	}
	return false;
}

PPathKeyPos CPlaceNpc::FindNextKeyPos(PPathKeyPos pPreKeyPos)
{
	Path_List::iterator iter = pathList.begin();
	for ( ; iter != pathList.end(); iter++)
	{
		if (*iter == pPreKeyPos)
		{
			iter++;
			break;
		}
	}
	if (iter == pathList.end())
	{
		return NULL;
	}
	else
	{
		return *iter;
	}
}

//��ɫΪ -1��������
void FillRect(CDC* pDC,const Pos& centernPos, int size,COLORREF color, COLORREF frameColor = -1)
{
	int x = centernPos.x - size/2;
	int y = centernPos.y - size/2;
	if(color != -1)
	{
		pDC->FillSolidRect(x, y, size, size, color);
	}
	if (frameColor != -1)
	{
		CRect frameRect(x , y , x + size, y + size);
		pDC->FrameRect(&frameRect, &CBrush(frameColor));
	}
}

inline double GetRadianDir(int dir)
{
	return dir * DIR_TO_RADIAN_RATE;
}



void CPlaceNpc::Draw(CDC* pDC)
{
	Pos viewPos;
	g_pMapView->GridPosToViewPos(viewPos, fPos);
	COLORREF  color = g_eModeType == eMT_PATH_EDIT ? PATH_START_COLOR : NPC_IMAGE_COLOR;
	int size = int (IsMark(eSM_FOCUS) ? (OBJ_IMAGE_SIZE*1.5): OBJ_IMAGE_SIZE);
	if (colonyType == CEnumMgr::Inst().GetColonyTypeID("BOSS"))
	{
		color = BOSS_IMAGE_COLOR;
	}
	else if (colonyType == CEnumMgr::Inst().GetColonyTypeID("С��"))
	{
		color = SERVANT_IMAGE_COLOR;
	}
	else
	{
		PNPC pNPC = g_pOriginalData->GetNpc(name);
		if (pNPC)
		{
			if (pNPC->_NPCְҵ == 28)
			{
				color = SINGLE_ATTACK_IMAGE_COLOR;
			}
			else if (pNPC->_NPCְҵ == 29)
			{
				color = MULTI_ATTACK_IMAGE_COLOR;
			}
		}
	}
	
	FillRect(pDC, viewPos, size, color, color);
	FillRect(pDC, viewPos, size + 1, -1, RGB(0,0,0));
	if (IsMark(eSM_SELECTED))
	{
		FillRect(pDC, viewPos, SELECT_MARK_SIZE, -1, SELECT_MARK_COLOR);
	}
}

void CPlaceNpc::CopyeNpc(OUT PPlaceNpc pNpcDest, PPlaceNpc pNpcSrc)
{
	pNpcDest->fPos = pNpcSrc->fPos;
	pNpcDest->name = pNpcSrc->name;
	pNpcDest->moveType = pNpcSrc->moveType;
	pNpcDest->moveRange = pNpcSrc->moveRange;
	pNpcDest->direction = pNpcSrc->direction;
	pNpcDest->minDelayTime = pNpcSrc->minDelayTime;
	pNpcDest->maxDelayTime = pNpcSrc->maxDelayTime;
	pNpcDest->speed = pNpcSrc->speed;
	pNpcDest->colonyId = pNpcSrc->colonyId;
	pNpcDest->colonyType = pNpcSrc->colonyType;
	pNpcDest->idleType = pNpcSrc->idleType;
	pNpcDest->AIType = pNpcSrc->AIType;
	pNpcDest->type = pNpcSrc->type;
	pNpcDest->Arrow.Reset();
}

bool CPlaceNpc::CheckNpc(const PlaceNpc& npc, OUT CString& errorMessage, bool isPut /* =true */)
{
	if (npc.name == "")
	{
		errorMessage = "����Ϊ��";
		return false;
	}
	else
	{
		PNPC pNPC = g_pOriginalData->GetNpc(npc.name);
		if (pNPC == NULL && g_eModeType != eMT_PATH_EDIT)
		{
			errorMessage = "����Ϊ " + npc.name + "  ����Npc_Common����";
			return false;
		}
	}
	if (CEnumMgr::Inst().GetMoveTypeStr(npc.moveType ) == ERROR_STR )
	{
		errorMessage = "��֧�ֵ� �ƶ�����";
		return false;
	}
	if(npc.moveRange < 0)
	{
		errorMessage = "�ƶ���Χ ����С�� 0";
		return false;
	}
	if(npc.direction < 0 || npc.direction > 255)
	{
		errorMessage = "���� ������ 0 - 255 ֮��";
		return false;
	}
	if(npc.minDelayTime < 0 || npc.maxDelayTime < 0)
	{
		errorMessage = "��Ϣʱ�䲻��С��0 ";
		return false;
	}
	if (npc.minDelayTime > npc.maxDelayTime)
	{
		errorMessage = "��С��Ϣʱ�䲻�ܴ��������Ϣʱ��";
		return false;
	}
	if (npc.moveType != CEnumMgr::Inst().GetMoveStateID("�̶�")  && CEnumMgr::Inst().GetMoveStateStr(npc.speed) == ERROR_STR )
	{
		errorMessage = "��֧�ֵ� ���߷�ʽ";
		return false;
	}
	if (CEnumMgr::Inst().GetColonyTypeStr(npc.colonyType) == ERROR_STR)
	{
		errorMessage = "��֧�ֵ� Ⱥ���λ";
		return false;
	}
	if(CEnumMgr::Inst().GetIdleTypeStr(npc.idleType) == ERROR_STR)
	{
		errorMessage = "��֧�ֵ� ��������";
		return false;
	}
	
	if (!isPut)//�ǰڷ�ʱ�򲻼��Ⱥ��
	{
		return true;
	}

	if (npc.AIType.Find("Ⱥ���Ա") >= 0 )
	{	
		if(npc.colonyId == 0) 
		{
			errorMessage = "Ⱥ���Ա AI ��Ҫ�� Ⱥ��Id";
			return false;
		}
		if(npc.colonyType != CEnumMgr::Inst().GetColonyTypeID("С��"))
		{
			errorMessage = "Ⱥ���Ա AI, Ⱥ���λ����ΪС��";
			return false;
		}
		if(npc.type != "�Ŷӳ�Ա")
		{
			errorMessage = "Ⱥ���Ա AI, ���� ����Ϊ'�Ŷӳ�Ա'";
			return false;
		}
	}
	else if(npc.AIType == "����Npc֧��Ѳ��")
	{
		if(npc.colonyId == 0) 
		{
			errorMessage = "AIΪ'����Npc֧��Ѳ��' ��Ҫ�� Ⱥ��Id";
			return false;
		}
		if(npc.colonyType != CEnumMgr::Inst().GetColonyTypeID("BOSS"))
		{
			errorMessage = "AIΪ'����Npc֧��Ѳ��' Ⱥ���λ����ΪBOSS";
			return false;
		}
		if(npc.type != "����Npc")
		{
			errorMessage = "AIΪ'����Npc֧��Ѳ��' ���� ����Ϊ'����Npc'";
			return false;
		}
	}
	else if(npc.colonyType != CEnumMgr::Inst().GetColonyTypeID("�޹���ʿ"))
	{
		errorMessage = "�� Ⱥ����ص�AI ֻ������Ϊ �޹���ʿ";
		return false;
	}
	else
	{
		if(npc.colonyId != 0)
		{
			errorMessage = CEnumMgr::Inst().GetColonyTypeStr(npc.colonyType) + " Ⱥ��ID ����Ϊ0";
			return false;
		}
	}

	if(npc.colonyType == CEnumMgr::Inst().GetColonyTypeID("BOSS"))
	{
		PPlaceNpc pColonyBoss = g_pCache->FindGroupBoss(npc.colonyId);
		if (pColonyBoss)
		{
			errorMessage.Format("Ⱥ�� %d ,�Ѿ����� BOSS", npc.colonyId) ;
			return false;
		}
	}
	else if (npc.colonyType == CEnumMgr::Inst().GetColonyTypeID("С��"))
	{
		PPlaceNpc pColonyBoss = g_pCache->FindGroupBoss(npc.colonyId);
		if (!pColonyBoss)
		{
			errorMessage.Format("Ⱥ�� %d ,��û�� BOSS, ���Ȱ�BOSS.", npc.colonyId) ;
			return false;
		}
	}
	return true;
}

bool CPlaceNpc::IsBoos(CElement* pElement)
{
	if (pElement->eType == eET_NPC)
	{
		return IsBoos(static_cast<CPlaceNpc*>(pElement));
	}
	return false;
}

bool CPlaceNpc::IsBoos(CPlaceNpc* pNpc)
{
	return pNpc->colonyType == CEnumMgr::Inst().GetColonyTypeID("BOSS");
}

bool CPlaceNpc::IsServant(CElement* pElement)
{
	if (pElement->eType == eET_NPC)
	{
		return IsServant(static_cast<CPlaceNpc*>(pElement));
	}
	return false;
}

bool CPlaceNpc::IsServant(CPlaceNpc* pNpc)
{
	return pNpc->colonyType == CEnumMgr::Inst().GetColonyTypeID("С��");
}


void CPlaceObj::Draw(CDC* pDC)
{
	Pos viewPos;
	g_pMapView->GridPosToViewPos(viewPos, fPos);
	if (IsMark(eSM_FOCUS))
	{
		FillRect(pDC, viewPos, int (TRAP_IMAGE_SIZE*1.5), OBJ_IMAGE_COLOR);
	}
	else
	{
		FillRect(pDC, viewPos, OBJ_IMAGE_SIZE, OBJ_IMAGE_COLOR);
	}
	if (IsMark(eSM_SELECTED))
	{
		FillRect(pDC, viewPos, SELECT_MARK_SIZE, -1, SELECT_MARK_COLOR);
	}
}
void CPlaceObj::SetPos(const FPos& fPos)
{
	this->fPos = fPos;
	Arrow.Reset();
}

void CPlaceObj::SetDir(int gameDir)
{
	Arrow.SetDir(gameDir);
}

void CPlaceTrap::Draw(CDC* pDC)
{
	Pos viewPos;
	g_pMapView->GridPosToViewPos(viewPos, fPos);
	if (IsMark(eSM_FOCUS))
	{
		FillRect(pDC, viewPos, int (TRAP_IMAGE_SIZE*1.5), TRAP_IMAGE_COLOR);
	}
	else
	{
		FillRect(pDC, viewPos, TRAP_IMAGE_SIZE, TRAP_IMAGE_COLOR);
	}
	if (IsMark(eSM_SELECTED))
	{
		FillRect(pDC, viewPos, SELECT_MARK_SIZE, -1, SELECT_MARK_COLOR);
	}
}

void CPlaceTrap::SetPos(const FPos& fPos)
{
	this->fPos = fPos;
	Arrow.Reset();
}

void CPlaceTrap::SetDir(int gameDir)
{
	Arrow.SetDir(gameDir);
}

void CPathKeyPos::Draw(CDC* pDC)
{
	Pos viewPos;
	g_pMapView->GridPosToViewPos(viewPos, fPos);
	if (IsMark(eSM_FOCUS))
	{
		FillRect(pDC, viewPos,int (PATH_KEY_POS_SIZE*1.5), PATH_KEY_POS_COLOR);
	}
	else
	{
		FillRect(pDC, viewPos, PATH_KEY_POS_SIZE, PATH_KEY_POS_COLOR);
	}
	if (IsMark(eSM_SELECTED))
	{
		FillRect(pDC, viewPos, SELECT_MARK_SIZE, -1, SELECT_MARK_COLOR);
	}
}
void CArrow::SetOwner(CElement* _pOwner)
{
	pOwner = _pOwner;
	Reset();
}

void CArrow::SetDir(int gameDir)
{
	gameDir = TransDir(gameDir);
	pOwner->direction = gameDir;
	double radian = GetRadianDir(gameDir);
	float cosa = float (cos(radian));
	float sina =  float (sin(radian));
	float topX = 0;
	float topY = -DIR_ARROW_SIZE;
	float leftX = -4.0;
	float leftY = -0.7 * DIR_ARROW_SIZE;
	float rightX = 4.0;
	float rightY = -0.7 * DIR_ARROW_SIZE;
	arrowHeadPos.x = int(topX * cosa - topY * sina);
	arrowHeadPos.y = int(topY * cosa + topX * sina);
	leftPos.x = int(leftX * cosa - leftY * sina);
	leftPos.y = int(leftY * cosa + leftX * sina);
	rightPos.x = int(rightX * cosa - rightY * sina);
	rightPos.y = int(rightY * cosa + rightX * sina);
}

void CArrow::Reset()
{	
	fPos = pOwner->fPos;
	SetDir(pOwner->direction);
}

void CArrow::Draw(CDC* pDC)
{
	static CPen pen(PS_SOLID,DIR_ARROW_LINE_SIZE, DIR_ARROW_COLOR);
	static CPen pen2(PS_SOLID,DIR_ARROW_LINE_SIZE, DIR_ARROW_FOCUS_COLOR);

	if (g_eModeType == eMT_PATH_EDIT)
	{
		return;
	}
	Pos viewPos;
	g_pMapView->GridPosToViewPos(viewPos, fPos);
	Pos arrowHeadPos = viewPos+ this->arrowHeadPos ;
	Pos	leftPos = viewPos+  this->leftPos;
	Pos rightPos = viewPos+  this->rightPos;

	if (IsMark(eSM_FOCUS))
	{
		pDC->SelectObject(&pen2);
	}
	else
	{
		pDC->SelectObject(&pen);
	}
	pDC->MoveTo(viewPos.x, viewPos.y);
	pDC->LineTo(arrowHeadPos.x, arrowHeadPos.y);
	pDC->LineTo(leftPos.x, leftPos.y);
	pDC->MoveTo(arrowHeadPos.x, arrowHeadPos.y);
	pDC->LineTo(rightPos.x, rightPos.y);
}

void CPathLine::Draw(CDC* pDC)
{
	static CPen pen(PS_SOLID,PATH_LINE_SIZE, PATH_LINE_FOCUS_COLOR);
	static CPen pen2(PS_SOLID,PATH_LINE_SIZE, PATH_LINE_COLOR);

	if (pBackKeyPos == NULL)
	{
		return;
	}

	if (IsMark(eSM_FOCUS))
	{
		pDC->SelectObject(&pen);
	}
	else
	{
		pDC->SelectObject(&pen2);
	}
	
	Pos pos1, pos2;
	g_pMapView->GridPosToViewPos(pos1, (pFrontKeyPos != NULL) ? pFrontKeyPos->fPos : pBackKeyPos->pOwner->fPos);
	pDC->MoveTo(pos1.x, pos1.y);
	g_pMapView->GridPosToViewPos(pos2, pBackKeyPos->fPos);
	pDC->LineTo(pos2.x, pos2.y);
}