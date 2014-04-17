#pragma once

#include <string>
#include <vector>
#include <map>
#include <d3d9.h>
#include <math.h>
#include <D3dx9math.h>
#include <fstream>
#include "iresprocessormgr.h"
#include "GraphicBase.h"
#include "GraphicCommon.h"

//�����ĵ���SOCKET
struct CResSkeletalSocket
{
	IDNAME						m_SocketName;			//SOCKET����
	D3DXMATRIX					m_Socket;				//SOCKET����
};

//ÿ��������SOCKET����
struct CResSkeletalUnit
{
	D3DXMATRIX					m_InverseMatrix;		//ÿ�����������Ƥ������ȫ�ֿռ䵽�����ֲ��ľ���
	vector<CResSkeletalSocket>		m_SocketList;			//������SOCKET���б�

	int BinLoad( ifstream& File );
};

//��������
class CResSkeletal
{
	BYTE						m_SkeletalID;           //��ͷID
	string						m_SkeletalName;			//��ͷ����

	vector<CResSkeletal*>			m_Children;				//�ӹؽ�

public:
	int		BinLoad( ifstream& File );
};

//������
class CSkeFrame
{
	BYTE						m_SkeletalTotalNum;		//��������

	float						m_FPS;
	D3DXMATRIX					m_Globe;

	CResSkeletal					m_SkeletalRoot;			//������
	vector<CResSkeletalUnit*>		m_SkeletalList;			//���й�������SOCKET����Ԫ�б�
	//vector<IDNAME>				m_AnimationList;		//���ж����б�

public:
	int BinLoad( string FileName, CBaseResProcessor* pProcessor );
};

class CAraProcessor : public CBaseResProcessor
{
public:
	CAraProcessor(void);
	virtual ~CAraProcessor(void);

	virtual bool			RunProcess( const char* szFilename );
	virtual size_t			GetCountCanBeProcess(){ return 1; };
	virtual unsigned long	GetTypeCanBeProcess( size_t nNum ){ return 'ske'; }
};
