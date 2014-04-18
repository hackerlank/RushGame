// D3d.h: interface for the CD3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFXSQRD3D_H__D5890401_B53C_11D6_96FF_5254AB25D0BE__INCLUDED_)
#define AFXSQRD3D_H__D5890401_B53C_11D6_96FF_5254AB25D0BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#include "../../src/Graphic/Graphic_dx9/Dx9Base.h"
#include "GraphicBase.h"

#endif // _MSC_VER > 1000
#define D3DOK_RESET			MAKE_D3DSTATUS(1)
#define D3DOK_RECREATE		MAKE_D3DSTATUS(2)

struct UpdateRes
{
	virtual LRESULT RestoreRes()=0;
	virtual LRESULT ReleaseRes()=0;
};

class CD3d  
{
	//D3D�豸���
	LPDIRECT3D9           m_lpD3d;              //D3D�ӿ�
	LPDIRDEV              m_lpD3DDc;            //D3D�豸

	//�豸����
	D3DPRS_PARAM          m_PresentParam;       //������ʾ��ʽ
	D3DCAPS               m_D3dCaps;            //�豸����
	DWORD                 m_VertexProcess;      //���㴦��ʽ
	D3DDEVTYPE            m_DeviceType;         //�豸����

	RENDSTATE             m_FogMode;            //���ģʽ
	D3DFORMAT             m_TexMode;            //��ͼ��ʽ
	DWORD                 m_FogColor;           //�����ɫ

	int                   m_DisModeNum;         //��ʾģʽ����
	D3DDISMD*             m_DisList;            //��ʾģʽ�б�
	D3DDISMD              m_CurDis;             //��ǰ��ʾģʽ
	HWND                  m_hWnd;               //Ӧ�ó��������ھ��
	UpdateRes*            m_UpdateRes;          //Ӧ�ó���ָ��

	//���뵭��
	LPVERBUF              m_FadeVer;            //���뵭�����ö���
	DWORD                 m_FadeColor;          //���뵭��������ɫ
	DWORD                 m_FadeTime;           //���뵭����ʼʱ��
	DWORD                 m_FadePeriod;         //���뵭������ʱ��
	FADE_MODE             m_FadeMode;           //���뵭�����÷�ʽ

public:
	//d3dd.cpp
	int CreateD3d(HWND hWnd,UpdateRes* UDRes,int Width=0,int Height=0,
		D3DFORMAT Format=D3DFMT_R5G6B5,D3DDEVTYPE type=D3DDEVTYPE_HAL);
	int ReleaseD3d();
	int ListDisPlayMode();
	int GetDisPlayMode(D3DDISPLAYMODE& mode);
	int SetEnvironment();
	int ResetD3d(int Width=0,int Height=0,D3DFORMAT Format=D3DFMT_R5G6B5);

	int CreateUtilRes();
	int ReleaseUtilRes();
		
	int GetDisModeNum();
	D3DDISMD* GetDisMode(int num=-1);
	D3DFORMAT GetTextFormat();
	LPDIRDEV  GetDevice();
	D3DCAPS&  GetCaps();
	
	int RenderBegin();
	int RenderEnd();
	int RendFade();
	
	int GetTransform(TANSSTATE tanstate,D3DMATRIX* mat);
	int SetTransform(TANSSTATE tanstate,D3DMATRIX* mat);
	
	int SetFVF(DWORD FVF);
	int SetFade(FADE_MODE mode,DWORD color=0x00000000,DWORD period=1000);
	int SetFog(DWORD color=0,float start=0,float end=0,float density=0);
	int GetFogColor();
	int SetTextAlpha(BYTE Enable=1);
	int SetTexture(int stage,LPDIRTEX text);
	int SetStreamSource(UINT streamnum,LPVERBUF verbuf,UINT Off,UINT stride);
	int SetIndices(LPINDBUF indbuf);
	int SetRenderState( D3DRENDERSTATETYPE State, DWORD Value );
	int SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
	int SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value );

	int SetVertexShaderConstantF( UINT streamnum, float* pData, UINT Vector4fCount );
	int SetVexBlend();

	int DrawIndexedPrimitive(VERTYPE Type,INT BaseVerInd,UINT mindex,UINT vernum,UINT startind,UINT polygen);
	int DrawPrimitive(VERTYPE Type,UINT StartVer,UINT VerNum);

	CD3d();
	virtual ~CD3d();
};
	
