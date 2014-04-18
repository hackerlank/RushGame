#pragma warning(disable:4996) 
#include "stdafx.h"
//#include <list>
//#include <vector>
//#include <set>
//#include <cassert>
//#include <string>
//#include <iostream>
#include <fstream>
//#include <Windows.h>
#include <sstream>
#include "PropResChecker.h"
#include "CGraphic.h"
#include "IGraphicModule.h"
#include "BaseHelper.h"
#include "IEffectMgrModule.h"
#include "CRenderGroup.h"
//#include "CRenderObject.h"
//#include "CoreClient"
//#include "CRenderSystem.h"
//#include "../../../../engine/include/CGraphic.h"
//#include "CGraphic.h"
//#include "ieffectmanager.h"

using namespace sqr;

set<string> PropResChecker::m_setPiece;
uint32 PropResChecker::m_iLine;
MapChkFile PropResChecker::m_mapCheckedFile[eRT_End];
MapChkPart PropResChecker::m_mapCheckedPart[eRT_End];
string PropResChecker::m_sResTypeName[eRT_End];
RES_TYPE PropResChecker::m_arrResType[eRT_End];
map<RES_TYPE, ERes_Type> PropResChecker::m_mapResIndex;
MapWeaponAttackCount PropResChecker::m_mapWAAnimCount;
MapWeaponAttackCount PropResChecker::m_mapWAEffCount;
VecWAEff PropResChecker::m_arrWAEff[WA_EFF_UBOUND];
MapPropRes PropResChecker::m_mapPropRes;
SetAnimPrefix PropResChecker::m_setAnimPrefix;
bool PropResChecker::__init = Init();
CheckEffectHandler CheckEffectHandler::StaticHandle;
CGraphic* PropResChecker::m_pGraphic;
IEffectManager* PropResChecker::m_pEffectMgr;
std::ofstream* PropResChecker::m_pLogFile;

bool PropResChecker::Init()
{
	m_setPiece.insert("body");
	m_setPiece.insert("armet");
	m_setPiece.insert("back");
	m_setPiece.insert("shoe");
	m_setPiece.insert("hair");
	m_setPiece.insert("face");
	m_setPiece.insert("weapon");
	m_setPiece.insert("pocketweapon");
	m_setPiece.insert("offweapon");
	m_setPiece.insert("shoulder");
	m_setPiece.insert("hand");
	m_setPiece.insert("arm");

	m_arrResType[eRT_Model] = RFT_ARP;
	m_arrResType[eRT_Anim] = RFT_ARA;
	m_arrResType[eRT_Eff] = RFT_ARE;

	m_mapResIndex[RFT_ARP] = eRT_Model;
	m_mapResIndex[RFT_ARA] = eRT_Anim;
	m_mapResIndex[RFT_ARE] = eRT_Eff;

	m_sResTypeName[eRT_Model] = "ģ��";
	m_sResTypeName[eRT_Anim] = "����";
	m_sResTypeName[eRT_Eff] = "��Ч";



	m_mapWAAnimCount["js_ds"] = 5;
	m_mapWAEffCount["js_ds"] = 5;

	m_mapWAAnimCount["js_ss"] = 4;
	m_mapWAEffCount["js_ss"] = 4;

	m_mapWAAnimCount["fs_ds"] = 2;
	m_mapWAEffCount["fs_ds"] = 2;

	m_mapWAAnimCount["fs_ss"] = 2;
	m_mapWAEffCount["fs_ss"] = 2;

	m_mapWAAnimCount["gj_ss"] = 4;
	m_mapWAEffCount["gj_ss"] = 0;

	m_mapWAAnimCount["nj_ss"] = 4;
	m_mapWAEffCount["nj_ss"] = 0;
	
	return true;
}

