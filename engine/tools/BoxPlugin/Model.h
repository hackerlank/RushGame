#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include "GraphicCommon.h"
#include "GLobD3d.h"
#include <map>
#include <vector>
#include "iparamm.h"
#include "dummy.h"
#include "Simpobj.h"
#include "surf_api.h"
#include "MNMath.h"
#include "PolyObj.h"
#include "CAxisAlignedBox.h"
using namespace sqr;
#define ANI_SCALE				0x00000001L

using namespace std;
//--------------------------------------------------------------------------------
// �ļ���ExpPlugin.h
// ���ڣ�2004.3.22
// ���ܣ�����ģ�ʹ���ṹ
//------------------------------------------------------------------------------------
// ���ڣ�2007.1.19
// ���ܣ�����������ؽṹ :)
//------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////
// ������ײ���ҽṹ



// 3DMAXģ�ͽڵ������
enum PIECETYPE 
{ 
	enumPT_VERTEXANI,               //���㶯��
	enumPT_SKELETALANI,				//��������
	enumPT_NONEANI,					//��������
	enumPT_SKELETAL,				//����
	enumPT_SOCKET,					//SOCKET
	enumPT_UNKNOW=0xffffffff,		//δ���������
};

enum PIECECPTYPE
{
	enumPT_COMPRESS,				//ѹ��
	enumPT_UNCOMPRESS,				//��ѹ��
};

enum NODETYPE
{
	enumNT_MESH,
	enumNT_SKELETALMESH,
	enumNT_SKELETAL,
	enumNT_UNKNOW,
};

struct ListNode
{
	ListNode( NODETYPE NodeType, INode* MaxNode )
	{
		m_NodeType = NodeType;
		m_MaxNode = MaxNode;
		if( NodeType == enumNT_SKELETAL )
			m_ExpType = enumPT_SKELETAL;
		if( NodeType == enumNT_MESH )
			m_ExpType = enumPT_NONEANI;
		if( NodeType == enumNT_SKELETALMESH )
			m_ExpType = enumPT_SKELETALANI;
	};

	PIECETYPE m_ExpType;
	NODETYPE m_NodeType;
	INode*   m_MaxNode;
};



//��Ӧ���ַ���
static const TCHAR s_PICENAME[4][20]={"PT_VERTEXANI", "PT_SKELETALANI", "PT_SKELETAL", "PT_SOCKET"};

class ExportNode
{
public:
	INode*				m_3DMaxNode;	// 3DMax�ڵ�
	ExportNode*			m_Parent;		// ���ڵ�
	vector<ExportNode*> m_Children; 	// �ӽڵ�
	PIECETYPE			m_PieceType;	// �ڵ�����
	BOOL				m_bSoftLinkBone;
    BOOL                m_bSoftCloth;   // ��������)

    int                 m_nCapsules;        // ���ұ��
    int                 m_nCapsuleSide;    // ��ʼ 1 ͷ�� 2 β

	map<float,int>		m_AddBoneWeight;

	vector<D3DXMATRIX>	m_Matrix;
	vector<D3DXMATRIX>	m_InvMatrix;

	int					m_nMaxFrame;


	ExportNode() : m_3DMaxNode(NULL), m_Parent(NULL){};
	~ExportNode()
	{
		
		for( UINT i = 0; i < m_Children.size(); i++ )delete m_Children[i]; 
	};
};

//���ڱ�־��������
class IDNAME
{
	INT64      m_Buf[2]; 

public:
	IDNAME()
	{ 
		memset( m_Buf, 0, 16 ); 
	}
	IDNAME( LPCTSTR str )
	{
		memset( m_Buf, 0, 16 );
		if( str )
		{
			size_t len = min( strlen( str ), 15 );
			memcpy( m_Buf, str, len );
		}
	} 

	const IDNAME& operator= ( const IDNAME& BN )
	{
		memset(m_Buf,0,16);
		m_Buf[0] = BN.m_Buf[0];
		m_Buf[1] = BN.m_Buf[1];
		/*m_Buf[2] = BN.m_Buf[2];
		m_Buf[3] = BN.m_Buf[3];*/
		return *this;
	}

