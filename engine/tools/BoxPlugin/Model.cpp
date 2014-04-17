
//--------------------------------------------------------------------------------
// �ļ���ExpPlugin.cpp
// ���ڣ�2004.3.22
// ���ܣ���3DMAX�������ģ�����ݣ�����Ҫ�����
//------------------------------------------------------------------------------------
// ���ڣ�2007.1.19
// ���ܣ����� ���� �ĵ��� :)
//------------------------------------------------------------------------------------

#include "stdAfx.h"
#include "model.h"
#include "stdmat.h"
#include <algorithm>
#include <fstream>
#include <set>
#include <algorithm>
#include <map>
#include "Phyexp.h"
#include "Bipexp.h"
#include "decomp.h"
#include "ISkin.h"
#include "iiksys.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#define   MINIMUMWEIGHT    0.0001f


//#define	  SingleUV
#define	  DoubleUV
#define GETVERSION(VER)	0x0000ffff&VER
#define GETCP(VER)  (0x00010000&VER)>>16
#define GETBOX(VER)	 (0x00100000&VER)>>20
#define ENABLECP(VER)		0x00010000|VER	
#define ENABLEBOX(VER)		0x00100000|VER
int  FaceIndex[2][3]={ { 0, 1, 2 },{ 0, 2, 1, } };

#define MAX_X		0x2000
#define MAX_Y		0x2010
#define MAX_Z       0x2020
#define MIN_X		0x2030
#define MIN_Y		0x2040
#define MIN_Z		0x2050

SAVEPARAM g_SaveParam;
float	  g_fCurNode;

std::map< string, string >	BoneFixMap;
bool					    HaveBoneFix;
enum ESoftType 
{
	SoftPlane,    //ƽ��״����:���� �� һƬ
	SoftCross,    //����״���壺���� �� ��Ƭ
};

//////////////////////////////////////////////////////////////////////////
//���ڼ�¼·����ȫ����
string g_ARPPath = "";
string g_TexPath = "";
string g_RSName = "";
//////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////
// class CBallNode
/////////////////////////////////////////////////

//enum EVerType{VerPos, VerColor3D, VerNorTex, VerNor, VerNorDif}; //Ԥ���嶥������
#define  VerFormat   VerNorTex //��ƥ�Ķ����ʽ,��λ�ã����ߺ���������

struct NodePhyPara
{
	float      m_fAccGravity;     //�������ٶȣ�ţ��/ǧ�ˣ�
	float      m_fMass;           //ÿ���ʵ������
	float      m_fCoeDamp;        //����ϵ��
};


struct WeightOfBone
{
	WeightOfBone( float f, BYTE ID ) : m_f(f), m_ID(ID){}
	float m_f;
	BYTE  m_ID;

	bool operator< ( const WeightOfBone& a )const { return m_f < a.m_f; }
};

////////////////////////////////////////////////////////////////////////
// ���ܣ���������ɫֵת����˫����ɫ
// ������Point3 color ������ɫֵ
// ���أ�˫����ɫ
////////////////////////////////////////////////////////////////////////
inline DWORD RGBFromPoint3( Point3 color )
{
	color.x = min( 1.0f, color.x );
	color.x = max( 0.0f, color.x );
	color.y = min( 1.0f, color.y );
	color.y = max( 0.0f, color.y );
	color.z = min( 1.0f, color.z );
	color.z = max( 0.0f, color.z );

	return 0xff000000 || ( ( (DWORD)( 255.1f*color.x ) )<<24 )
		||( ( (DWORD)( 255.1f*color.y ) )<<16 )||( (DWORD)( 255.1f*color.z ) );
}

////////////////////////////////////////////////////////////////////////
// ���ܣ���3DMAX����ת����D3DXMATRIX
// ������const Matrix3& original 3DMAX����
// ���أ�ת�����D3DXMATRIX
////////////////////////////////////////////////////////////////////////// 
BOOL Parity( const D3DXMATRIX& original )
{
	if ( D3DXMatrixIsIdentity( &original ) ) 
		return FALSE;
	D3DXVECTOR3 X( original.m[0] );
	D3DXVECTOR3 Y( original.m[1] );
	D3DXVECTOR3 Z( original.m[2] );
	D3DXVECTOR3 A;
	D3DXVec3Cross( &A, &X, &Y );
	if( D3DXVec3Dot( &A, &Z ) > 0 )
		return FALSE;

	return TRUE;
}

const Matrix3 ConvertMatrix3(const Matrix3& original)
{
	Matrix3 result;
	Point3 p0,p1,p2,p3;
	result.IdentityMatrix();
	p0.x = original.GetRow(0).x;
	p0.y = original.GetRow(0).z;
	p0.z = original.GetRow(0).y;
	p1.x = original.GetRow(2).x;
	p1.y = original.GetRow(2).z;
	p1.z = original.GetRow(2).y;
	p2.x = original.GetRow(1).x;
	p2.y = original.GetRow(1).z;
	p2.z = original.GetRow(1).y;
	p3.x = original.GetRow(3).x;
	p3.y = original.GetRow(3).z;
	p3.z = original.GetRow(3).y;

	result.SetRow(0,p0);
	result.SetRow(1,p1);
	result.SetRow(2,p2);
	result.SetRow(3,p3);
	return result;
}
const Matrix3 ConvertMatrix4To3(const D3DXMATRIX& original)
{
	Matrix3 result;
	Point3 p0,p1,p2,p3;
	result.IdentityMatrix();
	p0.x = original.m[0][0];
	p0.y = original.m[0][1];
	p0.z = original.m[0][2];
	p1.x = original.m[1][0];
	p1.y = original.m[1][1];
	p1.z = original.m[1][2];
	p2.x = original.m[2][0];
	p2.y = original.m[2][1];
	p2.z = original.m[2][2];
	p3.x = original.m[3][0];
	p3.y = original.m[3][1];
	p3.z = original.m[3][2];

	result.SetRow(0,p0);
	result.SetRow(1,p1);
	result.SetRow(2,p2);
	result.SetRow(3,p3);
	return result;
}

