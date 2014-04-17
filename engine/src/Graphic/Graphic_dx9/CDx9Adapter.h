#pragma once
#include "Dx9Base.h"
#include "DisplayMode.h"
#include "TSingleton.h"

//------------------------------------------------------------------------------
namespace sqr
{

/// ������.
class Dx9Adapter : public CGraphicMallocObject
{
public:
	/// Ĭ�Ϲ��캯��
	Dx9Adapter();
	/// ���캯��
	Dx9Adapter(uint no, const D3DADAPTER_IDENTIFIER9& identifier, const D3DDISPLAYMODE& mode);
	/// ö�ٿ��õ���ʾģʽ
	void EnumerateDisplayModes(IDirect3D9* d3d);
	/// �����豸�����ַ���
	string GetDescription() const;
	/// �������������
	index_t GetAdapterNo() const;
	/// ����������ʾģʽ
	const D3DDISPLAYMODE& GetDesktopMode() const;
	/// ���ؿ��õ���ʾģʽ����
	size_t GetDisplayModeNum() const;
	/// ���ؿ�����ʾģʽ
	const DisplayMode& GetDisplayMode(index_t index);
	/// ������ʾģʽ�б�
	const DisplayModeArray& GetDisplayModeArray() const;

	/// ������ȡ����ӽ�����ʾģʽ
	bool GetBestDisplayMode(DisplayMode& mode) const;

private:
	uint adapterNo;				///< ���������
	D3DADAPTER_IDENTIFIER9 d3dAdpaterIdentifier;
	D3DDISPLAYMODE	desktopMode;	///< ������ʾģʽ

	DisplayModeArray modelist;		///< ���õ���ʾģʽ
};

typedef GVector<Dx9Adapter> AdapterArray;

////////////////////////////////////////////////////////////////////////////////
/// �����������б�.
class AdapterList : public Singleton<AdapterList>
{
public:
	/// Ĭ�Ϲ��캯��
	AdapterList();
	/// ö��������
	void EnumerateApdaters(IDirect3D9* d3d);
	/// ��������������
	size_t GetAdapterNum() const;
	/// ���ط���ָ��������
	const Dx9Adapter& GetAdapter(index_t index);
	/// ���ص�ǰ������
	const Dx9Adapter& GetCurrentAdapter() const;
	/// ���ص�ǰ������������
	const index_t GetCurrentAdapterIndex() const;
	/// ���õ�ǰ������
	void SetCurrentAdapterIndex(index_t index);

private:
	AdapterArray adapters;	///< ����������
	uint curAdapter;		///< ��ǰ������
};


//------------------------------------------------------------------------------
inline string Dx9Adapter::GetDescription() const
{
	return string(this->d3dAdpaterIdentifier.Description);
}

//------------------------------------------------------------------------------
inline index_t Dx9Adapter::GetAdapterNo() const
{
	return this->adapterNo;
}

//------------------------------------------------------------------------------
inline const D3DDISPLAYMODE& Dx9Adapter::GetDesktopMode() const
{
	return this->desktopMode;
}

//------------------------------------------------------------------------------
inline size_t Dx9Adapter::GetDisplayModeNum() const
{
	return this->modelist.size();
}

//------------------------------------------------------------------------------
inline const DisplayMode& Dx9Adapter::GetDisplayMode( index_t index )
{
	Ast(index < this->GetDisplayModeNum());
	return this->modelist[index];
}

//------------------------------------------------------------------------------
inline const DisplayModeArray&
Dx9Adapter::GetDisplayModeArray() const
{
	return this->modelist;
}

//------------------------------------------------------------------------------
inline size_t AdapterList::GetAdapterNum() const
{
	return this->adapters.size();
}

//------------------------------------------------------------------------------
inline const Dx9Adapter& AdapterList::GetAdapter( index_t index )
{
	Ast(index < this->GetAdapterNum());
	return this->adapters[index];
}

//------------------------------------------------------------------------------
inline const index_t AdapterList::GetCurrentAdapterIndex() const
{
	return this->curAdapter;
}

//------------------------------------------------------------------------------
inline void AdapterList::SetCurrentAdapterIndex( index_t index )
{
	this->curAdapter = index;
}

//------------------------------------------------------------------------------
inline const Dx9Adapter& AdapterList::GetCurrentAdapter() const
{
	Ast(this->curAdapter < this->GetAdapterNum());
	return this->adapters[this->curAdapter];
}

}// namespace sqr