	const IDNAME& operator= ( LPCTSTR str )
	{
		memset( m_Buf, 0, 16 );
		if( str )
		{
			size_t len = min( strlen( str ), 15 );
			memcpy( m_Buf, str, len );
		}
		return *this;
	}

	BOOL operator== ( const IDNAME& BN )
	{
		return 	m_Buf[0] == BN.m_Buf[0] && m_Buf[1] == BN.m_Buf[1];
	}

	operator LPCTSTR() const
	{
		return (LPCTSTR)m_Buf;
	}
};

enum SaveType
{
	eST_Piece			= 0,
	eST_Skeletal		= 1,
	eST_SoftBody		= 2,
    eST_SoftCloth       = 3,
};

//�������
struct SAVEPARAM
{
	SAVEPARAM()
	{ 
		memset( this, 0, sizeof( SAVEPARAM ) );
		m_uiSP_Size = sizeof( SAVEPARAM );
	}
	~SAVEPARAM()
	{
		
		memset( this, 0, sizeof( SAVEPARAM ) );
		m_uiSP_Size = sizeof( SAVEPARAM );
	}

	Interface*		m_uiSP_pInterface;
	BOOL			m_uiSP_Select;

	UINT			m_uiSP_Size;				//���ṹ��С
	UINT			m_uiSP_WeightPerVertex;		//ÿ��������༸��Ȩ��
	UINT			m_uiSP_FramePerSample;		//������֡ȡ��һ֡
	LPCTSTR			m_uiSP_AnimationName;	    //��������
	void*			m_uiSP_pNodeType;			//��������
	SaveType        m_eSP_SaveType;				//��������
	UINT			m_uiSP_NodeNum;				//�ڵ�����
	BOOL			m_bSP_SmoothAll;			//�⻬������
	BOOL			m_bSP_RootScale;			//���Ÿ�����
	UINT			m_fLevel;					//��������
	BOOL			m_dwSP_Reserved[6];			//����
public:
	void Free()
	{
		memset( this, 0, sizeof( SAVEPARAM ) );
		m_uiSP_Size = sizeof( SAVEPARAM );
	}
};

//��Ⱦ����
struct CRenderStyle
{
	string		m_RenderStyleName;			//��Ⱦ����
	string		m_RSIndexName;
	vector<string>      m_TextureName;				//��ͼ
	int m_Num;
	//int ASCSave( CSaxXmlWriter& Node );
	void			SetNum(int i)					{ m_Num = i;}
};

struct TransFrameInfo
{
	D3DXVECTOR3					m_FrameTrans;			//�����ؼ�֡ƽ��
	CTinyVector					m_FrameTinyTrans;
};

struct QuatFrameInfo
{
	D3DXQUATERNION				m_FrameQuat;			//�����ؼ�֡��ת
	CTinyQuaternion				m_FrameTinyQuat;		//�����ؼ�֡��ת
};

struct CKeyFrames
{
	map<WORD,TransFrameInfo>	m_KeyTransFrames;		//�ؼ�֡�ļ���
	map<WORD,QuatFrameInfo>		m_KeyQuatFrames;		//�ؼ�֡�ļ���
};

class CAnimate
{
	int							m_MaxFrame;				//�������֡��
	float						m_FrameRatio;			//֡��
	float						m_fScale;
	float						m_fTransParam;
	float						m_GroundSpeed;			//ÿ�������Եص��ٶ�
	float						m_fMaxLength;
	IDNAME						m_AnimationeName;		//������
	vector<CKeyFrames*>			m_SkeletalList;			//�����ļ���
	map<WORD,IDNAME>			m_FrameString;			//�ؼ�֡�ַ���

public:
	CAnimate():m_MaxFrame(0),m_fScale(1.0f),m_fMaxLength(0),m_GroundSpeed(0){}
	~CAnimate()
	{
		for( int i = 0; i < (int)m_SkeletalList.size(); i++ )
			delete m_SkeletalList[i];
		
	}

