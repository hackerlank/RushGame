#include "stdafx.h"
#include <hash_set>
#include "CGraphicExp.h"
#include "ErrLogHelper.h"
#include "ExpHelper.h"
#include "CComputerInfo.h"
#include "StringHelper.h"
#include "DebugHelper.h"
#define  GraphicFlag "ͼ����:"
#include "CPkgFile.h"
#include "CGraphic.h"

namespace sqr
{
	static stdext::hash_set<string> ExpPool;
	static string st_ErrState = "";
	static string st_ErrAudioState = "";
	
	void GfkLogExpOnce(CError& exp)
	{
		string Temp = exp.ErrorTitle();
		Temp += exp.ErrorMsg();
		if(ExpPool.find(Temp)==ExpPool.end())
		{
			ExpPool.insert(Temp);
			GfkLogExp(exp);
		}
	}

	void GfkLogAudioExpOnce(CError& exp)
	{
		string Temp = exp.ErrorTitle();
		if(ExpPool.find(Temp)==ExpPool.end())
		{
			ExpPool.insert(Temp);

			exp.AppendType(GraphicFlag,false);
			exp.AppendMsg(st_ErrAudioState + ":",false);

			LogExp(exp);
		}
	}

	void GfkLogErrOnce(const string& strType, const string& strMsg)
	{
		CError error(strType);
		error.AppendMsg(strMsg);
		GfkLogExpOnce(error);
	}

	void GfkLogAudioErrOnce(const string& strType, const string& strMsg)
	{
		CError error(strType);
		error.AppendMsg(strMsg);
		GfkLogAudioExpOnce(error);
	}

	void GfkLogErr(const string& strType, const string& strMsg)
	{
		CError error(strType);
		error.AppendMsg(strMsg);
		GfkLogExp(error);
	}

	void GfkLogExp(CError& exp)
	{
		exp.AppendType(GraphicFlag,false);
		exp.AppendMsg(st_ErrState + ":",false);

		LogExp(exp);
	}

	void GfkAudioLogExp(CError& exp)
	{
		exp.AppendType(GraphicFlag,false);
		exp.AppendMsg(st_ErrAudioState + ":",false);

		LogExp(exp);
	}

	void GfkDrvLog(const string& strType, const string& strMsg)
	{
		CError error(strType);
		error.AppendMsg(strMsg);
		error.SetCategory("�����Դ���");
		GfkLogExp(error);
	}

	static void ShowPkgUnknownErr( CError& exp, const char* szFileName )
	{
		string msg = format("���ذ��ļ���%s ʱ����δ֪����", szFileName);
		exp.AppendMsg(msg);
		GfkLogExpOnce(exp);
	}
	
	void GfkVerifyPkg( CError& inExp, const char* szFileName )
	{
		bool bAsync = CPkgFile::IsEnableAsyncRead();
		SQR_TRY
		{
			CPkgFile::EnableAsyncRead(false);
			CPkgFile tmpFile(PATH_ALIAS_RES.c_str(), szFileName);
			if ( !CheckPkgCrash(tmpFile) )
				GfkLogExp(inExp);
		}
		SQR_CATCH(exp)
		{
			exp.AppendType("У���ļ�ʧ��");
			GfkLogExp(exp);
		}
		SQR_TRY_END;
		CPkgFile::EnableAsyncRead(bAsync);
	}

	void GfkLogPkgErr( CError& exp, const char* szFileName, const CRefBufFile& refBuf )
	{
		if ( !CheckPkgCrash(refBuf) )
			ShowPkgUnknownErr(exp, szFileName);
	}

	void GfkLogPkgErr( CError& exp, const char* szFileName, CPkgFile& pkgFile )
	{
		if ( !CheckPkgCrash(pkgFile) )
			ShowPkgUnknownErr(exp, szFileName);
	}

	namespace GraphicErr
	{
		const string strGreateErr		= "���ʼ��ʧ��";
		const string strFileVerErr		= "�ļ��汾��ƥ��";
		const string strFileFormatErr	= "�޷�ʶ���ļ���ʽ";
		const string strFileReadErr		= "�ļ���ȡʧ��";
		const string strOutUseErr		= "ͼ�ο��ⲿʹ�ô���";
		const string strBufferUseErr	= "Bufferʹ�ô���";
		const string strFontErr			= "������ش���";	
		const string strMemoryErr		= "�ڴ治��";
		const string strTextureCreateErr= "����������";
		const string strTextureLockErr	= "��������ʧ��";
		const string strShaderInitErr	= "Shader��ʼ������";
		const string strShaderRuntimeErr= "Shader��������ش���";
		const string strRenderRuntimeErr= "ͼ�ο������ڴ���";
		const string strInvalidIndexErr	= "��ͼ��������";
		const string strAllocMemFailed	= "�ڴ����ʧ��";
		const string strCallBackErr		= "�ص���������";
		const string strPkgOpenErr		= "����ʧ��";
		const string strPkgDamagedErr	= "����";

		const string*			g_ErrMap[ErrCount];
		map<string , string*>	g_ErrInfoMap;

		const string strNullInfo = "�޸�����Ϣ"; 

		void  InitializeGraphicErr(void)
		{
			g_ErrMap[Create_Err]		= &strGreateErr;
			g_ErrMap[FileVer_Err]		= &strFileVerErr;
			g_ErrMap[FileFormat_Err]	= &strFileFormatErr;
			g_ErrMap[FileRead_Err]		= &strFileReadErr;
			g_ErrMap[OutUse_Err]		= &strOutUseErr;
			g_ErrMap[BufferUse_Err]		= &strBufferUseErr;
			g_ErrMap[Font_Err]			= &strFontErr;
			g_ErrMap[Memory_Err]		= &strMemoryErr;
			g_ErrMap[TextureCreate_Err] = &strTextureCreateErr;
			g_ErrMap[TextureLock_Err]	= &strTextureLockErr;
			g_ErrMap[ShaderInit_Err]	= &strShaderInitErr;
			g_ErrMap[ShaderRuntime_Err] = &strShaderRuntimeErr;
			g_ErrMap[RenderRuntime_Err] = &strRenderRuntimeErr;
			g_ErrMap[InvalidIndex_Err]	= &strInvalidIndexErr;
			g_ErrMap[AllocMemory_Err]	= &strAllocMemFailed;
			g_ErrMap[CallBack_Err]		= &strCallBackErr;
			g_ErrMap[PkgOpen_Err]		= &strPkgOpenErr;
			g_ErrMap[PkgDamaged_Err]	= &strPkgDamagedErr;
		}

		const string& GetErrTypeStr(GraphicErrType et)
		{
			return *g_ErrMap[et];
		}
		const string& GetErrInfo(GraphicErrType et)
		{
			return GetErrInfo(GetErrTypeStr(et));
		}

		const string& GetErrInfo(const string& ErrTypeStr)
		{
			return strNullInfo;
		}

		void SetErrState(const string& ErrState)
		{
			CGraphic::GetInst()->ClearFrameCount();
			st_ErrState = ErrState;
		}

		void SetErrAudioState(const string& ErrAudioState)
		{
			st_ErrAudioState = ErrAudioState;
		}
	}

}

