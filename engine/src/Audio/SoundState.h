#pragma once
#include "CAudioMallocObject.h"

namespace sqr
{

/// ��Ƶ״̬.
struct SoundState : public CAudioMallocObject
{
	enum State
	{
		Invalid		= 0,	
		Created		= 1<<0,	///< �������
		Preparing	= 1<<1,	///< ׼����
		Prepared	= 1<<2,	///< ׼�����
		Playing		= 1<<3,	///< ������
		Stopping	= 1<<4,	///< ֹͣ��
		Stopped		= 1<<5,	///< ֹͣ
		Paused		= 1<<6	///< ��ͣ
	};
	typedef uint	Mask;
};

}// namespace sqr