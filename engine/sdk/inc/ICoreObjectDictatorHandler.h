#pragma once
//$id$

#include "ICoreObjectServerHandler.h"


namespace sqr
{
	class ICoreObjectMediatorHandler;

	class ICoreObjectDictatorHandler
		:public ICoreObjectServerHandler
	{
	public:
		//����ע�⣬SyncAoiϵ�еĻص���ViewAoiϵ�еĻص����ԵĻص�˳�����ϸ�֤�ģ���������֮���ʱ���ǲ���֤��

		virtual void OnCaughtSyncSightOf(uint32 uObjGlobalId, uint32 uDimension) {}
		virtual ICoreObjectDictatorHandler* CastToObjDicHandler()const
		{
			return const_cast<ICoreObjectDictatorHandler*>(this);
		}

		virtual ICoreObjectMediatorHandler* CastToObjMedHandler()const
		{
			return NULL;
		}
	};
}
