#pragma once 
#include "CPos.h"
#include "CError.h"

namespace sqr
{
	/************************************************************************/
	/*                                                                      
		Aoi�ڶ��߳������У����и����̵߳Ļص�������Ϊ�˲������ͻ��
		���ǽ��ص�����һ��������У�Ȼ������ѭ������ȡ������
		��������һ�����⣬�������ѭ�������ʱ��aoi�����Ѿ���ɾ���ˣ��ڴ���ص�
		�ͻ���ִ�������������������id����ʾ�����id��Ӧ��aoi�����Ѿ���ɾ����
		���ǾͲ���������ص���
	*/
	/************************************************************************/
	class CAoiResult
	{
	public:	
		static void* operator new(size_t stSize);
		static void operator delete(void* pMem){}

		void* AllocMem(size_t stSize);
		char* CloneString(const char* szStr);
		void* CloneData(const void* pData, size_t stSize);

		void Add();
		virtual ~CAoiResult(){}
	
	};

	class CAoiRelationChangedResult : public CAoiResult
	{
	protected:
		CAoiRelationChangedResult(uint32 uId, uint32 uOtId, bool bSee, uint32 uDimension)
			: m_uId(uId), m_uOtId(uOtId), m_bSee(bSee), m_uDimension(uDimension) {}
	protected:
		uint32	m_uId;
		uint32	m_uOtId;
		bool	m_bSee;
		uint32	m_uDimension;
	};

	class CAoiViewChangedResult : public CAoiRelationChangedResult
	{
	public:
		CAoiViewChangedResult(uint32 uId, uint32 uOtId, bool bSee, uint32 uDimension);
		~CAoiViewChangedResult();
	};

	class CAoiSyncChangedResult : public CAoiRelationChangedResult
	{
	public:
		CAoiSyncChangedResult(uint32 uId, uint32 uOtId, bool bSee, uint32 uDimension);
		~CAoiSyncChangedResult();
	private:
	};

	class CAoiObjHaltedResult : public CAoiResult
	{
	public:
		CAoiObjHaltedResult(uint32 uId);
		~CAoiObjHaltedResult();
	private:
		uint32	m_uId;
	};

	class CAoiMsgFromViewerHandledResult : public CAoiResult
	{
	public:
		CAoiMsgFromViewerHandledResult(uint32 uId, const void* pContext);
		~CAoiMsgFromViewerHandledResult();
	private:
		uint32 m_uId;
		const void*	m_pContext;
	};

	class CAoiMsgFromVieweeHandledResult : public CAoiResult
	{
	public:
		CAoiMsgFromVieweeHandledResult(uint32 uId, const void* pContext);
		~CAoiMsgFromVieweeHandledResult();
	private:
		uint32 m_uId;
		const void*	m_pContext;
	};

	class CAoiMsgToVieweeHandledEndedResult : public CAoiResult
	{
	public:
		CAoiMsgToVieweeHandledEndedResult(uint32 uId, const void* pContext);
		~CAoiMsgToVieweeHandledEndedResult();
	private:
		uint32 m_uId;
		const void* m_pContext;
	};

	class CAoiMsgToViewerHandledEndedResult : public CAoiResult
	{
	public:
		CAoiMsgToViewerHandledEndedResult(uint32 uId, const void* pContext);
		~CAoiMsgToViewerHandledEndedResult();

	private:
		uint32 m_uId;
		const void* m_pContext;
	};

	class CAoiMsgToSelfHandledResult : public CAoiResult
	{
	public:
		CAoiMsgToSelfHandledResult(uint32 uId, const void* pContext);
		~CAoiMsgToSelfHandledResult();

	private:
		uint32 m_uId;
		const void* m_pContext;
	};

	class CAoiMsgFromSynceeHandledResult : public CAoiResult
	{
	public:
		CAoiMsgFromSynceeHandledResult(uint32 uId, const void* pContext);
		~CAoiMsgFromSynceeHandledResult();

	private:
		uint32 m_uId;
		const void*	m_pContext;
	};

	class CAoiMsgFromSyncerHandledResult : public CAoiResult
	{
	public:
		CAoiMsgFromSyncerHandledResult(uint32 uId, const void* pContext);
		~CAoiMsgFromSyncerHandledResult();

	private:
		uint32 m_uId;
		const void*	m_pContext;
	};

	class CAoiMsgToSynceeHandledEndedResult : public CAoiResult
	{
	public:
		CAoiMsgToSynceeHandledEndedResult(uint32 uId, const void* pContext);
		~CAoiMsgToSynceeHandledEndedResult();

	private:
		uint32 m_uId;
		const void* m_pContext;
	};

	class CAoiMsgToSyncerHandledEndedResult : public CAoiResult
	{
	public:
		CAoiMsgToSyncerHandledEndedResult(uint32 uId, const void* pContext);
		~CAoiMsgToSyncerHandledEndedResult();

	private:
		uint32 m_uId;
		const void* m_pContext;
	};

	class CAoiTellErrMsgResult : public CAoiResult
	{
	public:
		CAoiTellErrMsgResult(const char* szError);
		~CAoiTellErrMsgResult();

	private:
		char* m_sErrorMsg;	
	};

	class CAoiSceneHaltedResult : public CAoiResult
	{
	public:
		CAoiSceneHaltedResult(uint32 uId);
		~CAoiSceneHaltedResult();

	private:
		uint32	m_uId;
	};
}