//��һ�������г�ȡ��ת����,���������ת���ӿ��Թ���һ����ת����
Point3 GetScalePoint(Matrix3 tm)
{
	Point3 vScale;
	Point3 scaleGeneX(tm.GetRow(0).x,tm.GetRow(0).y,tm.GetRow(0).z);
	Point3 scaleGeneY(tm.GetRow(1).x,tm.GetRow(1).y,tm.GetRow(1).z);
	Point3 scaleGeneZ(tm.GetRow(2).x,tm.GetRow(2).y,tm.GetRow(2).z);

	vScale.x = scaleGeneX.Length();
	vScale.y = scaleGeneY.Length();
	vScale.z = scaleGeneZ.Length();
	return vScale;
}
////////////////////////////////////////////////////////////////////////
// ���ܣ���3DMAX����ת����D3DXMATRIX
// ������const Matrix3& original 3DMAX����
// ���أ�ת�����D3DXMATRIX
////////////////////////////////////////////////////////////////////////// 
const D3DXMATRIX ConvertMatrix( const Matrix3& original, BOOL Scale )
{
	D3DXMATRIX result;

	if( Scale )
	{
		//��Y��Z����
		D3DXMatrixIdentity( &result );
		result.m[0][0] = original.GetRow(0).x;
		result.m[0][1] = original.GetRow(0).z;
		result.m[0][2] = original.GetRow(0).y;
		result.m[1][0] = original.GetRow(2).x;
		result.m[1][1] = original.GetRow(2).z;
		result.m[1][2] = original.GetRow(2).y;
		result.m[2][0] = original.GetRow(1).x;
		result.m[2][1] = original.GetRow(1).z;
		result.m[2][2] = original.GetRow(1).y;
		result.m[3][0] = original.GetRow(3).x;
		result.m[3][1] = original.GetRow(3).z;
		result.m[3][2] = original.GetRow(3).y;
	}
	else
	{
		// �ֽ�
		AffineParts affineParts;
		decomp_affine( original, &affineParts );

		// ��Ԫ����ת
		D3DXQUATERNION Quat( affineParts.q.x, affineParts.q.z, affineParts.q.y, affineParts.q.w );
		D3DXMatrixRotationQuaternion( &result, &Quat );

		// ƽ��
		result.m[3][0]= affineParts.t.x;
		result.m[3][1]= affineParts.t.z;
		result.m[3][2]= affineParts.t.y;
	}

	return result;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ��õ�ָ���ڵ��ȫ�־���
// ������INode* MaxNode 3DMAX�ڵ�,
//       int Time       ʱ��
// ���أ�ָ���ڵ��D3DXMATRIX
////////////////////////////////////////////////////////////////////////// 
inline const D3DXMATRIX GetNodeGlobeMatrix( INode* MaxNode, int Time, BOOL Scale )
{
	return ConvertMatrix( MaxNode->GetObjTMBeforeWSM( Time ), Scale );
}

////////////////////////////////////////////////////////////////////////
// ���ܣ��õ�ָ���ڵ�ľֲ�����
// ������INode* MaxNode 3DMAX�ڵ�,
//       int Time       ʱ��
// ���أ�ָ���ڵ��D3DXMATRIX
////////////////////////////////////////////////////////////////////////// 
inline const D3DXMATRIX GetNodeLocalMatrix( INode* MaxNode, int Time, BOOL Scale )
{
	D3DXMATRIX CurNodeMat = GetNodeGlobeMatrix( MaxNode, Time, Scale );
	if( INode* Parent = MaxNode->GetParentNode() )
	{
		D3DXMATRIX ParentMat = GetNodeGlobeMatrix( Parent, Time, Scale );
		D3DXMatrixInverse( &ParentMat, NULL, &ParentMat );
		CurNodeMat = CurNodeMat*ParentMat;
	}
	return CurNodeMat;
}

inline const Matrix3 GetNodeGlobeMatrix3(INode* MaxNode)
{
	return MaxNode->GetNodeTM(0);
}

inline const Matrix3 GetNodeLocalMatrix3(INode* MaxNode)
{
	Matrix3 CurNodeMat = GetNodeGlobeMatrix3( MaxNode);
	if( INode* Parent = MaxNode->GetParentNode() )
	{
		Matrix3 ParentMat = GetNodeGlobeMatrix3( Parent);
		ParentMat.Invert();
		CurNodeMat = CurNodeMat*ParentMat;
	}
	return CurNodeMat;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ���ȡ�ؼ�֡
// ������Animatable* animatable �����ӿ�, 
//       list<int>& KeyFrame    �ؼ�֡�б�, 
//       Interval& range        ��������ʱ�䷶Χ
// ���أ���
////////////////////////////////////////////////////////////////////////// 
void InsertAnimatableKeyFrames( Animatable* animatable, list<int>& KeyFrame, Interval& range )
{
	int numKeyFrames = animatable->NumKeys();
	if( numKeyFrames == NOT_KEYFRAMEABLE )
		return;

	TimeValue curTime;

	for( int i = 0; i < numKeyFrames; i++ )
	{
		curTime = animatable->GetKeyTime( i );
		if( curTime >= range.Start() && curTime <= range.End() )
			KeyFrame.push_back( curTime/GetTicksPerFrame() );
	}
}


////////////////////////////////////////////////////////////////////////
// ���ܣ���ȡ�ؼ�֡
// ������Control* controller	�����ӿ�, 
//       list<int>& KeyFrame    �ؼ�֡�б�, 
//       Interval& range        ��������ʱ�䷶Χ
// ���أ���
////////////////////////////////////////////////////////////////////////// 
void InsertControlKeyFrames( Control* pController, list<int>& KeyFrame, Interval& range )
{
	InsertAnimatableKeyFrames( pController, KeyFrame, range );

	if( pController->GetPositionController() )
		InsertAnimatableKeyFrames( pController->GetPositionController(), KeyFrame, range );
	if( pController->GetRotationController() )
		InsertAnimatableKeyFrames( pController->GetRotationController(), KeyFrame, range );
}

////////////////////////////////////////////////////////////////////////
// ���ܣ�����μ�����֡
// ������Control* controller    �����ؼ�֡���ƽӿ�, 
//       list<int>& KeyFrame    �ؼ�֡�б�, 
//       Interval& range        ��������ʱ�䷶Χ
// ���أ���
////////////////////////////////////////////////////////////////////////// 
void InsertSubKeyFrames( Control* controller, list<int>& KeyFrame, Interval& range )
{
	for( int i = 0; i < controller->NumSubs(); i++ )
	{
		Animatable* subAnim = controller->SubAnim( i );
		if( !subAnim )
			continue;
		InsertAnimatableKeyFrames( subAnim, KeyFrame, range );
	}
}

////////////////////////////////////////////////////////////////////////
// ���ܣ���ȡIK�ؼ�֡
// ������Control* pControl		���ƽӿ�, 
//       list<int>& KeyFrame    �ؼ�֡�б�, 
//       Interval& range        ��������ʱ�䷶Χ
// ���أ���
////////////////////////////////////////////////////////////////////////// 
void InsertIKKeyFrames( Control* pControl, list<int>& KeyFrame, Interval& range )
{
	TSTR ClassName;
	pControl->GetClassName( ClassName );
	//printf( "Controler:%s\n", ClassName );
	IIKControl* pIK = (IIKControl*)pControl->GetInterface( I_IKCONTROL );
	if( pIK ||
		( pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ) ||
		( pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID ) ||
		( pControl->ClassID() == FOOTPRINT_CLASS_ID ) )
	{
		for( int i = range.Start(); i < range.End(); i += GetTicksPerFrame() )
			KeyFrame.push_back( i/GetTicksPerFrame() );
		KeyFrame.push_back( range.End()/GetTicksPerFrame() );
	}
}

////////////////////////////////////////////////////////////////////////
// ���ܣ����������
// ��������
// ���أ���
////////////////////////////////////////////////////////////////////////// 

////////////////////////////////////////////////////////////////////////
// ���ܣ����������
// ��������
// ���أ���
////////////////////////////////////////////////////////////////////////// 
void CRenderMatrix::SetAnimateGroup( CAnimateGroup* AnimateGroup )
{
	delete []m_MatrixUnit;

	if( !AnimateGroup )
	{
		m_SkeletalNum = 1;
		m_MatrixUnit = new MatrixUnit[ m_SkeletalNum ];
		return;
	}

	memset( this, 0, sizeof( CRenderMatrix ) );
	m_SkeletalNum = AnimateGroup->GetSkeletalNum();
	m_MatrixUnit = new MatrixUnit[ m_SkeletalNum ];
	m_IsBlendStart = TRUE;
	m_IsAniBlend = FALSE;
	m_IsLoop = TRUE;
	for( UINT i = 0; i < m_SkeletalNum; i++ )
		m_MatrixUnit[i].m_SkeletalUnit = AnimateGroup->GetSkeleteUnit( i );
}

CRenderMatrix::CRenderMatrix()
{
	memset( this, 0, sizeof( CRenderMatrix ) );
	m_SkeletalNum = 1;
	m_MatrixUnit = new MatrixUnit[ m_SkeletalNum ];
}

CRenderMatrix::~CRenderMatrix()
{
	delete []m_MatrixUnit;
}

CModel::CModel(void)
{
	max = Point3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	min = Point3(  FLT_MAX,  FLT_MAX,  FLT_MAX );
	m_ip = NULL;
	m_dumObj = NULL;
	m_createInterface = NULL;
	m_Root   = NULL;
}

CModel::~CModel(void)
{
	Free();
}

void CModel::Free()
{
	delete m_Root;
	m_NodeType.clear();
	m_NodeAndMatrix.clear();

	if(!m_vecNode.empty())
	{
		//for(int i=0;i<m_vecNode.size();i++)
		//	m_ip->DeleteNode(m_vecNode[i].node);
		m_vecNode.clear();
		m_ip->RedrawViews(0);
	}
	
	m_Pieces.Free();
	m_Motions.Free();
	m_SaveParam.Free();
	//m_SelectBone.clear();
	m_vecMesh.clear();
	m_SelectNodeName.clear();
	m_Root   = NULL;
	m_dumObj = NULL;
}

CPieceGroup& CModel::GetCPieceGroup()
{
	return m_Pieces;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ�����ѡ�����ṹ
// ������const SAVEPARAM& SaveParam    �洢ѡ��Ͳ���,
// ���أ�>0�ɹ�,����ʧ��
////////////////////////////////////////////////////////////////////////// 
int CModel::FillFrom3DMax(const SAVEPARAM& SaveParam)
{
	g_fCurNode		= 0;
	g_SaveParam.Free();
	g_SaveParam     = SaveParam;
	m_Root          = NodeEnum( g_SaveParam.m_uiSP_pInterface->GetRootNode(), NULL );  //ö�ٽڵ�

	FixParent( m_Root );

	//��䶯����Ϣ
	if( m_Root )
		m_Motions.FillFromExportNode( m_Root ); 

	//��䶥����Ϣ
	m_Pieces.GetMeshFromExpNode( m_Motions );

	return 0;
}
void CModel::SetAnimateGroup() 
{
	if( m_Motions.GetSkeletalNum() == 0 )
		return;

}

void CModel::FixParent( ExportNode* Root )
{
	std::map < std::string, ExportNode * > String2ExportNode;
	std::list< ExportNode * > BLStack;
	
	BLStack.push_back( Root );
	while( !BLStack.empty() )
	{
		ExportNode * p = BLStack.front();
		BLStack.pop_front();

		String2ExportNode[ p->m_3DMaxNode->GetName() ] = p;
		
		for( int i = 0; i < p->m_Children.size(); ++i)
		{
			BLStack.push_back( p->m_Children[i] );
		}
	}

	BLStack.push_back( Root );
	while( !BLStack.empty() )
	{
		ExportNode * p = BLStack.front();
		BLStack.pop_front();
	
		INode * MaxNode = p->m_3DMaxNode;
		CStr t;
		string pname;
		bool b = false;
		MaxNode->GetUserPropBuffer( t );
		
		string & SName = string(MaxNode->GetName());
		if( BoneFixMap[SName] != "" )
			pname = BoneFixMap[SName];
		else
		{
			for(int i=0;i<t.length();++i)
			{
				if( t[i] == '[' ) 
				{
					b = true;
				} else if( t[i] == ']' )
				{
					b = false;				
				} else if( b == true )
				{
					pname = pname + t[i];
				}
			}
		}
		
		if ( !pname.empty() )
		{
			vector< ExportNode*>::iterator iter = find( p->m_Parent->m_Children.begin(), p->m_Parent->m_Children.end(), p);
			p->m_Parent->m_Children.erase( iter );

			String2ExportNode[pname]->m_Children.push_back( p );
			p->m_Parent = String2ExportNode[pname];
		}
		
		for( int i = 0; i < p->m_Children.size(); ++i)
		{
			BLStack.push_back( p->m_Children[i] );
		}
	}

}

//////////////////////////////////////////////////////////
// ���ܣ��������нڵ㲢����䵽��Ӧ�Ľṹ����
// ������INode* MaxNode     ��ǰ3DMAX�ڵ�, 
//       ExportNode* Parent ��һ������ڵ�
// ���أ��´����ĵ�ǰ�ڵ�
//////////////////////////////////////////////////////////
ExportNode* CModel::NodeEnum( INode* MaxNode, ExportNode* Parent )
{	
	if( !MaxNode )
		return NULL;

	map< INode*, PIECETYPE >* NodeType = ( map< INode*, PIECETYPE >* )g_SaveParam.m_uiSP_pNodeType;

	ExportNode* ExpNode = new ExportNode();

	if( !ExpNode )
		return NULL;

	//��䵱ǰ�ڵ�

	ExpNode->m_3DMaxNode = MaxNode;
	ExpNode->m_Parent    = Parent;

	if(NodeType->find( MaxNode )!=NodeType->end())
	{
		ExpNode->m_PieceType = NodeType->find( MaxNode )->second;

		//����Ч���������
		if( enumPT_VERTEXANI   == ExpNode->m_PieceType ||
			enumPT_SKELETALANI == ExpNode->m_PieceType ||
			enumPT_NONEANI     == ExpNode->m_PieceType )
		{
			if( MaxNode->GetUserPropBool( "Cloth", ExpNode->m_bSoftLinkBone ) == FALSE )
				ExpNode->m_bSoftLinkBone = FALSE;
			if( MaxNode->GetUserPropBool( "SoftCloth", ExpNode->m_bSoftCloth ) == FALSE )
				ExpNode->m_bSoftCloth = FALSE;
			if( MaxNode->GetUserPropInt( "CapsuleIndex", ExpNode->m_nCapsules ) == FALSE )
				ExpNode->m_nCapsules = -1;
			if( MaxNode->GetUserPropInt( "CapsuleSide", ExpNode->m_nCapsuleSide ) == FALSE )
				ExpNode->m_nCapsuleSide = 0;  


			float fAddBoneWeight = 0.0f;
			if( MaxNode->GetUserPropFloat( "AddOneWhenWeight", fAddBoneWeight ) )
			{
				ExpNode->m_AddBoneWeight[fAddBoneWeight] = 1;
				//printf( "AddOneWhenWeight = %f", fAddBoneWeight );
			}
			if( MaxNode->GetUserPropFloat( "AddTwoWhenWeight", fAddBoneWeight ) )
			{
				ExpNode->m_AddBoneWeight[fAddBoneWeight] = 2;
				//printf( "AddTwoWhenWeight = %f", fAddBoneWeight );
			}
			CPiece Piece;
			CRenderStyle RenderStyle;
			CMesh* Mesh = new CMesh;
			if( !Mesh )
				return NULL;
			Piece.SetMesh( Mesh );
			Piece.GetPieceName() = MaxNode->GetName();
			Mesh->SetExpNode( ExpNode );


			Mtl* Material = MaxNode->GetMtl();
			TSTR className;

			if( Material )
			{
				if (Material->NumSubMtls() > 0)  
				{
					for (int i=1; i<Material->NumSubMtls(); i++) 
					{
						Mtl* subMtl = Material->GetSubMtl(i);
						if (subMtl) {
							className = subMtl->GetFullName();
							Texmap* tex = subMtl->GetSubTexmap( ID_DI );
							if( tex )
							{
								if( tex->ClassID() == Class_ID( BMTEX_CLASS_ID, 0 ) )
								{
									TCHAR* pChar = ((BitmapTex*)tex)->GetMapName();

									int i;
									for( i = (int)strlen( pChar ); i >= 0 ; i-- )
									{
										if( pChar[i] == '\\' || pChar[i] == '/' )
											break;
									}
									string tmpTexName = pChar + i + 1;
									RenderStyle.m_TextureName.push_back(tmpTexName);
								}
							}
						}
					}
					Piece.AddRenderStyle( RenderStyle );
				}
				else if(Material->ClassID() == Class_ID( DMTL_CLASS_ID,0 ))
				{
					// �õ�diffuse��ͼ
					Texmap* tex = Material->GetSubTexmap( ID_DI );
					if( tex )
					{
						if( tex->ClassID() == Class_ID( BMTEX_CLASS_ID, 0 ) )
						{
							TCHAR* pChar = ((BitmapTex*)tex)->GetMapName();


							int i;
							for( i = (int)strlen( pChar ); i >= 0 ; i-- )
							{
								if( pChar[i] == '\\' || pChar[i] == '/' )
									break;
							}
							string tmpTexName = pChar + i + 1;
							RenderStyle.m_TextureName.push_back(tmpTexName);
							//RenderStyle.m_TextureName = pChar + i + 1;
						}
					}	

					Piece.AddRenderStyle( RenderStyle );
				}
				else
				{
					TCHAR Buf[256];
					sprintf_s( Buf, "Material->ClassID() = %x(), !=Class_ID( DMTL_CLASS_ID,0 )", Material->ClassID() );
					MessageBox( 0, Buf, "��ͼID����", 0 );
				}
			}
			m_Pieces.AddPiece( Piece );
		}
	}
	//���������ӽڵ�
	for ( int i=0; i<MaxNode->NumberOfChildren(); i++ )
	{
		ExportNode* childNode = NodeEnum( MaxNode->GetChildNode(i), ExpNode );
		if ( !childNode )
			return NULL;
		ExpNode->m_Children.push_back( childNode );
	}

	return ExpNode;
}
inline void MultPosMat( D3DXVECTOR3& Out, const D3DXVECTOR3& In, const D3DXMATRIX& mat )
{
	Out.x = In.x*mat._11 + In.y*mat._21 + In.z*mat._31 + mat._41;
	Out.y = In.x*mat._12 + In.y*mat._22 + In.z*mat._32 + mat._42;
	Out.z = In.x*mat._13 + In.y*mat._23 + In.z*mat._33 + mat._43;
}

inline void MultPointMatrix3(Point3& Out,const Point3& In,const Matrix3& mat)
{
	Out.x = In.x*mat.GetRow(0).x +In.y*mat.GetRow(1).x +In.z*mat.GetRow(2).x + mat.GetRow(3).x;
	Out.y = In.x*mat.GetRow(0).y +In.y*mat.GetRow(1).y +In.z*mat.GetRow(2).y + mat.GetRow(3).y;
	Out.z = In.x*mat.GetRow(0).z +In.y*mat.GetRow(1).z +In.z*mat.GetRow(2).z + mat.GetRow(3).z;
}

void CModel::FixPose()
{
	Matrix3 TmpMat;
	for (int i=0;i<m_NodeAndMatrix.size();i++)
	{
		TmpMat.IdentityMatrix();
		m_NodeAndMatrix[i].node->SetNodeTM(0,TmpMat);
	}
	m_ip->RedrawViews(0);
}

void CModel::ResumePose()
{
	Matrix3 TmpMat;
	for (int i=0;i<m_NodeAndMatrix.size();i++)
	{
		TmpMat = m_NodeAndMatrix[i].mat;
		m_NodeAndMatrix[i].node->SetNodeTM(0,TmpMat);
	}
	m_ip->RedrawViews(0);
}
void CModel::ClearBoxMap()
{
	vector<CPiece>::iterator iter;
	iter = m_Pieces.m_Piece.begin();
	while(iter!=m_Pieces.m_Piece.end())
	{
		if( iter->GetMash()->GetVertexType() != VT_SOFTBODYANI && iter->GetPieceName().find("weapon")==string::npos)
		{
			CMesh* pMesh = iter->GetMash();
			pMesh->m_BoxMap.clear();
		}
		iter++;
	}
}
int CModel::AddSkeletalBox( const string& SkName )
{
	INode* node = GetINodeByName(SkName);
	if(node==NULL)
		return -1;

	//vector<string>::iterator iter = find(m_SelectBone.begin(),m_SelectBone.end(),SkName);
	int index = 0;
	//if(iter==m_SelectBone.end())
		//index = -1;
	int ID = m_Motions.GetSkeletalIDByName(SkName);

	Matrix3 tMatrix = ConvertMatrix4To3(GetNodeGlobeMatrix(node,0,true));/*node->GetParentNode()->GetNodeTM(0);*/
	if(/*index==-1&& */ID!=-1)
	{
		D3DXMATRIX TmpMat;
		D3DXMATRIX InvWorld;
		D3DXMATRIX Temp;
		D3DXMATRIX TmpMatID;

		GetMatrix(*&TmpMat,ID,SkName,false);
		D3DXMatrixInverse(&TmpMat,NULL,&TmpMat);
		sqr::CVector3f TempPos;
		sqr::CVector3f Pos;
		CBaseAABB aabb;

		vector<CPiece>::iterator iter;
		iter = m_Pieces.m_Piece.begin();
		while(iter!=m_Pieces.m_Piece.end())
		{
			if( iter->GetMash()->GetVertexType() != VT_SOFTBODYANI && iter->GetPieceName().find("weapon")==string::npos)
			{
				CAxisAlignedBox		AAB;
				vector<int>			tmpInd;
				CMesh* pMesh = iter->GetMash();

				Object* obj = pMesh->GetExpNode()->m_3DMaxNode->EvalWorldState( 0 ).obj;
				TriObject* triObj = (TriObject*)obj->ConvertToType( 0, triObjectClassID );
				Mesh& mesh = triObj->GetMesh();	
				Mesh tmpMesh(mesh);
				
				CMeshLevel* pLevel = (CMeshLevel*)pMesh->GetMeshLevels()[0];
				tmpMesh.setNumVerts(pLevel->GetVerNum());

				sqr::CVector3f tmpPos;
				int nCount = 0;
				for(int i=0;i<pLevel->GetVerNum();++i)
				{
					if(pLevel->m_SkeletalIndex[i].b[0] == ID ||//���piece�ϵ�ÿ�����������ͷ��Ӱ��
						pLevel->m_SkeletalIndex[i].b[1] == ID ||
						pLevel->m_SkeletalIndex[i].b[2] == ID ||
						pLevel->m_SkeletalIndex[i].b[3] == ID )
					{
						TempPos.x = 0.0f;
						TempPos.y = 0.0f;
						TempPos.z = 0.0f;
						for(int j = 0; j<4; j++ )
						{
							if( !pLevel->m_SkeletalIndex[i].b[j] )
							{
								if( j == 0 )
									//MultPosMat( *(D3DXVECTOR3*)&TempPos, pLevel->m_FramesVertex[0][i], m_RenderMatrix.m_MatrixUnit[0].m_GlobeSkeletalMatrix );
									break;
							}
							
							TempPos.x = pLevel->m_FramesVertex[0][i].x ;
							TempPos.y = pLevel->m_FramesVertex[0][i].z ;
							TempPos.z = pLevel->m_FramesVertex[0][i].y ;

							tmpPos = TempPos;

							tmpMesh.setVert(nCount,TempPos.x,TempPos.y,TempPos.z);
							tmpInd.push_back(nCount);
							
						}
						nCount++;
						AAB.mergeBox(TempPos);
					}
				}
				for(int j = 0;j<tmpMesh.getNumVerts();j++)
				{
					vector<int>::iterator iter = find(tmpInd.begin(),tmpInd.end(),j);
					if(iter==tmpInd.end())
					{
						tmpMesh.setVert(j,tmpPos.x,tmpPos.y,tmpPos.z);
					}
				}
				if(!AAB.isNull())
				{
					AAB.transformBox(*(CMatrix*)&TmpMat);
					tmpMesh.buildBoundingBox();
					Box3 box = tmpMesh.getBoundingBox();
					aabb.m_vMinimum.x = box.pmin.x;
					aabb.m_vMinimum.y = box.pmin.y;
					aabb.m_vMinimum.z = box.pmin.z;
					aabb.m_vMaximum.x	= box.pmax.x;
					aabb.m_vMaximum.y	= box.pmax.y;
					aabb.m_vMaximum.z	= box.pmax.z;
					pMesh->m_BoxMap.insert(CMesh::BoxMap::value_type(sqr::uint16(ID),aabb));
					m_vecMesh.push_back(pMesh);
				}
			}
			iter++;
		}	
		//m_SelectBone.push_back(SkName);
		return 1;
	}

	return -1;
}

void CModel::BuildAABBBox()
{
	AddSkeletalBox("Bip01 Head");
	AddSkeletalBox("Bip01 Pelvis");
	AddSkeletalBox("Bip01 Spine");
	AddSkeletalBox("Bip01 Spine1");
	AddSkeletalBox("Bip01 R UpperArm");
	AddSkeletalBox("Bip01 L UpperArm");
	AddSkeletalBox("Bip01 R Forearm");
	AddSkeletalBox("Bip01 L Forearm");
	AddSkeletalBox("Bip01 R Calf");
	AddSkeletalBox("Bip01 L Calf");

	AddSkeletalBox("Bip01 R Foot");
	AddSkeletalBox("Bip01 L Foot");
}

void CModel::BuildAllBox()
{
	string nodname;
	for(int i = 0;i<m_NodeAndMatrix.size();i++)
	{
		nodname = m_NodeAndMatrix[i].node->GetName();
		if(nodname.find("Bip")==string::npos)
			AddSkeletalBox(nodname);
	}
}

void CModel::UpdateBox()
{
	m_vecNode.clear();
//1.�õ����ڵ�boxnode
	UINT count	= NodeEnum( m_ip->GetRootNode());  //ö�ٽڵ�
}


void CModel::ReCalBox()
{
	Matrix3 TmpMat;
	for (int i=0;i<m_vecNode.size();i++)
	{
		TmpMat.IdentityMatrix();
		m_vecNode[i].newMat = m_vecNode[i].node->GetNodeTM(0);



		Point3 vScale,vTrans;
		vScale = GetScalePoint(m_vecNode[i].newMat);//���µľ���õ���������
		TmpMat.SetScale(vScale);
		
		vTrans.x = m_vecNode[i].newMat.GetRow(3).x - m_vecNode[i].mat.GetRow(3).x;
		vTrans.y = m_vecNode[i].newMat.GetRow(3).y - m_vecNode[i].mat.GetRow(3).y;
		vTrans.z = m_vecNode[i].newMat.GetRow(3).z - m_vecNode[i].mat.GetRow(3).z;

		//��node�����µ�����
		Point3 saveMax,saveMin;

		Point3 ppOffset =  m_vecNode[i].node->GetObjOffsetPos();
		Box3 boxObj;
		m_vecNode[i].node->GetObjectRef()->GetLocalBoundBox(0,m_vecNode[i].node,NULL,boxObj);
		saveMax = boxObj.pmax + ppOffset;
		saveMin = boxObj.pmin + ppOffset;
		//saveMax.x += vTrans.x;
		//saveMax.y += vTrans.y;
		//saveMax.z += vTrans.z;
		//saveMin.x += vTrans.x;
		//saveMin.y += vTrans.y;
		//saveMin.z += vTrans.z;
		MultPointMatrix3(saveMax,saveMax,TmpMat);
		MultPointMatrix3(saveMin,saveMin,TmpMat);



		float maxX = saveMax.x;
		float maxY = saveMax.z;
		float maxZ = saveMax.y;
		float minX = saveMin.x;
		float minY = saveMin.z;
		float minZ = saveMin.y;
		float *pmaxX,*pmaxY,*pmaxZ,*pminX,*pminY,*pminZ;
		pmaxX = new float;
		pmaxY = new float;
		pmaxZ = new float;
		pminX = new float;
		pminY = new float;
		pminZ = new float;
		*pmaxX = maxX;
		*pmaxY = maxY;
		*pmaxZ = maxZ;
		*pminX = minX;
		*pminY = minY;
		*pminZ = minZ;
		m_vecNode[i].node->ClearAllAppData();
		m_vecNode[i].node->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_X,sizeof(float),pmaxX);
		m_vecNode[i].node->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_Y,sizeof(float),pmaxY);
		m_vecNode[i].node->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_Z,sizeof(float),pmaxZ);
		m_vecNode[i].node->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_X,sizeof(float),pminX);
		m_vecNode[i].node->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_Y,sizeof(float),pminY);
		m_vecNode[i].node->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_Z,sizeof(float),pminZ);
	}
	m_ip->RedrawViews(0);
}

