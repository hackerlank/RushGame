#pragma once
#include "CDynamicObject.h"
using namespace sqr;

typedef class tag_Npc : public virtual CDynamicObject
{
	public:
	uint32		uID;							// ID
	CString		_����;					// ����
	CString 		_����;					// ��������
	CString      _AI����;
	CString      _AI����;
	uint8			_��͵ȼ�;			// ��͵ȼ�
	uint8			_��ߵȼ�;			// ��ߵȼ�
	uint32		_��С����;
	uint32		_��󹥻�;
	uint32		_����;
	uint32		_Ѫ;
	float			_�����ٶ�;
	uint8			_�����˺�����;
	uint32		_�����˺�ֵ;
	uint32		_����;
	uint32		_����;
	uint32		_��;
	uint32		_���;
	uint32		_����;
	uint32		_��;
	uint32		_����;
	uint32		_�翹;
	uint32		_����һ��;
	uint32		_����;
	uint32		_�ƶ�����;
	uint32		_�ƶ���Χ;
	uint32		_����������;
	uint32		_��Ұ��Χ;
	uint32		_��������;
	uint32		_����ģʽ;
	uint32		_���ܱ��;
	uint32		_�ͷż��;
	uint32		_�ͷŸ���;
	uint32		_׷��ʱ��;
	uint32		_׷������;
	uint32		_�Ƿ񷵻�;
	uint32		_���з�Χ;
	uint32		_������Ӫ;
	CString		ModelFilePath;
	CString		FxFilePath;
	CString		HurtFxFilePath;
	CString		HitDownFxFilePath;
	CString		SoundFilePath;
	uint32		RenderStyleID;
	uint32		GoldenRenderStyleID;
	CString		GoldenFxFilePath;
	//	uint8		_����,
	uint8		_Npc�Ƿ�Ψһ;
	uint8		_����NPC����;
	uint8		_ս��NPC����;
	uint8		_NPC����;
	uint8		_NPC��Ӫ;
	uint8		_NPCְҵ;
	uint8		_NPC���;
	uint8		_�ȼ�����;
	uint8		_�ȼ�����;
	uint8		_�ƶ���ʽ;
	//		uint32		_�ƶ���Χ;
	uint32		_�ƶ��ٶ�;
	uint32		_ˢ��ʱ��;
	uint32		_���䷶Χ;
	uint32		_��ȷ�Χ;

}NPC, *PNPC;

typedef struct tag_Obj
{
	CString name;
	uint8 isSingle;
	uint8 isBarrier;
}OBJ, *POBJ;

typedef struct tag_Trap
{
	CString name;
	uint8 isSingle;
}TRAP, *PTRAP;

typedef struct tag_SceneCfg
{
	tag_SceneCfg(CString sMap, CString sScene, CString sArea):mapFile(sMap),sceneFile(sScene),areaFile(sArea){}
	CString mapFile;
	CString sceneFile;
	CString areaFile;
}SceneCfg, *PSceneCfg;


typedef struct tag_AreaData
{
	tag_AreaData(const CString& strName):name(strName),isFb(false),color(-1),ratingPlayerNum(0),maxPlayerNum(0),music("��"){};
	CString name;
	bool	isFb;
	COLORREF color;
	uint32 ratingPlayerNum;	//�����
	uint32 maxPlayerNum;	//������� Ϊ��������Ч
	CString music;
}AreaData, *PAreaData;



template <typename> struct T_Rect;
template <typename T> struct T_Pos;
template <typename T>
struct T_Pos 
{
	T_Pos(){}
	T_Pos(T _x, T _y):x(_x),y(_y){}
	T_Pos(CPoint pt):x(pt.x),y(pt.y){}
	inline bool IsInRect(const T_Rect<T>& rect) const
	{
		return rect.IsContainPos(*this);
	}
	CPoint ToCPoint()const
	{
		return CPoint(x,y);
	}
	inline T_Pos<T> operator +(const T_Pos<T> &  other)const
	{
		return T_Pos(x+ other.x, y + other.y);
	}
	inline T_Pos<T>& operator += (const T_Pos<T> &  other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	inline T_Pos<T> operator -(const T_Pos<T> &  other)const
	{
		return T_Pos(x- other.x, y - other.y);
	}
	inline T_Pos<T>& operator -= (const T_Pos<T> &  other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline T_Pos operator*(T value)const
	{
		return T_Pos(x * value ,y * value);
	}
	//���
	inline T operator*(const T_Pos<T> &  other)const
	{
		return x * other.x + y * other.y;
	}
	//���
	inline T operator^(const T_Pos<T> &  other)const
	{
		return x * other.y - y * other.x;
	}

	T_Pos<T> Rotate(double radian, bool isClockwise)const
	{
		const double cosa = cos(radian);
		const double sina = sin(radian);
		if (isClockwise)
			return T_Pos<T>(static_cast<T>(x * cosa + y * sina), static_cast<T>(y * cosa - x * sina));
		else
			return T_Pos<T>(static_cast<T>(x * cosa - y * sina), static_cast<T>(y * cosa + x * sina));
	}

	inline T Length() const
	{
		return (T)sqrt(float(x*x + y*y));
	}
	T x;
	T y;
};

typedef T_Pos<int>   Pos;
typedef T_Pos<float> FPos;
typedef T_Pos<float> Vector2;

template <typename T>
struct T_Rect 
{
	union
	{
		struct  
		{
			T left, top, right, bottom;
		};
		struct  
		{
			T minX, minY, maxX, maxY;
		};
		struct  
		{
			T_Pos<T> left_top, right_bottom;
		};
	};
	T_Rect(){}
	T_Rect(T _left, T _top, T _right, T _bottom):left(_left),top(_top),right(_right),bottom(_bottom){}
	T_Rect(const T_Pos<T> _left_top, T_Pos<T> _right_bottom)
		:left_top(_left_top),right_bottom(_right_bottom){}
	
	inline bool IsContainPos(const T_Pos<T>& pos)const
	{
		return (pos.x >= left && pos.x <= right && pos.y >= top && pos.y <= bottom);
	}
	CRect ToCRect()
	{
		return CRect(left,top,right,bottom);
	}
	T_Rect<T> Correct()const
	{
		T_Rect<T> rect = *this;
		T temp;
		if (rect.left > rect.right)
		{
			temp = rect.left;
			rect.left = rect.right;
			rect.right = temp;
		}
		if (rect.top > rect.bottom)
		{
			temp = rect.top;
			rect.top = rect.bottom;
			rect.bottom = temp;
		}
		return rect;
	}
	void GetCenter(T_Pos<T>& fPosCenter)
	{
		fPosCenter.x = (left + right)/2;
		fPosCenter.y = (top + bottom)/2;
	}
	T_Pos<T> GetCenter()
	{
		T_Pos<T> center;
		GetCenter(center);
		return center;
	}
};

typedef T_Rect<int>   Rect;
typedef T_Rect<float> FRect;
