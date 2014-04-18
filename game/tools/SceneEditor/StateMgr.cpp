#include "stdafx.h"
#include "StateMgr.h"

EMainState CStateMgr::m_eCurState = eMS_NONE;
EMainState CStateMgr::m_eFrontState = eMS_NONE;
int		   CStateMgr::m_iAssistantState = eAS_NONE;


bool CStateMgr::IsState(EMainState eState)
{
	return m_eCurState == eState;
}

bool CStateMgr::IsFronState(EMainState eState)
{
	return m_eFrontState == eState;
}

bool CStateMgr::IsAssistState(int mark)
{
	return (m_iAssistantState & mark) == mark;
}

EMainState CStateMgr::GetCurState()
{
	return m_eCurState;
}


CStateMgr::CStateMgr(void)
{
	SetState(eMS_NONE);
	REGISTER_STATER_TABLE
}

CStateMgr::~CStateMgr(void)
{
	UNREGISTER_STATER_TABLE
}

CStateMgr& CStateMgr::Inst()
{
	static CStateMgr instance;
	return instance;
}



void CStateMgr::DoEvent(EEvent eEvent)
{
	UpdateAssistantState(eEvent);
	EMainState distState = GetDistState(eANY_STATE, eEvent);//�ȵ�����״̬�Ĺ�ͬ�¼������в���
	if (distState == eANY_STATE)//δ�ҵ��ٵ���ǰ״̬�в���
	{
		distState = GetDistState(m_eCurState, eEvent);
	}
	if (distState == eANY_STATE)
	{
		cout << "eMS_ANY_STATE ֻ�Ǹ����Ե� ����״̬, ������Ϊ ADD_TRANSIT_STATE ����ת״̬" << endl;
		return;
	}
	if (distState == m_eCurState)
	{
		return;
	}
	SetState(distState);
}

void CStateMgr::UpdateAssistantState(EEvent eEvent)
{
	if (eEvent == eEVENT_CLEAR_AS || eEvent == eEVENT_EIXT)
	{
		RemoveAssistantState(eAS_ALL);
		return;
	}
	if(ppStateTable[eASSISTANT_STATE] == NULL)
	{
		cout << "����״̬δ�����¼�  " << endl;
		return;
	}
	int n = ppStateTable[eASSISTANT_STATE][0] -1;//��һ��Ԫ�� �������ĸ���
	int *pStransitTable = ppStateTable[eASSISTANT_STATE]+1;// ƫ�Ƶ�һ��Ԫ�� 
	for ( int i = 0; i < n; i+=3)
	{
		if (pStransitTable[i] == eEvent)//�ҵ��¼� i+1 ��Ϊ���¼���Ӧ��״̬
		{
			if (pStransitTable[i+2] == 1)
			{
				SetAssistantState(EAssistantState(pStransitTable[i+1]));
			}
			else if (pStransitTable[i+2] == 0)
			{
				RemoveAssistantState(EAssistantState(pStransitTable[i+1]));
			}
			else
			{
				cout << "ATTACH_ASSISTANT_STATE �� " << eEvent << "  �¼���3������ 1�������, 0�����Ƴ�, ����δ����" << endl;
			}
			return;
		}
	}

}



void CStateMgr::SetState(EMainState eState)
{
	if(eMS_BACK == eState)//����֮ǰ״̬
	{
		if(m_stackState.size() <= 1)
		{
			cout << " ֻʣһ��״̬��, �޷�����״̬(m_stackState ���뱣��һ�� eMS_NONE״̬)" << endl;
			return;
		}
		m_stackState.pop_back();
		m_eCurState = m_stackState.back();
		if (m_stackState.size() < 2)
		{
			m_eFrontState = eMS_NONE;
		}
		else
		{
			m_eFrontState = m_stackState[m_stackState.size()-2];
		}
		return;
	}
	if (IsStateExisted(eState))
	{
		while (eState != m_stackState.back())
		{
			m_stackState.pop_back();
		}
	}
	else
	{
		m_stackState.push_back(eState);
	}
	m_eCurState = m_stackState.back();
	if (m_stackState.size() < 2)
	{
		m_eFrontState = eMS_NONE;
	}
	else
	{
		m_eFrontState = m_stackState[m_stackState.size()-2];
	}
}

EMainState CStateMgr::GetDistState(EMainState curState, EEvent eEvent) const
{
	if(ppStateTable[curState] == NULL)
	{
		cout << "״̬  " << curState << " û�������κ���ת��ϵ, ����Զͣ�ڴ�״̬!!!" << endl;
		return curState;
	}
	int n = ppStateTable[curState][0] -1;//��һ��Ԫ�� �������ĸ���  -1���� StransitTable �ĸ���
	int *pStransitTable = ppStateTable[curState]+1;// ƫ�Ƶ�һ��Ԫ�� 
	for ( int i = 0; i < n; i+=2)
	{
		if (pStransitTable[i] == eEvent || pStransitTable[i] == eEVENT_THE_OTHERS)//�ҵ��¼� i+1 ��Ϊ���¼���Ӧ��״̬
		{
			return (EMainState)(pStransitTable[i+1]);
		}
	}
	// cout << "״̬  " << curState << " û�����ö� �¼� " << eEvent << " �Ĵ���" << endl;
	return curState;
}


bool CStateMgr::IsStateExisted(EMainState eState) const
{
	for (int i = m_stackState.size() - 1; i >=0; i--)
	{
		if (m_stackState[i] == eState)
		{
			return true;
		}
	}
	return false;
}



void CStateMgr::SetAssistantState(EAssistantState eAssistantState)
{
	if (eAssistantState == eAS_NONE)
	{
		m_iAssistantState = eAS_NONE;
		return;
	}
	m_iAssistantState |= eAssistantState;
}

void CStateMgr::RemoveAssistantState(EAssistantState eAssistantState)
{
	m_iAssistantState &= ~eAssistantState;
}