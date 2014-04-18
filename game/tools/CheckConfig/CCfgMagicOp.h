#pragma once
#include "FightDef.h"
#include "CCfgCalc.h"
#include "CBaseMagicOp.h"
#include "CSkillRuleDef.h"

class CCfgMagicOp;

typedef bool (*MOP_MATCH_CHECKER)(CCfgMagicOp*);

struct MagicEffFilter
{
	MagicEffFilter():m_eObjectFilter(eOF_None),m_FilterPara(NULL) {}
	~MagicEffFilter()	{delete m_FilterPara;}
	EObjFilterType	m_eObjectFilter;	// Ŀ��ɸѡ����
	CCfgCalc*		m_FilterPara;		// ɸѡ����
};

class CCfgMagicOp
{
public:
	CCfgMagicOp(int32 nID);
	~CCfgMagicOp();

	static vector<string> Split(const string& src, string delimit);

	bool CheckMagicOpAvail();													// ���ħ����������Ч��
	bool CheckRadiusMaxNum2Object();											// ���ɸѡ�뾶��Ŀ�����޶����ٴ�ɸѡĿ�����Ч��
	bool CheckMagicOpRelationAvail(MOP_MATCH_CHECKER pFunChecker);				// ���ħ��������ϵԼ������Ч��
	bool CheckFilterObj2MOPType();												// ��ʱ������Ŀ���ٴ�ɸѡ����ֻ��Ϊ����
	bool CheckNpcSkillFilterObj2MagicEff(const string sRuleName, string sSkillName, ESRTargetType eSRTargetType);
	EValueMagicOpType GetOpValueMagicOpType();

	static bool CheckFilterObj2CentralMS(CCfgMagicOp* pMagicOp);	// ����������Ϊ���е�ħ��״̬��ϵԼ������Ч��
	static bool CheckCascade2CancelableMagicEff(CCfgMagicOp* pMagicOp);	// ��� (�ɳ���Ч��)��İ�װ״̬����ΪΨһ
	static bool CheckMOPType2CancelableMagicEff(CCfgMagicOp* pMagicOp);	// ��� (���������˺����״̬�ɳ���Ч��)��ķ�������������Ϊ������
	static bool CheckFilterObj2CancelableMagicEff(CCfgMagicOp* pMagicOp);
	static bool CheckFilterObj2CalcNeedTargetMOPInMS(CCfgMagicOp* pMagicOp);
	static bool CheckNoCond(CCfgMagicOp* pMagicOp);
	static bool CheckHasNoCond(CCfgMagicOp* pMagicOp);
	static bool CheckHasNoValueChangedOp(CCfgMagicOp* pMagicOp);
	static bool CheckCanExecuteWithTargetIsDead(CCfgMagicOp* pMagicOp);
	static bool CheckTempMopForState(CCfgMagicOp* pMagicOp);		// �����ʱ����
	static bool CheckForbidCancellableValueChangedMop(CCfgMagicOp* pMagicOp);
	static bool CheckNonPersistentObj(CCfgMagicOp* pMagicOp);
	static bool CheckPingPongObjPersistent(CCfgMagicOp* pMagicOp);
	static bool CheckNoSingleFilterSelf2Eff(CCfgMagicOp* pMagicOp);	// ���ɳ���ħ��������ɸѡ��Ч��
	static bool CheckNoSingleFilterObj2Eff(CCfgMagicOp* pMagicOp);	// ����������Ϊ���е�ħ��״̬��ϵԼ������Ч��
	static bool CheckNoSingleAndSelfFilterObj2Eff(CCfgMagicOp* pMagicOp);	// ����������Ϊ���е�ħ��״̬��ϵԼ������Ч��
	static bool CheckCancellableMopArgIsNotConst(CCfgMagicOp* pMagicOp);	// ���ɳ���ħ�������Ĳ�������Ϊ�ɱ�ֵ
	static bool CheckCancelableMopSetupAureMagicNotSave(CCfgMagicOp* pMagicOp);	// ���ɳ���ħ���������ﰲװ��̬������ǿ�Ʋ����
	static bool CheckCancelableMopSetupStateNotSave(CCfgMagicOp* pMagicOp);		// ���ɳ���ħ���������ﰲװ��״̬������ǿ�Ʋ����
	static bool CheckCancelEffOfState(CCfgMagicOp* pMagicOp);					// ����ض��ɳ���ħ��Ч���Ƿ���״̬�Ŀɳ���Ч��
	static bool CheckPosMagicSetupStateType(CCfgMagicOp* pMagicOp);				// ���λ��ħ���Ĵ���Ч���簲װ״̬�����Ƿ���ȷ
	static bool CheckMutexPosMagicSetupStateType(CCfgMagicOp* pMagicOp);		// ��⻥��λ��ħ���Ĵ���Ч���簲װ״̬�����Ƿ���ȷ

	string GetMOPType() const {return m_strMOPType;}
	CBaseMagicOp* GetBaseMagicOp() const {return m_pMagicOp;}
	int32 GetID() const {return m_nID;}

private:
	typedef	vector<MagicEffFilter*>	VecMagicEffFilter;
	string						m_strMagicOpName;
	string						m_strMOPType;
	VecMagicEffFilter			m_vecFilterPipe;	// ɸѡ�ܵ�
	CCfgCalc*					m_MagicOpArg;
	CBaseMagicOp*				m_pMagicOp;
	int32						m_nID;
	bool						m_bIsCondition;
};
