#include "stdafx.h"
#include "DataAccess.h"
#include "CScript.h"
#include "ScriptCommon.h"
#include "CScriptDebuger.h"
#include "CScriptData.h"
#include "CBaseType.h"
#include "CClass.h"
#include "CBaseObject.h"
#include "CDataType.h"
#include "ErrLogHelper.h"
#include "ExpHelper.h"
#include "ScriptCallBaseData.h"
#include "DataMake.h"
#include "CVariant.h"
#include "TSqrAllocator.inl"

//ѹջѹ4���ֽڵ�������
static inline size_t Alignit( size_t nLen )
{
	return nLen == 0 ? 0 : ( (nLen-1)/sizeof(size_t) + 1 )*sizeof(size_t); 
}

//===============================	CScriptProc 		===============================
CScriptProc::CScriptProc( CScript& Script, const char* szFormat, const char* szFunName )
: m_pData( new CallBaseData )
{	
	string FunName,parameter;
	if(szFunName != NULL)	
	{
		FunName = szFunName;
		parameter = szFormat;
	}
	m_pData->m_nReturnNum = 0;
	CScriptDataMaker Factory( &Script, szFormat );

	CallBaseData::VecType_t& vecParam=m_pData->m_vecParam;

	do
	{
		CBaseType *pData = Factory.MakeNextData( m_pData->m_uBufSize );
		if ( Factory.IsErr() )
		{
			ostringstream strm;
			strm<<"������"<<FunName<<" �����ַ�����ʽ����"<<" �����ַ�����"<<parameter;
			GenErr(strm.str());

		}

		if( pData!=NULL )
		{
			if( Factory.IsParam() )
			{
				if (pData->HasVariant())
				{
					m_pData->m_bHasVariant = true;
				}
				else if (pData->HasArgList())
				{
					m_pData->m_bHasArgList = true;
				}

				vecParam.insert( vecParam.begin() , pData );	//��ߵĲ�������list�������		
			}
			else if( Factory.IsResult() )
			{
				if (pData->HasArgList())
					m_pData->m_bRetArgList = true;
				
				m_pData->m_pResult = pData;
				++m_pData->m_nReturnNum;
			}
		}
	}
	while( !Factory.IsEnd() );
}

CScriptProc::~CScriptProc()
{
	CallBaseData::VecType_t::iterator i,end;
	i = m_pData->m_vecParam.begin();
	end = m_pData->m_vecParam.end();
	for(; i!=end; ++i)
		delete (*i);

	if( m_pData->m_pResult!=NULL )
		delete m_pData->m_pResult;

	delete m_pData;
}

size_t CScriptProc::GetParamBufSize()
{
	return m_pData->m_uBufSize;
}

