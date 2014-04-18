#pragma once
//------------------------------------------------------------------------------
/**
    @class CLightMapWriter

	��CLightMapReceiver���ɵ�surface��������

    (C) 2009 ThreeOGCMan
*/
#include "CRefObject.h"
#include "GraphicBase.h"
#include "CColor.h"

namespace sqr
{

	//------------------------------------------------------------------------------
	SmartPointer(CLightMapWriter);
	class CLightMapWriter : public CRefObject
	{
	public:
		virtual ~CLightMapWriter();
		/// ����lightmap��Ŀ
		void SetNumLightMaps(uint width , uint depth);
		/// ����
		virtual void Write(index_t x, index_t z, const ITexture* surface);

	protected:
		CLightMapWriter();
		uint width;
		uint depth;
		ITexture*		m_srcTexture;
		//ITexture*		m_srcSurface;
	};
	//------------------------------------------------------------------------------
	/// д���ļ� 
	class CLMFileWriter : public CLightMapWriter
	{
	public:
		CLMFileWriter();
		/// ����
		void Write(index_t x, index_t z, const ITexture* surface);
	};
	//------------------------------------------------------------------------------
	/// д���߼���Ӱ����
	class CLogicalShadowWriter : public CLightMapWriter
	{
	public:
		CLogicalShadowWriter();
		/// д������
		void Write(index_t x, index_t z, const ITexture* surface);
		/// ����
		void Save(int offset);

	private:
		byte GetBlockColor(index_t x, index_t y, int pitch, const CColor* data);
	private:
		size_t blockSize;
		vector<byte> buffer;
	};
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	inline void
	CLightMapWriter::SetNumLightMaps( uint w , uint d )
	{
		this->width = w;
		this->depth = d;
	}
}