#pragma once
#include "TSingleton.h"
#include "CAxisAlignedBox.h"

/**
	@class CShadowClipper

	ͶӰ����Ĳü�.

	��ΪĿǰȱ��LightEntity��ô������, ��д�������պ�����.
	�����Ǽ�������ͶӰ����İ�Χ��,�ͽ�����Ӱ����İ�Χ��
 */

namespace sqr
{

class CShadowClipper
{
public:
	CShadowClipper();
	/// ����һ��AABB
	void AddCasterBox(const CMatrix& transform, const CAxisAlignedBox& box);
	void AddReceiverBox(const CAxisAlignedBox& box);
	/// ����AABB����
	const CAxisAlignedBox& GetCastersAABB(const CMatrix& transform);
	const CAxisAlignedBox& GetCastersAABBWithProj(const CMatrix& transform);
	const CAxisAlignedBox& GetReceiverAABB(const CMatrix& transform);

	/// ���AABB, ���¼���
	void Reset();
private:
	friend class CShadowMapFilter;
	CAxisAlignedBox			m_CasterBoxes;
	CAxisAlignedBox			m_ReceiverBoxes;
};

//------------------------------------------------------------------------------
inline void
CShadowClipper::Reset()
{
	m_CasterBoxes.setNull();
	m_ReceiverBoxes.setNull();
}

}// namespace sqr
