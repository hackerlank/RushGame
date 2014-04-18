#pragma once
#include "CPos.h"


enum ELinkRegionState
{
	eLRS_UN_USED,
	eLRS_OPEN,	
	eLRS_CLOSE
};


class CLinkRegionBuildNode;
class CLinkInfoBuild;
class CLinkRegionBuild;


//���ڼ���LinkRegionʹ�õĽڵ� (���ܶ�Ϊ�����Ч�ʶ����ڵ�����)
class CLinkRegionBuildNode
{
	typedef	 uint8		Num_t;
	typedef vector<CLinkRegionBuildNode*>	VecRegionList_t;
	typedef vector<CLinkInfoBuild*>	VecLinkList_t;

	friend CLinkRegionBuild;
public:
	CLinkRegionBuildNode();
	~CLinkRegionBuildNode();

	bool IsInOtherLinkRegion(CLinkRegionBuildNode* pOhterNode);
	uint32 GetLayerLv();
	inline Num_t GetChildNum(){return m_nChildRegionNum;};
	inline CLinkRegionBuildNode* GetChild(uint32 index){return m_ppChildRegion[index];};
	
	void SetChildLinkRegion(VecRegionList_t* pVecLinkRegionList);
	void SetLinkInfo(VecLinkList_t* pVecLinkInfoList);
	void SetFatherLinkRegion(CLinkRegionBuildNode* pFatherLinkRegion);


	inline const CPos& GetKeyPos(){return m_KeyPos;};
	inline bool IsOpen(uint32 nSearchId){return m_nSearchId == nSearchId && m_eState == eLRS_OPEN;};
	inline bool IsClose(uint32 nSearchId){return m_nSearchId == nSearchId && m_eState == eLRS_CLOSE;};
	inline void Open(uint32 nSearchId){	m_eState = eLRS_OPEN;m_nSearchId = nSearchId;};
	inline void Close(){m_eState = eLRS_CLOSE;};
protected:
//��¼Ѱ·ʱ���ݵ�����
	CLinkRegionBuildNode*			m_pFrontLinkRegion;		//·����ǰ���ڵ�
	float							m_fG;				//Ѱ·��㵽����ͨ����Ĵ���
	ELinkRegionState				m_eState;
	uint32							m_nSearchId;

	
//�̶�����==============================================================================================
	CPos				m_KeyPos;

	
	CLinkRegionBuildNode*			m_pFatherRegion;		//����ͨ����(������һ������ͨ����, this ���� m_pFatherRegion->m_ppChildRegion ��)
	Num_t							m_nChildRegionNum;	//����ͨ����ĸ���
	CLinkRegionBuildNode**			m_ppChildRegion;		//����ͨ�����б�
	Num_t							m_nLinkNum;		//ͬ����ͨ�����������ͨ������������ͨ�������
	CLinkInfoBuild**				m_ppLinkInfo;		//��������ͨ������Ϣ�б�
	uint32							m_nGlobalIndex;//����ʱ���õ�
};

class CLinkInfoBuild
{
	friend CLinkRegionBuild;
public:
	CLinkInfoBuild(CLinkRegionBuildNode* pDestLinkRegion, float fFare);
private:
	CLinkRegionBuildNode*			m_pDestLinkRegion;		//Ŀ����ͨ����,
	float				m_fFare;			//��Ŀ����ͨ����ķ���
};