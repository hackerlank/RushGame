#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include "Exp.h"
#include "GraphicBase.h"
#include "GraphicCommon.h"
#include "CAxisAlignedBox.h"
#include "CMesh.h"
#include "CEditPieceGroup.h"
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

#pragma pack( push, 1 )
struct CapsuleNode{
    CVector3f   Begin;
    CVector3f   End;
    float       Raidus;
    CTinyWeight w;		// float       w[4];
    BYTE        b[4];   
	CapsuleNode():w(0,0,0,0){}
};
#pragma pack(pop)

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

	vector<CMatrix>	m_Matrix;
	vector<CMatrix>	m_InvMatrix;

	int					m_nMaxFrame;


	ExportNode() : m_3DMaxNode(NULL), m_Parent(NULL){};
	~ExportNode()
	{
		
		for( UINT i = 0; i < m_Children.size(); i++ )delete m_Children[i]; 
	};
};

//���ڱ�־��������
//class IDNAME
//{
//	INT64      m_Buf[2]; 
//
//public:
//	IDNAME()
//	{ 
//		memset( m_Buf, 0, 16 ); 
//	}
//	IDNAME( LPCTSTR str )
//	{
//		memset( m_Buf, 0, 16 );
//		if( str )
//		{
//			size_t len = min( strlen( str ), 15 );
//			memcpy( m_Buf, str, len );
//		}
//	} 
//
//	const IDNAME& operator= ( const IDNAME& BN )
//	{
//		memset(m_Buf,0,16);
//		m_Buf[0] = BN.m_Buf[0];
//		m_Buf[1] = BN.m_Buf[1];
//		/*m_Buf[2] = BN.m_Buf[2];
//		m_Buf[3] = BN.m_Buf[3];*/
//		return *this;
//	}
//
//	const IDNAME& operator= ( LPCTSTR str )
//	{
//		memset( m_Buf, 0, 16 );
//		if( str )
//		{
//			size_t len = min( strlen( str ), 15 );
//			memcpy( m_Buf, str, len );
//		}
//		return *this;
//	}
//
//	BOOL operator== ( const IDNAME& BN )
//	{
//		return 	m_Buf[0] == BN.m_Buf[0] && m_Buf[1] == BN.m_Buf[1];
//	}
//
//	operator LPCTSTR() const
//	{
//		return (LPCTSTR)m_Buf;
//	}
//};

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

struct BoxNode
{
	INode* m_Node;
	string m_BoxName;
	string m_MeshName;
	string m_BoneName;
	CVector3f m_vMinimum;
	CVector3f m_vMaximum;
	uint16 m_Id;
};

//��Ⱦ����
struct CPluginRenderStyle : public CPieceRenderStyle
{
public:
	int					m_Num;
	CPluginRenderStyle*		m_oldrenderStyle;
	CPluginRenderStyle()	{ m_RSIndexName=""; m_oldrenderStyle = NULL;}
	
	int					BinSave( ofstream& File );
	int					BinLoad( ifstream& File, const string LoadName );
	void				SetNum(int i)						{ m_Num = i;}
	void				SetVersion(DWORD ver)				{ m_Version = ver; }
	void				SetOldRenderSyle(CPluginRenderStyle* oldrendersytle){m_oldrenderStyle = oldrendersytle;}

	using CPieceRenderStyle::m_TextureName;
	using CPieceRenderStyle::m_RSIndexName;
	using CPieceRenderStyle::m_RenderStyleName;

protected:
	virtual HRESULT 
			_CreateRenderStyle( const char* szName, CRenderStyle** pRS );

	virtual HRESULT	
			_CreateStyleTexture( const TCHAR* szTexName );
};

struct TransFrameInfo
{
	CVector3f					m_FrameTrans;			//�����ؼ�֡ƽ��

	void Inverse(void)
	{
		m_FrameTrans = CVector3f(-m_FrameTrans.x,-m_FrameTrans.y,-m_FrameTrans.z);
	}
};

struct QuatFrameInfo
{
	D3DXQUATERNION				m_FrameQuat;			//�����ؼ�֡��ת
	CPluginTinyQuaternion				m_FrameTinyQuat;		//�����ؼ�֡��ת

	const QuatFrameInfo& operator = ( const QuatFrameInfo& Quat )
	{
		m_FrameQuat = Quat.m_FrameQuat;
		m_FrameTinyQuat = Quat.m_FrameTinyQuat;
		return *this;
	}

