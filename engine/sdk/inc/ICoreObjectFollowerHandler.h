#pragma once
//$id$

#include "ICoreObjectClientHandler.h"

namespace sqr
{
	class ICoreObjectDirectorHandler;

	class ICoreObjectFollowerHandler
		:public ICoreObjectClientHandler
	{
	public:
		virtual void OnPositionSet() {}

		//��Follower���߳��ڱ���̬��Director�ſ��ܴ�������ص�
		virtual void OnSpeedChanged() {}

		virtual void OnEnteredSyncAoi(){}
		virtual void OnLeftedSyncAoi(){}
		virtual ICoreObjectFollowerHandler* CastToObjFolHandler()const
		{
			return const_cast<ICoreObjectFollowerHandler*>(this);
		}
		virtual ICoreObjectDirectorHandler* CastToObjDirHandler()const
		{
			return NULL;
		}
	};
}
