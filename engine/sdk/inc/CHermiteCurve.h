#pragma	once
#include "BaseMath.h"
#include "CVector3.h"
#include "CMathMallocObject.h"

namespace sqr
{
	class CHermiteCurve : public CMathMallocObject
	{
	public:
		static const float	MIN_TIME_DELTA;
		static const int	INVALID_KEY_POS = -1;

	private:
		struct KeyStruct
		{
			CVector3f data;
			CVector3f tangentIn;
			CVector3f tangentOut;
			float xTime;
			KeyStruct()
				: xTime(0.0f)
			{
			}
		};

		typedef MathVector<KeyStruct> KeyVector; // ������vector����Ϊ��Ϸ��ֻ�м���������
											 // �����в��롢ɾ������, ���Բ�Ҫ��Ϊlist

		KeyVector			m_vecKeyFrame;
		uint32				m_nFramesCount;
		int32				m_nCurrentKey;
		float				m_fBasis[ 4 ];

		inline void _BuildBasis( const float h );
		int			_GetFirst( const float time );
		int			_GetFirstLoop( const float time );
		void		_AddNewKeyAtTail( const CVector3f &key, const float fNewKeyTime );
		int			_InsertKey( const CVector3f &key, const float fAtTime );

	public:
		CHermiteCurve();
		~CHermiteCurve();

		void		Clear();
		int			Add( CVector3f key, const float fNewKeyTime );
		
		void		Delete( uint32 uKeyIndex );
		CVector3f	Get( float time );

		void		BuildCardinal( const float bias = 0.5f );

		uint32		GetNumberOfKeys() const;
		CVector3f	GetKeyValue( const int keyNumber ) const;
		float		GetKeyTime( const int keyNumber ) const;
		void		SetKeyValue( const int keyNumber, const CVector3f &data );
		void		SetKeyTime( const int keyNumber, const float time );
		
		int32		GetCurKeyIndex() const { return m_nCurrentKey; }
	};
}

