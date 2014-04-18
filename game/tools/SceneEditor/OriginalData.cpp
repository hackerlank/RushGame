#include "stdafx.h"
#include "OriginalData.h"
#include "DataType.h"


using namespace std;


COriginalData::COriginalData(void)
{
	
}

COriginalData::~COriginalData(void)
{
	DelAll();
}


PNPC COriginalData::Exp_AddNpc(const char* _����,
					   const char*		_����,
					   const char*  _AI����,
					   uint8		_Npc�Ƿ�Ψһ,
					   uint8		_NPC��Ӫ,
					   uint8		_�ȼ�����,
					   uint8		_�ȼ�����,
					   uint32		_ˢ��ʱ��,
					   uint8		_ְҵ
					   )
{

	PNPC		pNewNpc = new NPC;
	pNewNpc->_���� =  _����;
	pNewNpc->_���� =  _����;
	pNewNpc->_AI���� = _AI����;
	pNewNpc->_Npc�Ƿ�Ψһ = _Npc�Ƿ�Ψһ;
	pNewNpc->_�ȼ�����    = _�ȼ�����;
	pNewNpc->_�ȼ�����    = _�ȼ�����;
	pNewNpc->_ˢ��ʱ��    = _ˢ��ʱ��;
	pNewNpc->_NPCְҵ	= _ְҵ;

	m_mapNpcData.insert(std::make_pair(pNewNpc->_����,pNewNpc));
	return pNewNpc;
}

void COriginalData::Exp_AddIntObj(const char* name, uint8 IsSingle, uint8 IsBarrier)
{
	POBJ pNewObj = new OBJ;
	pNewObj->name = name;
	pNewObj->isSingle = IsSingle;
	pNewObj->isBarrier = IsBarrier;
	m_mapObjData.insert(std::make_pair(pNewObj->name,pNewObj));
	if (IsBarrier != 0)
	{
		m_vecBarrierObj.push_back(name);
	}
}



void COriginalData::Exp_AddTrap(const char* name, uint8 IsSingle)
{
	PTRAP pNewTrap = new TRAP;
	pNewTrap->name = name;
	pNewTrap->isSingle = IsSingle;
	m_mapTrapData.insert(std::make_pair(pNewTrap->name,pNewTrap));
}

bool COriginalData::Exp_AddSceneCfg(const char* sceneName, const char* mapFile, const char* sceneFile, const char* areaFile)
{
	if (!sceneName || !mapFile)
	{
		return false;
	}
	SceneCfg* pCfg = new SceneCfg(mapFile, sceneFile, areaFile);
	return m_mapSceneCfg.insert(make_pair(sceneName, pCfg)).second;
}


void COriginalData::DelAll()
{
	NpcData_Iter npcIt = m_mapNpcData.begin();
	while (npcIt != m_mapNpcData.end())
	{
		if(npcIt->second)
		{
			delete npcIt->second;
		}
		npcIt = m_mapNpcData.erase(npcIt);
	}
	m_mapNpcData.clear();

	ObjData_Iter objIt = m_mapObjData.begin();
	while (objIt != m_mapObjData.end())
	{
		if(objIt->second)
		{
			delete objIt->second;
		}
		objIt = m_mapObjData.erase(objIt);
	}
	m_mapObjData.clear();


	TrapData_Iter trapIt = m_mapTrapData.begin();
	while (trapIt != m_mapTrapData.end())
	{
		if(trapIt->second)
		{
			delete trapIt->second;
		}
		trapIt = m_mapTrapData.erase(trapIt);
	}
	m_mapTrapData.clear();

	SceneCfg_Iter sceneIt = m_mapSceneCfg.begin();
	while (sceneIt != m_mapSceneCfg.end())
	{
		if(sceneIt->second)
		{
			delete sceneIt->second;
		}
		sceneIt = m_mapSceneCfg.erase(sceneIt);
	}
	m_mapSceneCfg.clear();

}

PNPC COriginalData::GetNpc(const CString& name)
{

	NpcData_Iter iter = m_mapNpcData.find(name);
	if (iter == m_mapNpcData.end())
	{
		return NULL;
	}
	return iter->second;
}

bool COriginalData::IsSingleNpc(const CString& name)
{
	NpcData_Iter iter = m_mapNpcData.find(name);
	if (iter == m_mapNpcData.end())
	{
		cout << name << " ���� COriginalData ������"<< endl;
		return false;
	}
	if (iter->second->_Npc�Ƿ�Ψһ == 1)
	{
		return true;
	}
	return false;
}

bool COriginalData::IsSingleObj(const CString& name)
{
	ObjData_Iter iter = m_mapObjData.find(name);
	if (iter == m_mapObjData.end())
	{
		cout << name << " ���� COriginalData ������"<< endl;
		return false;
	}
	if (iter->second->isSingle == 1)
	{
		return true;
	}
	return false;
}

bool COriginalData::IsSingleTrap(const CString& name)
{
	TrapData_Iter iter = m_mapTrapData.find(name);
	if (iter == m_mapTrapData.end())
	{
		cout << name << " ���� COriginalData ������"<< endl;
		return false;
	}
	if (iter->second->isSingle == 1)
	{
		return true;
	}
	return false;
}

bool COriginalData::IsNpcInCommonCfg(const CString& name)
{
	return m_mapNpcData.find(name) != m_mapNpcData.end();
}

bool COriginalData::IsObjInCommonCfg(const CString& name)
{
	return m_mapObjData.find(name) != m_mapObjData.end();
}

bool COriginalData::IsTrapInCommonCfg(const CString& name)
{
	return m_mapTrapData.find(name) != m_mapTrapData.end();
}

bool COriginalData::IsBarrierObj(const CString& name)
{
	ObjData_Iter iter = m_mapObjData.find(name);
	if (iter == m_mapObjData.end())
	{
		cout << name << " ���� COriginalData ������"<< endl;
		return false;
	}
	if (iter->second->isBarrier != 0)
	{
		return true;
	}
	return false;
}

CString COriginalData::GetMapFile(const CString& sceneName)
{
	SceneCfg_Map::iterator iter = m_mapSceneCfg.find(sceneName);
	if (iter != m_mapSceneCfg.end())
	{
		return (iter->second)->mapFile;
	}
	else
		return "";
}

CString COriginalData::GetSceneFile(const CString& sceneName)
{
	SceneCfg_Map::iterator iter = m_mapSceneCfg.find(sceneName);
	if (iter != m_mapSceneCfg.end())
	{
		return (iter->second)->sceneFile;
	}
	else
		return "";
}

CString COriginalData::GetAreaFile(const CString& sceneName)
{
	SceneCfg_Map::iterator iter = m_mapSceneCfg.find(sceneName);
	if (iter != m_mapSceneCfg.end())
	{
		return (iter->second)->areaFile;
	}
	else
		return "";
}