	void Inverse(void)
	{
		float InvNorm = m_FrameQuat.x * m_FrameQuat.x + 
			m_FrameQuat.y * m_FrameQuat.y + 
			m_FrameQuat.z * m_FrameQuat.z + 
			m_FrameQuat.w * m_FrameQuat.w;

		if( InvNorm  > 0.0 )
		{	
			InvNorm = float(1.0/InvNorm);
			m_FrameQuat = D3DXQUATERNION( -InvNorm*m_FrameQuat.x, -InvNorm*m_FrameQuat.y, -InvNorm*m_FrameQuat.z, InvNorm*m_FrameQuat.w);
		}
		else
			m_FrameQuat = D3DXQUATERNION(0.0f,0.0f,0.0f,0.0f);
		m_FrameTinyQuat = CPluginTinyQuaternion(m_FrameQuat);
	}
};

struct CKeyFrames
{
	map<WORD,TransFrameInfo>	m_KeyTransFrames;		//�ؼ�֡�ļ���
	map<WORD,QuatFrameInfo>		m_KeyQuatFrames;		//�ؼ�֡�ļ���

	TransFrameInfo GetTransFrameInfo(WORD FrameNo);
	QuatFrameInfo GetQuatFrameInfo(WORD FrameNo);

	bool InsertFrame( WORD FrameNo, const TransFrameInfo& Tran,const QuatFrameInfo& Quat );
	bool IsHaveTransFrame( WORD FrameNo );
	bool IsHaveQuatFrame( WORD FrameNo );
};

class CPluginAnimate
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
	bool						m_IsCpAni;				//�Ƿ�ѹ����������

public:
	CPluginAnimate():m_MaxFrame(0),m_fScale(1.0f),m_fMaxLength(0),m_GroundSpeed(0),m_IsCpAni(true){}
	~CPluginAnimate()
	{
		for( int i = 0; i < (int)m_SkeletalList.size(); i++ )
			delete m_SkeletalList[i];
	}

	int		GetSkeletalNum()								{ return (int)m_SkeletalList.size(); }
	IDNAME	GetName()										{ return m_AnimationeName; };
	void	SetName( IDNAME& Name )							{ m_AnimationeName = Name; };
	void	SetMaxFrame( int MaxFrame )						{ m_MaxFrame = max( MaxFrame, m_MaxFrame ); }
	void	AddSkeletalKeyFrames( CKeyFrames* KeyFrames )	{ m_SkeletalList.push_back( KeyFrames ); }
	void	SetScale( float fScale )						{ m_fScale = fScale;	}
	void	SetMaxLength( float fMaxLength )				{ m_fMaxLength = max( abs( fMaxLength ), m_fMaxLength );	}
	float	GetMaxLength()									{ return m_fMaxLength;	}
	CKeyFrames*	GetKeyFrames(BYTE SkeId)					{ return m_SkeletalList[SkeId]; }
	//int ASCSave( CSaxXmlWriter& Node );
	int BinSave( string SaveName );
};

//�����ĵ���SOCKET
//struct CSkeletalSocket
//{
//	CSkeletalSocket(){ m_SocketName = 0; D3DXMatrixIdentity( &m_Socket ); }
//
//	IDNAME						m_SocketName;			//SOCKET����
//	CMatrix					m_Socket;				//SOCKET����
//};
class CPluginSkeletal;
//ÿ��������SOCKET����
struct CPluginSkeletalUnit
{
	CPluginSkeletalUnit(){}
	CPluginSkeletalUnit( CPluginSkeletal* pSkeletal ) : m_pSkeletal(pSkeletal){}
	CPluginSkeletal*					m_pSkeletal;
	CMatrix					m_InverseMatrix;		//ÿ�����������Ƥ������ȫ�ֿռ䵽�����ֲ��ľ���
	vector<CSkeletalSocket>		m_SocketList;			//������SOCKET���б�

	//int ASCSave( CSaxXmlWriter& Node );
	int BinSave( ofstream& File );
	int BinLoad( ifstream& File );
};

