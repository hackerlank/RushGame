#pragma once

#include <string>
#include <vector>
#include <map>
//#include <d3d9.h>
#include <math.h>
//#include <D3dx9math.h>
#include <fstream>
#include "iresprocessormgr.h"
#include "GraphicBase.h"
#include "GraphicCommon.h"
#include "CSimcloth.h"
#include "cfourcc.h"
#include "CAxisAlignedBox.h"
#include "CCoderProcessor.h"

enum VERTEXTYPE 
{ 
	VT_VERTEXANI	= 0, 
	VT_STATICVERTEX	= 1,
	VT_SKELETALANI	= 2,
	VT_SOFTBODYANI	= 3, 
	VT_SOFTCLOTHANI = 4,
	VT_UNKNOW=0xffffffff,  
};

struct SaveInfo
{
	SaveInfo(){}
	SaveInfo( CVector3f Scale, CVector3f Off, float fTextScale, VERTEXTYPE eType ) : eVetexType( eType )
	{
		int16 xOff = (int16)( Off.x*4 + 0.5f );
		int16 yOff = (int16)( Off.y*4 + 0.5f );
		int16 zOff = (int16)( Off.z*4 + 0.5f );
		int16 xScl = (int16)( Scale.x*4 + 0.5f );
		int16 yScl = (int16)( Scale.y*4 + 0.5f );
		int16 zScl = (int16)( Scale.z*4 + 0.5f );

		sPos = 0;
		sPos = max( sPos, abs( xScl ) );
		sPos = max( sPos, abs( yScl ) );
		sPos = max( sPos, abs( zScl ) );

		xStr = xOff - xScl/2;
		yStr = yOff - yScl/2;
		zStr = zOff - zScl/2;
		sTex = fTextScale;
	}

	VERTEXTYPE eVetexType;

	int16 xStr;
	int16 yStr;
	int16 zStr;
	int16 sPos;
	float sTex;
};

struct VerTexBuf
{
	float x,y,z;
	float nx,ny,nz;
	float u,v;
	float u1,v1;
	float u2,v2;
};

struct VerAnimation
{
	IDNAME							m_AnimationeName;	//������
	DWORD							m_FramePerKeyFrame;	//���㶯��������
	vector< vector<VerTexBuf> >	m_FramesVertex;		//��֡���㻺����
};

class CProMesh;
struct CRenderPiece;

struct CMeshLevel
{
	CMeshLevel ( CProMesh* pMesh ) : m_Dist(0), m_pMesh(pMesh){}
	virtual ~CMeshLevel(){}
	CProMesh*				m_pMesh;
	float				m_Dist;		//��־	
	virtual int BinLoad( ifstream& File, SaveInfo& SI ,int m_Channel){ return 0; }
	virtual int BinLoadFull( ifstream& File, SaveInfo& SI ,int m_Channel){ return 0; }
};

struct CModelLevel : public CMeshLevel
{
	CModelLevel( CProMesh* pMesh ):CMeshLevel(pMesh){}

	vector<VerAnimation>			m_VerAnimate;
	vector<uint32>					m_Weight;			//�������Ȩ��
	vector<uint32>					m_SkeletalIndex;	//�����������
	vector<WORD>					m_VertexIndex;		//�滺����

	int BinLoad( ifstream& File, SaveInfo& SI ,int m_Channel=1);
	int BinLoadFull( ifstream& File, SaveInfo& SI ,int m_Channel=1);
};

//////////////////////////////////////////////////////////////////////////
//����ṹ

/////////////////////////////////////////////////
// class CBallNode
/////////////////////////////////////////////////


enum ESoftType 
{
	SoftPlane,    //ƽ��״����:���� �� һƬ
	SoftCross,    //����״���壺���� �� ��Ƭ
};

struct NodePhyPara
{
	float      m_fAccGravity;     //�������ٶȣ�ţ��/ǧ�ˣ�
	float      m_fMass;           //ÿ���ʵ������
	float      m_fCoeDamp;        //����ϵ��
};

struct CVerNode
{
	float			  m_fTU1;			 //���������
	float			  m_fTV1;			 //����������
	float			  m_fTU2;			 //���������
	float			  m_fTV2;			 //����������
	float			  m_fWidth;			 //�ڵ��
	float			  m_fNatualLen;		 //��Ȼ����
};

struct CBallNode
{
	CVector3f		  m_vPos;			 //����λ��,
	CVector3f		  m_vNormal;		 //���㷨��,
	CVector3f         m_vForce;          //�������ܵ���
	CVector3f         m_vAccel;          //�������ܵļ��ٶ�
	CVector3f         m_vSpeed;          //�����ٶ�

