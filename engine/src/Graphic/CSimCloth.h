#pragma once
#include "CVector3.h"
#include "CGraphicMallocObject.h"

namespace sqr
{
namespace Cloth
{
///�����ʵ�.
class CSoftVerNode : public CGraphicMallocObject
{
public:
	CVector3f*		m_Vertex;	///< ����(λ��)

	CVector3f		m_Force;	///< ��
	CVector3f		m_Accel;	///< ���ٶ�
	CVector3f		m_Speed;	///< �ٶ�
	CVector3f		m_Wind;		///< ����
	float           m_MASS;			///< ����
	bool            m_IsFix;		///< �Ƿ�Ϊ�̶���
	float           m_GRAVITY;		///< �������ٶ�
	float           m_GASSFORCE;
	float           m_WindPower;	///< ����
	float           m_WindPowerH;

	CSoftVerNode() {};
	CSoftVerNode( CVector3f* pVertex, float mass, float GRAVITY, float GASSFORCE);

	void ResetForce();

	/// ���½��λ��.
	/// @param	float deltatime	�����ϴμ����ʱ��
	int32	Calculate( float deltatime );
	void	SetFix( bool Fix )
	{
		m_IsFix  = Fix;
	}
	void SetWind( CVector3f& W, float windpower, float windpowerH = 0 )
	{
		m_Wind   = W;
		m_WindPower   = windpower;
		m_WindPowerH = windpowerH;
	}
	/// ʩ��һ���µ���
	void	AddForce( CVector3f& F )
	{
		m_Force += F;
	}
	bool	IsFixed()
	{
		return m_IsFix;
	}

	CVector3f& GetVertex()
	{
		return *m_Vertex;
	}
};

/// ����.
/// ���˶���: F(��)=k(��ǿϵ��) * d(�����α�)
class CSpring : public CGraphicMallocObject
{

public:
	CSoftVerNode*   m_Node1;	///< �ʵ�1
	CSoftVerNode*   m_Node2;	///< �ʵ�2
	float   m_StaticLen;		///< ��̬����
	float   m_K;				///< ��ǿϵ��
	float   m_LengthMax;		///< ��С����
	float   m_LengthMin;		///< ��󳤶�

	CSpring( CSoftVerNode* Node1, CSoftVerNode* Node2 ,  float k, float StaticLen, float fLengthMax, float fLengthMin);

	/// ����ʩ�����ʵ��ϵ���
	bool CalculateForce();
};

/// ����.
/// ���á��ʵ�-���ɡ�ģ�ͽ���ģ��
class CCloth : public CGraphicMallocObject
{
public:

	GVector<CSpring>			m_Spring;	///< ���ɼ���
	GVector<CSoftVerNode>	m_Node;		///< �ʵ㼯��

	CVector3f			m_Wind;			///< ����
	int16               m_WindPowerMin;	///< ��С����
	int16               m_WindPowerMax;	///< ������
	int16               m_WindPowerMaxH;
	float				m_GRAVITY;      //< �������ٶ�
	float				m_GASSFORCE;	//< ����������ţ��/����/�룩 ��
	float               m_TimeBase;		///< ��ʼʱ��
	float               m_TimePass;		///< ��ȥʱ��


	DWORD				m_LastUpdateTime;		///< �ϴθ���ʱ��
	DWORD				m_LastUpdateWindTime;	///< �ϴη�������ʱ��

	CCloth();
	~CCloth(void);

	void    Calculate( const CVector3f& vDir );
	void    SetFixNode( uint16 Index );
	void    SetWind( uint16 WindPowerMin, uint16 WindPowerMax, uint16 m_WindPowerMaxH, float TimePass );
	uint16  AddSpring( uint16 Index1, uint16 Index2, float k, float StaticLen, float fLengthMax, float fLengthMin);
	uint16  AddNode( const CSoftVerNode& Node );

	GVector<CSoftVerNode>&   GetNode()
	{
		return m_Node;
	}
	GVector<CSpring>&        GetSpring()
	{
		return m_Spring;
	}
};

};
};
