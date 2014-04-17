#include "stdafx.h"
#include "CClass.h"
#include "CScriptData.h"
#include "ScriptCommon.h"
#include "TSqrAllocator.inl"
#include "ExpHelper.h"
#include "ErrLogHelper.h"

//=============================== CClass ===============================

CClass::CClass( CScript* pScript, const char* szClassName )
: m_pScript( pScript ), m_sClassName( szClassName )
, m_bIsCallBack(false)
, m_iClassRef(LUA_NOREF)
, m_eType(eSCT_None)
{
}

CClass::~CClass()
{
}

inline bool CClass::IsAbstract()
{
	return !m_pFunCreateObj;
}

EScriptClassType CClass::GetType()
{
	return m_eType;
}

bool CClass::IsIsolatedClass()
{
	return (m_eType == eSCT_Isolated);
}

bool CClass::IsStaticClass()
{
	return (m_eType == eSCT_Static);
}

bool CClass::IsShareClass()
{
	return (m_eType == eSCT_Share);
}

bool CClass::IsDynamicClass()
{
	return (m_eType == eSCT_Dynamic);
}

void* CClass::Create(lua_State *pState)
{
	void* pObj = NULL;

	SQR_TRY
	{
		if(IsAbstract())
		{
			ostringstream strm;
			strm << GetClassName() << " ��ע���lua�ĳ����࣬��������lua�д�����" << endl;
			GenErr(strm.str().c_str());
		}

		pObj = m_pFunCreateObj();
	}
	SQR_CATCH(exp)
	{
		ostringstream str;
		str << GetClassName() << "��lua�д��������ʱ��������" << endl;

		exp.AppendMsg(str.str().c_str());
		LogExp(exp);
	}
	SQR_TRY_END;

	return pObj;
}

void CClass::Release(lua_State* pState, void * pObject )
{
	SQR_TRY
	{
		if(IsAbstract())
		{
			ostringstream strm;
			strm << GetClassName() << " ��ע���lua�ĳ����࣬��������lua��������" << endl;
			GenErr(strm.str().c_str());
		}

		m_pFunDeleteObj(pObject);
	}
	SQR_CATCH(exp)
	{
		ostringstream str;
		str << GetClassName() << "��lua�����������ʱ��������" << endl;

		exp.AppendMsg(str.str().c_str());
		LogExp(exp);
	}
	SQR_TRY_END;
}

const char* CClass::GetClassName()const
{
	return m_sClassName.c_str();
}

void* CClass::_FindCallBackFunc(const String_t& sFuncName)
{
	MapCallBackFunc_t::iterator iter = m_mapCallBackFunc.find(sFuncName);
	if(iter != m_mapCallBackFunc.end())
	{
		return iter->second;
	}

	for( size_t i = 0; i < m_vpBaseRegist.size(); i++ )
	{
		void* pFunc = m_vpBaseRegist[i]->_FindCallBackFunc(sFuncName);
		if(pFunc)
			return pFunc;
	}

	return NULL;
}

void* CClass::GetCallBackFunc(const char* szFunName)
{
	//������running callback������ң����û�У����Ǿ�Ҫ��ע���callback����������
	//��Ȼ��һ�����û�Ƚ���������������Ժ�Ĳ�ѯ�ٶ�
	MapRunningCallBack_t::iterator iter = m_mapRunningCallBack.find(szFunName);
	if (iter != m_mapRunningCallBack.end())
	{
		return iter->second;
	}
		
	void* pFunc = _FindCallBackFunc(szFunName);

	m_mapRunningCallBack.insert(make_pair(szFunName, pFunc));
	
	return pFunc;
}

void CClass::_CheckRegCallBackFunc(CClass* pInfo, const char* szFunName)
{
	if(pInfo->m_bIsCallBack) //���ע���˻ص������Ǿʹ�������������ϱ���
	{
		MapCallBackFunc_t::iterator iter = pInfo->m_mapCallBackFunc.find(szFunName);
		if(iter != pInfo->m_mapCallBackFunc.end())
		{
			stringstream str;
			str << GetClassName() << "��ע��callback���� " << szFunName << "����"
				<< endl << "�ڸ���" << pInfo->GetClassName() << " �����Ѿ�ע���˸ú���" << endl;
			GenErr(str.str().c_str());
		}
	}

	for( size_t i = 0; i < pInfo->m_vpBaseRegist.size(); i++ )
	{
		_CheckRegCallBackFunc(pInfo->m_vpBaseRegist[i], szFunName);
	}
}

void CClass::RegCallBackFunc(const char* szFunName, void* pContext)
{
	if(IsIsolatedClass())
	{
		ostringstream strm;
		strm << "��" << m_sClassName 
			<< "��isolated ����ֻ������Ϊ������������ʹ�ã�������ע��callback�ص�����"
			<< endl;
		GenErr(strm.str());
	}

	//���Ȳ���ע��ĸ��������ǲ���Ҳ��ͬ���ĺ���������У���Ҫ�����ˣ�
	//�̳������治����ͬ����callback
	for( size_t i = 0; i < m_vpBaseRegist.size(); i++ )
	{
		_CheckRegCallBackFunc(m_vpBaseRegist[i], szFunName);
	}
	
	m_bIsCallBack = true;

	Ver(m_mapCallBackFunc.insert(make_pair(szFunName, pContext)).second);
}

bool CClass::IsCallBack()
{
	if( m_bIsCallBack)
		return true;
	size_t stSize = m_vpBaseRegist.size();
	for( size_t i = 0; i < stSize; ++i )
	{
		if( m_vpBaseRegist[i]->IsCallBack() )
		{
			//��Ϊ�ҵĸ�����callback�ģ���ô��Ҳ��callback��
			m_bIsCallBack = true;
			return true;
		}
	}
	return false;
}

void CClass::AddBaseClass( CClass* pRegist )
{
	if( pRegist )
	{
		m_vpBaseRegist.push_back( pRegist );
	}
}


bool CClass::FindBase( CClass* pRegistBase )
{
	if( pRegistBase == this )
		return true;

	size_t stSize = m_vpBaseRegist.size();
	for( size_t i = 0; i < stSize; i++ )
	{
		if( m_vpBaseRegist[i]->FindBase( pRegistBase ) )
		{
			return true;
		}
	}
	return false;
}


void CClass::SetClassRef(int32 iRef)
{
	m_iClassRef = iRef;
}

int32 CClass::GetClassRef()
{
	return m_iClassRef;
}