//��������
class CPluginSkeletal
{
public:
	BYTE						m_SkeletalID;           //��ͷID
	string						m_SkeletalName;			//��ͷ����
	vector<CPluginSkeletal*>			m_Children;				//�ӹؽ�
public:
	CPluginSkeletal();
	~CPluginSkeletal();
	void Free();
	int FillFromExportNode( ExportNode* ExpNode, CPluginAnimate& Animate, vector<CPluginSkeletalUnit*>& SkeletalList ,BOOL IsExpAllFrame);
	int GetSkeletalIDByName( const string& Name );
	CPluginSkeletal* GetSkeletalByName(const string& Name);

	//int ASCSave( CSaxXmlWriter& Node );
	int BinSave( ofstream& File );
	int		BinLoad( ifstream& File, CPluginSkeletal** Buf );
	BOOL	IsFitForMe( const CPluginSkeletal* Skeletal, string& ErrSkeletal );	//ƥ�����
};

//������
class CAnimateGroup
{
	BYTE						m_SkeletalTotalNum;		//��������

	float						m_FrameCircle;
	CMatrix					m_Globe;
	CPluginSkeletal					m_SkeletalRoot;			//������
	vector<CPluginSkeletalUnit*>		m_SkeletalList;			//���й�������SOCKET����Ԫ�б�
	vector<CPluginAnimate*>			m_AnimationList;		//���ж����б�
	vector<IDNAME>				m_IdNameList;
	BOOL						m_IsExpAllFrame;
	string						m_IndexSkeName;

	string						m_AniName;

public:
	CAnimateGroup();
	~CAnimateGroup();

	//�õ������б�
	void SetAniName(string& aniname){m_AniName = aniname;}
	vector<CPluginSkeletalUnit*>& GetSkeList(){return m_SkeletalList;}
	vector<CPluginAnimate*>& GetAnimateList()
	{ 
		return m_AnimationList; 
	}
	void SetExpFrame(BOOL IsExpAllFrame)
	{
		m_IsExpAllFrame = IsExpAllFrame;
	}
	vector<IDNAME>& GetIdNameList()
	{
		return	m_IdNameList;
	}
	void  Free();

	int   FillFromExportNode( ExportNode* Root );
	DWORD GetTotalNum()								{ return m_SkeletalTotalNum; }
	void  SetIndxSkeName(string& name)				{ m_IndexSkeName = name; }
	string& GetIndxSkeName()						{ return m_IndexSkeName; }
	int   GetSkeletalIDByName( const string Name )	{ return m_SkeletalRoot.GetSkeletalIDByName( Name ); }

//	int   ASCSave( CSaxXmlWriter& Node );
	int		BinSave( ofstream& File, string SaveName );
	int		BinSaveFromFile( ofstream& File, string SaveName );
	int		BinSaveFromMax( ofstream& File, string SaveName,vector<IDNAME>& IdNameList);
	int		BinLoad( string FileName );
	int		BinLoadSke( string LoadName );

	int		BinLoadAgp( string LoadName );
	int		BinSaveSke( ofstream& File,string SaveName );
	int		BinSaveAgp( string SaveName );
	//����ƥ��
	BOOL  IsFitForMe( CAnimateGroup& AG, string& ErrSkeletal );
	BOOL  IsFitSkeForMe( CAnimateGroup& AG, string& ErrSkeletal );

	//	  ת��Ϊ���°���������
	bool  TransToProAni(void);
};

//enum VERTEXTYPE 
//{ 
//	VT_VERTEXANI	= 0, 
//	VT_STATICVERTEX	= 1,
//	VT_SKELETALANI	= 2,
//	VT_SOFTBODYANI	= 3, 
//    VT_SOFTCLOTHANI = 4,
//	VT_UNKNOW=0xffffffff
//};

//��Ӧ���ַ���
static const TCHAR s_VERTEXTYPE[2][20]={ "VT_VERTEXANI", "VT_SKELETALANI" };

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


//struct CBaseAABB //���ڴ洢�Ľṹ
//{
//	CVector3f m_vMinimum;
//	CVector3f m_vMaximum;
//};

namespace sqr
{
	class CPluginMesh;
}

class CPluginPiece;
class CPluginPieceGroup : public CEditPieceGroup
{
public:
	CPluginPieceGroup()
	{
		m_Piece.resize(1);
	}
	~CPluginPieceGroup()				{ Free(); }

	//vector<CPluginPiece>			m_Piece;
	DWORD					m_Version;
	CPluginPieceGroup*			m_OldPieceGroup;