////////////////////////////////////////////////////////////////////////////
// ���ܣ�Ԥ����Ⱦ
// ��������
// ���أ�>=0 �ɹ���<0 ʧ��
///////////////////////////////////////////////////////////////////////////
inline int CD3d::RenderBegin()
{ 
	m_lpD3DDc->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,m_FogColor,1.0f,0);
	
	HRESULT hr;
	if( FAILED( hr = m_lpD3DDc->TestCooperativeLevel() ) )
    {
        // ��D3DDevice��ʧʱ,Ҫ�ȵ��豸��Ч�󷽿ɼ���
        if( D3DERR_DEVICELOST==hr )
            return hr;
		
        // Reset D3DDevice
        if( D3DERR_DEVICENOTRESET==hr )
        {
			// ��Resetǰ����Ҫ�ͷ���Դ(һЩ����Lost����bmp��)
			ReleaseUtilRes();

            if( FAILED( hr=m_lpD3DDc->Reset(&m_PresentParam) ) ) 
			{
				return hr;
			}

			// ��Reset�����Ҫ����������Դ(��ReleaseResource�ͷŵ���)
			SetEnvironment();
			CreateUtilRes();
		}

        return hr;
    }

	return m_lpD3DDc->BeginScene(); 
}

/////////////////////////////////////////////////////////////////
// ���ܣ�������Ⱦ
// ��������
// ���أ�>=0 �ɹ���<0 ʧ��
//////////////////////////////////////////////////////////////////
inline int CD3d::RenderEnd()
{ 
	m_lpD3DDc->EndScene(); 
	return m_lpD3DDc->Present(NULL,NULL,NULL,NULL); 
}

/////////////////////////////////////////////////////////////////
// ���ܣ����õ��뵭������
// ������FADE_MODE mode ��ʽ,DWORD color ��ɫ,DWORD period ʱ��
// ���أ�>=0 �ɹ���<0 ʧ��
//////////////////////////////////////////////////////////////////
inline int CD3d::SetFade(FADE_MODE mode,DWORD color,DWORD period)
{
	m_FadeMode=mode;
	m_FadeTime=GetTickCount();
	m_FadeColor=((m_FadeMode<0)?0xff000000:0)|(color&0x00ffffff);
	m_FadePeriod=period;
	return 0;
}

/////////////////////////////////////////////////////////////////
// ���ܣ��õ��豸����
// ��������
// ���أ��豸����
//////////////////////////////////////////////////////////////////
inline D3DCAPS& CD3d::GetCaps()
{
	return m_D3dCaps;
}

/////////////////////////////////////////////////////////////////
// ���ܣ��õ���ʾģʽ����
// ��������
// ���أ���ʾģʽ����
//////////////////////////////////////////////////////////////////
inline int CD3d::GetDisModeNum()
{
	return m_DisModeNum;
}

/////////////////////////////////////////////////////////////////
// ���ܣ��õ���ʾģʽ
// ������int num <0��ǰ��ʾģʽ��>=0�б����ʾģʽ
// ���أ���ʾģʽ�����ɹ�����NULL
//////////////////////////////////////////////////////////////////
inline D3DDISMD* CD3d::GetDisMode(int num)
{
	return (num<0)?&m_CurDis:((num<m_DisModeNum)?&m_DisList[num]:NULL);
}

