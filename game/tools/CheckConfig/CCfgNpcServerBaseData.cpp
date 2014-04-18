#include "stdafx.h"
#include "CCfgNpcServerBaseData.h"
#include "NpcInfoDefs.h"
#include "CCfgNPCFighterBaseDataCheck.h"
#include "CCfgNPCResCommonCheck.h"
#include "CCfgNPCAIBaseDataCheck.h"
#include "ExpHelper.h"

set<string> CCfgNpcServerBaseData::m_setCfgNpcBaseData;

void CCfgNpcServerBaseData::Create(const TCHAR* szName, const TCHAR* szNpcType, const TCHAR* szAIType, 
							  const TCHAR* szAIData, uint32 uRace, uint32 uCamp, uint32 uClass, uint32 uSex, uint32 uLevel)
{
	bool bSuccess = true;
	if (szNpcType == NULL)
	{
		bSuccess = false;
		cout << "Npc����Ϊ��\n";
	}

	if (szName == NULL)
	{
		bSuccess = false;
		cout << "Npc��Ϊ��\n";
	}

	if (szAIType == NULL)
	{
		bSuccess = false;
		cout << "Npc " << szName << " AI����Ϊ��\n";
	}

	if (szAIData == NULL)
	{
		bSuccess = false;
		cout << "Npc " << szName << " AI����Ϊ��\n";
	}

	string sNpcName(szName);
	set<string>::iterator iter = m_setCfgNpcBaseData.find(sNpcName);
	if (iter != m_setCfgNpcBaseData.end())
	{
		bSuccess = false;
		cout << "Npc " << szName <<" ��Npc_Common�����ظ���\n";
	}
	else
	{
		m_setCfgNpcBaseData.insert(sNpcName);
	}

	if (!CCfgNPCFighterBaseDataCheck::BeExist(sNpcName))
	{
		bSuccess = false;
		cout << "Npc " << szName <<" ս����Ϣ�����ڣ�\n";
	}

	string sAIDataName(szAIData);
	if (!CCfgNPCAIBaseDataCheck::BeExist(sAIDataName))
	{
		bSuccess = false;
		cout << "Npc " << szName << " AI���� " << szAIData << " �����ڣ�\n";
	}

	if (!CCfgNPCResCommonCheck::BeExist(sNpcName))
	{
		bSuccess = false;
		cout << "Npc  " << szName << "�ڡ�NpcRes_Common�����в����ڣ����ʵ��\n";
	}

	//����Npc����
	TypeMapRgst::mapNpcType_itr iter1 = NpcType.mapNpcTypeMap.find(szNpcType);
	if (iter1 == NpcType.mapNpcTypeMap.end())
	{
		bSuccess = false;
		cout << "Npc " << szName << "  Type�� " << szNpcType << " ��δע��\n";
	}

	//����Aoi����
	AITypeAoiRgst::mapObjectAoi_itr iter2 = sObjectAoiType.mapObjectAoiType.find(szAIType);
	if (iter2 == sObjectAoiType.mapObjectAoiType.end())
	{
		bSuccess = false;
		cout << "Npc " << szName << "  AIType�� " << szAIType << " ��δע��AOI���ͣ�\n";
	}

	//����AIType����
	AITypeMap::mapNpcAIType_itr iter3 = sNpcAIType.mapNpcAIType.find( szAIType);
	if (iter3 == sNpcAIType.mapNpcAIType.end())
	{
		bSuccess = false;
		cout << "Npc " << szName << "  AIType��" << szAIType << " ��δע��AI���ͣ�\n";
	}

	if (!bSuccess)
	{
		string str1;
		str1 = "�������Npc������Ϣ��ʧ�ܣ����ʵ��";
		GenErr(str1, szName);
	}

	if (uClass == 29)
	{
		string strSkillRuleName("ͨ�ú��Ⱥ�ɫ�������");
		string strNpcName(szName);
		bool bSuccess = CCfgNPCFighterBaseDataCheck::CheckSkillRule(strNpcName, strSkillRuleName);
		if (!bSuccess)
		{
			ostringstream strm;
			cout<<"�����棺�� Npc��"<< szName<<" ��ְҵ��29,������NpcFighterBaseData���ж�Ӧ�ļ��ܹ���Ϊ: [ͨ�ú��Ⱥ�ɫ�������], �����Ƿ���ȷ��"<< endl;
		}
	}
}

bool CCfgNpcServerBaseData::FindNpc(string sNpcName)
{
	return m_setCfgNpcBaseData.find(sNpcName) != m_setCfgNpcBaseData.end();
}