	void Init( CVerNode& Node, float y )
	{
		m_vPos		= CVector3f( 0, y, 0 );
		m_vNormal	= CVector3f( 0, 0, -1 );
		m_vForce	= m_vAccel = m_vSpeed = CVector3f( 0, 0, 0 );
	}
};

struct SpringInfo 
{
	float         m_fRatioCompress;        //�������̱���
	float         m_fRatioPull;            //������������
	float         m_fCoe;
};

//��ײ����ṹ
struct CollideStatic
{
	CollideStatic(){};
	CollideStatic(BYTE nID):m_nBnID(nID){};
	CollideStatic(BYTE nID, float fRadius):m_nBnID(nID), m_fRadius(fRadius){};

	BYTE      m_nBnID;  //Ҫ������ײ���Ĺ���ID
	float     m_fRadius;//�뾶
};

//��ײ����ṹ
struct CollideDynamic
{
	CVector3f m_vBegin; 
	CVector3f m_vEnd;  
}; 

struct CSoftLevel : public CMeshLevel
{
	struct TopVer
	{
		CVector3f	p;
		float		f[4];
		BYTE		b[4];
	};

	CSoftLevel( CProMesh* pMesh );

	NodePhyPara				m_nodePhyPara;			//����������
	SpringInfo				m_spInfo;				//���ɲ���
	ESoftType				m_eSoftType;			//��������
	vector<CVerNode>		m_arrVertex;			//�洢����������
	vector<CollideStatic>	m_arrCollideStatic;		//��ײ�����������
	BYTE					m_bySkeletalID;			//���ӵĹ���
	TopVer					m_vecTop[2];

	vector<CBallNode>		m_arrSpNode;			//�洢��������
	vector<CollideDynamic>	m_arrCollideDynamic;	//��ײ�����������

	int BinLoad( ifstream& File, SaveInfo& SI );
};

class CProMesh
{
	VERTEXTYPE				m_eVertexType;					//��������
	float					m_fBaize;						//zƫ��
	CVector3f				m_Scale;						//ģ������������ռ�Ĵ�С
	CVector3f				m_Off;							//ģ�����Ķ���������ռ�ԭ���ƫ����
	CVector3f				m_OrgScale;						//ģ������������ռ�Ĵ�С
	CVector3f				m_OrgOff;							//ģ�����Ķ���������ռ�ԭ���ƫ����
	float					m_sTex;
	bool					m_IsCompress;
	int						m_Channel;						//��mesh�ж��ٸ�mapchannel
	vector<CMeshLevel*>		m_MeshLevels;					//���LOD

	typedef	map<uint16, CBaseAABB>					BoxMap;
	BoxMap											m_BoxMap;//��Χ���б�

public:
	CProMesh() {m_Channel = 1;m_IsCompress=true;}
	~CProMesh()
	{ 
		for( UINT i = 0; i < m_MeshLevels.size(); i++ )
			delete m_MeshLevels[i];  
	}

	int BinLoad( ifstream& File ,DWORD Version);
};

//��Ⱦ����
struct CPieceRenderStyleProcessor
{
	string				m_RenderStyleName;			//��Ⱦ����
	string				m_RSIndexName;
	DWORD				m_Version;
	vector<string>		m_Texture;

	void SetVersion(DWORD Ver);
	int BinLoad( ifstream& File, string LoadName, CBaseResProcessor* pProcessor );
};

//PIECE
class CPieceProcessor
{
	string						m_Name;				//Piece����
	string						m_ClassName;		//Piece������
	CProMesh						m_Mesh;

	vector<CPieceRenderStyleProcessor*>  m_RenderStyle;		//Piece����Ⱦ����

public:
	int BinLoad( ifstream& File, string LoadName, CBaseResProcessor* pProcessor ,DWORD Version);	
};

/// ͷ����Ϣ
struct PortraitInfo
{
	FourCC		fourcc;
	CVector3f translate;
	CVector3f cameraPos;
	float		fov;
};

//PieceȺ��
struct CPieceProcessorGroup
{
	vector<CPieceProcessor*>			m_Piece;
	PortraitInfo			m_portraitInfo;

	int BinSave( string FileName );
	int BinLoad( string FileName, CBaseResProcessor* pProcessor );

	DWORD m_dwVersion;
};

//////////////////////////////////////////////////////////////////////////
// �᲼(����)�ṹ
//////////////////////////////////////////////////////////////////////////
struct SklIndex{ BYTE  b[4]; };

struct SoftClothProp{
	uint8   Width;
	uint8   Height;
	uint16  MassA;
	uint16  MassB;
	uint16  MassC;
	uint16  WindMin;
	uint16  WindMax;   
	uint16  uWindMaxH;
	float   Gravity;
	float   GassForce;
	float   CoefficientK;
	float   LongRatio;
	float   ShortRatio;
	float   TimePass;
};