bool PropResChecker::CreateWAEffVector(TCHAR* sWAEff[], const size_t iLength)
{
	for(size_t i = 0; i < iLength; ++i)
	{
		uint32 uIndex = atoi(&sWAEff[i][strlen(sWAEff[i]) - 1]);
		if(uIndex >= WA_EFF_UBOUND)
		{
			stringstream sWAEffInfo;
			sWAEffInfo << "�չ���Ч��� [" << sWAEff[i] << "] ��׺" << uIndex << "��������" << (WA_EFF_UBOUND - 1) << "\n";
			cout << sWAEffInfo.str();
			(*m_pLogFile) << sWAEffInfo.str();
		}
		else
		{
			m_arrWAEff[uIndex].push_back(sWAEff[i]);
		}
	}
	return true;
}

uint32 PropResChecker::GetWAAnimCount(const string& sName)
{
	size_t l = sName.length();
	string sType = sName.substr(l - 5);
	return m_mapWAAnimCount[sType];
}

uint32 PropResChecker::GetWAEffCount(const string& sName)
{
	size_t l = sName.length();
	size_t start = sName.find("_") + 1;
	size_t end = sName.find("_", start);
	end = sName.find("_", end + 1);

	string sType = sName.substr(start, end - start);
	return m_mapWAEffCount[sType];
}

bool PropResChecker::CheckEmpty(const string& sItem, const string& sName)
{
	if(sItem.empty())
	{
		stringstream str;
		str << "ģ�ͱ�" << m_iLine << "�� [" << sName << "] �е�ֵ����Ϊ��\n";
		cout << str.str();
		(*m_pLogFile) << str.str();
		return false;
	}
	return true;
}

void PropResChecker::CheckInPieceMap(const string& sPieceName)
{
	if(m_setPiece.find(sPieceName) == m_setPiece.end())
	{
		stringstream str;
		str << "ģ�ͱ�" << m_iLine << "�� [��λ] �е�ֵ [" << sPieceName << "] ��������MAP��\n";
		cout << str.str();
		(*m_pLogFile) << str.str();
	}
}

void PropResChecker::SplitString(const string& sContent, TCHAR cSplitter, string& sPart1, string& sPart2)
{
	size_t iIndex = sContent.find(cSplitter);
	if(iIndex == string::npos)
	{
		sPart1 = sContent;
		sPart2 = "";
	}
	else
	{
		sPart1 = sContent.substr(0, iIndex);
		sPart2 = sContent.substr(iIndex + 1);
	}
}


void PropResChecker::BeginCheck()
{
	HWND hWnd = ::GetDesktopWindow();
	RenderSettings settings;
	settings.SetWindowHandle(hWnd);
	m_pGraphic = GetIGraphicModule()->CreateGraphic(settings) ;
	CMainWindowTarget::GetInst()->SetRenderGroup(CRenderGroupMgr::GetInst()->CreateGroup());
	m_pEffectMgr = GetIEffectMgrModule()->CreateEffectMgr( m_pGraphic, &CheckEffectHandler::StaticHandle );
	//new CRenderSystem(hWnd);
	_chdir(g_ResPath.c_str());

	m_pLogFile = new std::ofstream("PropResCheckerLog.txt");
}

void PropResChecker::EndCheck()
{
	//delete CRenderSystem::Inst();
	m_pLogFile->close();
}


