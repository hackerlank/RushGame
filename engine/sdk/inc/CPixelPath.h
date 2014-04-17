#pragma once

#include "FindPathDefs.h"
#include "CDynamicObject.h"
#include "CPos.h"
#include "CFindPathMallocObject.h"


namespace sqr
{
	template<class T> struct TPos;
	typedef TPos<int32> CPos;
	typedef TPos<float> CFPos;

	template<typename Traits>
	class TMetaScene;

	//�����Ķ���ֻ����TMetaScene��������
	class CPixelPath
		:public virtual CDynamicObject
		,public CFindPathMallocObject
	{
	public:
		void Release();

		CPixelPath* Clone()const;

		const IPath* GetGridPath()const;

		void GetBeginPixelPos(CFPos& PixelPos)const;
		void GetEndPixelPos(CFPos& PixelPos)const;
		size_t GetPathSize()const;

		//�ú����Ե�һ��waypoint���������⴦����һ��WayPoint������Ϊ�ƶ��������������꣬������·�����ĸ������꾭��ת�������������
		bool GetWayPointPixelPosOfPath(CFPos& PixelPos,uint16 uIndex)const;
		uint16 GetPixelPosOfPath(CFPos& PixelPos,float fDist)const;
		float GetPathPixelLength()const;
		uint16 GetNextWayPointInPixelPos(float fMovedDist, CFPos& posNextWayPoint)const;
		uint16 GetLastWayPointInPixelPos(float fMovedDist, CFPos& posLastWayPoint)const;

		void PrintPath()const;
	private:
		//���ܳɹ�����ʧ�ܣ��������غ�uBeginWayPoint�������ʱ���������һ��WayPoint������,fDist������һ��WayPoint��ָ���ܾ���ľ���
		//ʧ�ܵ�ʱ��,PixelPosû�б��ı�
		bool GetPixelPosOfPath(CFPos& PixelPos,uint16& uCurWayPoint,float fCurDist,float fDist)const;

		template<typename Traits>
		friend class TMetaScene;

		CFPos	m_BeginPixelPos;
		CFPos	m_EndPixelPos;
		
		IPath*			m_pGridPath;		//�ƶ�·��

		CPixelPath(IPath* pPath,const CFPos& BeginPixelPos,const CFPos& EndPixelPos);

		float			m_fPathLength;

		float CalPathPixelLength()const;

		~CPixelPath(void);
	};
}