//-------------------------------------------------------
//===============================	CFunction 		===============================
int32 CFunction::GetParamFromLuaStack( lua_State* pState, char* pDataBuf, bool bPack )
{
	CallBaseData::VecType_t::iterator it,end;
	it = m_pData->m_vecParam.begin();
	end = m_pData->m_vecParam.end();

	int32 nSize = 0;
	int32 nLen = 0;
	int32 nParamNum = (int32)m_pData->m_vecParam.size();

	//���пɱ�������ѿɱ�������ֵ����������ҿɱ��������λ�ڲ����б�֮��Ҳ����listparam�ĵ�һ��������
	if ( m_pData->m_bHasArgList )
	{
		//���ڿɱ������lua����unpack��ʱ�����Ϊ�գ���ʱ��top������һ��ֵ
		Ast(nParamNum <= lua_gettop(pState) + 1);

		Ast((*it)->HasArgList());
		CTypeArgList* pObj = static_cast<CTypeArgList*>((*it));
		pObj->GetArgListFromLuaStack(pState, nParamNum);
	}
	else
	{
		//lua��֧��Ĭ�ϲ��������Ե���c++�ĺ�����������ƥ�䣬���ע���˿ɱ亯����������ɱ�Ĵ���
		int32 nTop = lua_gettop(pState);
		if(nParamNum != nTop)
		{
			if(m_bClass)
			{
				nParamNum -= 1;
				nTop -= 1;
			}

			stringstream str;
			str << "lua����c++����" <<  m_sFunName.c_str() << " ������������ȷ��������Ҫ����: " << nParamNum;
			str << " ʵ�ʴ����������: " << nTop << endl;
			GenErr(str.str().c_str());
		}
	}

	int32 LuaStkId = nParamNum;
	SQR_TRY
	{
		for(; it != end; ++it, --LuaStkId )
		{

				(*it)->GetFromLuaStack( pState, pDataBuf, LuaStkId );			//Lua�������ұߵĲ�������Lua stack��ջ��,
				//����m_vecParam�ĵ�һ����Ա��
				if( bPack )
					nLen = (int32)(*it)->GetPackBufLen( pDataBuf );
				else
					nLen = (int32)Alignit( (int32)(*it)->GetLen() ); 

				nSize += nLen;
			}
	}
	SQR_CATCH(exp)
	{
		stringstream str;
		str <<  exp.ErrorMsg()<< " ��"<<LuaStkId<<"����������ȷ"<<endl;
		GenErr(exp.ErrorTitle(),str.str().c_str());
	}
	SQR_TRY_END;


#if defined(_WIN32)
	if( m_bClass && nSize )
		nSize -= nLen;
#endif

	return nSize;
}

void CFunction::FreeParamFromCppStack(char* pDataBuf)
{
	//���û��arglist��variant��ֱ�ӷ���
	if (!m_pData->m_bHasArgList && !m_pData->m_bHasVariant)
		return;
	
	size_t uNum = m_pData->m_vecParam.size();
	CallBaseData::VecType_t::iterator iter = m_pData->m_vecParam.begin();

	for( size_t i = 0; i < uNum - m_bClass; ++iter,++i )
	{
		if ((*iter)->HasArgList())
		{
			vector<CVariant*> *pArgList;
			(*iter)->GetValue(pDataBuf, &pArgList);
			CScript::Inst()->GetData()->ReleaseVariantList(pArgList);
			
		}
		else if((*iter)->HasVariant())
		{
			CVariant* pObj;
			(*iter)->GetValue(pDataBuf, &pObj);
			CScript::Inst()->GetData()->ReleaseVariant(pObj);
		}

	}
}

void CFunction::FillParamToCppStack( char* pDataBuf, char* pBuf, int32 nSize, bool bPack )
{
	//============		WARNING		================
	// gcc��,GetValue ���޸� pBufָ����ڴ棬��������pDataBuf, pBuf, nSize, bPackҲ�ᱻ�޸ģ�
	// ������Ҫ���Ᵽ����Щֵ,���Call��ʵ��

	char* pInDataBuf = pDataBuf;
	char* pInBuf = pBuf + nSize;
	bool  bInPack = bPack;
	//============		WARNING END		================

	size_t uNum = m_pData->m_vecParam.size();
	CallBaseData::VecType_t::iterator iter = m_pData->m_vecParam.begin();

	for( size_t i = 0; i < uNum - m_bClass; ++iter,++i )
	{
		size_t nLen = 0;
		if( bInPack )
		{
			pInBuf -= (int32)(*iter)->GetPackBufLen( pInDataBuf );
			(*iter)->GetPacker( pInDataBuf, pInBuf );
		}
		else
		{
			nLen = (*iter)->GetLen();
			nLen = nLen == 0 ? 0 : ( (nLen-1)/sizeof(size_t) + 1 )*sizeof(size_t); //ѹջѹ4���ֽڵ�������
			pInBuf -= nLen;//���ղ����б��෴˳��ѹ��cpp��ջ�����������������ȷ˳��
			(*iter)->GetValue( pInDataBuf, pInBuf );//��pInDataBuf��ָ���ֵѹ��pInBuf�ĵ�ַ��
		}
	}
}