void CModel::Update()
{
	UpdateBox();
	ReCalBox();
}
void CModel::GetSelectNodes()
{
	m_SelectNodeName.clear();
	m_vecMesh.clear();
	int nodeCount = m_ip->GetSelNodeCount();
	for(int i=0;i<nodeCount;i++)
	{
		INode* node;
		node = m_ip->GetSelNode(i);

		NODETYPE NodeType = GetNodeType( node, m_ip, false );

		if( NodeType == enumNT_SKELETAL )
		{
			string skename = node->GetName();
			m_SelectNodeName.push_back(skename);
		}
	}
}

bool CModel::IsFindBoxNode(string nodename)
{
	for(int i=0;i<m_vecNode.size();i++)
	{
		if(nodename==m_vecNode[i].BoxNodeName)
			return true;
	}
	return false;
}
void CModel::AddBoxs()
{
	for(int i=0;i<m_SelectNodeName.size();i++)
	{
		AddSkeletalBox(m_SelectNodeName[i]);
	}
	for(int j=0;j<m_vecMesh.size();j++)
	{
		CMesh* pMesh = m_vecMesh[j];
		string meshName = pMesh->GetMeshName();
		if(pMesh->m_BoxMap.size()>0)
		{
			CMesh::BoxMap::iterator iter = pMesh->m_BoxMap.begin();
			while(iter!=pMesh->m_BoxMap.end())
			{
				string maxName;
				string nodeName = m_Motions.GetSkeletalNameByID(iter->first);
				sqr::uint16 id = iter->first;
				stringstream strId;
				strId<<id;
				string nodeId;
				nodeId = strId.str();

				maxName = meshName;
				maxName += "&";
				maxName += nodeName;
				maxName += "&";
				maxName += nodeId;
				maxName += "&";
				maxName += "ABox";

				if(IsFindBoxNode(maxName))
				{
					iter++;
					continue;
				}

				Matrix3 tmatrix;
				INode *tNode = GetINodeByName(nodeName);
				tmatrix = tNode->GetNodeTM(0);
				//tmatrix.Invert();
				//tmatrix *=rotate;

				Point3 max,min;
				min.x = iter->second.m_vMinimum.x;
				min.y = iter->second.m_vMinimum.y; 
				min.z = iter->second.m_vMinimum.z;
				max.x = iter->second.m_vMaximum.x;
				max.y = iter->second.m_vMaximum.y;
				max.z = iter->second.m_vMaximum.z;

				Point3 center;
				Box3 centerBox(min,max);
				center = centerBox.Center();

				iter++;
				INode *dummyNode;
				m_dumObj = (DummyObject *)m_createInterface->CreateInstance(HELPER_CLASS_ID,Class_ID(DUMMY_CLASS_ID,0)); 			
				assert(m_dumObj);
				dummyNode = m_createInterface->CreateObjectNode(m_dumObj);
				Box3 tbox(min,max);
				m_dumObj->SetBox(tbox);

				dummyNode->CenterPivot(0,FALSE);
				dummyNode->SetNodeTM(0,tmatrix);

				//Matrix3 tm ;
				//tm = tmatrix;
				//AngAxis aa;
				//aa.Set(tm);
				//tm.IdentityMatrix();

				//dummyNode->Rotate(0,tm,aa,TRUE,TRUE,PIV_NONE,true);

				Point3 saveMax,saveMin;
				Point3 ppOffset = dummyNode->GetObjOffsetPos();
				Box3 boxObj = m_dumObj->GetBox();
				saveMax = boxObj.pmax + ppOffset;
				saveMin = boxObj.pmin + ppOffset;

				float maxX = saveMax.x;
				float maxY = saveMax.z;
				float maxZ = saveMax.y;
				float minX = saveMin.x;
				float minY = saveMin.z;
				float minZ = saveMin.y;
				float *pmaxX,*pmaxY,*pmaxZ,*pminX,*pminY,*pminZ;
				pmaxX = new float;
				pmaxY = new float;
				pmaxZ = new float;
				pminX = new float;
				pminY = new float;
				pminZ = new float;
				*pmaxX = maxX;
				*pmaxY = maxY;
				*pmaxZ = maxZ;
				*pminX = minX;
				*pminY = minY;
				*pminZ = minZ;

				dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_X,sizeof(float),pmaxX);
				dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_Y,sizeof(float),pmaxY);
				dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_Z,sizeof(float),pmaxZ);
				dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_X,sizeof(float),pminX);
				dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_Y,sizeof(float),pminY);
				dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_Z,sizeof(float),pminZ);


				CStr strName = maxName.c_str();
				dummyNode->SetName(strName);

				BoxNode tmpNodeMat;
				tmpNodeMat.node = dummyNode;
				tmpNodeMat.BoxNodeName = dummyNode->GetName();
				tmpNodeMat.mat = tmatrix;
				tmpNodeMat.boxCenter = center;
				tmpNodeMat.pmax = max;
				tmpNodeMat.pmin = min;

				m_vecNode.push_back(tmpNodeMat);
			}
		}
	}
}
void CModel::SetAABBBox()
{
	vector<CPiece>::iterator iter;
	iter = m_Pieces.m_Piece.begin();
	while(iter!=m_Pieces.m_Piece.end())
	{
		if( iter->GetMash()->GetVertexType() != VT_SOFTBODYANI )
		{
			CMesh* pMesh = iter->GetMash();
			string meshName = pMesh->GetMeshName();

			if(pMesh->m_BoxMap.size()>0)
			{
				CMesh::BoxMap::iterator iter = pMesh->m_BoxMap.begin();
				while(iter!=pMesh->m_BoxMap.end())
				{
					string maxName;
					string nodeName = m_Motions.GetSkeletalNameByID(iter->first);
					sqr::uint16 id = iter->first;
					stringstream strId;
					strId<<id;
					string nodeId;
					nodeId = strId.str();

					D3DXMATRIX tmpMatrix;
					Matrix3 tmatrix;
					INode *tNode = GetINodeByName(nodeName);
					tmatrix = tNode->GetNodeTM(0);

					Point3 max,min;
					min.x = iter->second.m_vMinimum.x;
					min.y = iter->second.m_vMinimum.y; 
					min.z = iter->second.m_vMinimum.z;
					max.x = iter->second.m_vMaximum.x;
					max.y = iter->second.m_vMaximum.y;
					max.z = iter->second.m_vMaximum.z;

					Point3 center;
					Box3 centerBox(min,max);
					center = centerBox.Center();

					iter++;
					INode *dummyNode;
					m_dumObj = (DummyObject *)m_createInterface->CreateInstance(HELPER_CLASS_ID,Class_ID(DUMMY_CLASS_ID,0)); 			
					assert(m_dumObj);
					dummyNode = m_createInterface->CreateObjectNode(m_dumObj);
					Box3 tbox(min,max);
					m_dumObj->SetBox(tbox);

					dummyNode->CenterPivot(0,FALSE);
					dummyNode->SetNodeTM(0,tmatrix);

					Point3 saveMax,saveMin;
					Point3 ppOffset = dummyNode->GetObjOffsetPos();
					//Box3 tBoundBox;
					//dummyNode->GetObjectRef()->GetLocalBoundBox(0,dummyNode,NULL,tBoundBox);
					Box3 boxObj = m_dumObj->GetBox();
					saveMax = boxObj.pmax + ppOffset;
					saveMin = boxObj.pmin + ppOffset;

					float maxX = saveMax.x;
					float maxY = saveMax.z;
					float maxZ = saveMax.y;
					float minX = saveMin.x;
					float minY = saveMin.z;
					float minZ = saveMin.y;
					float *pmaxX,*pmaxY,*pmaxZ,*pminX,*pminY,*pminZ;
					pmaxX = new float;
					pmaxY = new float;
					pmaxZ = new float;
					pminX = new float;
					pminY = new float;
					pminZ = new float;
					*pmaxX = maxX;
					*pmaxY = maxY;
					*pmaxZ = maxZ;
					*pminX = minX;
					*pminY = minY;
					*pminZ = minZ;

					dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_X,sizeof(float),pmaxX);
					dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_Y,sizeof(float),pmaxY);
					dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MAX_Z,sizeof(float),pmaxZ);
					dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_X,sizeof(float),pminX);
					dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_Y,sizeof(float),pminY);
					dummyNode->AddAppDataChunk(triObjectClassID,DUMMY_CLASS_ID,MIN_Z,sizeof(float),pminZ);

					maxName = meshName;
					maxName += "&";
					maxName += nodeName;
					maxName += "&";
					maxName += nodeId;
					maxName += "&";
					maxName += "ABox";
					CStr strName = maxName.c_str();
					dummyNode->SetName(strName);

					BoxNode tmpNodeMat;
					tmpNodeMat.node = dummyNode;
					tmpNodeMat.BoxNodeName = dummyNode->GetName();
					tmpNodeMat.mat = tmatrix;
					tmpNodeMat.boxCenter = center;
					tmpNodeMat.pmax = max;
					tmpNodeMat.pmin = min;

					m_vecNode.push_back(tmpNodeMat);
				}
			}
		}
		iter++;
	}
}