	void GetMeshFromExpNode( CAnimateGroup& AnimateGroup );
	CPluginPiece* GetOldPieceByName(string pieceName);
	void AddPiece( CPluginPiece* Piece );
	void Free()						
	{ 
		m_OldPieceGroup = NULL;
	}
	void SetPieceGroup(CPluginPieceGroup* oldpiecegroup){m_OldPieceGroup = oldpiecegroup;}
	int  GetPieceNum()				{ return (int)m_Piece[0].size(); }	
	
	int PluginBinSave( ofstream& File, string SaveName );
	int PluginBinLoad( string FileName );

	using CPieceGroup::m_Piece;

	CPluginPiece* GetPiece( size_t nClass, size_t nPiece );
	size_t	GetPieceTotalNum();

	CPluginPiece* 
			GetPieceByName( const string& pieceName );
};

//PIECE
//class CPluginPieceGroup;
class CPluginPiece : public CEditPiece
{
public:
	//string						m_sPieceName;				//Piece����
	//string						m_ClassName;		//Piece������
    CPluginMesh*				m_pPluginMesh;				//Piece��MESH
	//vector<CPluginRenderStyle>  m_RenderStyle;		//Piece����Ⱦ����
	CPluginPiece*				m_oldpiece;

	using CEditPiece::m_sPieceName;
	using CEditPiece::m_RenderStyle;

public:
	CPluginPiece( CPluginPieceGroup* pGroup )
		: CEditPiece(pGroup)
	{
	}

	~CPluginPiece() 
	{
		//m_RenderStyle.clear();
		m_oldpiece = NULL;
	}
    
	GString&	GetPieceName() { return m_sPieceName; }
	bool	GetMeshFromExpNode( CAnimateGroup& AnimateGroup );
	void	SetPluginMesh( CPluginMesh* pMesh );			
	void	SetMesh( CMesh* pMesh );
	void	AddRenderStyle( CPluginRenderStyle* pRenderStyle )	{ m_RenderStyle.push_back( pRenderStyle );};

	CPluginRenderStyle* 
			GetPluginRenderStyle( size_t i );

    CPluginMesh*  GetMash()       { return m_pPluginMesh; }
	void	SetPiece(CPluginPiece* piece){m_oldpiece = piece;}
	//int ASCSave( CSaxXmlWriter& Node );
	int BinSave( ofstream& File, string SaveName, vector<CapsuleNode> & CapsuleNodes );
	int BinLoad( ifstream& File, const string LoadName,DWORD Version);
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

class CPluginModel
{
	CPluginPieceGroup				m_PieceGroup;           //����
	CPluginPieceGroup				m_PieceGroupOld;		//��Ҫ��������Ϣ
	ExportNode*				m_Root;				//���ڵ�
	CAnimateGroup			m_Motions;			//����MAX
	CAnimateGroup			m_AnimateGroup;		//�����ļ�
	BOOL					m_IsExpAllFrame;	

	string					m_AniName;
public:
	
	CPluginModel(void);
	~CPluginModel(void);
	void Free();
	void SetGlobeIsExist(bool IsExist);
	void GetOldModInfoFromFile(string FileName);
	void ClearModData();
	bool IsMatching() 
	{
		return (m_PieceGroup.GetPieceTotalNum() == m_PieceGroupOld.GetPieceTotalNum())?TRUE:FALSE;
	}

	BOOL CheckTrouser();

	BOOL SkeMatching();

	int Save(string FileName);
	int SaveAgp(string FileName);
	int SaveFromFile(string FileName);
	int SaveFrom3DMax(string FileName);

	void SynAniname();

private:

	ExportNode*	NodeEnum( INode* MaxNode, ExportNode* Parent );

public:
	CPluginPieceGroup& GetCPieceGroup();
	int			FillFrom3DMax( const SAVEPARAM& SaveParam );
	int			FillFromFile( string FileName,string& aniname );
	bool		LoadSkeFromFile(string FileName);
	int			FillAgpFromFile(string FileName);
	string&		GetIndxNameFromAgp();
	bool		IsFit(CString FileName);
	void		FixParent( ExportNode* Root );
	void		SetExpFrame(BOOL IsExpAllFrame);
	void		SetIndxSkeName(string& name);
};




#endif
