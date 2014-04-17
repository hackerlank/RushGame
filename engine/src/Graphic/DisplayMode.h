#pragma once
#include "GraphicRenderStyle.h"
#include "CGraphicMallocObject.h"
/// ��ʾģʽ.
class DisplayMode : public CGraphicMallocObject
{
public:
	/// ���캯��
	DisplayMode();
	/// ���ù��캯��
	DisplayMode(uint w, uint h, uint format);
	/// ȡ��ˮƽ�ֱ���
	uint GetWidth() const;
	/// ����ˮƽ�ֱ���
	void SetWidth(uint w);
	/// ȡ�ô�ֱ�ֱ���
	uint GetHeight() const;
	/// ���ô�ֱ�ֱ���
	void SetHeight(uint h);
	/// ȡ�����ظ�ʽ
	uint GetPixelFormat() const;
	/// �������ظ�ʽ
	void SetPixelFormat(uint format);
	/// ȡ����ɫ���
	uint GetColorDepth() const;
	/// ȡ�ÿ�߱�
	float GetAspectRatio() const;

	/// ���� operator
	bool operator==(const DisplayMode& rhs) const;
	/// ������ operator
	bool operator!=(const DisplayMode& rhs) const;

private:
	uint width;			///< ˮƽ�ֱ���
	uint height;		///< ��ֱ�ֱ���
	uint pixelFormat;	///< ���ظ�ʽ
};

typedef GVector<DisplayMode> DisplayModeArray;


//------------------------------------------------------------------------------
inline
DisplayMode::DisplayMode()
		: width(0)
		, height(0)
		, pixelFormat(TFMT_UNKNOWN)
{
	// empty
}

//------------------------------------------------------------------------------
inline
DisplayMode::DisplayMode( uint w, uint h, uint format )
		: width(w)
		, height(h)
		, pixelFormat(format)
{
	// empty
}

//------------------------------------------------------------------------------
inline uint
DisplayMode::GetWidth() const
{
	return this->width;
}

//------------------------------------------------------------------------------
inline void
DisplayMode::SetWidth(uint w)
{
	this->width = w;
}

//------------------------------------------------------------------------------
inline uint
DisplayMode::GetHeight() const
{
	return this->height;
}

//------------------------------------------------------------------------------
inline void
DisplayMode::SetHeight(uint h)
{
	this->height = h;
}

//------------------------------------------------------------------------------
inline uint
DisplayMode::GetPixelFormat() const
{
	return this->pixelFormat;
}

//------------------------------------------------------------------------------
inline void
DisplayMode::SetPixelFormat(uint format)
{
	this->pixelFormat = format;
}

//------------------------------------------------------------------------------
inline uint
DisplayMode::GetColorDepth() const
{
	switch (this->pixelFormat)
	{
		case TFMT_X8R8G8B8:
		case TFMT_A8R8G8B8:
		case TFMT_R8G8B8:
			return 32;
		default:
			return 16;
	}
}

//------------------------------------------------------------------------------
inline float
DisplayMode::GetAspectRatio() const
{
	return (float(this->width) / float(this->height));
}

//------------------------------------------------------------------------------
inline bool
DisplayMode::operator==(const DisplayMode& rhs) const
{
	return ((this->width == rhs.width) &&
	        (this->height == rhs.height) &&
	        (this->pixelFormat == rhs.pixelFormat));
}

//------------------------------------------------------------------------------
inline bool
DisplayMode::operator!=(const DisplayMode& rhs) const
{
	return !(*this == rhs);
}