void CModel::DelAllBox()
{
	if(!m_vecNode.empty())
	{
		for(int i=0;i<m_vecNode.size();i++)
			m_ip->DeleteNode(m_vecNode[i].node);
		m_vecNode.clear();
		m_ip->RedrawViews(0);
	}
}
void CModel::SelAllNode()
{
	INodeTab nodeTab;
	if(!m_vecNode.empty())
	{
		for(int i=0;i<m_vecNode.size();i++)
			nodeTab.AppendNode(m_vecNode[i].node);
		m_ip->SelectNodeTab(nodeTab,true);
	}
}
void CModel::BuildBigBox()
{
	int i;
	for(i = 0;i<(int)m_NodeType.size();i++)
	{
		if(m_NodeType[i]->m_NodeType==enumNT_MESH||m_NodeType[i]->m_NodeType==enumNT_SKELETALMESH)
		{
			string maxName = m_NodeType[i]->m_MaxNode->GetName();
			if(maxName.find("weapon")==string::npos)
			{
				Object* obj = m_NodeType[i]->m_MaxNode->EvalWorldState( 0 ).obj;
				TriObject* triObj = (TriObject*)obj->ConvertToType( 0, triObjectClassID );//������·����,����ûɾ��,��ô�����ڵڶ��ε��õ�ʱ������
				bool deleteTriObj = (triObj != obj);	// ���triObj���·���Ķ��������ɾ��
				// �õ��ڵ�LOCAL����
				//D3DXMATRIX Mat = GetNodeGlobeMatrix(  m_NodeType[i]->m_MaxNode, 0, TRUE );	
				Mesh& mesh = triObj->GetMesh();	

				mesh.buildBoundingBox();
				Box3 box = mesh.getBoundingBox();
				max.x = max( box.pmax.x, max.x);
				max.y = max( box.pmax.y, max.y);
				max.z = max( box.pmax.z, max.z);
				min.x = min( box.pmin.x, min.x);
				min.y = min( box.pmin.y, min.y);
				min.z = min( box.pmin.z, min.z);
			}			
		}
	}
}

