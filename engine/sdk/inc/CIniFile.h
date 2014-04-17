/*
*	�����ϵͳ�޹صĶ�дIni�ļ���ʵ��
*/

#ifndef _VFS_INIFILE_H_
#define _VFS_INIFILE_H_

#include "CommonDefs.h"
#include <sstream>

namespace sqr
{
//---------------------------------------------------------------------------
class COMMON_API CIniFile
{
private:
	typedef map<string, string> KeyMap;
	typedef map<string, KeyMap> SectionMap;
	wstring fileName;
	SectionMap valueMap;
	bool    Prepare(char* lpBuffer);

public:
	CIniFile();
	~CIniFile();

	// ����һ���µ� .Ini�ļ���Ϊд(����)��׼��
	bool	Create(const wchar_t* szFileName);

	// ���ļ���ɨ��һ�飬Ϊ�Ժ�Ķ���������׼��(Get...)
	bool	Open(const wchar_t* alias, const wchar_t* szFileName);

	// �������е����ݱ��浽�����ļ�
	bool	Save(void);
	bool	Save(const wstring& FileName);

	// �ر��ļ�,�ͷ��ڴ�
	void	Close();

	// ��Ϊ����ʵ��, ����ʹ�� [2/26/2009 xoyojank]
	template <typename TYPE> TYPE GetValue( const string& section, const string& key,
	                                        const TYPE& defaultValue) const;
	template <typename TYPE> void SetValue( const string& section, const string& key,
	                                        const TYPE& value);
};

//------------------------------------------------------------------------------
template <typename TYPE>
void
CIniFile::SetValue( const string& section, const string& key, const TYPE& value )
{
	stringstream ss;
	ss << value;
	this->valueMap[section][key] = ss.str();
}
//------------------------------------------------------------------------------
template <typename TYPE>
TYPE
CIniFile::GetValue( const string& section, const string& key, const TYPE& defaultValue ) const
{
	SectionMap::const_iterator itSection = this->valueMap.find(section);
	if (this->valueMap.end() != itSection)
	{
		KeyMap::const_iterator itKey = itSection->second.find(key);
		if (itSection->second.end() != itKey)
		{
			stringstream ss;
			ss << itKey->second;
			TYPE returnValue;
			ss >> returnValue;
			return returnValue;
		}
	}
	// not exist
	return defaultValue;
}

}

#endif

