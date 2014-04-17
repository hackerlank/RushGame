#pragma once
#include <string>
#include <vector>
#include "Script.h"
#include "ScriptDef.h"
#include "CDataFormat.h"
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "lua/luafile.h"

static const int32 PROTO_OK = 0;
static const int32 PROTO_ERROR = -1;



// Զ�̵��ø�ʽ����
class CProtoManager;

class SCRIPT_API CNetProtocol
{
	friend class CProtoManager;

private:
	std::vector<CCallBassArg*> m_vecArg;	// �����б�
	std::string m_strFuncName;				// ����ӿ����ƣ�������ʵ��
	int32 m_uUnpackRef;						// ����ӿ����ã��ű���ʵ��
	uint16 m_uId;							// Э��id���й������Զ�����
	bool  m_bIsMaker;						// �Ƿ�ΪԶ�̺���������
	CProtoManager * m_pProMgr;
protected:

public:
	CNetProtocol(uint16 uId,CProtoManager *pProMgr);
	~CNetProtocol();

	int32 UnPack(lua_State*L,const char* pBuf,int32 nLeftLen);
	void Load(const char * pFileName,lua_State* L);
	void ProcessType(const char* path);
	void ConvertProtocol(lua_State*L);
	void FuncProc(lua_State* L);
	void ArglistProc(lua_State* L);
protected:
	static int32 Pack(lua_State* L); // ���Э���������Զ�ע����lua������й�Զ�̵�����ʹ��

};


// Э�������
class SCRIPT_API CProtoManager
{
	friend class CNetProtocol;

private:
	std::vector<CNetProtocol*> m_Protocols;
	IPipe * m_IPipe;
	lua_State* m_L;
public:
	CProtoManager(IPipe * pIPipe,lua_State* L);
	~CProtoManager();

	// ���������ӿ�
	int32 UnpackData();
	void SendError(std::string const & strError = "Write error");
	void ReadError(std::string const & strError = "Read error");
	// ����Ľű��ӿ�
//		static int add_arg_type(lua_State*L); // ջ��Ϊ���ݸ�ʽ�����ļ�·��
	void AddProtocol(const char * pFileName);
};


