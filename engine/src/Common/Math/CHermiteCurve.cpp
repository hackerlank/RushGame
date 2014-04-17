#include "stdafx.h"
#include "CHermiteCurve.h"
#include <cassert>
#include "TSqrAllocator.inl"

namespace sqr
{
	const float CHermiteCurve::MIN_TIME_DELTA = 10.0f;

	CHermiteCurve::CHermiteCurve()
	{
		Clear();
	}

	CHermiteCurve::~CHermiteCurve()
	{
		Clear();
	}

	void CHermiteCurve::_AddNewKeyAtTail( const CVector3f &key, const float fNewKeyTime )
	{
		// �ڶ���β�������µ�֡
		KeyStruct sKeyTemp;
		sKeyTemp.data	= key;
		sKeyTemp.xTime	= fNewKeyTime;

		m_vecKeyFrame.push_back( sKeyTemp );
		++m_nFramesCount;
	}

	void CHermiteCurve::Delete( uint32 uKeyIndex )
	{
		assert( uKeyIndex >=0 && uKeyIndex < m_nFramesCount );

		m_vecKeyFrame.erase( m_vecKeyFrame.begin() + uKeyIndex );
		--m_nFramesCount;
	}



	void CHermiteCurve::Clear()
	{
		m_vecKeyFrame.clear();
		m_nFramesCount	= 0;
		m_nCurrentKey	= -1;
	}

	// ----------------------------------------------------------------------------------------
	// ���ܣ�����ؼ�֡
	//
	// ���أ�����֡��λ�ã�0��ʼ��
	// ----------------------------------------------------------------------------------------
	int CHermiteCurve::Add( CVector3f key, const float fNewKeyTime )
	{
		assert( fNewKeyTime >= 0.0f );

		if ( m_vecKeyFrame.empty() )
		{
			_AddNewKeyAtTail( key, fNewKeyTime );
			return 0;
		}

		float fTimeDelta = fNewKeyTime - m_vecKeyFrame.back().xTime; 
		if ( fTimeDelta >= 0.0f )
		{
			// �ڶ���β������֡, �������֡��ԭ���һ֡�Ĳ��Ƿ��㹻��
			if ( fTimeDelta < MIN_TIME_DELTA )
			{
				return INVALID_KEY_POS;
			}
			else
			{
				_AddNewKeyAtTail( key, fNewKeyTime );
				return m_nFramesCount-1;
			}
		}
		else
		{
			// �ڶ����м�����µ�֡
			return _InsertKey( key, fNewKeyTime );
		}
	}


	int CHermiteCurve::_InsertKey( const CVector3f &key, const float fAtTime )
	{
		int	nInsertPos = _GetFirst( fAtTime );
		KeyVector::iterator itInsertPos = m_vecKeyFrame.begin();

		// ������λ���Ƿ��ڵ�һ֡ǰ�棬�����ǣ���itInserPos��Ϊǰһ֡
		if ( nInsertPos != -1 )
		{
			itInsertPos += nInsertPos;

			// ����ı䣬���ڷ����²���֡��λ��
			++ nInsertPos;
		}
		else
		{
			// ����ı䣬���ڷ����²���֡��λ��
			nInsertPos = 0; 
		}

		// ����²���֡��ʱ����ǰһ֡��ʱ����Ƿ��С
		if ( fAtTime - itInsertPos->xTime < MIN_TIME_DELTA )
		{
			return INVALID_KEY_POS;
		}

		// ����²���֡��ʱ�����һ֡��ʱ����Ƿ��С
		++itInsertPos;
		if ( itInsertPos->xTime - fAtTime < MIN_TIME_DELTA )
		{
			return INVALID_KEY_POS;
		}

		KeyStruct sKeyTemp;
		sKeyTemp.data	= key;
		sKeyTemp.xTime	= fAtTime;
		m_vecKeyFrame.insert( itInsertPos, sKeyTemp );
		++m_nFramesCount;

		return nInsertPos;
	}


	inline void CHermiteCurve::_BuildBasis( const float h )
	{
		const float h2 = h*h;
		const float h3 = h2*h;
		m_fBasis[ 0 ] =  2*h3 - 3*h2 + 1.0f;
		m_fBasis[ 1 ] = -2*h3 + 3*h2;
		m_fBasis[ 2 ] =  h3   - 2*h2 + h;
		m_fBasis[ 3 ] =  h3   - h2;
	}