int32 CFunction::PutResultToLuaStack( lua_State* pState, char* pDataBuf )
{
	lua_pop( pState, lua_gettop(pState) );

	CBaseType* pResult = m_pData->m_pResult;

	luaL_checkstack(pState, 1, "CFunction PutResultToLuaStack Error");
	
	if( pResult )
		pResult->Push2LuaStack( pState, pDataBuf );
	
	//�������ֵΪ�ɱ���������ؿɱ������Ŀ
	if( m_pData->m_bRetArgList )
	{
		CTypeArgList *pArgList = static_cast<CTypeArgList*>( pResult );
		return pArgList->ParamNumber();
	}

	return m_pData->m_nReturnNum;
}


//===============================	CScriptCallBackBase 		===============================

CBaseCallBack::CBaseCallBack( CScript& Script, const char* szFormat, const char* szFunName )
: CScriptProc( Script, szFormat, szFunName)
, m_sFunName( szFunName )
, m_pDefaultState( _L( Script ) )
{
	size_t nBufSize = 0;
	for( CallBaseData::VecType_t::iterator iter = m_pData->m_vecParam.begin(); 
		iter != m_pData->m_vecParam.end(); ++iter )
		nBufSize += Alignit( (*iter)->GetLen() );
	m_sArgBuf.resize( nBufSize );
}

CBaseCallBack::~CBaseCallBack()
{
}

void CBaseCallBack::GetFromCppStack( char* pDataBuf )
{
	char* pArg = (char*)m_sArgBuf.c_str();
	CallBaseData::VecType_t::reverse_iterator iter,end;
	iter = m_pData->m_vecParam.rbegin();
	end = m_pData->m_vecParam.rend();

	for( ; iter!= end; ++iter )
	{
		(*iter)->SetValue( pDataBuf, pArg );
		pArg += Alignit( (*iter)->GetLen() );
	}	
}

void CBaseCallBack::PushParam2LuaStack( lua_State* pState, char* pDataBuf )
{
	CallBaseData::VecType_t::reverse_iterator iter,end;
	iter = m_pData->m_vecParam.rbegin();
	end = m_pData->m_vecParam.rend();

	for(; iter!= end; ++iter )
		(*iter)->Push2LuaStack( pState, pDataBuf );
}


//--------------------------------------------------------------------------------

static int32 FindTypeByFormat(const char* str)
{
	static map<const char*, int32> ls_mapStr2Type;
	if(ls_mapStr2Type.empty())
	{
		ls_mapStr2Type[(typeid(CTypeInt8).name())] = eFPT_Int8;
		ls_mapStr2Type[(typeid(CTypeInt16).name())] = eFPT_Int16;
		ls_mapStr2Type[(typeid(CTypeInt32).name())] = eFPT_Int32;
		ls_mapStr2Type[(typeid(CTypeInt64).name())] = eFPT_Int64;
		ls_mapStr2Type[(typeid(CTypeUint8).name())] = eFPT_UInt8;
		ls_mapStr2Type[(typeid(CTypeUint16).name())] = eFPT_UInt16;
		ls_mapStr2Type[(typeid(CTypeUint32).name())] = eFPT_UInt32;
		ls_mapStr2Type[(typeid(CTypeUint64).name())] = eFPT_UInt64;
		ls_mapStr2Type[(typeid(CTypeFloat).name())] = eFPT_Float;
		ls_mapStr2Type[(typeid(CTypeDouble).name())] = eFPT_Double;
		ls_mapStr2Type[(typeid(CTypeBool).name())] = eFPT_Bool;
		
		if(sizeof(size_t) == 4)
			ls_mapStr2Type[(typeid(CTypeSize).name())] = eFPT_UInt32;
		else if(sizeof(size_t) == 8)
			ls_mapStr2Type[(typeid(CTypeSize).name())] = eFPT_UInt64;		
	}

	map<const char*, int32>::iterator iter = ls_mapStr2Type.find(str);
	if( iter != ls_mapStr2Type.end())
	{
		return iter->second;
	}

	return eFPT_Ptr;
}