void PropResChecker::CheckPropResLine(uint32 iLine, const TCHAR* sName, const TCHAR* sModelFile,
									  const TCHAR* sAniFile, const TCHAR* sEffectFile, const TCHAR* sPieceName,
									  const TCHAR* sRenderStyle, const TCHAR* sHidePiece, bool bDynamic)
{
	//m_sName = sName;
	//m_sModelFile = sModelFile;
	//m_sAniFile = sAniFile;
	//m_sEffectFile = sEffectFile;
	//m_sPieceName = sPieceName;
	//m_RenderStyle = RenderStyle;
	//m_sHidePiece = sHidePiece;
	//m_bDynamic = bDynamic;

	SetLineNo(iLine);
	if(!CheckEmpty(sName, "����")) return;
	if(!CheckEmpty(sModelFile, "ģ���ļ�")) return;
	if(!CheckEmpty(sAniFile, "�����ļ�")) return;
	if(!CheckEmpty(sPieceName, "��λ����")) return;

	CheckInPieceMap(sPieceName);

	PropResChecker* pCfg = new PropResChecker;
	pCfg->m_sName = sName;
	pCfg->m_sModelFile = sModelFile;
	pCfg->m_sAniFile = sAniFile;
	pCfg->m_sEffFile = sEffectFile;
	pCfg->m_sPieceName = sPieceName;
	pCfg->m_sRenderStyle = sRenderStyle;
	pCfg->m_sHidePiece = sHidePiece;
	pCfg->m_bDynamic = bDynamic;

	pair<MapPropRes::iterator, bool> pr = m_mapPropRes.insert(make_pair(pCfg->m_sName, pCfg));
	if(pr.second)
	{
		stringstream str;
		str << "PropRes�� [" << pCfg->m_sName << "] ���ظ�\n";
		cout << str.str();
		(*m_pLogFile) << str.str();
	}

	if(pCfg->m_bDynamic)
	{
		m_setAnimPrefix.insert(sAniFile);
	}

	string sModelFilename;
	//���PropRes��ģ��
	string sTitleName = "PropResģ��:";
	sTitleName += sName;
	CheckExistModelFiles(sTitleName, sModelFile, bDynamic, sModelFilename, sPieceName);
	//���PropRes������Ч
	if(sEffectFile[0] != 0)
	{
		string sTitleName2 = "PropRes��Ч:";
		sTitleName2 += sName;
		CheckExistLuminiEffectFiles(sTitleName2, sModelFilename, sEffectFile, sRenderStyle, sPieceName);
	}
}

void PropResChecker::CheckWeaponLine(uint32 iLine, const TCHAR* sName, const TCHAR* sResID,
									 const TCHAR* sAttackEffect, const TCHAR* sAniFile)
{
	string sTitleName = "StaticWeapon����:";
	sTitleName += sName;
	//���StaticWeapon����
	CheckExistAnimFiles(sTitleName, sAniFile, true, true);
	//����չ���Ч

	string sTitelName2 = "StaticWeapon��Ч:";
	sTitelName2 += sName;
	CheckWAExistEffFiles(sTitelName2, sAttackEffect);
}

void PropResChecker::CheckSkillAllAnim(const string& sName, const string& sPieceName,const string& sTitle)
{
	if(sPieceName.empty()) return;

	string sTitleName = sTitle + ":" + sName;
	for(SetAnimPrefix::iterator itr = m_setAnimPrefix.begin(); itr != m_setAnimPrefix.end(); ++itr)
	{
		//��鼼�ܶ���
		CheckExistAnimFiles(sTitleName, *itr, 1, false, sPieceName.c_str());
	}
}

void PropResChecker::CheckSkillAllEff(const string& sName, const string& sEff, bool bDynamic, const string& sTitle)
{
	if(sEff.empty()) return;

	string sTitleName = sTitle + ":" + sName;

	string sEffFile, sEffPiece;
	SplitString(sEff, ',', sEffFile, sEffPiece);
	//��鼼����Ч
	CheckSkillExistEffFiles(sTitleName, sEffFile, bDynamic, sEffPiece.c_str());
}

