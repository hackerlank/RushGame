#pragma once
//------------------------------------------------------------------------------
/**
	@class CDx9DeviceCapacity

	���ڼ���Կ��ļ�����.
	����Ϸ������ShaderModel2.0���ϵ��Կ�, ����һЩ���Կ���֧�ֵ����ԾͲ������.
	����һЩ�����û򲻱�׼������(��N����A��ֻ��һ��֧��), Ҳ�����
	�󲿷ּ�������������intel�ļ����Կ���, ��ʼǱ����û�Ҳ��С, ���ò����...

	(C) 2009 ThreeOGCMan
*/
#include "CHardwareCapacity.h"
#include "Dx9Base.h"

namespace sqr
{

class Dx9Adapter;
class CDx9DeviceCapacity : public CHardwareCapacity
{

public:
	/// �Ƿ�֧��Ӳ��T&L(intel��Щ̫�࿨��֧��)
	bool IsSupportHwTandL();

	/// ���ⶥ���ʽ֧��, ��D3DDTCAPS_UBYTE4֮��
	bool IsSupportVertexDeclType(DWORD type);
	/// ���Ķ�������, ��16/32λ��index buffer
	uint GetMaxVertexIndex();

	/// ��������
	uint GetMaxTextureWidth();
	/// ��������
	uint GetMaxTextureHeight();
	/// �Ƿ�����Զ�����mipmap(Ҳ��intel�ı��ò���)
	bool IsTextureCanAutoGenMipmap();
	/// �Ƿ�������֧�ַ�2^n��С������, �������, ��Ҫ�����⼸������:
	/// 1. D3DTADDRESS_CLAMPѰַ 2. D3DRS_WRAP���� 3. ��ʹ��Mipmap 4. ��DXTn��ʽ
	bool IsTextureSupportNonPowerOf2();

	/// ���Vertex Shader�汾
	bool IsSupportVS(uint major, uint minor);
	/// ���Pixel Shader�汾
	bool IsSupportPS(uint major, uint minor);
	/// �Ƿ�֧��������֧�������, ��if
	uint GetVS2DynamicFlowControlDepth();
	uint GetPS2DynamicFlowControlDepth();
	/// �Ƿ�֧�־�̬���̿������, ��for
	uint GetVS2StaticFlowControlDepth();
	uint GetPS2StaticFlowControlDepth();

	/// ���ָ�������ʽ�Ƿ�֧��
	bool IsSupportTexture(texFORMAT format);
	/// ���ָ��RT��ʽ�Ƿ�֧��
	bool IsSupportRenderTarget(texFORMAT format);

	bool IsSupportDepthTexture(texFORMAT format);

	/// ����Ƿ�֧��Ӳ��ShadowMap, ������������ʽ. ��֧�ֵķ���D3DFMT_UNKNOWN
	texFORMAT GetHwDepthTextureFormat() const;

	/// Multi-RenderTarget���(�Ⱥ�����)
	//@{
	//uint GetNumSimultaneousRTs();
	//bool IsSupportMRTBitDepths();
	//bool IsSupportMRTPostPixelShaderBlending();
	//@}

	/// �Ƿ�֧�ַ����(����intel�Ĳ�֧��)
	bool IsCanAntiAlias(texFORMAT format);

	void Initialize();
private:

	friend class DX9Graphic;
	friend class DX9MainWindowTarget;

	IDirect3D9* d3d9;
	D3DCAPS9	d3dcaps;
	Dx9Adapter* adapter;
};

//------------------------------------------------------------------------------

}// namespace sqr