	float	GetTransParam()										{ return m_fTransParam;	}
	int		GetSkeletalNum()								{ return (int)m_SkeletalList.size(); }
	IDNAME	GetName()										{ return m_AnimationeName; };
	void	SetName( IDNAME& Name )							{ m_AnimationeName = Name; };
	void	SetMaxFrame( int MaxFrame )						{ m_MaxFrame = max( MaxFrame, m_MaxFrame ); }
	void	AddSkeletalKeyFrames( CKeyFrames* KeyFrames )	{ m_SkeletalList.push_back( KeyFrames ); }
	void	SetScale( float fScale )						{ m_fScale = fScale;	}
	float	GetScale()										{ return m_fScale; }
	void	SetMaxLength( float fMaxLength )				{ m_fMaxLength = max( abs( fMaxLength ), m_fMaxLength );	}
	float	GetMaxLength()									{ return m_fMaxLength;	}
	const CKeyFrames& GetKeyFrame( UINT SkeletalID )
	{
		if( m_SkeletalList.size() == 1 )
			return *m_SkeletalList[0];

		//if(m_SkeletalList.empty())
		//	return nu
		return *m_SkeletalList[SkeletalID];
	}
	//int ASCSave( CSaxXmlWriter& Node );
};

//�����ĵ���SOCKET
struct CSkeletalSocket
{
	CSkeletalSocket(){ m_SocketName = 0; D3DXMatrixIdentity( &m_Socket ); }

	IDNAME						m_SocketName;			//SOCKET����
	D3DXMATRIX					m_Socket;				//SOCKET����
};
class CSkeletal;
//ÿ��������SOCKET����
struct CSkeletalUnit
{
	CSkeletalUnit(){}
	CSkeletalUnit( CSkeletal* pSkeletal ) : m_pSkeletal(pSkeletal){}
	CSkeletal*					m_pSkeletal;
	D3DXMATRIX					m_InverseMatrix;		//ÿ�����������Ƥ������ȫ�ֿռ䵽�����ֲ��ľ���
	vector<CSkeletalSocket>		m_SocketList;			//������SOCKET���б�

	//int ASCSave( CSaxXmlWriter& Node );

};
struct CRenderMatrix;
//��������
class CSkeletal
{
	BYTE						m_SkeletalID;           //��ͷID
	string						m_SkeletalName;			//��ͷ����

	vector<CSkeletal*>			m_Children;				//�ӹؽ�

public:
	CSkeletal();
	~CSkeletal();
	void Free();
	int FillFromExportNode( ExportNode* ExpNode, CAnimate& Animate, vector<CSkeletalUnit*>& SkeletalList );
	int GetSkeletalIDByName( const string& Name );
	/// ���ݹ���ID�õ�������������������ӹ�����
	string	GetSkeletalNameByID( BYTE SkeletalID ) const;
	/// �����������
	void	CalculateMatrix(const D3DXMATRIX& matWorld
		, CRenderMatrix& RenderMatrix
		, const vector<CAnimate*>& Animates
		, const vector<CSkeletalUnit*>& SkeletalUnit
		, bool& CompleteFlag ) const;


	//int ASCSave( CSaxXmlWriter& Node );
	int BinSave( ofstream& File );
	int		BinLoad( ifstream& File, CSkeletal** Buf );
	BOOL	IsFitForMe( const CSkeletal* Skeletal, string& ErrSkeletal );	//ƥ�����
};

//������
class CAnimateGroup
{
	BYTE						m_SkeletalTotalNum;		//��������

	float						m_FrameCircle;
	D3DXMATRIX					m_Globe;
	CSkeletal					m_SkeletalRoot;			//������
	vector<CSkeletalUnit*>		m_SkeletalList;			//���й�������SOCKET����Ԫ�б�
	vector<CAnimate*>			m_AnimationList;		//���ж����б�
	vector<IDNAME>				m_IdNameList;

public:
	CAnimateGroup();
	~CAnimateGroup();

	//�õ������б�
	vector<CAnimate*>& GetAnimateList()
	{ 
		return m_AnimationList; 
	}
	vector<IDNAME>& GetIdNameList()
	{
		return	m_IdNameList;
	}
	void  Free();

	int   FillFromExportNode( ExportNode* Root );
	DWORD GetTotalNum()								{ return m_SkeletalTotalNum; }
	int   GetSkeletalIDByName( const string Name )	{ return m_SkeletalRoot.GetSkeletalIDByName( Name ); }
	/// �õ�ָ����������Ԫ
	CSkeletalUnit* GetSkeleteUnit( int Num );

	/// ���ݹ���ID�õ�������������������ӹ�����
	string	GetSkeletalNameByID( BYTE SkeletalID );