template<typename BaseType>
CBaseFuncCall<BaseType>::CBaseFuncCall(CScript& Script, const char* szFormat, 
									   const char* szFunName)
									   : BaseType(Script, szFormat,szFunName)
{
	CallBaseData::VecType_t::reverse_iterator iter = this->m_pData->m_vecParam.rbegin();
	while(iter != this->m_pData->m_vecParam.rend())
	{
		CBaseType* pType = (*iter);
		size_t stSize = pType->GetOffset();
		m_vecParamOffset.push_back(stSize);

		int32 type = FindTypeByFormat(typeid(*(*iter)).name());
		m_vecParamType.push_back(type);

		size_t stLen = pType->GetLen();
		m_vecParamLen.push_back(stLen);

		++iter;
	}

	if(this->m_pData->m_pResult)
	{
		m_stRetOffset = this->m_pData->m_pResult->GetOffset();
		m_iRetType = FindTypeByFormat(typeid(*(this->m_pData->m_pResult)).name());
		m_stRetLen = this->m_pData->m_pResult->GetLen();
	}
	else
	{
		m_stRetOffset = 0;
		m_iRetType = eFPT_Void;
		m_stRetLen = 0;
	}
}

//��ʾʵ����
template class CBaseFuncCall<CFunction>;
template class CBaseFuncCall<CBaseCallBack>;


//-------------------------------------------------------------------
// ������Ա����
//-------------------------------------------------------------------
CMemberVariable::CMemberVariable( CScript& Script, const char* szFormat, int32 nOff )
: CScriptProc( Script, szFormat, NULL), m_nOffset(nOff)
{
	Ast( m_pData->m_vecParam.size() == 2 );
	(*m_pData->m_vecParam.begin())->SetOffset(0);
	(*m_pData->m_vecParam.rbegin())->SetOffset(0);

	CBaseType* pMember = (*m_pData->m_vecParam.begin());
	if(pMember->IsCppObj())
	{
		CTypeCppObj* pObj = static_cast<CTypeCppObj*>(pMember);
		ostringstream strm;
		strm << "ע����� (" << pObj->GetClassName() << ") ����" << endl;
		strm << "������Ա����ֻ�����л������������ͣ��������������" << endl;
		GenErr(strm.str().c_str());
	}
}

CMemberVariable::~CMemberVariable()
{
}

int32 CMemberVariable::GetCppMemberVar( lua_State* pState )
{
	SQR_TRY
	{
		CMemberVariable* pScript	= (CMemberVariable*)lua_touserdata( pState, lua_upvalueindex(1) );
		CBaseType* pMember		= *pScript->m_pData->m_vecParam.begin();
		CBaseType* pObject		= *pScript->m_pData->m_vecParam.rbegin();
		char* pDataBuf			= NULL;

		pObject->GetFromLuaStack( pState, (char*)&pDataBuf, -2 );
		pDataBuf += pScript->m_nOffset;
		if( !lua_isnil( pState, -1 ) )
			pMember->GetFromLuaStack( pState, pDataBuf, -1 );
		lua_pop( pState, 2 );
		pMember->Push2LuaStack( pState, pDataBuf );
	}
	SQR_CATCH(exp)
	{
		if (!CatchErrorEnabled())
		{
			GenErr(exp.ErrorTitle());
		}	
		else
		{
			LogExp(exp);
			return 0;
		}
	}
	SQR_TRY_END;
	return 1;
}

int32 CMemberVariable::IsCppMemberVar( lua_State* pState )
{
	bool bIsCppMemberVar = lua_tocfunction( pState, -1 ) == (lua_CFunction)CMemberVariable::GetCppMemberVar;
	lua_pop( pState, 1 );
	lua_pushboolean( pState, bIsCppMemberVar );
	return 1;
}