////////////////////////////////////////////////////////////////
// ������ײ���ҽṹ
class CTinyWeight
{
	uint8 _w[4];
public:
	CTinyWeight(){}
	CTinyWeight( float w0, float w1, float w2, float w3 )
	{
		_w[0] = (uint8)( w0*200 + 0.5f );
		_w[1] = (uint8)( w1*200 + 0.5f );
		_w[2] = (uint8)( w2*200 + 0.5f );
		_w[3] = (uint8)( w3*200 + 0.5f );

		int n = 0;
		uint8 max = 0;
		int32 sum = 0;
		for( int i = 0; i < 4; i++ )
		{
			if( _w[i] > max )
				n = i;
			sum += _w[i];
		}

		if( sum && sum != 200 )
		{
			sum = _w[n] + 200 - sum;
			if( sum > 200 || sum < 0 )
				throw "����Ȩ�ش���";
			_w[n] = (uint8)sum;
		}
	}

	float GetWeight( uint32 nIndex ) const { return _w[nIndex]*0.005f;}
};

struct CapsuleNode{
	CVector3f   Begin;
	CVector3f   End;
	float       Raidus;
	CTinyWeight w;
	BYTE        b[4];    
};

struct CDynamicSoftCloth
{
	struct VertexFomat{

		VertexFomat( VerNorTex& vnt, float* wi, BYTE* bi):VertexNT(vnt)
		{ 
			*((uint32*)b)   = *((uint32*)bi);
			w[0] = wi[0]; w[1] = wi[1]; w[2] = wi[2]; w[3] = wi[3];
		}

		VerNorTex   VertexNT;
		float       w[4];
		BYTE        b[4];
	};

	struct Capsule {
		CVector3f m_Begin;
		CVector3f m_End;

		Capsule(){}
		Capsule( CVector3f & b, CVector3f & e ):m_Begin(b),m_End(e){ }
	};

	CDynamicSoftCloth():m_VertexBuf(NULL),m_Cloth(NULL),m_ClothProp(NULL),m_NorCache(NULL),m_VertexNum(0)
		,m_Width(0),m_Height(0),m_HSprings(NULL),m_VSprings(NULL){}

		CDynamicSoftCloth( const CDynamicSoftCloth& b)
		{
			this->m_Width        = b.m_Width;
			this->m_Height       = b.m_Height;
			this->m_VertexNum    = b.m_VertexNum;
			this->m_ClothProp    = b.m_ClothProp;
			this->m_HSprings	 = b.m_HSprings;
			this->m_VSprings	 = b.m_VSprings;
			if ( this->m_VertexNum > 0 ) 
			{
				this->m_TopFixVerBuf = b.m_TopFixVerBuf;
				this->m_Capsules     = b.m_Capsules;

				this->m_VertexBuf    = new VerNorTex[ this->m_VertexNum ];
				this->m_NorCache     = new CVector3f[ this->m_VertexNum ];
				this->m_Cloth        = new Cloth::CCloth();
				this->m_Cloth->m_WindPowerMin = b.m_Cloth->m_WindPowerMin;
				this->m_Cloth->m_WindPowerMax = b.m_Cloth->m_WindPowerMax;
				this->m_Cloth->m_TimePass     = b.m_Cloth->m_TimePass;

				for( int i=0; i< this->m_VertexNum; ++i)
				{
					float x = (float)abs ( (int)(i % (int)m_ClothProp->Width - ((int)m_ClothProp->Width-1) / 2 )) / (((int)m_ClothProp->Width-1) / 2 ); x *= x;
					float y = (float)(i / (int)m_ClothProp->Width) / ((int)m_ClothProp->Height -1); y*=y;

					this->m_Cloth->AddNode( Cloth::CSoftVerNode( this->m_VertexBuf + i, this->m_VertexBuf[i].p, this->m_VertexBuf[i].np,
						this->m_VertexBuf[i].tu, this->m_VertexBuf[i].tv,
						(int32)m_ClothProp->MassA/100.0f + ((int32) m_ClothProp->MassB - (int32)m_ClothProp->MassA)/100.0f * y + (float)((int32)m_ClothProp->MassC - (int32)m_ClothProp->MassB ) / 100.0f * x,
						this->m_ClothProp->Gravity, this->m_ClothProp->GassForce) );

					if ( i < this->m_Width ) this->m_Cloth->SetFixNode( i );
				}
				memcpy( (void*)this->m_VertexBuf, (void*)b.m_VertexBuf, sizeof(VerNorTex) * m_VertexNum );

				int basei = 0;
				for(int i=0 ; i< this->m_Width; ++i)
				{
					for(int j=0; j<this->m_Height-1; ++j)
					{
						this->m_Cloth->AddSpring( this->m_Width * j + i, this->m_Width * (j + 1) + i, this->m_ClothProp->CoefficientK, 
							(*m_VSprings)[basei++] , this->m_ClothProp->LongRatio, this->m_ClothProp->ShortRatio );
					}
				}

				uint8 hw = (this->m_Width-1)/2;
				for( int i=1; i< this->m_Height; ++i)
				{
					for(int j=0; j<hw; ++j)
					{
						this->m_Cloth->AddSpring( this->m_Width * i + (hw - j), this->m_Width * i + (hw - j - 1), this->m_ClothProp->CoefficientK, 
							(*m_HSprings)[ (this->m_Width-1) * i + hw - 1 - j], this->m_ClothProp->LongRatio, this->m_ClothProp->ShortRatio );
						this->m_Cloth->AddSpring( this->m_Width * i + (hw + j), this->m_Width * i + (hw + j + 1), this->m_ClothProp->CoefficientK,
							(*m_HSprings)[ (this->m_Width-1) * i + hw + j], this->m_ClothProp->LongRatio, this->m_ClothProp->ShortRatio );
					}
				}
			} 
			else
			{
				this->m_TopFixVerBuf.clear();
				this->m_Capsules.clear();
				this->m_VertexBuf = NULL;
				this->m_Cloth     = NULL;
				this->m_NorCache  = NULL;
			}
		}

