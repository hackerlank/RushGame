#pragma once
#include "CVector3.h"
#include "CColor.h"
#include "TRefPointer.h"
#include "CSceneCommon.h"
#include "CSceneMallocObject.h"

namespace sqr
{

	class ITexture;
	
	struct CRegionReadData : public CSceneMallocObject
	{
	};

	struct CClientReadData : public CRegionReadData
	{
		CClientReadData()
			: ampRgnInfo(0)
			, argPos(0)
		{

		}

		size_t	ampRgnInfo;
		size_t	argPos;
		//void*	pScene;
	};

	enum ESceneFileType
	{
		ESFT_ARS	= 0,
		ESFT_ARG	= 1,
		ESFT_AMP	= 2,
		ESFT_COUNT,
	};


	struct CREATE_ITEM : public CSceneMallocObject
	{
		uint32				dwResType;		//��Դ����:ARA,ARP,ARS,ARE,ARAS		(����)
		SString				szFileName;		//��Դ�ļ���						(����)
		SString				szClassName;	//��Դ����							(����)
		SString				szParam;		//���Ͳ���
	};

	struct OBJECT_CREATE_PARAM : public CSceneMallocObject
	{
		SVector<CREATE_ITEM> Items;
	};

	class CTerrainSetUnit : public CSceneMallocObject
	{
	public:
		SVector<OBJECT_CREATE_PARAM> m_UnitPart;
	};

	struct	SubRegionData : public CSceneMallocObject
	{
		uint16				m_nSetRegionIndex;		///�����ĸ�������������
		CVector3f			m_vCenter;				///����
		SVector<int>			m_vSubRgnAjenctList;	///��������
		bool				m_bNeedRealCal;			///�Ƿ���Ҫʵʱ����
	};

	class URegionID : public CSceneMallocObject
	{
	public:
		union 
		{	
			struct
			{
				int16	X,Z; 
			};
			uint32	ID;
		};

	public:
		URegionID():ID(0){}
		URegionID(uint32 id):ID(id){}
		URegionID(int16 x, int16 y):X(x),Z(y){}
		
		inline bool operator > (URegionID id) const { return ID>id.ID; };
		inline bool operator < (URegionID id) const { return ID<id.ID; };
		//void operator / (uint32);
		//operator uint32();
	};

	//���ݸ��������ƫ�Ƶõ�������������
	inline float GetFloatPixelOfGrid( int32 LogicCrd, float Offset = 32.0f )
	{
		return static_cast<float>( LogicCrd << 6 ) + Offset;// return LogicCrd*64 + Offset;
	}

	// ========================================================================

	//���ݸ��������Լ����Ӷ���������¶����ƫ�Ƶõ���������
	inline int32 GetRegionVertexIndex( uint8 uRegionGridIndex, int32 nX, int32 nY )
	{
		return ( ( uRegionGridIndex >> 4 ) + nY )*( eRegionSpan + 1 ) + ( uRegionGridIndex&0xf ) + nX;
	}

	//���ݸ��������Լ����Ӷ���������¶����ƫ�Ƶõ���������
	inline uint8 GetRegionGridIndex( int32 nGridX, int32 nGridY )
	{
		return (uint8)( ( ( nGridY&0xf ) << 4 ) + ( nGridX&0xf ) );
	}

	//���ݸ��������Լ����Ӷ���������¶����ƫ�Ƶõ�������
	inline int32 GetRegionByGrid( int32 nGrid )
	{
		return nGrid >> 4;
	}

	//���ݸ�����������õ���������
	inline int32 GetGridByFloatPixel( float fPixelCrd )
	{
		return static_cast<int32>(fPixelCrd) >> 6;// return LogicCrd*64 + Offset;
	}

	//���ݸ�����������õ�����������ƫ��
	inline float GetFloatOfsInGrid( float fPixelCrd )
	{
		float fOffset = fPixelCrd - static_cast<int32>(fPixelCrd);
		return (static_cast<int32>(fPixelCrd) & 0x3f) + fOffset;// return LogicCrd*64 + Offset;
	}

	//������������õ�����������ƫ��
	inline float GetOfsInGrid( float fPixelCrd )
	{
		float fOffset = fPixelCrd - static_cast<int32>(fPixelCrd);
		return (static_cast<int32>(fPixelCrd) & 0x3f) + fOffset;// return LogicCrd*64 + Offset;
	}
}
