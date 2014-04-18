#pragma once
#include "BaseDefs.h"

// ---- �ӿڶ��� ------------------------------------------------------------------------------

	class IIndexGenerator
	{
	public:
		struct IndexStruct
		{
			uint32		x;
			uint32		y;
			string		name;
		};

	public:
		// ---- ���������Ⱥ͸߶ȣ��Դ��������з�������Ŀ
		virtual void	SetRaginInfo( uint32 uWidth, uint32 uHeight ) = 0;

		// ---- �����и������η����и������Լ��зֽ��ͼƬ����
		//		��������������������ϣ��򷵻�false
		virtual bool	GetNextIndex( IndexStruct *sIndex ) = 0;

		// ---- �ͷ�������
		virtual void	Release() = 0;
	};