	/// ���������Ⱦ����
	void  CalculateMatrix( const D3DXMATRIX& matWorld, CRenderMatrix& RenderMatrix, bool& CompleteFlag ) const;

//	int   ASCSave( CSaxXmlWriter& Node );

	//����ƥ��
	BOOL IsFitForMe( const CAnimateGroup& AG, string& ErrSkeletal );
	inline int GetSkeletalNum()
	{
		return (int)m_SkeletalList.size();
	}
};

enum VERTEXTYPE 
{ 
	VT_VERTEXANI	= 0, 
	VT_STATICVERTEX	= 1,
	VT_SKELETALANI	= 2,
	VT_SOFTBODYANI	= 3, 
    VT_SOFTCLOTHANI = 4,
	VT_UNKNOW=0xffffffff
};

//��Ӧ���ַ���
static const TCHAR s_VERTEXTYPE[2][20]={ "VT_VERTEXANI", "VT_SKELETALANI" };

struct SklWeight
{ 
	float f[4]; 
	float GetWeight( unsigned nIndex ) const { return f[nIndex]*0.005f;}
};
struct SkltIndex{ BYTE  b[4]; };

struct CVertexAndGroup
{
	int				m_index;
	DWORD			m_nGroup;

	CVertexAndGroup(const int& ind,DWORD nGroup)
	{
		m_index=ind;
		m_nGroup=nGroup;
	}

	bool operator<(const CVertexAndGroup& ano)const
	{
		if (m_index<ano.m_index)
			return true;
		else if(m_index>ano.m_index)
			return false;
		else
			return m_nGroup<ano.m_nGroup;
	}

	bool operator ==(const CVertexAndGroup& ano)const
	{
		return m_index == ano.m_index && m_nGroup == ano.m_nGroup;
	}
};
struct CMeshLevel
{
	CMeshLevel() : m_fDist(0){}
	virtual ~CMeshLevel(){}

	float							m_fDist;			//���þ���	
	DWORD							m_FramePerKeyFrame;	//���㶯��������
	vector< vector<D3DXVECTOR3> >	m_FramesVertex;		//��֡���㻺����
	vector< vector<D3DXVECTOR3> >	m_FramesNormal;		//��֡���߻�����
	vector<DWORD>					m_Diffuse;			//������ɫ
	vector<D3DXVECTOR2>				m_TextCoord;		//��ͼ����
	vector<D3DXVECTOR2>				m_TextCoord1;
	vector<D3DXVECTOR2>				m_TextCoord2;
	vector<SklWeight>				m_Weight;			//�������Ȩ��
	vector<SkltIndex>				m_SkeletalIndex;	//�����������
	vector<WORD>					m_VertexIndex;		//�滺����
	int		GetVerNum()							{ return (int)m_FramesVertex[0].size(); }
};

class CMesh
{	
	VERTEXTYPE				m_eVertexType;					//��������
	float					m_fBaize;						//zƫ��
	D3DXVECTOR3				m_Scale;						//ģ������������ռ�Ĵ�С
	D3DXVECTOR3				m_Off;							//ģ�����Ķ���������ռ�ԭ���ƫ����
	vector<CMeshLevel*>		m_MeshLevels;					//���LOD
	D3DXVECTOR2				m_MinUV;
	D3DXVECTOR2				m_MaxUV;

	vector<UVVert>			m_uvBuffer;						//��ʱ����һ��uv����
	string					m_ObjectName;					//Mesn����
	int						m_Channel;						//�ж��ٸ�
	bool					m_IsCompress;					//�Ƿ�ѹ�����piece
	bool					m_tIsCompress;					
	
	ExportNode*				m_ExpNode;
public:
	typedef	map<sqr::uint16, CBaseAABB>					BoxMap;

	BoxMap					m_BoxMap;						//��Χ���б�
	CMesh();
	~CMesh();