/////////////////////////////////////////////////////////////////
// ���ܣ��õ��ʺϵ���ͼ��ʽ
// ��������
// ���أ���ͼ��ʽ
//////////////////////////////////////////////////////////////////
inline D3DFORMAT CD3d::GetTextFormat()
{ 
	return m_TexMode; 
}

//////////////////////////////////////////////////////////////////////////////////////////////
//���溯���ṩ�����Աm_lpD3DDc�ĺ�������
//////////////////////////////////////////////////////////////////////////////////////////////
inline LPDIRDEV CD3d::GetDevice()
{ 
	return m_lpD3DDc; 
}

inline int CD3d::GetTransform(TANSSTATE Transtate,D3DMATRIX* Mat)
{
	return m_lpD3DDc->GetTransform(Transtate,Mat);
}

inline int CD3d::SetFVF(DWORD FVF)
{
	return m_lpD3DDc->SetFVF(FVF);
}

inline int CD3d::SetFog(DWORD color,float start,float end,float density)
{
	if(m_FogMode==D3DRS_FORCE_DWORD)
		return 1;
	if(color)
		m_FogColor=color;
	if(start==0&&end==0&&density==0)
		return m_lpD3DDc->SetRenderState(D3DRS_FOGENABLE,FALSE);
	m_lpD3DDc->SetRenderState(D3DRS_FOGENABLE,TRUE);
	m_lpD3DDc->SetRenderState(D3DRS_FOGSTART,*((DWORD*)&start));
    m_lpD3DDc->SetRenderState(D3DRS_FOGEND,*((DWORD*)&end));
    m_lpD3DDc->SetRenderState(D3DRS_FOGDENSITY,*((DWORD*)&density));
    m_lpD3DDc->SetRenderState(D3DRS_FOGCOLOR,color);
    m_lpD3DDc->SetRenderState(m_FogMode,D3DFOG_LINEAR);
    m_lpD3DDc->SetRenderState(D3DRS_RANGEFOGENABLE,0);

	return 0;
}

inline int CD3d::GetFogColor()
{
	return m_FogColor;
}

inline int CD3d::SetTexture(int stage,LPDIRTEX text)
{
	return m_lpD3DDc->SetTexture(stage,text);
}

inline int CD3d::SetTextAlpha(BYTE Enable)
{
	if(Enable)
	{
        m_lpD3DDc->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		m_lpD3DDc->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		m_lpD3DDc->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
	}
	else
		m_lpD3DDc->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	return 0; 
}

inline int CD3d::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	return m_lpD3DDc->SetRenderState( State, Value );
}

inline int CD3d::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
{
	return m_lpD3DDc->SetSamplerState(Sampler,Type,Value);
}

inline int CD3d::SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	return m_lpD3DDc->SetTextureStageState(Stage,Type,Value);
}

inline int CD3d::SetTransform(TANSSTATE Transtate,D3DMATRIX* Mat)
{
	return m_lpD3DDc->SetTransform(Transtate,Mat);
}

inline int CD3d::SetStreamSource(UINT StreamNum,LPVERBUF VerBuf,UINT Off,UINT Stride)
{
	return m_lpD3DDc->SetStreamSource(StreamNum,VerBuf,Off,Stride);
}

inline int CD3d::SetIndices(LPINDBUF IndBuf)
{
	return m_lpD3DDc->SetIndices(IndBuf);
}

inline int CD3d::DrawIndexedPrimitive(VERTYPE Type,INT BaseVerInd,UINT MinInd,UINT VerNum,UINT StartInd,UINT Polygen)
{
	return m_lpD3DDc->DrawIndexedPrimitive(Type,BaseVerInd,MinInd,VerNum,StartInd,Polygen);
}

inline int CD3d::DrawPrimitive(VERTYPE Type,UINT StartVer,UINT VerNum)
{
	return m_lpD3DDc->DrawPrimitive(Type,StartVer,VerNum);
}

//////////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFXSQRD3D_H__D5890401_B53C_11D6_96FF_5254AB25D0BE__INCLUDED_)