void CModel::SetBigBox()
{
	INode *dummyNode;
	m_dumObj = (DummyObject *)m_createInterface->CreateInstance(HELPER_CLASS_ID,Class_ID(DUMMY_CLASS_ID,0)); 			
	assert(m_dumObj);
	dummyNode = m_createInterface->CreateObjectNode(m_dumObj);

	m_dumObj->SetBox(Box3(min,max));
	Matrix3 tmatrix;

	INode *tNode = GetINodeByName("Bip01");
	tmatrix = tNode->GetNodeTM(0);

	dummyNode->CenterPivot(0,FALSE);
	Matrix3 rotateMatrix;
	rotateMatrix.IdentityMatrix();
	rotateMatrix.SetRotateZ(DegToRad(90));
	tmatrix *= rotateMatrix;
	dummyNode->SetNodeTM(0,tmatrix);
	BoxNode tmpNodeMat;
	tmpNodeMat.node = dummyNode;
	tmpNodeMat.BoxNodeName = dummyNode->GetName();
	tmpNodeMat.mat = tmatrix;
	tmpNodeMat.pmax = max;
	tmpNodeMat.pmin = min;
	m_vecNode.push_back(tmpNodeMat);
	

	CStr strName = "Big_ABox";
	dummyNode->SetName(strName);
}
void CModel::Init(Interface *ip)
{
	m_ip = ip;
	m_createInterface = ip->GetIObjCreate();
}
void CModel::UnInit()
{
	m_ip = NULL;
	m_createInterface = NULL;
}
//////////////////////////////////////////////////////////
// ���ܣ�����Ƿ����
// ������INode* MaxNode     3DMAX�ڵ�
// ���أ���
//////////////////////////////////////////////////////////
bool CModel::IsSkeletal( INode* MaxNode )
{
	ObjectState os = MaxNode->EvalWorldState(0);
	if ( os.obj && os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ) 
	{
		if( BONE_OBJ_CLASSID == os.obj->ClassID() )
			return true;

		Control *c = MaxNode->GetTMController();
		if( BIPSLAVE_CONTROL_CLASS_ID == c->ClassID() || 
			BIPBODY_CONTROL_CLASS_ID == c->ClassID() ||
			FOOTPRINT_CLASS_ID == c->ClassID() )
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////
// ���ܣ�����Ƿ�MESH
// ������INode* MaxNode     3DMAX�ڵ�
// ���أ���
//////////////////////////////////////////////////////////
bool CModel::IsNodeMesh( INode* MaxNode, Interface* pInterface )
{
	Object* obj = MaxNode->EvalWorldState( pInterface->GetAnimRange().Start() ).obj;

	if( obj )
		return (TRUE == obj->CanConvertToType( triObjectClassID ));
	else
		return false;
}

NODETYPE CModel::GetNodeType( INode* MaxNode, Interface* pInterface, BOOL IsSelect )
{
	assert( MaxNode );

	//����Ƿ�����ڵ�
	if( IsSkeletal( MaxNode ) )
	{
		return enumNT_SKELETAL;
	}
	// ����ǲ���MESH
	if( IsNodeMesh( MaxNode, pInterface ) && MaxNode->Renderable() && !MaxNode->IsHidden() )
	{
		// ȷ����ǰMESH��Ҫ���
		if( IsSelect && !MaxNode->Selected() )
			return enumNT_UNKNOW;

		// ͨ�����޸Ķ�ջ�в���PHYSIQUE�޸��������Ƿ��������
		Object* obj = MaxNode->GetObjectRef();
		while( obj && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID )
		{
			IDerivedObject* derObj = (IDerivedObject*)obj;
			for( int modStackIndex = 0; modStackIndex < derObj->NumModifiers(); modStackIndex++ )
			{
				Modifier* mod = derObj->GetModifier( modStackIndex );
				if( mod->ClassID() == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ) )
					return enumNT_SKELETALMESH;
				if( mod->ClassID() == SKIN_CLASSID  )
					return enumNT_SKELETALMESH;
				if( mod->ClassID() == BP_CLASS_ID_OSM  )
					return enumNT_SKELETALMESH;
			}
			obj = derObj->GetObjRef();
		}
		return enumNT_MESH;
	}

	return enumNT_UNKNOW;
}
UINT CModel::NodeEnum( INode* MaxNode, Interface* pInterface, BOOL IsSelect,vector<ListNode*>& List, UINT& nSkeletal )
{	
	if( !MaxNode )
		return 0;

	//��䵱ǰ�ڵ�
	NODETYPE NodeType = GetNodeType( MaxNode, pInterface, IsSelect );

	BoxNode nod;
	if( NodeType == enumNT_SKELETAL )
	{
		string str = MaxNode->GetName();
		if(str!="Bip01 Spine" && str!="Bip01 Spine1" && str!="Bip01 Neck" )
		{
			nod.node = MaxNode;
			nod.mat = MaxNode->GetNodeTM(0);
			m_NodeAndMatrix.push_back(nod);
		}
		nSkeletal++;
	}
	//����Ч���������
	if( NodeType == enumNT_MESH || NodeType == enumNT_SKELETALMESH || NodeType == enumNT_SKELETAL )
	{
		ListNode* Node = new ListNode( NodeType, MaxNode );
		List.push_back( Node );
		m_NodeType.push_back(Node);
		if(  NodeType == enumNT_MESH || NodeType == enumNT_SKELETALMESH )
		{
			string str = MaxNode->GetName();
		}
	}

	UINT NodeNum = 0;
	//���������ӽڵ�
	for ( int i=0; i<MaxNode->NumberOfChildren(); i++ )
	{
		int re = NodeEnum( MaxNode->GetChildNode(i), pInterface, IsSelect,List, nSkeletal );
		if ( !re )
			return 0;
		NodeNum += re;
	}
	NodeNum++;

	return NodeNum;
}

UINT CModel::NodeEnum( INode* MaxNode)
{	
	if( !MaxNode )
		return 0;
	
	string nodeName = MaxNode->GetName();
	string boxName ;
	size_t posABox = nodeName.rfind("&");
	if(posABox!=string::npos)
	{
		boxName = nodeName.substr(posABox+1,nodeName.size());
		if(boxName=="ABox")
		{
			BoxNode boxnode;
			boxnode.node = MaxNode;
			boxnode.BoxNodeName = MaxNode->GetName();

			size_t	posId = nodeName.rfind("&",posABox-1);
			int m_Id = atoi(nodeName.substr(posId+1,posABox-posId-1).c_str());
			size_t posBone = nodeName.rfind("&",posId-1);
			string m_BoneName = nodeName.substr(posBone+1,posId-posBone-1);
			size_t posMesh = nodeName.rfind("&",posBone-1);
			string m_MeshName = nodeName.substr(posMesh+1,posBone-posMesh-1);
			INode *node = m_ip->GetINodeByName(m_BoneName.c_str());
			boxnode.mat = node->GetNodeTM(0);

			m_vecNode.push_back(boxnode);
		}
	}
	
	UINT NodeNum = 0;
	//���������ӽڵ�
	for ( int i=0; i<MaxNode->NumberOfChildren(); i++ )
	{
		int re = NodeEnum( MaxNode->GetChildNode(i));
		if ( !re )
			return 0;
		NodeNum += re;
	}
	NodeNum++;

	return NodeNum;
}

void CModel::GetMatrix( D3DXMATRIX& Matrix, UINT SkeletalID,string nodeName ,bool isGlobe)
{
	D3DXMATRIX* pMat = &Matrix;
	if(  SkeletalID < (UINT)m_Motions.GetSkeletalNum() )
	{
		D3DXMATRIX tmpMatrix;
		for(int i=0;i<m_NodeType.size();i++)
		{
			if(nodeName == m_NodeType[i]->m_MaxNode->GetName() && isGlobe)
				(*pMat) = GetNodeGlobeMatrix(m_NodeType[i]->m_MaxNode,0,true);
			if(nodeName == m_NodeType[i]->m_MaxNode->GetName() && !isGlobe)
				(*pMat) = GetNodeLocalMatrix(m_NodeType[i]->m_MaxNode,0,true);
		}
	}
}

INode* CModel::GetINodeByName(string nodeName)
{
	for (int i=0;i<m_NodeType.size();i++)
	{
		if(nodeName == m_NodeType[i]->m_MaxNode->GetName())
			return m_NodeType[i]->m_MaxNode;
	}
	return NULL;
}

//////////////////////////////////////////////////////////
// ���ܣ����MESH��Ϣ
// ������CMotion& Motion       ������κͶ���
// ���أ���
//////////////////////////////////////////////////////////
bool CPiece::GetMeshFromExpNode( CAnimateGroup& AnimateGroup )
{
	m_Mesh->SetMeshName(m_Name);
	return m_Mesh->GetMeshFromExpNode( AnimateGroup );
}
CPiece::CPiece()
{

}
CPiece::~CPiece()
{
	//if(m_Mesh)
	//	delete m_Mesh;
	//m_RenderStyle.clear();
}

//////////////////////////////////////////////////////////
// ���ܣ�������Ӧ��m_ExpNode���MESH��Ϣ
// ������CMotion& Motion       ������κͶ���
// ���أ���
//////////////////////////////////////////////////////////
void CPieceGroup::GetMeshFromExpNode( CAnimateGroup& AnimateGroup )
{
	for( int i = 0; i < (int)m_Piece.size(); )
	{
		string tmp = m_Piece[i].GetPieceName();
		if( m_Piece[i].GetMeshFromExpNode( AnimateGroup ) )
			i++;
		else
			m_Piece.erase( m_Piece.begin() + i );
	}
}

////////////////////////////////////////////////////////////////
// ���ܣ�VerSkinNCT �ȽϺ���
// ��������
// ���أ���
////////////////////////////////////////////////////////////////

bool operator > ( const VerSkinNCT& v1, const VerSkinNCT& v2 )
{
	return memcmp( &v1, &v2, sizeof(VerSkinNCT) ) > 0;
}

bool operator < ( const VerSkinNCT& v1, const VerSkinNCT& v2 )
{
	return memcmp( &v1, &v2, sizeof(VerSkinNCT) ) < 0;
}

//////////////////////////////////////////////////////////
// ���ܣ�������Ӧ��m_ExpNode���MESH��Ϣ
// ������CMotion& Motion       ������κͶ���
// ���أ���
//////////////////////////////////////////////////////////
bool CMesh::GetMeshFromExpNode( CAnimateGroup& AnimateGroup )
{	
	
	g_SaveParam.m_uiSP_pInterface->ProgressUpdate( int( g_fCurNode += 0.1f )*100/g_SaveParam.m_uiSP_NodeNum );
	
	INode* MaxNode = m_ExpNode->m_3DMaxNode;
	CMeshLevel* MeshLevel = new CMeshLevel;
	MeshLevel->m_FramePerKeyFrame = (float)g_SaveParam.m_uiSP_FramePerSample;
	m_MeshLevels.push_back( MeshLevel );

	int Frame = 0;
	int FrameTime = GetTicksPerFrame()*g_SaveParam.m_uiSP_FramePerSample;
	TimeValue Start = g_SaveParam.m_uiSP_pInterface->GetAnimRange().Start();
	TimeValue End   = g_SaveParam.m_uiSP_pInterface->GetAnimRange().End();

	DWORD Diff = 0xff000000;
	Mtl* Material = MaxNode->GetMtl();
	if( Material && Material->ClassID() == Class_ID( DMTL_CLASS_ID,0 ) )
	{
		StdMat* mat = (StdMat*)Material;
		DWORD c = mat->GetAmbient( Start ).toRGB();//yhl_mod
		BYTE* bc = (BYTE*)&c;
		Diff |= bc[2]|(((DWORD)bc[1])<<8)|(((DWORD)bc[0])<<16);
	}

	//������Ƕ��㶯��
	if( enumPT_VERTEXANI == m_ExpNode->m_PieceType )
		m_eVertexType = VT_VERTEXANI;
	else if( enumPT_NONEANI == m_ExpNode->m_PieceType )
		m_eVertexType = VT_STATICVERTEX;
	else if( m_ExpNode->m_bSoftLinkBone ) 
		m_eVertexType = VT_SOFTBODYANI;
    else if( m_ExpNode->m_bSoftCloth ) 
        m_eVertexType = VT_SOFTCLOTHANI;
	else
		m_eVertexType = VT_SKELETALANI;

	if( VT_VERTEXANI != m_ExpNode->m_PieceType )
		End = Start + 1;

	int NewVerNum;
	vector<WORD> NewIndex;                                  
	D3DXVECTOR3 max = D3DXVECTOR3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	D3DXVECTOR3 min = D3DXVECTOR3(  FLT_MAX,  FLT_MAX,  FLT_MAX );
	
	for( TimeValue curTime = Start; curTime<End; curTime += FrameTime, Frame++ )
	{
		
		Object* obj = MaxNode->EvalWorldState( curTime ).obj;
		
		TriObject* triObj = (TriObject*)obj->ConvertToType( curTime, triObjectClassID );//������·����,����ûɾ��,��ô�����ڵڶ��ε��õ�ʱ������
		
		bool deleteTriObj = (triObj != obj);	// ���triObj���·���Ķ��������ɾ��

		// �õ��ڵ�LOCAL����
		D3DXMATRIX Mat = GetNodeGlobeMatrix( MaxNode, curTime, TRUE );	

		// ȡ��MESH�����Ϣ
		int i,j;
		
		Mesh& mesh = triObj->GetMesh();	
		
		int numVerts = mesh.getNumVerts();//������
		int numTris = mesh.getNumFaces();//��������
		int numCVerts = mesh.getNumVertCol();//������ɫ��
		int numTVerts = mesh.getNumTVerts();//��ͼ����,texture channel 1

		int numMaps  = mesh.getNumMaps();

		//��ʼ����ʱ������
		vector<D3DXVECTOR3>		Ver( numVerts );	
		vector<D3DXVECTOR3>		Nor( numVerts );
		vector<SklWeight>		Wgt( numVerts );			
		vector<SkltIndex>		SInd( numVerts );
		map< CVertexAndGroup , D3DXVECTOR3>  mapNormal;
		vector<BOOL>			NorSmooth( numTris*3, FALSE );
		//vector<WORD>			VInd( numTris*3 );

		//�����������㻺����
		vector<VerSkinNCT> VerList( numTris*3 );

		int* Index = FaceIndex[1];
		if( Parity( Mat ) )
			Index = FaceIndex[0];

		//�ҳ�Ҫ�⻬�Ķ��㷨��
		
		if( !g_SaveParam.m_bSP_SmoothAll )
		{
			//˫�ر���������,�ҳ�Ҫƽ���ĵ�
			for( i = 0; i < numTris; i++ ) 
			{
				BOOL* pSmooth = &( NorSmooth[0] ) + i*3;
				for( j = 0; j < numTris; j++ )
				{
					if(i==j)
						continue;
					if( mesh.faces[i].smGroup&mesh.faces[j].smGroup )
					{
						//���������������
						for( int m = 0; m < 3; m++ )
						{
							for( int n = 0; n < 3; n++ )
							{
								if( mesh.faces[i].v[m] == mesh.faces[j].v[n] )
								{
									pSmooth[m] = TRUE;
									break;
								}
							}
						}
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		for( i = 0; i < numTris; i++ ) 
		{
			for (int j=0; j<32; j++) 
			{
				if (mesh.faces[i].smGroup & (1<<j)) 
				{
					if ((j<31) && (mesh.faces[i].smGroup>>(j+1))) 
					{
						//fprintf(pStream,"%d,",j+1); // Add extra comma
						
					} 
					else 
					{
						//fprintf(pStream,"%d ",j+1);
						
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		for( i = 0; i < numVerts; i++ )
			Nor[i] = D3DXVECTOR3( 0, 0, 0 );

		if( m_eVertexType == VT_SKELETALANI || m_eVertexType == VT_SOFTBODYANI || m_eVertexType == VT_SOFTCLOTHANI )//����ǹ�������������Ҫ��ȡ����Ȩ��
		{
			//���½�ȡ��Ȩ�أ���䶥�㻺����
			Object* obj = MaxNode->GetObjectRef();
			while( obj && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID )
			{
				IDerivedObject* derObj = (IDerivedObject*)obj;
				//��3DMAX�༭��ջ�л�����һ��PHYSIQUE�༭����������Ȩ��
				int modStackIndex;
				for( modStackIndex = 0; modStackIndex < derObj->NumModifiers(); modStackIndex++ )
				{
					Modifier* mod = derObj->GetModifier( modStackIndex );
					if( mod->ClassID() == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ) )
					{
						//ȡ��Ȩ�أ���䶥�㻺����
						GetPhysiqueWeight( Wgt, SInd, mod, AnimateGroup );
						break;
					}
					if( mod->ClassID() == SKIN_CLASSID  )
					{
						GetSkinWeight( Wgt, SInd, mod, AnimateGroup );
						break;
					}
					if( mod->ClassID() == BP_CLASS_ID_OSM  )
					{
						GetBPWeight( Wgt, SInd, mod, AnimateGroup );
						break;
					}
				}
				if( modStackIndex < derObj->NumModifiers() )
					break;
				obj = derObj->GetObjRef();
			}
		}

		//��Ϊ�����������������ͼ�������������ɫ������δ����ͬ������Ҫ��������������Ҫ�����ʵ�����
		//1�������������ɷ���������
		for( i = 0; i < numTris; i++ )
		{
			for( j = 0; j < 3; j++ )
			{
				int Ind = mesh.faces[i].v[j]; 
				
//				VInd[ i*3 + Index[j] ] = Ind;

				VerSkinNCT& v = VerList[ i*3 + Index[j] ];
				

				//��䶥������
				v.p = D3DXVECTOR3( mesh.verts[Ind].x, mesh.verts[Ind].z, mesh.verts[Ind].y );			
				D3DXVec3TransformCoord( &v.p, &v.p, &Mat );
				Ver[Ind] = v.p;
				v.b1 = Wgt[Ind].f[0];
				v.b2 = Wgt[Ind].f[1];
				v.b3 = Wgt[Ind].f[2];
				memcpy( v.Index, SInd[Ind].b, 4 );

				//��䶥����ɫ
				if( numCVerts>0 )
					v.diffuse = RGBFromPoint3( mesh.vertCol[ mesh.vcFace[i].t[j] ] )|0xff000000;   
				else
					v.diffuse = Diff;

				//�����ͼ����
				//if( numTVerts>0 )
				//{
				//	UVVert curVertUV = mesh.getTVert( mesh.tvFace[i].t[j] );
				//	v.tu = curVertUV.x;
				//	v.tv = 1.0f - curVertUV.y;

				//��䷨����
				if( g_SaveParam.m_bSP_SmoothAll || NorSmooth[ i*3 + j ] )
				{
					Point3 MaxNor = mesh.FaceNormal( i, true );
					map< CVertexAndGroup , D3DXVECTOR3>::iterator iter;
					iter = mapNormal.find(CVertexAndGroup(Ind,mesh.faces[i].smGroup));
					if(iter==mapNormal.end())
					{
						mapNormal[CVertexAndGroup(Ind,mesh.faces[i].smGroup)] = D3DXVECTOR3(0.0f,0.0f,0.0f);
					}
					mapNormal[CVertexAndGroup(Ind,mesh.faces[i].smGroup)]+=D3DXVECTOR3( MaxNor.x, MaxNor.z, MaxNor.y );
					Nor[Ind] += D3DXVECTOR3( MaxNor.x, MaxNor.z, MaxNor.y );
				}
				else
				{
					Point3 MaxNor = mesh.FaceNormal( i, true );
					v.np = D3DXVECTOR3( MaxNor.x, MaxNor.z, MaxNor.y );					
				}
			}
		}
		
		//�����ͼͨ��map channel
		
		int chanloop;
		vector<int> NumOfUV;
#ifdef DoubleUV
		for (chanloop = 1; chanloop < 10; chanloop++)
		{
#endif
			
			#ifdef SingleUV
			chanloop = 1;
			#endif

			if (mesh.mapSupport(chanloop)) 
			{

				m_Channel++;
				numTVerts = mesh.getNumMapVerts(chanloop);//�õ���ͨ������ͼ����������Ϊ��˵����ͨ��û��ʹ��
				NumOfUV.push_back(numTVerts);
				if(m_Channel!=1)
				{
					for(int j = 0;j<NumOfUV.size()-1;j++)
					{
						if(NumOfUV[j]!=NumOfUV[j+1])
						{
							TCHAR Buf[256];
							sprintf_s( Buf, "ObjectName = %s,UV�������㲻һ��!", m_ObjectName.c_str() );
							MessageBox( 0, Buf, "����", 0 );

							return false;
						}
					}
				}

				for (i = 0; i < numTVerts; i++) 
				{
					UVVert tvert =  mesh.mapVerts(chanloop)[i];//��ǰchannelÿ����ͼ������uv����
					if(m_Channel==1)
						m_uvBuffer.push_back(tvert);
					else
						mesh.setMapVert(1,i,tvert);
					
				}
				for (i = 0; i <  mesh.getNumFaces(); i++) 
				{
					TVFace tface =  mesh.mapFaces(chanloop)[i];//���Եõ���ǰchannel��������������Ķ�������
					switch(chanloop)
					{
					case 0:
					case 1:
						for(j = 0; j < 3; j++ )
						{
							VerSkinNCT& v = VerList[ i*3 + Index[j] ];
							UVVert curVertUV = mesh.getTVert( tface.t[j] );
							v.tu = curVertUV.x;
							v.tv = 1.0f - curVertUV.y;
						}
						break;
					case 2:
						for(j = 0; j < 3; j++ )
						{
							VerSkinNCT& v = VerList[ i*3 + Index[j] ];
							UVVert curVertUV = mesh.getTVert( tface.t[j] );
							v.tu1 = curVertUV.x;
							v.tv1 = 1.0f - curVertUV.y;
						}
						break;
					default:
						for(j = 0; j < 3; j++ )
						{
							VerSkinNCT& v = VerList[ i*3 + Index[j] ];
							UVVert curVertUV = mesh.getTVert( tface.t[j] );
							v.tu2 = curVertUV.x;
							v.tv2 = 1.0f - curVertUV.y;
						}
						break;
					}
				}
			}
#ifdef DoubleUV
		}
#endif
		//������ֵ��������������
		for( i = 0; i < numTris; i++ )
		{
			for( j = 0; j < 3; j++ )
			{
				VerSkinNCT& v = VerList[ i*3 + Index[j] ];
				if( g_SaveParam.m_bSP_SmoothAll || NorSmooth[ i*3 + j ] )
					v.np = mapNormal[CVertexAndGroup( mesh.faces[i].v[j] ,mesh.faces[i].smGroup)];
				//v.np = Nor[ mesh.faces[i].v[j] ];
				D3DXVec3TransformNormal( &v.np, &v.np, &Mat );
				D3DXVec3Normalize( &v.np, &v.np ); 
			}
		}
		

		//����ǵ�һ֡������ɫ����ͼ���ꡢ����Ȩ�ء���������һЩ�Ͷ��㶯��֡�޹ص��������ȹ���
		if( !Frame )
		{
			MeshLevel->m_VertexIndex.resize( numTris*3 );
			NewIndex.resize( numTris*3 );
			map< VerSkinNCT,WORD > VerMap;

			//����ÿһ����Ķ��㣬����ͬ�Ķ���������ϲ���һ��
			for( i = 0; i < numTris; i++ )
			{
				for( j = 0; j < 3; j++ )
				{
					VerSkinNCT& Ver = VerList[ i*3 + j ];
					map< VerSkinNCT,WORD >::iterator it = VerMap.find( Ver );
					if( it == VerMap.end() )
					{
						WORD NoVer = (WORD)VerMap.size();
						VerMap[Ver] = NoVer;

						if( m_eVertexType == VT_SKELETALANI || m_eVertexType == VT_SOFTBODYANI || m_eVertexType == VT_SOFTCLOTHANI)
						{
							SklWeight sw = { Ver.b1, Ver.b2, Ver.b3, 1 - Ver.b1 - Ver.b2 - Ver.b3 };
							MeshLevel->m_Weight.push_back( sw );
							
							SkltIndex si = { Ver.Index[0], Ver.Index[1], Ver.Index[2], Ver.Index[3] };
							MeshLevel->m_SkeletalIndex.push_back( si );
						}

						MeshLevel->m_Diffuse.push_back( Ver.diffuse );

						D3DXVECTOR2 tc( Ver.tu, Ver.tv );
						D3DXVECTOR2 tc1( Ver.tu1, Ver.tv1 );
						D3DXVECTOR2 tc2( Ver.tu2, Ver.tv2 );

						MeshLevel->m_TextCoord.push_back( tc );
						MeshLevel->m_TextCoord1.push_back(tc1);
						MeshLevel->m_TextCoord2.push_back(tc2);
						//�����㶯��֡������λ�õ�ԭ��λ�õ�����
						NewIndex[NoVer] = i*3 + j;
						MeshLevel->m_VertexIndex[ i*3 + j ] = NoVer;
					}
					else
                        MeshLevel->m_VertexIndex[ i*3 + j ] = it->second;
				}
			}
			NewVerNum = (int)VerMap.size();
		}
		
		//��䶥������ͷ���
		vector<D3DXVECTOR3>  temp;
		MeshLevel->m_FramesVertex.push_back( temp );
		MeshLevel->m_FramesNormal.push_back( temp );
		MeshLevel->m_FramesVertex[Frame].resize( NewVerNum );
		MeshLevel->m_FramesNormal[Frame].resize( NewVerNum );
		
		for( i = 0; i < NewVerNum; i++ )			
		{
			MeshLevel->m_FramesVertex[Frame][i] = VerList[ NewIndex[i] ].p;
			MeshLevel->m_FramesNormal[Frame][i] = VerList[ NewIndex[i] ].np;
			//��Χ��ƫ��
			max.x = max( MeshLevel->m_FramesVertex[Frame][i].x, max.x);
			max.y = max( MeshLevel->m_FramesVertex[Frame][i].y, max.y);
			max.z = max( MeshLevel->m_FramesVertex[Frame][i].z, max.z);
			min.x = min( MeshLevel->m_FramesVertex[Frame][i].x, min.x);
			min.y = min( MeshLevel->m_FramesVertex[Frame][i].y, min.y);
			min.z = min( MeshLevel->m_FramesVertex[Frame][i].z, min.z);
		}
		try
		{
			if( deleteTriObj )
			{
				if(triObj)
					triObj->DeleteThis();
			}
		}
		catch(...)
		{
			MessageBox(NULL,"��ͬ���UV������һ��","����",0);
			return FALSE;
		}

	}
	
	m_Scale = max - min;
	m_Off = ( max + min )/2.0f;
	m_Scale.x = ( (sqr::int32)( m_Scale.x*2 ) + 1 )*0.5f;
	m_Scale.y = ( (sqr::int32)( m_Scale.y*2 ) + 1 )*0.5f;
	m_Scale.z = ( (sqr::int32)( m_Scale.z*2 ) + 1 )*0.5f;
	m_Off.x = ( (sqr::int32)( m_Off.x*4 ) )*0.25f;
	m_Off.y = ( (sqr::int32)( m_Off.y*4 ) )*0.25f;
	m_Off.z = ( (sqr::int32)( m_Off.z*4 ) )*0.25f;
	
	if( min.x < -8000 || min.y < -8000 || min.z < -8000 || 
		max.x >  8000 || max.y >  8000 || max.z >  8000  )
	{
		MessageBox( NULL, "ģ�ͷ�Χ̫��,�޷�����", "����", 0 );
		return false;
	}

	D3DXVECTOR2 Delta = m_MaxUV - m_MinUV;
	if( Delta.x >  31 || Delta.y >  31 )
	{
		MessageBox( NULL, "��ͼ��Χ̫��,�޷�����", "����", 0 );
		return false;
	}


	sqr::uint32 TrunkID = 0;
	vector< sqr::int32 > MeshTrunk( MeshLevel->m_TextCoord.size(), -1 );
	for( sqr::uint32 j = 0; j < MeshLevel->m_VertexIndex.size(); j += 3 )
	{
		for( sqr::uint32 k = j ; k < j + 3; k++ )
		{
			if( MeshTrunk[ MeshLevel->m_VertexIndex[k] ] >= 0 )
			{
				sqr::int32 nID = MeshTrunk[ MeshLevel->m_VertexIndex[k] ];
				sqr::int32 nPreID[3];
				bool  bSetOther = false;
				for( sqr::uint32 n = 0; n < 3; n++ )
				{
					nPreID[n] = MeshTrunk[MeshLevel->m_VertexIndex[j+n]];
					MeshTrunk[MeshLevel->m_VertexIndex[j+n]] = nID;
					if( nPreID[n] < 0 )
						continue;
					if( nPreID[n] != nID )
						bSetOther = true;
				}

				if( bSetOther )
				{
					for( sqr::uint32 m = 0; m < j; m++ )
					{
						for( sqr::uint32 n = 0; n < 3; n++ )
						{
							if( MeshTrunk[MeshLevel->m_VertexIndex[m]] == nPreID[n] )
							{
								MeshTrunk[MeshLevel->m_VertexIndex[m]] = nID;
								break;
							}
						}
					}
				}
				break;
			}
			else if( k == j + 2 )
			{
				MeshTrunk[MeshLevel->m_VertexIndex[j+0]] = TrunkID;
				MeshTrunk[MeshLevel->m_VertexIndex[j+1]] = TrunkID;
				MeshTrunk[MeshLevel->m_VertexIndex[j+2]] = TrunkID;
				TrunkID++;
			}
		}
	}
	
	vector< D3DXVECTOR2 > maxUV( TrunkID, D3DXVECTOR2( -FLT_MAX, -FLT_MAX ) );
	vector< D3DXVECTOR2 > minUV( TrunkID, D3DXVECTOR2( FLT_MAX, FLT_MAX ) );
	for( sqr::uint32 j = 0; j < MeshLevel->m_TextCoord.size(); j++ )
	{
		sqr::uint32 uTunkID = MeshTrunk[j];
		maxUV[uTunkID].x = max( MeshLevel->m_TextCoord[j].x, maxUV[uTunkID].x );
		minUV[uTunkID].x = min( MeshLevel->m_TextCoord[j].x, minUV[uTunkID].x );
		maxUV[uTunkID].y = max( MeshLevel->m_TextCoord[j].y, maxUV[uTunkID].y );
		minUV[uTunkID].y = min( MeshLevel->m_TextCoord[j].y, minUV[uTunkID].y );
	}

	vector<POINT> strUV( TrunkID );
	for( sqr::uint32 j = 0; j < TrunkID; j++ )
	{
		if( maxUV[j].x < minUV[j].x )
			continue;

		strUV[j].x = minUV[j].x >= 0 ? (int)abs( minUV[j].x ) : -(int)abs( minUV[j].x );
		if( strUV[j].x > minUV[j].x )
			strUV[j].x--;
		strUV[j].y = minUV[j].y >= 0 ? (int)abs( minUV[j].y ) : -(int)abs( minUV[j].y );
		if( strUV[j].y > minUV[j].y )
			strUV[j].y--;
	}
	
	for( sqr::uint32 j = 0; j < MeshLevel->m_TextCoord.size(); j++ )
	{
		sqr::uint32 uTunkID = MeshTrunk[j];
		MeshLevel->m_TextCoord[j] -= D3DXVECTOR2( strUV[uTunkID].x, strUV[uTunkID].y );
		m_MinUV.x = min( m_MinUV.x, MeshLevel->m_TextCoord[j].x );
		m_MaxUV.x = max( m_MaxUV.x, MeshLevel->m_TextCoord[j].x );
		m_MinUV.y = min( m_MinUV.y, MeshLevel->m_TextCoord[j].y );
		m_MaxUV.y = max( m_MaxUV.y, MeshLevel->m_TextCoord[j].y );
	}
	return MeshLevel->m_VertexIndex.size() != 0;
}

//////////////////////////////////////////////////////////
// ���ܣ��õ������Ȩ��
// ������CMeshFrame* Frame   Ҫ���Ķ���֡, 
//       ExportNode* ExpNode ��Ӧ������ڵ�, 
//       Modifier* mod       �޸���, 
//       CMotion& Motion     �Ǽܲ�κͶ���
// ���أ���
//////////////////////////////////////////////////////////
void CMesh::GetPhysiqueWeight( vector<SklWeight>& VerWeight, vector<SkltIndex>& SkeIndex, Modifier* mod, CAnimateGroup& AnimateGroup )
{
	// �õ�IPhysiqueExport�ӿ�
	IPhysiqueExport* phyExport = (IPhysiqueExport*)mod->GetInterface( I_PHYEXPORT );

	// get the physique export context interface (context is specific to a node, as
	// the modifier may be applied across several nodes)
	IPhyContextExport* phyContext = (IPhyContextExport*)phyExport->GetContextInterface( m_ExpNode->m_3DMaxNode );
	phyContext->ConvertToRigid( TRUE );

	// get the nodes used for the skeletal deformation
	int numVerts = phyContext->GetNumberVertices();

	IPhyVertexExport* vert;				// physique base vertex interface
	IPhyBlendedRigidVertex* blendVert;	// physique rigid blended vertex interface
	IPhyRigidVertex* rigidVert;			// physique rigid vertex interface
	IPhyFloatingVertex* floatVert;		// physique floating vertex interface
	float floatingTmpWeight;			// temporary weight for floating bones
	bool canDoFloatingVerts = false;	// true if this is not physique 2.x


	// check the physique version
	if( phyExport->Version() >= 300 )
		canDoFloatingVerts = true;

	TimeValue Start = g_SaveParam.m_uiSP_pInterface->GetAnimRange().Start();
	Object* obj = m_ExpNode->m_3DMaxNode->EvalWorldState( Start ).obj;
	TriObject* triObj = (TriObject*)obj->ConvertToType( Start, triObjectClassID );
	float* pfWeight = triObj->GetMesh().getVertexWeights(); 

	int curNode;
	// ����ÿ�����㣬�����ӦȨ��
	for( int i = 0; i < numVerts; i++ )
	{
		VerWeight[i].f[0] = VerWeight[i].f[1] = VerWeight[i].f[2] = VerWeight[i].f[3] = 0.0f;
		SkeIndex[i].b[0] = SkeIndex[i].b[1] = SkeIndex[i].b[2] = SkeIndex[i].b[3] = 0;

		list<WeightOfBone> Weight;	
		int ID;
		vert = phyContext->GetVertexInterface( i );
		if( vert )
		{
			int type = vert->GetVertexType();
			switch( type )
			{
				//�����ܶ������Ӱ��
			case RIGID_BLENDED_TYPE:
				blendVert = (IPhyBlendedRigidVertex*)vert;
				for( curNode = 0; curNode < blendVert->GetNumberNodes(); curNode++ )
				{
					// ���Թ�С��Ȩ��
					if( blendVert->GetWeight( curNode ) < MINIMUMWEIGHT )
						continue;
					if( SUCCEEDED( ID = AnimateGroup.GetSkeletalIDByName( blendVert->GetNode( curNode )->GetName() ) ) )
						Weight.push_back( WeightOfBone( blendVert->GetWeight( curNode ), ID ) );
				}
				break;
				//������һ������Ӱ��
			case RIGID_TYPE:
				rigidVert = (IPhyRigidVertex*)vert;
				if( SUCCEEDED( ID = AnimateGroup.GetSkeletalIDByName( rigidVert->GetNode()->GetName() ) ) )
					Weight.push_back( WeightOfBone( 1.0f, ID ) );
				break;
			default:
				break;
			}
		}
		phyContext->ReleaseVertexInterface( vert );

		if( canDoFloatingVerts )
		{
			floatVert = (IPhyFloatingVertex*)phyContext->GetFloatingVertexInterface( i );
			if( floatVert )
			{
				for( curNode = 0; curNode < floatVert->GetNumberNodes(); curNode++ )
				{
					// ���Թ�С��Ȩ�� ������
					if( floatVert->GetWeight( curNode, floatingTmpWeight ) < MINIMUMWEIGHT )
						continue;
					if( SUCCEEDED( ID = AnimateGroup.GetSkeletalIDByName( floatVert->GetNode( curNode )->GetName() ) ) )
						Weight.push_back( WeightOfBone( floatVert->GetWeight( curNode, floatingTmpWeight ), ID ) );
				}
				phyContext->ReleaseVertexInterface( floatVert );
			}
		}

		//���潫�õ���Ȩ�����򣬱�������4�������ҵ�λ��������CMeshFrame* Frame
		Weight.sort();
		list<WeightOfBone>::reverse_iterator it = Weight.rbegin();
		int j = 0;
		float W[4] = { 0, 0, 0, 0 };
		float Total = 0;
		int nWeightPerVertex = g_SaveParam.m_uiSP_WeightPerVertex;
		map<float,int>::iterator itW = m_ExpNode->m_AddBoneWeight.find( pfWeight[i] );
		if( itW != m_ExpNode->m_AddBoneWeight.end() )
		{
			nWeightPerVertex = min( 4, nWeightPerVertex + itW->second );
			printf( "Vertex%d have %d bone weight\n", i, nWeightPerVertex );
		}

		for( ; it!=Weight.rend()&&j<(int)nWeightPerVertex; it++,j++ )
		{			
			SkeIndex[i].b[j] = (*it).m_ID;
			Total += W[j] = (*it).m_f;
		}
		if( Total > MINIMUMWEIGHT )
		{
			for( j = 0; j < (int)nWeightPerVertex; j++ )
				VerWeight[i].f[j] = W[j]/Total;
		}
	}
}

//////////////////////////////////////////////////////////
// ���ܣ��õ������Ȩ��
// ������CMeshFrame* Frame   Ҫ���Ķ���֡, 
//       ExportNode* ExpNode ��Ӧ������ڵ�, 
//       Modifier* mod       �޸���, 
//       CMotion& Motion     �Ǽܲ�κͶ���
// ���أ���
//////////////////////////////////////////////////////////
void CMesh::GetSkinWeight( vector<SklWeight>& VerWeight, vector<SkltIndex>& SkeIndex, Modifier* mod, CAnimateGroup& AnimateGroup )
{
	TimeValue Start = g_SaveParam.m_uiSP_pInterface->GetAnimRange().Start();
	Object* obj = m_ExpNode->m_3DMaxNode->EvalWorldState( Start ).obj;
	TriObject* triObj = (TriObject*)obj->ConvertToType( Start, triObjectClassID );
	float* pfWeight = triObj->GetMesh().getVertexWeights(); 

	ISkin* skinExport = (ISkin*)mod->GetInterface( I_SKIN );

	// get the skin export context interface (context is specific to a node, as
	// the modifier may be applied across several nodes)
	ISkinContextData* skinContext = (ISkinContextData*)skinExport->GetContextInterface( m_ExpNode->m_3DMaxNode );

	int numVerts = skinContext->GetNumPoints();

	int curNode;
	int curBoneIndex;
	// loop through verts adding bones to the skeletal nodes set as found
	for( int i = 0; i < numVerts; i++ )
	{
		VerWeight[i].f[0] = VerWeight[i].f[1] = VerWeight[i].f[2] = VerWeight[i].f[3] = 0.0f;
		SkeIndex[i].b[0] = SkeIndex[i].b[1] = SkeIndex[i].b[2] = SkeIndex[i].b[3] = 0;
		list<WeightOfBone> Weight;	
		int ID;
		for( curNode = 0; curNode < skinContext->GetNumAssignedBones( i ); curNode++ )
		{
			if( skinContext->GetBoneWeight( i, curNode ) < MINIMUMWEIGHT )
				continue;
			curBoneIndex = skinContext->GetAssignedBone( i, curNode );
			if( SUCCEEDED( ID = AnimateGroup.GetSkeletalIDByName( skinExport->GetBone( curBoneIndex )->GetName() ) ) )
				Weight.push_back( WeightOfBone( skinContext->GetBoneWeight( i, curNode ), ID ) );
		}
		Weight.sort();
		list<WeightOfBone>::reverse_iterator it = Weight.rbegin();
		UINT j = 0;
		float W[4] = { 0, 0, 0, 0 };
		float Total = 0;
		int nWeightPerVertex = g_SaveParam.m_uiSP_WeightPerVertex;
		map<float,int>::iterator itW = m_ExpNode->m_AddBoneWeight.find( pfWeight[i] );
		if( itW != m_ExpNode->m_AddBoneWeight.end() )
		{
			nWeightPerVertex = min( 4, nWeightPerVertex + itW->second );
			printf( "Vertex%d have %d bone weight\n", i, nWeightPerVertex );
		}

		for( ; it!=Weight.rend() && j < (UINT)nWeightPerVertex; it++,j++ )
		{			
			SkeIndex[i].b[j] = (*it).m_ID;
			Total += W[j] = (*it).m_f;
		}
		if( Total > MINIMUMWEIGHT )
		{
			for( j = 0; j < (UINT)nWeightPerVertex; j++ )
				VerWeight[i].f[j] = W[j]/Total;
		}
	}
}

//////////////////////////////////////////////////////////
// ���ܣ��õ������Ȩ��
// ������CMeshFrame* Frame   Ҫ���Ķ���֡, 
//       ExportNode* ExpNode ��Ӧ������ڵ�, 
//       Modifier* mod       �޸���, 
//       CMotion& Motion     �Ǽܲ�κͶ���
// ���أ���
//////////////////////////////////////////////////////////
void CMesh::GetBPWeight( vector<SklWeight>& VerWeight, vector<SkltIndex>& SkeIndex, Modifier* mod, CAnimateGroup& AnimateGroup )
{
	TimeValue Start = g_SaveParam.m_uiSP_pInterface->GetAnimRange().Start();
	Object* obj = m_ExpNode->m_3DMaxNode->EvalWorldState( Start ).obj;
	TriObject* triObj = (TriObject*)obj->ConvertToType( Start, triObjectClassID );
	float* pfWeight = triObj->GetMesh().getVertexWeights(); 

	//for( int i = 0; i < VerWeight.size(); i++ )
	//	printf( "Vertex%d W = %f\n", i, pfWeight[i] );

	BonesPro_WeightArray* wa = NULL;
	mod->SetProperty ( BP_PROPID_GET_WEIGHTS, &wa );

	if( wa == NULL )
	{
		TCHAR Buf[256];
		sprintf_s( Buf, "�޷��õ�BonesPro�޸�������%s�Ĺ���Ȩ��", m_ExpNode->m_3DMaxNode->GetName() );
		MessageBox( NULL, Buf, "Error" , 0 );
		return;
	}

	int numVerts = wa->nv;

	// loop through verts adding bones to the skeletal nodes set as found
	for( int i = 0; i < numVerts; i++ )
	{
		VerWeight[i].f[0] = VerWeight[i].f[1] = VerWeight[i].f[2] = VerWeight[i].f[3] = 0.0f;
		SkeIndex[i].b[0] = SkeIndex[i].b[1] = SkeIndex[i].b[2] = SkeIndex[i].b[3] = 0;
		list<WeightOfBone> Weight;	
		int ID;

		for( int b = 0; b < wa->nb; b++ )
		{
			if( wa->w[ i * wa->nb + b ] < MINIMUMWEIGHT )
				continue;
			BonesPro_Bone  bone;
			bone.t     = g_SaveParam.m_uiSP_pInterface->GetAnimRange().Start();
			bone.index = b;
			mod->SetProperty ( BP_PROPID_GET_BONE, &bone );

			if( SUCCEEDED( ID = AnimateGroup.GetSkeletalIDByName( bone.node->GetName() ) ) )
				Weight.push_back( WeightOfBone( wa->w[ i * wa->nb + b ], ID ) );
		}

		Weight.sort();
		list<WeightOfBone>::reverse_iterator it = Weight.rbegin();
		UINT j = 0;
		float W[4] = { 0, 0, 0, 0 };
		float Total = 0;
		int nWeightPerVertex = g_SaveParam.m_uiSP_WeightPerVertex;
		map<float,int>::iterator itW = m_ExpNode->m_AddBoneWeight.find( pfWeight[i] );
		if( itW != m_ExpNode->m_AddBoneWeight.end() )
		{
			nWeightPerVertex = min( 4, nWeightPerVertex + itW->second );
			//printf( "Vertex%d have %d bone weight\n", i, nWeightPerVertex );
		}

		for( ; it != Weight.rend() && j < (UINT)nWeightPerVertex; it++,j++ )
		{			
			SkeIndex[i].b[j] = (*it).m_ID;
			Total += W[j] = (*it).m_f;
		}
		if( Total > MINIMUMWEIGHT )
		{
			for( j = 0; j < (UINT)nWeightPerVertex; j++ )
				VerWeight[i].f[j] = W[j]/Total;
		}
	}
}

////////////////////////////////////////////////////////////////////////
// ���ܣ����������
// ��������
// ���أ���
////////////////////////////////////////////////////////////////////////// 
CMesh::CMesh()
{
	m_ExpNode = NULL;
	m_MinUV.x = FLT_MAX;
	m_MaxUV.x = -FLT_MAX;
	m_MinUV.y = FLT_MAX;
	m_MaxUV.y = -FLT_MAX;

	m_Channel=0;
	m_IsCompress = true;
	m_tIsCompress = true;
	m_fBaize = 0;
}

CMesh::~CMesh()
{ 
	for( UINT i = 0; i < m_MeshLevels.size(); i++ )
		delete m_MeshLevels[i];
	m_MeshLevels.clear();
	//m_BoxMap.clear();
	//if(m_ExpNode)
	//	delete m_ExpNode;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ����������
// ��������
// ���أ���
////////////////////////////////////////////////////////////////////////// 
CAnimateGroup::CAnimateGroup()
{
	m_SkeletalTotalNum = 0;
	m_FrameCircle=0;
	//D3DXMATRIX					m_Globe;
	//CSkeletal					m_SkeletalRoot;			//������
	m_SkeletalList.clear();			//���й�������SOCKET����Ԫ�б�
	m_AnimationList.clear();		//���ж����б�
	m_IdNameList.clear();
}

CAnimateGroup::~CAnimateGroup() 
{ 
	Free(); 
}

void CAnimateGroup::Free()
{ 
	int i;
	for( i = 0; i < (int)m_SkeletalList.size(); i++ )
		delete m_SkeletalList[i];
	for( i = 0; i < (int)m_AnimationList.size(); i++ )
		delete m_AnimationList[i];
	m_SkeletalTotalNum = 0;  
	m_SkeletalRoot.Free();
    m_SkeletalList.clear();
	m_AnimationList.clear();
	m_IdNameList.clear();
}

string	 CAnimateGroup::GetSkeletalNameByID( BYTE SkeletalID )
{
	return m_SkeletalRoot.GetSkeletalNameByID(SkeletalID);
}
CSkeletalUnit* CAnimateGroup::GetSkeleteUnit( int Num )
{
	return m_SkeletalList[Num];
}
////////////////////////////////////////////////////////////////////////
// ���ܣ���䶯����Ϣ
// ������ExportNode* Root      ���ڵ�
// ���أ�>0�ɹ�,����ʧ��
////////////////////////////////////////////////////////////////////////// 
int CAnimateGroup::FillFromExportNode( ExportNode* Root )
{
	m_SkeletalTotalNum = 0;
	int MaxFrame = 0;
	IDNAME Name = g_SaveParam.m_uiSP_AnimationName;
	
	CAnimate* Animate = new CAnimate;
	Animate->SetName( Name );
	m_SkeletalRoot.FillFromExportNode( Root, *Animate, m_SkeletalList );
	m_SkeletalTotalNum = Animate->GetSkeletalNum();
	m_AnimationList.push_back( Animate );

	return 0;
}
////////////////////////////////////////////////////////////////////////
// ���ܣ�����Ǽܾ���
// ������D3DXMATRIX& matWorld					�������,
//       CRenderMatrix& RenderMatrix			�����������״̬�Ľṹ,
//       vector<CAnimate*>& Animates			������, 
//       vector<CSkeletalUnit*>& SkeletalUnit	�����б�,
//		 BOOL& CompleteFlag                     ������Ⱦ��ɱ�־��������ֵ�ã�
// ���أ���
////////////////////////////////////////////////////////////////////////// 
void CSkeletal::CalculateMatrix( const D3DXMATRIX& matWorld, CRenderMatrix& RenderMatrix, const vector<CAnimate*>& Animates, 
								const vector<CSkeletalUnit*>& SkeletalUnit, bool& CompleteFlag ) const
{
	//���û�йؼ�֡������
	if( m_SkeletalID )
	{
		float fParam = Animates[ RenderMatrix.m_CurAnimate ]->GetTransParam();
		if( !RenderMatrix.m_IsAniBlend )//�������Ҫ�������ں�
		{
			const CKeyFrames& KeyFrameList = 
				Animates[ RenderMatrix.m_CurAnimate ]->GetKeyFrame( m_SkeletalID );

			//����������������
			map< WORD, TransFrameInfo >::const_iterator itTran = KeyFrameList.m_KeyTransFrames.end();
			map< WORD, QuatFrameInfo >::const_iterator itQuat = KeyFrameList.m_KeyQuatFrames.end();
			--itTran;
			--itQuat;
			float CurFrame = RenderMatrix.m_CurFrame;
			WORD wCurFrame = (WORD)CurFrame;
			D3DXVECTOR3& TranInfo = RenderMatrix.m_MatrixUnit[ m_SkeletalID ].m_FrameTrans;
			D3DXQUATERNION& QuatInfo = RenderMatrix.m_MatrixUnit[ m_SkeletalID ].m_FrameQuat;

			map< WORD, TransFrameInfo >::const_iterator nextTran = KeyFrameList.m_KeyTransFrames.lower_bound( 0 );
			if( wCurFrame == nextTran->first )	
				nextTran++;
			itTran = nextTran--;
			float wghTran = 1.0f;
			TranInfo = itTran->second.m_FrameTinyTrans.Get( fParam );

			map< WORD, QuatFrameInfo >::const_iterator nextQuat = KeyFrameList.m_KeyQuatFrames.lower_bound( 0 );
			if( wCurFrame == nextQuat->first )	
				nextQuat++;
			itQuat = nextQuat--;
			D3DXQUATERNION Q1 = (D3DXQUATERNION)itQuat->second.m_FrameQuat;
			QuatInfo = Q1;

			D3DXMATRIX mat;
			D3DXMatrixRotationQuaternion( &mat, &QuatInfo );
			mat.m[3][0] = TranInfo.x;
			mat.m[3][1] = TranInfo.y;
			mat.m[3][2] = TranInfo.z;
			RenderMatrix.m_MatrixUnit[m_SkeletalID].m_CurSkeletalMatrix = mat*matWorld;
		}
	}
	else
		RenderMatrix.m_MatrixUnit[m_SkeletalID].m_CurSkeletalMatrix = matWorld;

	//���������ӹ���
	for( int i=0; i<(int)m_Children.size(); i++ )
		m_Children[i]->CalculateMatrix( RenderMatrix.m_MatrixUnit[m_SkeletalID].m_CurSkeletalMatrix, 
		RenderMatrix, Animates, SkeletalUnit, CompleteFlag );

	if( abs( Animates[ RenderMatrix.m_CurAnimate ]->GetScale() - 1.0f ) > 0.001f )
	{
		D3DXMATRIX mat(
			Animates[ RenderMatrix.m_CurAnimate ]->GetScale(), 0, 0, 0,
			0, Animates[ RenderMatrix.m_CurAnimate ]->GetScale(), 0, 0,
			0, 0, Animates[ RenderMatrix.m_CurAnimate ]->GetScale(), 0,
			0, 0, 0, 1 );
		RenderMatrix.m_MatrixUnit[m_SkeletalID].m_CurSkeletalMatrix = 
			mat*RenderMatrix.m_MatrixUnit[m_SkeletalID].m_CurSkeletalMatrix;
	}

	D3DXMATRIX TmpMat = RenderMatrix.m_MatrixUnit[m_SkeletalID].m_CurSkeletalMatrix;
	RenderMatrix.m_MatrixUnit[m_SkeletalID].m_GlobeSkeletalMatrix = 
		RenderMatrix.m_MatrixUnit[m_SkeletalID].m_SkeletalUnit->m_InverseMatrix*
		RenderMatrix.m_MatrixUnit[m_SkeletalID].m_CurSkeletalMatrix;
}
////////////////////////////////////////////////////////////////////////
// ���ܣ���䶯����Ϣ
// ������ExportNode* ExpNode      ���ڵ�
//       CAnimate& Animate		  ������Ϣ
// ���أ�>0�ɹ�,����ʧ��
////////////////////////////////////////////////////////////////////////// 
int CSkeletal::FillFromExportNode( ExportNode* ExpNode, CAnimate& Animate, vector<CSkeletalUnit*>& SkeletalList )
{
	g_SaveParam.m_uiSP_pInterface->ProgressUpdate( int( g_fCurNode += 0.5f )*100/g_SaveParam.m_uiSP_NodeNum );

	m_SkeletalID   = Animate.GetSkeletalNum();
	m_SkeletalName = ExpNode->m_3DMaxNode->GetName();
	
	CKeyFrames KeyFrames;
	CKeyFrames* pKeyFrames = new CKeyFrames;
	Animate.AddSkeletalKeyFrames( pKeyFrames );
  
	CSkeletalUnit* SkeletalUnit = new CSkeletalUnit;
	SkeletalList.push_back( SkeletalUnit );
	D3DXMatrixIdentity( &SkeletalUnit->m_InverseMatrix );

	bool bIK = false;

	if( ExpNode->m_Parent )
	{
		WORD uMaxFrame = 0;
		list<int> KeyFrameList;
		//�õ��������
		INode* MaxNode = ExpNode->m_3DMaxNode;
		Control* controller = MaxNode->GetTMController();

		KeyFrameList.push_back( g_SaveParam.m_uiSP_pInterface->GetAnimRange().End()/GetTicksPerFrame() );
		KeyFrameList.push_back( g_SaveParam.m_uiSP_pInterface->GetAnimRange().Start()/GetTicksPerFrame() );
		if( controller )
		{
			//// ����ؼ�֡
			// ��������֡��Ϣ
			Interval& range = g_SaveParam.m_uiSP_pInterface->GetAnimRange();
			for( int i = range.Start(); i < range.End(); i += GetTicksPerFrame() )
				KeyFrameList.push_back( i/GetTicksPerFrame() );
			KeyFrameList.push_back( range.End()/GetTicksPerFrame() );


			bIK = controller->GetInterface( I_IKCONTROL )||
				  controller->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
				  controller->ClassID() == BIPBODY_CONTROL_CLASS_ID ||
				  controller->ClassID() == FOOTPRINT_CLASS_ID;
		}
		KeyFrameList.sort();
		KeyFrameList.unique();	
		//printf( ExpNode->m_3DMaxNode->NodeName() );
		//printf( " Keys:%d\n", KeyFrameList.size() );

		//�õ��ֲ���ȫ�ֵľ��󣬲��������
		SkeletalUnit->m_InverseMatrix = GetNodeGlobeMatrix( MaxNode, (*KeyFrameList.begin())*GetTicksPerFrame(), FALSE );
		D3DXMatrixInverse( &SkeletalUnit->m_InverseMatrix, NULL, &SkeletalUnit->m_InverseMatrix );
		for( list<int>::const_iterator it = KeyFrameList.begin(); it != KeyFrameList.end(); it++ )
		{
			int curTime = (*it)*GetTicksPerFrame();

			//�õ������ڵ��ת�����󣬲������ת��Ԫ����ƽ��ʸ��
			TransFrameInfo SingleTransFrame;
			QuatFrameInfo  SingleQuatFrame;
			WORD FrameNO = ( (*it) - *( KeyFrameList.begin() ) );
			D3DXMATRIX CurNodeMat = GetNodeGlobeMatrix( MaxNode, curTime, FALSE )*
				ExpNode->m_Parent->m_InvMatrix[ min( FrameNO, ExpNode->m_Parent->m_InvMatrix.size() - 1 ) ];
			D3DXQuaternionRotationMatrix( &SingleQuatFrame.m_FrameQuat, &CurNodeMat );
			SingleTransFrame.m_FrameTrans = D3DXVECTOR3( CurNodeMat.m[3][0], CurNodeMat.m[3][1], CurNodeMat.m[3][2] );
			SingleTransFrame.m_FrameTinyTrans = CTinyVector(SingleTransFrame.m_FrameTrans,1.0f);
			if( g_SaveParam.m_bSP_RootScale && m_SkeletalID == 1 && it == KeyFrameList.begin() )
			{
				D3DXMATRIX ScaleMat = GetNodeLocalMatrix( MaxNode, curTime, TRUE );
				D3DXVECTOR3 Scale( 1, 1, 1 );
				D3DXVec3TransformNormal( &Scale, &Scale, &ScaleMat );
				Animate.SetScale( D3DXVec3Length( &Scale )/1.732f );
				//printf( "Scale: %f\n", D3DXVec3Length( &Scale )/1.732f );
			}

			SingleQuatFrame.m_FrameTinyQuat = CTinyQuaternion( SingleQuatFrame.m_FrameQuat );
			KeyFrames.m_KeyTransFrames[FrameNO] = SingleTransFrame;
			KeyFrames.m_KeyQuatFrames[FrameNO] = SingleQuatFrame;
			uMaxFrame = max( uMaxFrame, FrameNO );
		}
		Animate.SetMaxFrame( uMaxFrame );

		if( KeyFrames.m_KeyTransFrames.size() )
		{
			map<WORD,TransFrameInfo>::iterator itPre = KeyFrames.m_KeyTransFrames.begin();
			map<WORD,TransFrameInfo>::iterator it = KeyFrames.m_KeyTransFrames.begin();
			pKeyFrames->m_KeyTransFrames[it->first] = it->second;
			Animate.SetMaxLength( D3DXVec3Length( &it->second.m_FrameTrans ) );
			for( it++; it != KeyFrames.m_KeyTransFrames.end(); it++ )
			{
				map<WORD,TransFrameInfo>::iterator itNext = it;
				itNext++;
				if( itNext == KeyFrames.m_KeyTransFrames.end() )
					break;

				TransFrameInfo& Pre = itPre->second;
				TransFrameInfo& Cur = it->second;
				TransFrameInfo& Nxt = itNext->second;
				float weight = float( itNext->first - it->first )/( itNext->first - itPre->first );
				D3DXVECTOR3 FrameTrans;
				FrameTrans = Pre.m_FrameTrans*weight + Nxt.m_FrameTrans*( 1.0f - weight );
				FrameTrans = FrameTrans - Cur.m_FrameTrans;

				if( D3DXVec3Length( (D3DXVECTOR3*)&FrameTrans ) > 0.1f )
				{
					itPre = it; 
					pKeyFrames->m_KeyTransFrames[it->first] = it->second;
					Animate.SetMaxLength( D3DXVec3Length( &it->second.m_FrameTrans ) );
				}
			}
			pKeyFrames->m_KeyTransFrames[it->first] = it->second;
			Animate.SetMaxLength( D3DXVec3Length( &it->second.m_FrameTrans ) );
			KeyFrames.m_KeyTransFrames.clear();
		}

		if( KeyFrames.m_KeyQuatFrames.size() )
		{
			map<WORD,QuatFrameInfo>::iterator itPre = KeyFrames.m_KeyQuatFrames.begin();
			map<WORD,QuatFrameInfo>::iterator it = KeyFrames.m_KeyQuatFrames.begin();
			pKeyFrames->m_KeyQuatFrames[it->first] = it->second;
			for( it++; it != KeyFrames.m_KeyQuatFrames.end(); it++ )
			{
				map<WORD,QuatFrameInfo>::iterator itNext = it;
				itNext++;
				if( itNext == KeyFrames.m_KeyQuatFrames.end() )
					break;

				QuatFrameInfo& Pre = itPre->second;
				QuatFrameInfo& Cur = it->second;
				QuatFrameInfo& Nxt = itNext->second;
				float weight = float( itNext->first - it->first )/( itNext->first - itPre->first );
				D3DXQUATERNION FrameQuat;
				D3DXQuaternionSlerp( &FrameQuat, &Pre.m_FrameQuat, &Nxt.m_FrameQuat, 1.0f - weight );
				if( CTinyQuaternion( FrameQuat )._q != CTinyQuaternion( Cur.m_FrameQuat )._q )
				{
					itPre = it; 
					pKeyFrames->m_KeyQuatFrames[it->first] = it->second;
				}
			}
			pKeyFrames->m_KeyQuatFrames[it->first] = it->second;
			KeyFrames.m_KeyQuatFrames.clear();
		}

		//�����������
		D3DXMATRIX mat;
		for( WORD i = 0; i < uMaxFrame; i++ )
		{
			map< WORD, TransFrameInfo >::const_iterator nextTran = pKeyFrames->m_KeyTransFrames.upper_bound( i );
			map< WORD, QuatFrameInfo >::const_iterator  nextQuat = pKeyFrames->m_KeyQuatFrames.upper_bound( i );
			map< WORD, TransFrameInfo >::const_iterator itTran = nextTran;
			map< WORD, QuatFrameInfo >::const_iterator  itQuat = nextQuat;
			itTran--;
			itQuat--;

			D3DXVECTOR3 FrameTrans;
			D3DXVECTOR3 FrameTrans1;
			D3DXVECTOR3 FrameTrans2;
			D3DXQUATERNION FrameQuat;
			D3DXQUATERNION FrameQuat1;
			D3DXQUATERNION FrameQuat2;
			float weightTran = float( nextTran->first - i )/( nextTran->first - itTran->first );
			float weightQuat = float( nextQuat->first - i )/( nextQuat->first - itQuat->first );

			
			FrameTrans1.x = int( itTran->second.m_FrameTrans.x*50 )/50.0f;
			FrameTrans1.y = int( itTran->second.m_FrameTrans.y*50 )/50.0f;
			FrameTrans1.z = int( itTran->second.m_FrameTrans.z*50 )/50.0f;
			FrameTrans2.x = int( nextTran->second.m_FrameTrans.x*50 )/50.0f;	
			FrameTrans2.y = int( nextTran->second.m_FrameTrans.y*50 )/50.0f;	
			FrameTrans2.z = int( nextTran->second.m_FrameTrans.z*50 )/50.0f;	
			FrameQuat1  = itQuat->second.m_FrameTinyQuat;
			FrameQuat2  = nextQuat->second.m_FrameTinyQuat;	
			FrameTrans  = FrameTrans1*weightTran + FrameTrans2*( 1.0f - weightTran );
			D3DXQuaternionSlerp( &FrameQuat, &FrameQuat1, &FrameQuat2, 1.0f - weightQuat );

			D3DXMatrixRotationQuaternion( &mat, &FrameQuat );
			mat.m[3][0] = FrameTrans.x;
			mat.m[3][1] = FrameTrans.y;
			mat.m[3][2] = FrameTrans.z;

			mat = mat*ExpNode->m_Parent->m_Matrix[ min( i, (int)ExpNode->m_Parent->m_Matrix.size() - 1 ) ];
			ExpNode->m_Matrix.push_back( mat );
			D3DXMatrixInverse( &mat, NULL, &mat );
			ExpNode->m_InvMatrix.push_back( mat );
		}

		map< WORD, TransFrameInfo >::const_iterator itTran = pKeyFrames->m_KeyTransFrames.end();
		map< WORD, QuatFrameInfo >::const_iterator  itQuat = pKeyFrames->m_KeyQuatFrames.end();
		itTran--;
		itQuat--;
		D3DXQUATERNION FrameQuat;
		FrameQuat = itQuat->second.m_FrameTinyQuat;
		D3DXMatrixRotationQuaternion( &mat, &FrameQuat );
		mat.m[3][0] = itTran->second.m_FrameTrans.x;
		mat.m[3][1] = itTran->second.m_FrameTrans.y;
		mat.m[3][2] = itTran->second.m_FrameTrans.z;

		mat = mat*ExpNode->m_Parent->m_Matrix[ min( uMaxFrame, ExpNode->m_Parent->m_Matrix.size() - 1 ) ];
		ExpNode->m_Matrix.push_back( mat );
		D3DXMatrixInverse( &mat, NULL, &mat );
		ExpNode->m_InvMatrix.push_back( mat );
	}
	else
	{
		ExpNode->m_Matrix.resize(1);
		ExpNode->m_InvMatrix.resize(1);
		D3DXMatrixIdentity( &ExpNode->m_Matrix[0] );
		D3DXMatrixIdentity( &ExpNode->m_InvMatrix[0] );
	}

	//printf( "%s:%d, %d\n", ExpNode->m_3DMaxNode->GetName(), m_SkeletalID, pKeyFrames->m_KeyFrames.size() );

	//���������ӽڵ�
	for( vector<ExportNode*>::iterator it = ExpNode->m_Children.begin(); it != ExpNode->m_Children.end(); it++ )
	{
		//string tmp = (*it)->m_3DMaxNode->GetName();
		//size_t found;
		//found=tmp.find("Nub");
		//if( (*it)->m_PieceType == enumPT_SKELETAL && !(found!=string::npos))
		if( (*it)->m_PieceType == enumPT_SKELETAL)
		{
			CSkeletal* SkeletalChild = new CSkeletal;
			SkeletalChild->FillFromExportNode( *it, Animate, SkeletalList );
			m_Children.push_back( SkeletalChild );
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ����ݹ������ַ��ع���ID
// ������const string Name  ��������
// ���أ�>0����ID,����ʧ��
////////////////////////////////////////////////////////////////////////// 
int CSkeletal::GetSkeletalIDByName( const string& Name )
{
	if( Name == m_SkeletalName )
		return m_SkeletalID;
	int ID = -1;
	for( int i = 0; i < (int)m_Children.size(); i++ )
		if( SUCCEEDED( ID = m_Children[i]->GetSkeletalIDByName( Name ) ) )
			return ID;
	return ID;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ����������
// ��������
// ���أ���
////////////////////////////////////////////////////////////////////////// 
CSkeletal::CSkeletal()
{
	m_SkeletalID      = 0;
	m_SkeletalName    = "";			//��ͷ����
	m_Children.clear();
}

CSkeletal::~CSkeletal()
{
	Free();
}

void CSkeletal::Free()
{
	for( int i=0; i<(int)m_Children.size(); i++ )
		delete m_Children[i];
	m_Children.clear();
}
//�õ���������
string	CSkeletal::GetSkeletalNameByID( BYTE SkeletalID ) const
{
	string Name = "";
	if( m_SkeletalID == SkeletalID )
		return m_SkeletalName;
	for( int i = 0; i< (int)m_Children.size(); i++ )
	{
		Name = m_Children[i]->GetSkeletalNameByID( SkeletalID );
		if( !Name.empty() )
			break;
	}
	return Name;
}

//������ƥ��
BOOL CSkeletal::IsFitForMe( const CSkeletal* Skeletal, string& ErrSkeletal )
{
	if( m_Children.size() == Skeletal->m_Children.size() )
	{
		for( int i = 0; i< (int)m_Children.size(); i++ )
		{
			if( !m_Children[i]->IsFitForMe( Skeletal->m_Children[i], ErrSkeletal ) )
				return FALSE;
		}
		return TRUE;
	}
	string Src = "ԭ������",Dest = "�¼ӹ���";
	Src += m_SkeletalName + "�ӹ���:";
	Dest += Skeletal->m_SkeletalName + "�ӹ���:";
	for( int i = 0; i < (int)m_Children.size(); i++ )
	{
		Src += m_Children[i]->m_SkeletalName;
		Src += ",";
	}
	for( int i = 0; i < (int)Skeletal->m_Children.size(); i++ )
	{
		Dest += Skeletal->m_Children[i]->m_SkeletalName;
		Dest += ",";
	}
	ErrSkeletal = Src + "<==>" + Dest;
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
// ���ܣ�����ṹ���浽�ļ�
// ��������
// ���أ���
////////////////////////////////////////////////////////////////////////// 
int CModel::SaveFrom3DMax(string FileName)
{
	ofstream File( FileName.data(), ios::binary|ios::out );

	size_t nPos = FileName.find("model");
	g_ARPPath = FileName.substr(0, nPos);
	g_ARPPath += "path\\";

	size_t nPos0 = FileName.rfind("\\");
	size_t nPos1 = FileName.find('.');

	g_ARPPath += FileName.substr(nPos0 + 1, nPos1 - nPos0 - 1);

	g_ARPPath += ".path";

	vector<IDNAME>& IdNameList = m_AnimateGroup.GetIdNameList();
	m_Motions.Free();
	File.close();
	return 0;
}
int CModel::SaveFromFile(string FileName)
{
	ofstream File( FileName.data(), ios::binary|ios::out );

	size_t nPos = FileName.find("model");
	g_ARPPath = FileName.substr(0, nPos);
	g_ARPPath += "path\\";

	size_t nPos0 = FileName.rfind("\\");
	size_t nPos1 = FileName.find('.');

	g_ARPPath += FileName.substr(nPos0 + 1, nPos1 - nPos0 - 1);

	g_ARPPath += ".path";

	m_AnimateGroup.Free();
	File.close();
	return 0;
}
int CModel::Save(string FileName)
{
	ofstream File( FileName.data(), ios::binary|ios::out );

	size_t nPos = FileName.find("model");
	g_ARPPath = FileName.substr(0, nPos);
	g_ARPPath += "path\\";

	size_t nPos0 = FileName.rfind("\\");
	size_t nPos1 = FileName.find('.');

	g_ARPPath += FileName.substr(nPos0 + 1, nPos1 - nPos0 - 1);

	g_ARPPath += ".path";

	
	if( g_SaveParam.m_eSP_SaveType == eST_Piece )
	{
	}
	else if( g_SaveParam.m_eSP_SaveType == eST_Skeletal )
	{
	}
	File.close();

	return 0;
}

struct VertextFace
{
	VertextFace():nNum(0),nNewIndex(-1){}
	int nNewIndex;
	int nNum;
	int nFace[4];
};

struct ClothFace
{   
    ClothFace( int v1, int v2, int v3 )
    {
        VerIndex0 = v1;
        VerIndex1 = v2;
        VerIndex2 = v3;
    }

    int InFace( int v)
    {
        if (VerIndex0 == v) return 0;
        if (VerIndex1 == v) return 1;
        if (VerIndex2 == v) return 2;
        return -1;
    }

    void GetOtherTwo( int v0, int& v1, int& v2)
    {
        switch(v0)
        {
            case 0:
            {
                v1 = VerIndex1;
                v2 = VerIndex2;
            }
            break;
            case 1:
                {
                    v1 = VerIndex0;
                    v2 = VerIndex2;
                }
            break;
            case 2:
                {
                    v1 = VerIndex0;
                    v2 = VerIndex1;
                }
            break;
        }
    }

    int VerIndex0;
    int VerIndex1;
    int VerIndex2;
};

struct SaveInfo
{
	SaveInfo( D3DXVECTOR3 Scale, D3DXVECTOR3 Off, D3DXVECTOR2 MinUV, D3DXVECTOR2 MaxUV )
	{
		int xOff = (sqr::int32)( Off.x*4 + 0.5f );
		int yOff = (sqr::int32)( Off.y*4 + 0.5f );
		int zOff = (sqr::int32)( Off.z*4 + 0.5f );
		int xScl = (sqr::int32)( Scale.x*4 + 0.5f );
		int yScl = (sqr::int32)( Scale.y*4 + 0.5f );
		int zScl = (sqr::int32)( Scale.z*4 + 0.5f );

		sPos = 0;
		sPos = max( sPos, abs( xScl ) );
		sPos = max( sPos, abs( yScl ) );
		sPos = max( sPos, abs( zScl ) );

		xStr = xOff - xScl/2;
		yStr = yOff - yScl/2;
		zStr = zOff - zScl/2;

		uStr = MinUV.x >= 0 ? (int)abs( MinUV.x ) : -(int)abs( MinUV.x );
		if( uStr > MinUV.x )
			uStr--;
		vStr = MinUV.y >= 0 ? (int)abs( MinUV.y ) : -(int)abs( MinUV.y );
		if( vStr > MinUV.y )
			vStr--;

		sTex = 0;
		sTex = max( sTex, abs( MaxUV.x - uStr ) );
		sTex = max( sTex, abs( MaxUV.y - vStr ) );
	}

	int xStr;
	int yStr;
	int zStr;
	int sPos;

	int uStr;
	int vStr;
	float sTex;
};

BOOL CAnimateGroup::IsFitForMe( const CAnimateGroup& AG, string& ErrSkeletal )
{
	return m_SkeletalRoot.IsFitForMe( &AG.m_SkeletalRoot, ErrSkeletal );
}

void CAnimateGroup::CalculateMatrix( const D3DXMATRIX& matWorld, CRenderMatrix& RenderMatrix, bool& CompleteFlag ) const
{
	if( !m_AnimationList.size() )
		return;
	m_SkeletalRoot.CalculateMatrix( matWorld, RenderMatrix, m_AnimationList, m_SkeletalList, CompleteFlag );
}