void PropResChecker::CheckSkillAllModel(const string& sName, const string& sModel, const string& sTitle)
{
	if(sModel.empty()) return;

	string sTitleName = sTitle + ":" + sName;


	MapPropRes::iterator itr = m_mapPropRes.find(sModel);
	if(itr == m_mapPropRes.end())
	{
		stringstream str;
		str << "[" << sTitleName << "] ��ģ�� [" << sModel << "] ��������ProperRes����\n";
		cout << str.str();
		(*m_pLogFile) << str.str();
	}
	else
	{
		string sModelFilename;
		for(set<string>::iterator itr2 = m_setPiece.begin(); itr2 != m_setPiece.end(); ++itr2)
		{
			sModelFilename = "";
			//��⼼��ģ��
			CheckExistModelFiles(sTitleName, itr->second->m_sModelFile, itr->second->m_bDynamic,
				sModelFilename, itr2->c_str());
		}
	}
}

















void PropResChecker::UniqueCheckExist(const string& sName, const string& sWholePath, const TCHAR* sPieceName, ERes_Type eResType)
{
	pair<bool, bool> uRet = make_pair(false, false);
	MapChkFile::iterator itrFile = m_mapCheckedFile[eResType].find(sWholePath);
	if(itrFile == m_mapCheckedFile[eResType].end())				//E E
	{
		uRet = CheckExistPiece(sWholePath.c_str(), m_arrResType[eResType], sPieceName);
		MapChkPart* pMapChkPart = new MapChkPart;
		m_mapCheckedFile[eResType].insert(make_pair(sWholePath, pMapChkPart));
		if(uRet.first)
		{
			pMapChkPart->insert(make_pair(sPieceName, uRet.second));
		}
	}
	else if(!itrFile->second->empty())							//T
	{
		MapChkPart::iterator itrPart = itrFile->second->find(sPieceName);
		if(itrPart == itrFile->second->end())			//T E
		{
			uRet = CheckExistPiece(sWholePath.c_str(), m_arrResType[eResType], sPieceName);
			if(uRet.first)
			{
				itrFile->second->insert(make_pair(sPieceName, uRet.second));
			}
		}
		else																	//T F/T
		{
			uRet = make_pair(true, itrPart->second);
		}
	}
	else														//F F/T
	{
		uRet = make_pair(false, false);
	}

	if(uRet.first)
	{
		if(!uRet.second)
		{
			stringstream str;
			str << "[" << sName << "] �в�����" << m_sResTypeName[eResType] << "�ļ� [" << sWholePath << "] �� ["
				<< sPieceName << "]\n"; 
			cout << str.str();
			(*m_pLogFile) << str.str();
		}
	}
	else
	{
		static stringstream sStaticPrintInfo;
		stringstream sTempPrintInfo;
		sTempPrintInfo << sName << "," << sWholePath;
		if(sTempPrintInfo.str() != sStaticPrintInfo.str())
		{
			stringstream str;
			str << "[" << sName << "] �в�����" << m_sResTypeName[eResType] << "�ļ� [" << sWholePath << "]\n"; 
			cout << str.str();
			(*m_pLogFile) << str.str();
			sStaticPrintInfo.str(sTempPrintInfo.str());
		}
	}
}

void PropResChecker::CheckExistModelFiles(const string& sName, const string& sModelFile, bool bDynamic, string& sModelFilename, const TCHAR* sPieceName)
{
	size_t ipos = sModelFile.find(',');
	if(ipos == string::npos)
	{
		if(bDynamic)
		{
			stringstream str;
			str << "Dynamic��Ϊ0ʱģ���ж��Ų�����\n";
			cout << str.str();
			(*m_pLogFile) << str.str();
		}
		else
		{
			UniqueCheckExist(sName, sModelFile, sPieceName, eRT_Model);
		}
		return;
	}
	else
	{
		stringstream sWholePath;
		string sModelPath = sModelFile.substr(0, ipos);
		string sModelFilename = sModelFile.substr(ipos + 1);
		if(bDynamic)
		{
			static TCHAR* sPart1Table[] = {"rlm", "rlw", "arm", "jlm", "jlw", "srm"};
			size_t l = sizeof(sPart1Table) / sizeof(TCHAR*);
			for(size_t i = 0; i < l; ++i)
			{
				sWholePath.str("");
				sWholePath << sModelPath << sPart1Table[i] << sModelFilename;
				UniqueCheckExist(sName, sWholePath.str(), sPieceName, eRT_Model);
			}
		}
	}
}

