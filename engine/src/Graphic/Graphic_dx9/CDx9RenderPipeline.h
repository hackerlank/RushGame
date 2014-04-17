#pragma once
#include "Dx9Base.h"
#include "CRenderGroup.h"
#include "CRenderPipeline.h"
#include "CDx9Adapter.h"

//�����������ŵ�DX9�ɼ���Χ�ڹ���ʹ��
//�Ժ������ɾ����
#define ToD3DMatrix(u)	type_cast<D3DXMATRIX>(u)
#define ToD3DVector(u)	type_cast<D3DXVECTOR3>(u)
#define ToD3DPlane(u)	type_cast<D3DXPLANE>(u)
#define ToCMatrix(u)	type_cast<CMatrix>(u)
#define ToCVector(u)	type_cast<CVector3f>(u)
#define ToCPlane(u)		type_cast<CPlane>(u)
#define ToDword(u)		type_cast<DWORD>(u)

//#define MaxBufferNum 8//������ǿ��Գ���������֣��Ǵ��������Ѿ���NB��
namespace sqr
{
class CCamera;
struct RenderParam;

class DX9RenderPipeline : public CRenderPipeline
{
public:
	friend class DX9Graphic;
public:
	DX9RenderPipeline(void);
	~DX9RenderPipeline(void);

	bool Begin(void);
	void End(void);

	void InitPipeline(void);
	void ReleasePipeline(void);
	void SetVertexShaderF(uint id,float Param1 = 0.0f,float Param2 = 0.0f,float Param3 = 0.0f,float Param4 = 0.0f);
	void SetVertexShaderF(uint id,const void* pdata,size_t float4Count);
	void GetVertexShaderF(uint id,void* pdata,size_t float4Count);
	void SetFragmentShaderF(uint id,float Param1 = 0.0f,float Param2 = 0.0f,float Param3 = 0.0f,float Param4 = 0.0f);
	void SetFragmentShaderF(uint id,const void* pdata,size_t float4Count);
	void GetFragmentShaderF(uint id,void* pdata,size_t float4Count);

	void SetTextureMatrixImmediate( uint32 uTextStage, const CMatrix* pMatrix );
	void ClearDepth(float depth = 1.0f);
	void ClearRender( const CColor& color );
	void ClearTarget( CRenderTarget* pTarget );
	void SetViewport( CIRect& Rect );

public:
	//��������CDynamicVB
	void SetZDelta( float Z );
	void SetAmbient( uint32 Color );
	void SetAmbientSrcAsMaterial( bool bMaterial );
	void ScreenShot( const char* pStr );
	/// ֻΪ�˼��ݹ̶����߶�����, light=NULLʱ�ر�
	void SetLight(index_t lightIndex, const LightStruct* light);
	void ResetLightAttribute();
	void UpdateFog();

	//��ʱ����
	bool boxProbe( const CVector3f& Min, const CVector3f& Max, const CVector3f& RayPosition, const CVector3f& RayDirection);
	/// ��֪��D3D��inverseզд��, �Լ�д�ľ��Ǹ����Ľ����һ��
	CMatrix& MatrixInverse(CMatrix& matrix);


	//֧�ֶ�������Ϊ����
	void _setVertexDeclaration(CVertexDeclaration* decl);
	void _setVertexBufferBinding(CVertexBufferBinding* binding);
	void _Flush(void);
public:
	void _SetRenderStyle( const RENDERSTATETYPE& rsType, DWORD fValue );
	void _SetSamplerState( uint i,const SAMPLERSTATETYPE& ssType, DWORD fValue );
	void _SetTextureStageState( uint i,const TEXTURESTAGESTATETYPE& tsType, DWORD fValue );

	void _SetTexture( uint i, ITexture* pText );
	void _SetMaterial( const sMaterial& material );
	void _SetClipPlane( uint i,const CPlane& Plane );
protected:
	bool	_BeginBatch(void);
	bool	_SetBatchBuffer(UINT Source, BufferBindingInfo* pVBInfo, DWORD Flag);
	bool	_EndBatch(void);
	bool	_Render(CRenderOperation* op);
	void	_UpdateView(void);

private:
	AdapterList			m_listAdapter;			///< ��ʾ�������б�
	LPDIRECT3DDEVICE9	m_pD3DDevice;
	IDirect3DQuery9*	m_pQuery;
	IDirect3DQuery9*	m_pQueryOcclusion;
	
public:
	static DWORD						get(Usage usage);
	static DWORD						get(LockOptions options, Usage usage);
	static D3DDECLTYPE					get(CVertexElementType vType);
	static D3DDECLUSAGE					get(CVertexElementSemantic sem);
	static D3DFORMAT					get(_texFORMAT format);
	static D3DXMATRIX&					get(const CMatrix& mat );
	static D3DRENDERSTATETYPE&			get( const RENDERSTATETYPE& rst );
	static D3DSAMPLERSTATETYPE&			get( const SAMPLERSTATETYPE& sst );
	static D3DTEXTURESTAGESTATETYPE&	get( const TEXTURESTAGESTATETYPE& tst );
	static D3DMATERIAL&					get( const sMaterial& material );
};
}