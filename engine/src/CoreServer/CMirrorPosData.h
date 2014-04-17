#pragma once
#include "CPos.h"
#include "CMirrorBufferData.h"

namespace sqr
{
	/*
		����triple buffer��˵��wb��ib��rb�������ݴ�С����һ����
	*/

	class CMirrorPosData : public CMirrorBufferData 
	{
	public:
		const CFPos& GetPos() const { return m_Pos; }
		CFPos& GetPos() { return m_Pos; }

	protected:
		CMirrorPosData(const CFPos& pos);
		~CMirrorPosData();		

		void RawSetPos(const CFPos& pos) { m_Pos = pos; }

	protected:
		CFPos	m_Pos;
	};
		
}