void PropResChecker::CheckExistAnimFiles(const string& sName, const string& sAnimFile,
	bool bDynamic, bool bIsWeapon, const TCHAR* sPieceName)
{
	stringstream sWholePath;
	if(bDynamic)
	{
		static TCHAR* sPart1Table[] = {"rlm_js_ds", "rlm_js_ss", "rlm_fs_ds", "rlm_fs_ss",
			"rlw_js_ds", "rlw_js_ss", "rlw_fs_ds", "rlw_fs_ss",
			"rlqm_js_ds", "rlqm_js_ss", "rlqw_js_ds", "rlqw_js_ss",
			"ar_js_ds", "arm_js_ds", "ar_js_ss", "arm_js_ss",
			"jlm_gj_ss", "jlm_nj_ss", "jlw_gj_ss", "jlw_nj_ss",
			"srm_js_ds", "srm_js_ss"};
		static size_t l = sizeof(sPart1Table) / sizeof(TCHAR*);
		for(size_t i = 0; i < l; ++i)
		{
			string sPart1 = sPart1Table[i];
			sWholePath.str("");
			sWholePath << sAnimFile << sPart1Table[i] << ".ske";
			if(bIsWeapon)
			{
				uint32 uCount = GetWAAnimCount(sPart1Table[i]);
				for(size_t j = 1; j <= uCount; ++j)
				{
					stringstream sPiecePart;
					sPiecePart << "attack0" << j;
					UniqueCheckExist(sName, sWholePath.str(), sPiecePart.str().c_str(), eRT_Anim);
				}
			}
			else
			{
				UniqueCheckExist(sName, sWholePath.str(), sPieceName, eRT_Anim);
			}
		}
	}
	else
	{
		sWholePath << sAnimFile;
		UniqueCheckExist(sName, sWholePath.str(), sPieceName, eRT_Anim);
	}
}

void PropResChecker::CheckSkillExistEffFiles(const string& sName, const string& sEffFile,
	bool bDynamic, const TCHAR* sPieceName)
{
	stringstream sWholePath;
	if(bDynamic)
	{
		static TCHAR* sPart1Table[] = {"rlm_js_ds", "rlm_js_ss", "rlm_fs_ds", "rlm_fs_ss",
			"rlw_js_ds", "rlw_js_ss", "rlw_fs_ds", "rlw_fs_ss",
			"rlqm_js_ds", "rlqm_js_ss", "rlqw_js_ds", "rlqw_js_ss",
			"ar_js_ds", "arm_js_ds", "ar_js_ss", "arm_js_ss",
			"jlm_gj_ss", "jlm_nj_ss", "jlw_gj_ss", "jlw_nj_ss",
			"srm_js_ds", "srm_js_ss"};

		static size_t l = sizeof(sPart1Table) / sizeof(TCHAR*);

		for(size_t i = 0; i < l; ++i)
		{
			string sPart1 = sPart1Table[i];
			sWholePath.str("");
			sWholePath << sEffFile << sPart1Table[i] << "_skill.efx";
			UniqueCheckExist(sName, sWholePath.str(), sPieceName, eRT_Eff);
		}
	}
	else
	{
		sWholePath << sEffFile;
		UniqueCheckExist(sName, sWholePath.str(), sPieceName, eRT_Eff);
	}
}