	int CHermiteCurve::_GetFirst( const float time )
	{
		if ( m_vecKeyFrame[0].xTime > time ) 
			return -1;
		if ( m_vecKeyFrame[m_nFramesCount-1].xTime <= time ) return m_nFramesCount;

		int sIndex = 0;
		int eIndex = m_nFramesCount-1;
		while ( (eIndex - sIndex) > 1 )
		{
			const int mIndex = (sIndex + eIndex) / 2;
			if ( m_vecKeyFrame[ mIndex ].xTime <= time )
			{
				sIndex = mIndex;
			}
			else
			{
				eIndex = mIndex;
			}
		}
		return sIndex;
	}


	int CHermiteCurve::_GetFirstLoop( const float time )
	{
		if ( m_vecKeyFrame[m_nFramesCount-1].xTime <= time ) return ( m_nFramesCount-1 );

		int sIndex = 0;
		int eIndex = m_nFramesCount-1;
		while ( (eIndex - sIndex) > 1 )
		{
			const int mIndex = (sIndex + eIndex) / 2;
			if ( m_vecKeyFrame[ mIndex ].xTime <= time )
			{
				sIndex = mIndex;
			}
			else
			{
				eIndex = mIndex;
			}
		}
		return sIndex;
	}

#ifdef _WIN32
#pragma warning( disable : 4700 )
#endif
	CVector3f CHermiteCurve::Get( float time )
	{
		if ( m_nFramesCount < 2 )
		{
			if ( m_nFramesCount == 1 )
			{
				m_nCurrentKey = 0;
				return m_vecKeyFrame[0].data;
			}
			else
			{
				m_nCurrentKey = -1;
				CVector3f noVal;
				return noVal;
			}
		};

		int first,next;
		first	= _GetFirst( time );
		next	= first + 1;

		if ( first < 0 ) 
		{
			m_nCurrentKey = 0;
			return m_vecKeyFrame[ 0 ].data;
		}

		if ( first == m_nFramesCount ) 
		{
			m_nCurrentKey = m_nFramesCount-1;
			return m_vecKeyFrame[ m_nFramesCount-1 ].data;
		}

		if ( m_vecKeyFrame[ first ].data == m_vecKeyFrame[next].data )
		{
			m_nCurrentKey = first;
			return m_vecKeyFrame[ first ].data;
		}

		const float timeDelta = m_vecKeyFrame[ next ].xTime - m_vecKeyFrame[ first ].xTime;
		const float inSegmentTime = time - m_vecKeyFrame[ first ].xTime;
		_BuildBasis( inSegmentTime / timeDelta );

		CVector3f returnValue;
		returnValue =  m_vecKeyFrame[ first ].data * m_fBasis[ 0 ];
		returnValue += m_vecKeyFrame[ next ].data * m_fBasis[ 1 ];
		returnValue += m_vecKeyFrame[ first ].tangentOut * m_fBasis[ 2 ];
		returnValue += m_vecKeyFrame[ next ].tangentIn * m_fBasis[ 3 ];

		m_nCurrentKey = first;
		return returnValue;
	}

#ifdef _WIN32
#pragma warning( default : 4311 )
#endif


	void CHermiteCurve::BuildCardinal( const float bias )
	{
		if ( m_nFramesCount < 2 ) return;
		for ( uint32 i=0; i<m_nFramesCount; ++i )
		{
			CVector3f ray;	
			if ( i<m_nFramesCount-1 )
			{

				ray = m_vecKeyFrame[ i+1 ].data;
			}
			else
			{
				ray = m_vecKeyFrame[ i ].data;
			}
			if ( i > 0 )
			{
				ray -= m_vecKeyFrame[i-1].data;
			}
			else
			{
				ray -= m_vecKeyFrame[ i ].data;
			}

			ray *= bias;
			m_vecKeyFrame[i].tangentIn = ray;
			m_vecKeyFrame[i].tangentIn = ray;
		}
	}

	uint32 CHermiteCurve::GetNumberOfKeys() const
	{
		return m_nFramesCount;
	}


	CVector3f CHermiteCurve::GetKeyValue( const int keyNumber ) const
	{
		assert( keyNumber>=0 && keyNumber < (int)m_nFramesCount );
		return m_vecKeyFrame[ keyNumber].data;
	}


	float CHermiteCurve::GetKeyTime( const int keyNumber ) const
	{
		assert( keyNumber>=0 && keyNumber < (int)m_nFramesCount );
		return m_vecKeyFrame[ keyNumber ].xTime;
	}


	void CHermiteCurve::SetKeyValue( const int keyNumber, const CVector3f &data )
	{
		assert( keyNumber>=0 && keyNumber < (int)m_nFramesCount );
		this->m_vecKeyFrame[ keyNumber ].data = data;
	}

	void CHermiteCurve::SetKeyTime( const int keyNumber, const float time )
	{
		assert( keyNumber>=0 && keyNumber < (int)m_nFramesCount );
		this->m_vecKeyFrame[ keyNumber ].xTime = time;
	}
}