	VERTEXTYPE	GetVertexType()			{ return m_eVertexType; }
	void	SetMeshName(string name) {m_ObjectName = name;}
	string	GetMeshName()	{return m_ObjectName;}
	void	SetIsCompress(bool bIsCompress) { m_tIsCompress = bIsCompress; }
	void	asynNewCompressState()	{m_IsCompress = m_tIsCompress ;}
	void	asynOldCompressStete()	{m_tIsCompress = m_IsCompress;}
	bool	GetIsCompress(){return m_IsCompress;}
    vector<CMeshLevel*>&	GetMeshLevels() { return m_MeshLevels; }
    ExportNode* GetExpNode() { return m_ExpNode; }
	void	SetExpNode( ExportNode* ExpNode ){ m_ExpNode = ExpNode; }
	bool	GetMeshFromExpNode( CAnimateGroup& AnimateGroup );
	void	GetPhysiqueWeight( vector<SklWeight>& VerWeight, vector<SkltIndex>& SkeIndex, Modifier* mod, CAnimateGroup& AnimateGroup );
	void	GetSkinWeight( vector<SklWeight>& VerWeight, vector<SkltIndex>& SkeIndex, Modifier* mod, CAnimateGroup& AnimateGroup );
	void	GetBPWeight( vector<SklWeight>& VerWeight, vector<SkltIndex>& SkeIndex, Modifier* mod, CAnimateGroup& AnimateGroup );
	void	FormatFromMaxMesh( Mesh& MaxMesh, vector<VerSkinNCT>& Vertex, vector<WORD>& VertexIndex );

	//int ASCSave( CSaxXmlWriter& Node );
};

//PIECE
class CPiece
{
	string					m_Name;				//Piece����
	string					m_ClassName;		//Piece������
    CMesh*					m_Mesh;				//Piece��MESH
	vector<CRenderStyle>    m_RenderStyle;		//Piece����Ⱦ����

public:
	CPiece();
	~CPiece();
	string&	GetPieceName() { return m_Name; }
	bool GetMeshFromExpNode( CAnimateGroup& AnimateGroup );
	void SetMesh( CMesh* Mesh )							{ m_Mesh = Mesh; };
	void AddRenderStyle( CRenderStyle& RenderStyle )	{ m_RenderStyle.push_back( RenderStyle );};
    CMesh*  GetMash()       { return m_Mesh; }
	//int ASCSave( CSaxXmlWriter& Node );
};

class CPieceGroup
{
public:
	vector<CPiece>			m_Piece;
	void GetMeshFromExpNode( CAnimateGroup& AnimateGroup );
	void AddPiece( CPiece& Piece ){ m_Piece.push_back( Piece ); }

	//int ASCSave( CSaxXmlWriter& Node );
	int BinSave( ofstream& File, string SaveName );
	void Free() { m_Piece.clear();}
};

struct RESTYPE
{
	RESTYPE(){ dwResFlag = 0; };
	RESTYPE( LPCTSTR Type )
	{ 
		dwResFlag = 0;
		memcpy( &dwResFlag, Type, min( 4, strlen( Type ) ) ); 
	};

	DWORD dwResFlag;

	operator DWORD(){ return dwResFlag;	}
};
struct MatrixUnit
{
	D3DXVECTOR3			m_FrameTrans;			///<��¼ÿ֡�����˶���ƽ�Ʊ任
	D3DXQUATERNION		m_FrameQuat;			///<��¼ÿ֡�����˶�����ת�任
	D3DXMATRIX			m_CurSkeletalMatrix;			///<��ǰ��������������ľ���
	D3DXMATRIX			m_GlobeSkeletalMatrix;			///<��ǰ��������������ľ���
	CSkeletalUnit*		m_SkeletalUnit;					///<�͹�������ϵ��SOCKET����
};
struct AniResult
{
	AniResult() : m_FrameString(NULL), m_SepFrameString(NULL), m_CurFrame(0.0f), m_IsAniEnd(false), m_InAniBlend(false), m_IsSepAniEnd(false) {}
	AniResult( const IDNAME* pFrameString, float fCurFrame, bool bIsAniEnd, bool bInAniBlend , const IDNAME& pAName = NULL, const IDNAME &pSAName = NULL)
		: m_FrameString(pFrameString), m_CurFrame(fCurFrame), m_IsAniEnd(bIsAniEnd), m_InAniBlend(bInAniBlend)
		, m_PreAnimationName(pAName), m_PreSepAnimationName(pSAName) 
		, m_AniframeIndex(999999), m_SepAniframeIndex(999999){}
	IDNAME			m_PreAnimationName;
	IDNAME			m_PreSepAnimationName;
	const IDNAME*	m_FrameString;		//֡�����ַ���
	const IDNAME*	m_SepFrameString;	//֡�����ַ���
	sqr::int32			m_AniframeIndex;
	sqr::int32			m_SepAniframeIndex;
	float			m_CurFrame;		//��Ⱦ�ĵ�ǰ֡
	bool			m_IsAniEnd;		//�����������
	bool			m_InAniBlend;   //������Ԫ�ں���

	bool			m_IsSepAniEnd;	//���붯����ϣ��ϰ������������)
};
struct CRenderMatrix
{
	CRenderMatrix();
	~CRenderMatrix();
	void SetAnimateGroup( CAnimateGroup* AnimateGroup );

	UINT				m_SkeletalNum;				///<������Ŀ

	BOOL				m_IsBlendStart;				///<�Ƿ����ǰ��֡���״̬
	BOOL				m_IsAniBlend;				///<�Ƿ���Ҫ����ǰ��֡���
	BOOL				m_IsLoop;

	float				m_PreFrameNO;				///<��һ֡��֡��

	MatrixUnit*			m_MatrixUnit;				///<������������

	UINT				m_CurAnimate;				///<��ǰ�ڼ���Ķ�����CAnimateGroup�е�λ��
	float				m_CurFrame;					///<��ǰ֡

	UINT				m_NextAnimate;				///<��һ��Ҫ���ŵĶ���
	float				m_NextBlendFrame;			///<����һ����������ǰ��Ҫ�����ںϵ�֡��

	DWORD				m_CurrentTime;
	AniResult		m_Result;
};
struct BoxNode
{
	INode* node;
	string BoxNodeName;
	Matrix3 mat;
	Matrix3 newMat;
	Point3 boxCenter;
	Point3 pmax;
	Point3 pmin;
};

class CModel
{
	CPieceGroup				m_Pieces;           //����
	ExportNode*				m_Root;				//���ڵ�
	CAnimateGroup			m_Motions;			//����
	CAnimateGroup			m_AnimateGroup;

	Interface*				m_ip;
	DummyObject*			m_dumObj;
	IObjCreate*				m_createInterface;
	Point3					max;
	Point3					min;
	SAVEPARAM               m_SaveParam;
	vector< ListNode* >		m_NodeType;
	vector<BoxNode>	m_vecNode;
	//vector<string>			m_SelectBone;
	vector<BoxNode>	m_NodeAndMatrix;
	vector<string>			m_SelectNodeName;
	vector<CMesh*>			m_vecMesh;
public:
	CModel(void);
	~CModel(void);
	void Free();

	int Save(string FileName);
	int SaveFromFile(string FileName);
	int SaveFrom3DMax(string FileName);
	bool IsSkeletal( INode* MaxNode );
	bool IsNodeMesh( INode* MaxNode, Interface* pInterface );
	NODETYPE GetNodeType( INode* MaxNode, Interface* pInterface, BOOL IsSelect );
	UINT NodeEnum( INode* MaxNode, Interface* pInterface, BOOL IsSelect, vector<ListNode*>& List,UINT& nSkeletal );
	void SetAnimateGroup(); 
	void BuildBigBox();
	void BuildAABBBox();
	void SetBigBox();
	void SetAABBBox();
	void Init(Interface *ip);
	void UnInit();
	/// �õ�����
	void GetMatrix( D3DXMATRIX& Matrix, UINT SkeletalID,string nodeName ,bool isGlobe);
	int AddSkeletalBox( const string& SkName );

	/// �������
	INode* GetINodeByName(string nodeName);
	void FixPose();
	void ResumePose();
	void DelAllBox();
	void SelAllNode();
	void ReCalBox();
	void UpdateBox();
	void Update();
	UINT NodeEnum(INode* MaxNode);
	void BuildAllBox();
	void GetSelectNodes();
	void AddBoxs();
	bool IsFindBoxNode(string nodename);
	void ClearBoxMap();
private:

	ExportNode*	NodeEnum( INode* MaxNode, ExportNode* Parent );

public:
	CPieceGroup& GetCPieceGroup();
	int			FillFrom3DMax(const SAVEPARAM& SaveParam);
	void		FixParent( ExportNode* Root );
};




#endif
