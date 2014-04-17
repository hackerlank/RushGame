#pragma once
#include "CMirrorMallocObject.h"

namespace sqr
{
	class CMirrorBuffer;
	class CMirrorWBPatch;

	class CMemPool;

	template<typename T>
	class TMemPoolAllocator;

	class CMirrorPatchCmd;

	class CMirrorWBPatchMgr : public CMirrorMallocObject
	{
	public:
		CMirrorWBPatchMgr();
		~CMirrorWBPatchMgr();

		//����һ��ָ���汾��patch
		CMirrorWBPatch* CreatePatch(uint64 uVersion);
		
		//ɾ��һ��ָ���汾��patch
		void DeletePatch(uint64 uVersion);

		//ɾ��ĳһ����Χ�����patch
		void DeletePatch(uint64 uMinVersion, uint64 uMaxVersion);

		//�õ���ǰ�汾��patch�����û�У��򴴽�һ��
		CMirrorWBPatch* GetCurPatch(uint64 uVersion);
		
		//��ĳһ���汾��patch
		void Patch(uint64 uVersion, CMirrorBuffer* pBuffer);
		
		//��ĳһ����Χ�����patch
		void Patch(uint64 uMinVersion, uint64 uMaxVersion, CMirrorBuffer* pBuffer);

	private:
		CMirrorWBPatch* GetPatch(uint64 uVersion);
		
		void IntDeletePatch(CMirrorWBPatch* pPatch);

	private:
		typedef deque<CMirrorWBPatch*, 
			CMirrorMallocAllocator<CMirrorWBPatch*> >	DequePatch_t;

		DequePatch_t					m_quePatch; //��patch������queue���棬
		//queue�����һ��patch�İ汾������С�ģ����һ��������

		typedef TMemPoolAllocator<CMirrorPatchCmd*>	MemPool_t;

		stack<MemPool_t*, deque<MemPool_t*, 
			CMirrorMallocAllocator<MemPool_t*> > >				m_stkMemPool;  //mem pool�Ķ����
	};
}