void PropResChecker::CheckWAExistEffFiles(const string& sName, const string& sEffFile)
{
	stringstream sWholePath;

	static TCHAR* sPart1Table[] = {"rlm_js_ds_1", "rlm_js_ds_2", "rlm_js_ds_3", "rlm_js_ss_4",
			"rlm_js_ss_5", "rlm_fs_ds_6", "rlm_fs_ds_7", "rlm_fs_ds_8",
			"rlm_fs_ds_9", "rlm_fs_ss_6", "rlm_fs_ss_7", "rlm_fs_ss_8",
			"rlm_fs_ss_9", "rlw_js_ds_1", "rlw_js_ds_2", "rlw_js_ds_3",
			"rlw_js_ss_4", "rlw_js_ss_5", "rlw_fs_ds_6", "rlw_fs_ds_7",
			"rlw_fs_ds_8", "rlw_fs_ds_9", "rlw_fs_ss_6", "rlw_fs_ss_7",
			"rlw_fs_ss_8", "rlw_fs_ss_9", "rlqm_js_ds_1", "rlqm_js_ds_2",
			"rlqm_js_ds_3", "rlqw_js_ds_1", "rlqw_js_ds_2", "rlqw_js_ds_3",
			"ar_js_ds_1", "ar_js_ds_2", "ar_js_ds_3", "arm_js_ds_1",
			"arm_js_ds_2", "arm_js_ds_3", "sr_js_ds_1", "sr_js_ds_2",
			"sr_js_ds_3", "sr_js_ss_5", "sr_js_ss_6"};

	static size_t l = sizeof(sPart1Table) / sizeof(TCHAR*);

	static bool bInit = CreateWAEffVector(sPart1Table, l);
	
	string sEffPathPart, sEffFilePart;
	SplitString(sEffFile, ',', sEffPathPart, sEffFilePart);

	if(sEffFilePart.empty())
	{
		return;
	}

	uint32 uIndex = atoi(sEffFilePart.c_str());
	if(uIndex <= 0 || uIndex >= WA_EFF_UBOUND)
	{
		stringstream str;
		str << "StaticWeapon�� [" << sName << "] ����Ч�к�׺���ֳ�����Χ\n";
		cout << str.str();
		(*m_pLogFile) << str.str();
	}

	size_t iLengthOfWAEff = m_arrWAEff[uIndex].size();

	for(size_t i = 0; i < iLengthOfWAEff; ++i)
	{
		sWholePath.str("");
		sWholePath << sEffPathPart << m_arrWAEff[uIndex][i] << ".efx";

		uint32 uCount = GetWAEffCount(m_arrWAEff[uIndex][i]);
		for(size_t j = 1; j <= uCount; ++j)
		{
			stringstream sPiecePart;
			sPiecePart << "attack0" << j;
			UniqueCheckExist(sName, sWholePath.str(), sPiecePart.str().c_str(), eRT_Anim);
		}
	}
}


void PropResChecker::CheckExistLuminiEffectFiles(const string& sName, const string& sModelFilename, const string& sWeaponEffect, const string& RenderStyle, const TCHAR* sPieceName)
{
	stringstream sWholePath;
	string sEffectPath = sWeaponEffect;
	for(uint32 i = 1; i <= 6; ++i)
	{
		sWholePath.str("");
		sWholePath << sEffectPath << sModelFilename << "_" << RenderStyle << "_" << i << ".efx";
		UniqueCheckExist(sName, sWholePath.str(), sPieceName, eRT_Eff);
	}
}

