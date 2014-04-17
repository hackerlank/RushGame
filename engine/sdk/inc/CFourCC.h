#ifndef _CFour_CC_H_
#define _CFour_CC_H_

namespace sqr
{
/// ���ַ���.
/// Four Character Code
class FourCC
{
public:
	/// Ĭ�Ϲ��캯��
	FourCC();
	/// ��32λֵ���� (�� FourCC('ABCD'))
	FourCC(DWORD id);
	/// ���ַ�������
	FourCC(const std::string& s);
	/// ����
	bool operator==(const FourCC& rhs) const;
	/// ������
	bool operator!=(const FourCC& rhs) const;
	/// С��
	bool operator<(const FourCC& rhs) const;
	/// С�ڵ���
	bool operator<=(const FourCC& rhs) const;
	/// ����
	bool operator>(const FourCC& rhs) const;
	/// ���ڵ���
	bool operator>=(const FourCC& rhs) const;
	/// �Ƿ�Ϸ�
	bool IsValid() const;
	/// ��32λֵ��ֵ
	void SetFromDWORD(DWORD id);
	/// ����32λֵ��ʽ
	DWORD AsDWORD() const;
	/// ���ַ�����ֵ
	void SetFromString(const std::string& s);
	/// �����ַ�����ʽ
	std::string AsString() const;
	/// ת��һ��FourCC���ַ���
	static std::string ToString(const FourCC& f);
	/// ת��һ���ַ�����FourCC
	static FourCC FromString(const std::string& s);

private:
	DWORD fourCC;
};

//------------------------------------------------------------------------------
inline
FourCC::FourCC() :
		fourCC(0)
{
	// empty
}

//------------------------------------------------------------------------------
inline
FourCC::FourCC(DWORD id) :
		fourCC(id)
{
	// empty
}

//------------------------------------------------------------------------------
inline
FourCC::FourCC(const std::string& s)
{
	this->SetFromString(s);
}

//------------------------------------------------------------------------------
inline bool
FourCC::operator==(const FourCC& rhs) const
{
	return (this->fourCC == rhs.fourCC);
}

//------------------------------------------------------------------------------
inline bool
FourCC::operator!=(const FourCC& rhs) const
{
	return (this->fourCC != rhs.fourCC);
}

//------------------------------------------------------------------------------
inline bool
FourCC::operator>(const FourCC& rhs) const
{
	return (this->fourCC > rhs.fourCC);
}

//------------------------------------------------------------------------------
inline bool
FourCC::operator>=(const FourCC& rhs) const
{
	return (this->fourCC >= rhs.fourCC);
}

//------------------------------------------------------------------------------
inline bool
FourCC::operator<(const FourCC& rhs) const
{
	return (this->fourCC < rhs.fourCC);
}

//------------------------------------------------------------------------------
inline bool
FourCC::operator<=(const FourCC& rhs) const
{
	return (this->fourCC <= rhs.fourCC);
}

//------------------------------------------------------------------------------
inline bool
FourCC::IsValid() const
{
	return (0 != this->fourCC);
}

//------------------------------------------------------------------------------
inline void
FourCC::SetFromDWORD(DWORD id)
{
	//assert(0 != id);
	this->fourCC = id;
}

//------------------------------------------------------------------------------
inline DWORD
FourCC::AsDWORD() const
{
	return this->fourCC;
}

//------------------------------------------------------------------------------
inline void
FourCC::SetFromString(const std::string& s)
{
	*this = FromString(s);
}

//------------------------------------------------------------------------------
inline std::string
FourCC::AsString() const
{
	return ToString(*this);
}

//------------------------------------------------------------------------------
inline std::string
FourCC::ToString(const FourCC& f)
{
	//assert(0 != f.fourCC);
	std::string str("xzyw");
	str[0] = (char) ((f.fourCC & 0xFF000000) >> 24);
	str[1] = (char) ((f.fourCC & 0x00FF0000) >> 16);
	str[2] = (char) ((f.fourCC & 0x0000FF00) >> 8);
	str[3] = (char) (f.fourCC & 0x000000FF);
	return str;
}

//------------------------------------------------------------------------------
inline FourCC
FourCC::FromString(const std::string& s)
{
	//assert(s.length() == 4);
	return FourCC(uint(s[0] | s[1]<<8 | s[2]<<16 | s[3]<<24));
}
}

#endif //_CFour_CC_H_