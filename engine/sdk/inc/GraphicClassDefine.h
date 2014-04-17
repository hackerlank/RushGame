#pragma once
#include "CGraphicMallocObject.h"

namespace sqr
{
	//Interface
	class IModelHandler;
	class CDataSources;
	class IWater;
	class CRenderFilter;

	class IGraphic;
	class CRendererBase;
	class CRenderStyle;
	class CGpuProgramMgr;

	class CResUnitGroup;
	class CResUintNode;
	class ITexture;
	class IResTexture;
	class CRenderFont;
	class CPiece;
	class CPieceGroup;
	
	//class
	class CHardwareBufferMgr;
	class CHardwareBuffer;
	class CIndexHardwareBuffer;
	class CVertexHardwareBuffer;
	class CShadowFilter;
	class CRenderGroup;
	class CPieceRenderStyle;
	class CRenderPipeline;
	class FilterNode;

	class CDynamicVB;

	class CViewport;
	class RenderSettings;
	class CWindowTarget;	
	class CRenderTarget;
	class CRenderTexture;
	
	class CVertexDeclaration;

	class CLightSystem;
	struct LightStruct;
	class CLight;
	
	class CGraphic;
	class CCamera;
	
	class CBaseModel;
	class CModelInst;
	class CModel;
	class CMesh;
	class CSkeletal;
	class CSkeletalFrame;
	class CSkeletalUnit;
	class CMatrixUnit;
	class CMatrixUnit;
	class IDNAME;
	class CAnimate;
	class CAnimationGroup;
	//class CAnimationsRes;
	class CAnimationController;
	class CAnimateSeparation;
	struct CRenderMatrix;
	struct SKELETAL_MASK;
	class CRenderPiece;
	

	class CResourceContainer;
	class CResourceRecycle;
	typedef GVector<CMatrixUnit*>			CMatrixUnitPtrVector;
	typedef GVector<CAnimateSeparation*>	CAnimateSeparationPtrVector;
	typedef GVector<CAnimationController*>	CAnimationControllerPtrVector;

	///����������÷�������֮�䲻�໥�ų�
	enum Usage
	{
		/** ��̬���棬һ������Ӧ�õ�ʱ��������ᱻ�޸ġ��޸�������������ݰ���performace hit
		*/
		HBU_STATIC = 1,
		/**Ӧ���������˵��CPU���Ծ����޸���������������ݡ�
		ʹ��������Դ����Ļ�����AGP�ڴ�(3D���ٻ���)��Ч�ʽ��������һ���ڴ档
		*/
		HBU_DYNAMIC = 2,
		/**Ӧ��������ԵĻ������е������ǲ���ͨ���������ر���ȡ�ģ���ֻ�ܲ��ϵ�д�����ݡ�
		����һ�������Ļ��������ǻ᷵��һ��ָ���������ָ�룬�հ׵��ڴ�����ӵ��ԭ
		�������Ļ���������Խ�����������˶Դ洢��ֱ�Ӵ�ȡ��Ϊ�������ʹ��ԭ�������ݵ�ͬʱ
		д��һ���µ����ݡ�
		*/
		HBU_WRITE_ONLY = 4,
		/// �����HBU_STATIC �� HBU_WRITE_ONLY ���ص�
		HBU_STATIC_WRITE_ONLY = 5,
		/// �����HBU_DYNAMIC �� HBU_WRITE_ONLY ���ص�
		HBU_DYNAMIC_WRITE_ONLY = 6
	};

	//coder
	class CFileFormatMgr;
	
	template<typename Type>
	inline void GfkSafeDelete(Type& p)
	{
		Type t = p;
		p = NULL;
		delete t;
	}
}