pair<bool, bool> PropResChecker::CheckExistPiece(const TCHAR* szFileName, RES_TYPE eResType, const TCHAR* sPieceName)
{
	CGraphic* pGraphic = m_pGraphic;
	IEffectManager* pEffectMgr = m_pEffectMgr;
	ResCreateInfo Info;
	Info.eResType = eResType;
	Info.szClassName = sPieceName ? sPieceName : "";
	Info.szParam = "";
	switch(eResType)
	{
	case RFT_ARP:
		if( FAILED( pGraphic->CreatePieceGroup( szFileName, (CPieceGroup**)&Info.pRes ) ) )
		{
			ostringstream strm;
			strm<<"ģ���ļ� " << szFileName << " ������\n";
			//cout << strm.str();
			//(*m_pLogFile) << strm.str();
			return make_pair(false, false);
		}
		else
		{
			CPieceGroup* pPieceGroup = (CPieceGroup*)Info.pRes;
			for( int32 i = 0; i < pPieceGroup->GetPieceClassNum(); i++ )
			{
				const TCHAR* szClassName = pPieceGroup->GetPieceClassName(i);
				if(strcmp(szClassName, sPieceName) == 0)
				{
					ostringstream strm;
					strm<<"���� ģ���ļ� " << szFileName << " [" << sPieceName << "]\n";
					//cout << strm.str();
					//(*m_pLogFile) << strm.str();
					return make_pair(true, true);
				}
				//pModel->AddMeshPiece( pPieceGroup, szClassName, rsNum );
			}
			ostringstream strm;
			//strm<<"ģ���ļ� " << szFileName << " [" << sPieceName << "] ������\n";
			//cout << strm.str();
			//(*m_pLogFile) << strm.str();
			return make_pair(true, false);
		}
		break;
	case RFT_ARA:
		if( FAILED( pGraphic->CreateAnimationsRes( szFileName, (CAnimationsRes**)&Info.pRes ) ) )
		{
			ostringstream strm;
			strm<<"�����ļ� " << szFileName << " ��[" << sPieceName << "] ������\n";
			//cout << strm.str();
			//(*m_pLogFile) << strm.str();
			return make_pair(false, false);
		}
		else
		{
			ostringstream strm;
			strm<<"���� �����ļ� " << szFileName << "\n";
			cout << strm.str();
			(*m_pLogFile) << strm.str();
			return make_pair(true, false);
		}
		break;
	case RFT_ARE:
		if( FAILED( pEffectMgr->CreateEffectGroup( szFileName, (IEffectGroup**)&Info.pRes ) ) )
		{
			ostringstream strm;
			strm<<"��Ч�ļ� " << szFileName << " ��[" << sPieceName << "] ������\n";
			//cout << strm.str();
			//(*m_pLogFile) << strm.str();
			return make_pair(false, false);
		}
		else
		{
			return make_pair(true, false);
		}
		break;
	}
	return make_pair(false, false);
}

//void PropResChecker::CheckExistFile(const TCHAR* szFileName, RES_TYPE eResType, const TCHAR* sPieceName)
//{
//	//CGraphic* pGraphic = CRenderSystem::Inst()->GetGraphic();
//	//IEffectManager* pEffectMgr = CRenderSystem::Inst()->GetEffectMgr();
//	//ResCreateInfo Info;
//	//Info.eResType = eResType;
//	//Info.szClassName = sPieceName ? sPieceName : "";
//	//Info.szParam = "";
//	//switch(eResType)
//	//{
//	//case RFT_ARP:
//	//	if( FAILED( pGraphic->CreatePieceGroup( szFileName, (CPieceGroup**)&Info.pRes ) ) )
//	//	{
//	//		ostringstream strm;
//	//		strm<<"ģ���ļ� " << szFileName << " ������\n";
//	//		cout << strm.str();
//	//	}
//	//	break;
//	//case RFT_ARA:
//	//	if( FAILED( pGraphic->CreateAnimationsRes( szFileName, (CAnimationsRes**)&Info.pRes ) ) )
//	//	{
//	//		ostringstream strm;
//	//		strm<<"�����ļ� " << szFileName << " ������\n";
//	//		cout << strm.str();
//	//	}
//	//	break;
//	//case RFT_ARE:
//	//	if( FAILED( pEffectMgr->CreateEffectGroup( szFileName, (IEffectGroup**)&Info.pRes ) ) )
//	//	{
//	//		ostringstream strm;
//	//		strm<<"��Ч�ļ� " << szFileName << " ������\n";
//	//		cout << strm.str();
//	//	}
//	//	break;
//	//}
//}