		~CDynamicSoftCloth(){
			if( m_NorCache )  delete[] m_NorCache;
			if( m_VertexBuf ) delete[] m_VertexBuf;
			if( m_Cloth )     delete m_Cloth;
		}

		vector< VertexFomat >   m_TopFixVerBuf;    
		VerNorTex *             m_VertexBuf;
		CVector3f *             m_NorCache;
		Cloth::CCloth *         m_Cloth;
		int                     m_VertexNum;
		uint8                   m_Width;
		uint8                   m_Height;

		vector<float>*			m_HSprings;
		vector<float>*			m_VSprings;

		vector< Capsule >       m_Capsules;
		SoftClothProp *         m_ClothProp;

};

class CSoftClothLevel : public CMeshLevel
{
	struct VertexFomat{

		VertexFomat( VerNorTex& vnt, float* wi, BYTE* bi):VertexNT(vnt)
		{ 
			*((uint32*)b)   = *((uint32*)bi);
			w[0] = wi[0]; w[1] = wi[1]; w[2] = wi[2]; w[3] = wi[3];
		}

		VerNorTex   VertexNT;
		float       w[4];
		BYTE        b[4];
	};

	struct CollideCapsule {
		CVector3f m_Begin;
		CVector3f m_End;
		float     m_Radius;
		float     BoneWeight[4];
		BYTE      BoneIndex[4];

		CollideCapsule():m_Begin(0),m_End(0),m_Radius(0){}
		CollideCapsule( CVector3f & b, CVector3f & e, float r, float wi[4], BYTE bi[4])
			:m_Begin(b),m_End(e),m_Radius(r){
				BoneWeight[0] = wi[0]; BoneWeight[1] = wi[1]; BoneWeight[2] = wi[2]; BoneWeight[3] = wi[3]; 
				*(uint32*)BoneIndex = *(uint32*)bi;
			}
	};

	CDynamicSoftCloth *                      m_FooCloth;

	vector< CollideCapsule >                 m_Collides;

	vector< CSoftClothLevel::VertexFomat >   m_TopFixVerBuf;    
	VerNorTex *                              m_VertexBuf;

	int                     m_VertexNum;    
	vector< uint16 >	    m_IndexBuf;

	uint8   m_ClothWidth;
	uint8   m_ClothHeight;

	vector<float>			m_HSprings;
	vector<float>			m_VSprings;

	vector<uint16>			m_TextureU;
	vector<uint16>			m_TextureV;

	SoftClothProp           m_ClothProp;
	CProMesh*                  m_pMesh;

public:
	//CSoftClothLevel();
	CSoftClothLevel( CProMesh* pMesh );

	~CSoftClothLevel();

	void	InitDynData( CDynamicSoftCloth& Data );
	int     BinLoad( ifstream& File, SaveInfo& SI );
	int     BinSave( ofstream& File, SaveInfo& SI );
};

class CArpProcessor : public CCoderProcessor
{
public:
	CArpProcessor(void);
	virtual ~CArpProcessor(void);

	virtual bool			RunProcess( const char* szFilename );
	virtual size_t			GetCountCanBeProcess(){ return 1; };
	virtual unsigned long	GetTypeCanBeProcess( size_t nNum ){ return 'mod'; }

private:
	bool	CheckIsCoderFormat( const char* szFileName );
};
