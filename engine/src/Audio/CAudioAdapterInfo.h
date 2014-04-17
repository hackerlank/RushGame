#pragma once
#include "CAudioMallocObject.h"

namespace sqr
{

/// ��Ƶ������(����)��Ϣ.
class CAudioAdapterInfo : public CAudioMallocObject
{
public:
	/// ���캯��
	CAudioAdapterInfo();

	/// ������ID
	void SetAdpaterId(const AString& id);
	const AString& GetAdapterId() const;

	/// ����
	void SetDisplayName(const AString& name);
	const AString& GetDisplayName() const;

	/// �Ƿ�ΪĬ��
	void SetDefaultAdapter(bool b);
	bool IsDefaultAdapter() const;

private:
	AString adapterId;		///< �����豸��ʶ
	AString displayName;		///< ��ʾ������, ��"Realtek HD Audio output"
	bool isDefaultAdapter;	///< �Ƿ�ΪĬ�ϵ�������, ������ʱ������
};


//------------------------------------------------------------------------------
inline CAudioAdapterInfo::CAudioAdapterInfo()
: isDefaultAdapter(false)
{
	// empty
}

//------------------------------------------------------------------------------
inline void CAudioAdapterInfo::SetAdpaterId( const AString& id )
{
	this->adapterId = id;
}

//------------------------------------------------------------------------------
inline const AString& CAudioAdapterInfo::GetAdapterId() const
{
	return this->adapterId;
}

//------------------------------------------------------------------------------
inline void CAudioAdapterInfo::SetDisplayName( const AString& name )
{
	this->displayName = name;
}

//------------------------------------------------------------------------------
inline const AString& CAudioAdapterInfo::GetDisplayName() const
{
	return this->displayName;
}

//------------------------------------------------------------------------------
inline void CAudioAdapterInfo::SetDefaultAdapter( bool b )
{
	this->isDefaultAdapter = b;
}

//------------------------------------------------------------------------------
inline bool CAudioAdapterInfo::IsDefaultAdapter() const
{
	return this->isDefaultAdapter;
}

}// namespace sqr
