#pragma once

namespace sqr
{
	template<class T>
	struct TVector3;
	typedef TVector3<float> CVector3f;

	/**
	* @param
	*	stacks 
	*		γ��������Ŀ
	*	slices 
	*		����������Ŀ
	*	vertex_num 
	*		������������Ķ�����Ŀ
	*	indices_num
	*		������������Ķ���������Ŀ
	*	theta_start
	*		γ����ʼ�Ƕ�(����)
	*	theta_end
	*		γ�Ƚ����Ƕ�(����)
	*	phi_start
	*		������ʼ�Ƕ�(����, ��������, ����Ϊ-PI/2 )
	*	phi_end
	*		���Ƚ����Ƕ�(����, ��������, ����ΪPI/2)
	*/
	bool CreateSphere(float radius, CVector3f** ppVertices, uint16** ppIndices, 
		int stacks, int slices, int* vertex_num, int* indices_num, 
		float theta_start, float theta_end, float phi_start, float phi_end
		);

}
