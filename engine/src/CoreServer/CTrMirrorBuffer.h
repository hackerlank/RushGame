#pragma once

/*
	�����������
	������Ϊ read buffer��write buffer����idle buffer������ȫ����rb��wb��ib���棩

	
	����ԭ��
	1��	���ȳ�ʼ��rb,wb��ib�������Ӧ�İ汾������Ϊ0
	2��	ÿ�����ݸ����߳̿�ʼ����wb�İ汾��1���������ݸ��µ�ʱ�򣬲���wb�������˴θ���
		��patch���档

		Ʃ�磺	
		wbԭʼ�汾Ϊ0������wb0�����涼��wbn��ʾ��n��ʾ�汾�ţ�
		��ô��һ�β���֮�󣬱��wb1������ĸ���ΪPatch1

	3��	��һ�����ݸ������֮�󣬽���ib��wb����ʱ��ib��������ݾ������µ����ݣ�ͬʱ��ib
		�İ汾������Ϊwb����֮��İ汾�ţ�wb������ʱ������ݾ���ib�����ݣ�������ǰib��
		�汾�����°汾�Աȣ��Խ������wb����Patch��
		
		Ʃ�磺
		wb1��ib0������֮��ib1��wb1����Ϊ��ǰib�İ汾Ϊ0����wb�İ汾Ϊ1
		���wb1����Patch1������
		�������ƣ������ǰibΪ1��wbΪ10�����wb10����Patch2 -- Patch10����

	4��	����Ҫ��ȡ���ݵ�ʱ�����ib�İ汾��rbҪ�ߣ���rb��ib���н�����ͬʱ��rb�İ汾���ó�ib�ģ�ib�İ汾���ó�rb��

		Ʃ�磺
		rb0��ib1������֮��rb1��ib0
		
		���ib�İ汾��rbһ����֤��wb��û�и�ib������������Ȼʹ��rb�����ݡ�

		�ڼ���2����
		��Ҫע����ǣ����wb����֮�󽻻�ib��ʱ�������ʱ���ib��rb�����ģ���ô�������Ӧ��
		�Ĳ���֮�����ǿ���ɾ����rb�汾֮ǰ��Ӧ��Patchs�ˡ�

		Ʃ�磺
		����֮ǰ rb5��ib1��wb6������֮��rb5��ib6��wb6��
		��wb6����Patch2 -- Patch6������
		��Ϊ��ʱ��ɵİ汾��5�����Կ���ɾ��Patch6֮ǰ���е�Patch��

	5��	����rb�̴߳���һ�������ʱ������Ҳ�����һ��patch�����patch�����������֡����
		�����´����Ķ��󣬵�rb��ib������ʱ�����ȿ��µ�rb��Ӧ��slot�ǲ��������ݣ������ݣ�����
		�´����Ķ����Ѿ�ͨ��wb������������ib���棬��ʱ�����Ǿͻ�ɾ��patch�����Ӧ������ݣ�
		���slotû�����ݣ����Ǿͽ�patch�����Ӧslot�����ݴ����µ�rb��
		��patch����û���κ�����֮������ɾ�����patch
	
	//////////////////////////////////////////////////////////////////////////////////////

		��Ϊ�������ǳ������̣���������read thread������
		
		����read thread
		1�����ȵõ�read buffer
		2����read buffer���洴��һ�����󣬴�����ʱ��read buffer���ṩΨһ��һ��slot���������
		   �����������ͨ�����slot����read buffer�����Ӧ������������
		   ͬʱÿ����һ�����󣬲���һ������command������һ��patch����
	    3����rb��ib������ʱ�򣬽�patch�����µ�rb���棬��Ϊ�µ�rb��û���´����Ķ����

	    ����write thread
		1�����ȵõ�write buffer
		2����write buffer���洴��һ�����󣬲���ʹ�õ���read buffer���洴����slot����Ϊ���ǵ����ж�������
		   read buffer���洴���ģ��������slot������Ψһȷ����
	    3�����write thread���������ݸ��£�����һ������command�������Ǹ��¶����slot�����¶��������
		   ����һ������command�б����棬���list command����pPatch
	    4����wb��ib������֮���ҵ���Ҫ�����pPatch����Patch1 -- Patch8�����ε���pPatch�����command
		   �����ʹ��ϲ�����
					
*/

#include "ThreadTypes.h"
#include "CMirrorMallocObject.h"

namespace sqr
{

	class CMirrorBuffer;
	
	class CMirrorWBPatchMgr;
	class CMirrorRBPatchMgr;

	class CMirrorPatch;
	class CMirrorWBPatch;
	class CMirrorRBPatch;
	class CMirrorPatchCmd;
	
	class CTrDiscardedCmdMgr;

	class CTrMirrorBuffer : public CMirrorMallocObject
	{
	public:
		friend class CTestSuiteTripleBuffer;

		CMirrorBuffer* GetWriteBuffer();
		CMirrorBuffer* GetReadBuffer();

		CMirrorPatch*		GetCurWBPatch();
		CMirrorPatch*		GetCurRBPatch();
		
		void AddDiscardedCmd(uint32 uSlot);
		bool IsDiscardedCmd(uint32 uSlot);

		virtual void Release();

		void OnRBUpdate();
		void OnWBUpdate();

	protected:
		CTrMirrorBuffer(uint32 uDataSize, uint32 uDataNum);
		virtual ~CTrMirrorBuffer();

		void CreateTripleBuffer(uint32 uDataSize, uint32 uDataNum);
		void DeleteTripleBuffer();

		//swap the write and idle buffer
		void SwapWIBuffer();  

		//swap the read and idle buffer
		void SwapRIBuffer();

	protected:
		CMirrorBuffer* m_pWriteBuffer;
		CMirrorBuffer* m_pReadBuffer;
		CMirrorBuffer* m_pIdleBuffer;

		
		CMirrorWBPatchMgr*	m_pWBPatchMgr;
		CMirrorRBPatchMgr*	m_pRBPatchMgr;

		CTrDiscardedCmdMgr*	m_pDiscardedCmdMgr;

		HLOCK	m_Lock;
	};

}
