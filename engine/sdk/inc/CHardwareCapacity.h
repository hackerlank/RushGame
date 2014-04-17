#pragma once
#include "GraphicBase.h"
#include "TSingleton.h"

namespace sqr
{
	class CHardwareCapacity 
		: public Singleton<CHardwareCapacity>
		, public CGraphicMallocObject
	{
	public:
		/// �Կ�����
		enum EVendorType
		{
			VT_NVIDIA,
			VT_ATI,
			VT_INTEL,
			VT_OTHER
		};

	public:
		CHardwareCapacity()
			: vendor(VT_OTHER)
			, VideoMem(0)
		{}
		/// �Ƿ�֧��Ӳ��T&L(intel��Щ̫�࿨��֧��)
		virtual bool IsSupportHwTandL();

		/// ���ⶥ���ʽ֧��, ��D3DDTCAPS_UBYTE4֮��
		virtual bool IsSupportVertexDeclType(DWORD type);
		/// ���Ķ�������, ��16/32λ��index buffer
		virtual uint GetMaxVertexIndex();

		/// ��������
		virtual uint GetMaxTextureWidth();
		/// ��������
		virtual uint GetMaxTextureHeight();
		/// �Ƿ�����Զ�����mipmap(Ҳ��intel�ı��ò���)
		virtual bool IsTextureCanAutoGenMipmap();
		/// �Ƿ�������֧�ַ�2^n��С������, �������, ��Ҫ�����⼸������:
		/// 1. D3DTADDRESS_CLAMPѰַ 2. D3DRS_WRAP���� 3. ��ʹ��Mipmap 4. ��DXTn��ʽ
		virtual bool IsTextureSupportNonPowerOf2();

		/// ���Vertex Shader�汾
		virtual bool IsSupportVS(uint major, uint minor);
		/// ���Pixel Shader�汾
		virtual bool IsSupportPS(uint major, uint minor);
		/// �Ƿ�֧��������֧�������, ��if
		virtual uint GetVS2DynamicFlowControlDepth();
		virtual uint GetPS2DynamicFlowControlDepth();
		/// �Ƿ�֧�־�̬���̿������, ��for
		virtual uint GetVS2StaticFlowControlDepth();
		virtual uint GetPS2StaticFlowControlDepth();

		/// ���ָ�������ʽ�Ƿ�֧��
		virtual bool IsSupportTexture(texFORMAT format);
		/// ���ָ��RT��ʽ�Ƿ�֧��
		virtual bool IsSupportRenderTarget(texFORMAT format);

		virtual bool IsSupportDepthTexture(texFORMAT format);

		/// ����Ƿ�֧��Ӳ��ShadowMap, ������������ʽ. ��֧�ֵķ���D3DFMT_UNKNOWN
		virtual texFORMAT GetHwDepthTextureFormat() const;

		/// Multi-RenderTarget���(�Ⱥ�����)
		//@{
		//uint GetNumSimultaneousRTs();
		//bool IsSupportMRTBitDepths();
		//bool IsSupportMRTPostPixelShaderBlending();
		//@}

		/// �Ƿ�֧�ַ����(����intel�Ĳ�֧��)
		virtual bool IsCanAntiAlias(texFORMAT format);

		/// ȡ���Կ�����
		EVendorType GetVendor() const;
		virtual void Initialize();
		UINT		GetVideoMem() const;
		UINT		GetAGPMem() const;

	protected:
		EVendorType vendor;
		UINT		VideoMem;
		UINT		AGPMem;
	};

	inline CHardwareCapacity::EVendorType CHardwareCapacity::GetVendor() const
	{
		return vendor;
	}

	inline UINT CHardwareCapacity::GetVideoMem() const
	{
		return VideoMem;
	}

	inline UINT CHardwareCapacity::GetAGPMem() const
	{
		return AGPMem;
	}
}