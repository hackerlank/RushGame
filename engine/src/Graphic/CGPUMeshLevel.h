//-------------------------------------------------
// �ļ�����	CGPUMeshLevel.h
// ˵����	����GPU��Mesh����������<D3D9ʵ��>
// �����ˣ�	����
// ����ʱ�䣺2008��6��27��
// ��ע��	���ڲ���
//-------------------------------------------------
#ifndef _GPU_MeshLevel_H_
#define _GPU_MeshLevel_H_
#include "CMeshLevel.h"

class CGSkeletalLevel : public CMeshLevel
{
public:
	struct VertexFomat
	{
		int16		x,y,z;
		uint16		n;
		uint16		u,v;
		CTinyWeight w;
		SkltIndex	i;
	};

	//DWORD FVF = FVF_XYZ|FVF_NORMAL|FVF_TEX1||D3DFVF_LASTBETA_UBYTE4
};
#endif //_GPUMeshLevel_H_