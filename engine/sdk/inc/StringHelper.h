#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <cassert>
#include "ExpHelper.h"


namespace sqr
{
	/// @brief An extension of STL's string providing additional functionality that is often availiable in
	///		higher-level languages such as C#, php, Python.
	/// @ref �ο���ext_stringʵ�֣�http://www.gotroot.ca/ext_string/
	/// @author yangmingzhe		mail.ymz@163.com
	extern const std::string WHITE_SPACE;			///< �հ��ַ� ����Ϊ: �ո�, ˮƽ�Ʊ��, �س�, ����, ��ֽ����, ��ֱ�Ʊ��
	extern const int32		 DEFAULT_BUFFER_SIZE;			///< vformatʱĬ�ϵĻ�������С
	
	/// @brief �ַ�����ʽ��
	std::string& format(std::string& resultOUT, const char* fmt, ...);
	/// @brief �ַ�����ʽ��
	std::string format(const char* fmt, ...);

	/**
	\brief
		���ַ�����ʽ��
	*/
	std::wstring format(std::wstring &result, const wchar_t *fmt, ...);

	/**
	\brief
		���ַ�����ʽ��
	*/
	std::wstring& vformat(std::wstring& resultOUT, const wchar_t *fmt, va_list argptr, int32 default_size = DEFAULT_BUFFER_SIZE);
	
	void substr_u8(string& result, const string& str, size_t start, size_t len);	


	/// @brief �ַ�����ʽ��
	/// @param default_size Ĭ�ϵĻ�������С,����û�������С���������2������ֱ���������㹻
	std::string& vformat(std::string& resultOUT, const char *fmt, va_list argptr, int32 default_size = DEFAULT_BUFFER_SIZE);
	
	/// @brief �ַ�����ʽ��
	/// @param default_size Ĭ�ϵĻ�������С,����û�������С���������2������ֱ���������㹻
	inline std::string vformat(const char *fmt, va_list argptr, int32 default_size = DEFAULT_BUFFER_SIZE);

	/// @brief ����C#�İ����±��������ַ�����ʽ��
	bool format_index(std::string& resultOUT, const char* fmt, const std::vector<std::string>& argList);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	inline bool format_index(std::string& resultOUT, const char* fmt);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0, typename Arg1>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0, typename Arg1, typename Arg2>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6);
	/// @brief ����C#�İ����±��������ַ�����ʽ��
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7);

	/// @brief ������ת�����ַ�����ʽ
	inline std::string& tostring(std::string& resultOUT, char c);
	/// @brief ������ת�����ַ�����ʽ
	inline std::string& tostring(std::string& resultOUT, const char* pstr);
	/// @brief ������ת�����ַ�����ʽ
	inline std::string& tostring(std::string& resultOUT, const std::string& str);
	/// @brief ������ת�����ַ�����ʽ
	inline std::string& tostring(std::string& resultOUT, bool value);
	/// @brief ������ת�����ַ�����ʽ
	template<typename ValueType>
	inline std::string& tostring(std::string& resultOUT, ValueType value);

	/// @brief �ַ���׷��
	/// @return str������
	std::string& append(std::string& str, const char* fmt, ...);
	/// @brief �ַ���׷��
	/// @return str������
	inline std::string& append(std::string& str, const std::string& cat);
	/// @brief �ַ���׷��
	/// @return str������
	inline std::string& append(std::string& str, char c);
	/// @brief �ַ���׷��
	/// @return str������
	inline std::string& append(std::string& str, bool value);
	/// @brief �ַ���׷��
	/// @return str������
	template<typename ValueType>
	inline std::string& append(std::string& str, ValueType value);

	/// @brief ��ȥ�ַ�����ͷ�Ŀհ�
	inline std::string trimbegin(std::string& str, const std::string& whiteSpace = WHITE_SPACE);
	/// @brief ��ȥ�ַ�����β�Ŀհ�
	inline std::string trimend(std::string& str, const std::string& whiteSpace = WHITE_SPACE);
	/// @brief ��ȥ�ַ�����ͷ�Ŀհ�
	inline std::string trim(std::string& str, const std::string& whiteSpace = WHITE_SPACE);

	/// @brief �ж��ַ������Ƿ�����������ַ���
	inline bool contains(const std::string& str, const std::string& tosearch);
	/// @brief �ж��ַ������Ƿ�����������ַ���
	inline bool contains(const std::string& str, const char* tosearch);
	/// @brief �ж��ַ����Ƿ��Ը������ַ�����ͷ
	inline bool startwith(const std::string& str, const std::string& needle);
	/// @brief �ж��ַ����Ƿ��Ը������ַ�����β
	inline bool endwith(const std::string& str, const std::string& needle);
	/// @brief �ж��ַ����Ƿ��Ը������ַ���β
	inline bool endwith(const std::string& str, char needle);
	inline bool endwith(const std::wstring& str, wchar_t needle);

	/// @brief �ַ��������
	inline std::string& padleft(std::string& str, std::string::size_type size, std::string::value_type paddingchar = ' ');
	/// @brief �ַ��������
	inline std::string& padright(std::string& str, std::string::size_type size, std::string::value_type paddingchar = ' ');

	/// @brief ���ַ�������ȥ��
	/// @remark StringListContainer can be std::list<std::string> or std::vector<std::string>
	template <typename StringListContainerFrom, typename StringListContainerTo>
	void unique(const StringListContainerFrom& strList, StringListContainerTo& resultOUT);
	/// @brief ���ַ�������ȥ��
	/// @remark StringListContainer can be std::list<std::string> or std::vector<std::string>
	template<typename StringListContainer>
	void unique(StringListContainer& strList);

	/// @brief �ַ����ָ�
	/// @param separator �ָ��ַ���ASCII�ַ���
	void split(const std::string& str, std::string::value_type separator, std::vector<std::string>& resultOUT, size_t resultLimit = -1);
	/// @brief �ַ����ָ�
	/// @param separator �ָ��ַ�����Ҳ����Ϊ���֣�
	void split(const std::string& str, const std::string& separator, std::vector<std::string>& resultOUT, size_t resultLimit = -1);
	/// @brief �ַ����ָ�
	/// @param separatorList �ָ��ַ������飬���д���������ηֱ����Ϊ�ָ���
	/// @remark StringListContainer can be std::list<std::string> or std::vector<std::string>
	template<typename StringListContainer>
	void split(const std::string& str, const StringListContainer& separatorList, std::vector<std::string>& resultOUT, size_t resultLimit = -1);
	/// @brief ���ַ������տհ׷ֿ������͵Ŀհװ���" \t\r\n\f\v"������Ϊ: �ո�, ˮƽ�Ʊ��, �س�, ����, ��ֽ����, ��ֱ�Ʊ����
	void split_white_space(const std::string& str, std::vector<std::string>& resultOUT, size_t resultLimit = -1);
	/// @brief ���ճ��ȵ��ַ����з֡��зֺ�֤�����ÿ���Ӵ��ĳ��Ȳ����ڸ������ȣ����ܱ�֤���ֲ��ᱻ��������
	/// @remark �Ƿ��зֳɹ�
	bool split_fix_size(const std::string& str, std::string::size_type fix_size, std::vector<std::string>& resultOUT);

	/// @brief �ַ���ճ��
	template <class InputIterator>
	inline std::string& join(const std::string &glue, InputIterator first, InputIterator last, std::string& resultOUT);
	/// @brief �ַ���ճ��
	template <class InputIterator>
	inline std::string join(const std::string &glue, InputIterator first, InputIterator last);

	/// @brief ���ַ���str�����е�from�滻Ϊto
	std::string& replace(std::string& str, const std::string& from, const std::string& to);
	std::wstring& replace(std::wstring& str, const std::wstring& from, const std::wstring& to);
	/// @brief ���ַ���str�����е�from�滻Ϊto
	std::string& replace(std::string& str, std::string::value_type from, std::string::value_type to);
	std::wstring& replace(std::wstring& str, std::wstring::value_type from, std::wstring::value_type to);

	/// @brief ���ַ���str/wstr�����е�to_removeɾ������ǰ����ֻɨ��һ��
	inline std::string& erase(std::string& str, const std::string& to_remove);
	inline std::wstring& erase(std::wstring& wstr, const std::wstring& to_remove);

	/// @brief ���ַ���ת��Ϊ���֣���ǰ��󾡿��ܶ��ת��
	int32 integer(const std::string& str);
	/// @brief ���ַ���ת��Ϊ���֣���ǰ��󾡿��ܶ��ת��
	/// @return �����Ƿ�ȫ����ת��
	bool integer(const std::string& str, int32& valueOut);
	/// @brief ���ַ�ת�����޷�������
	bool uinteger(const std::string& str, uint32& valueOut);
	/// @brief ���ַ���ת����u64����
	bool u64interger(const std::string& str, uint64& valueOut);

	/// @brief �ظ��ַ���
	std::string& repeat(const std::string& str, std::string::size_type times, std::string& resultOUT);
	/// @brief �ظ��ַ���
	inline std::string repeat(const std::string& str, std::string::size_type times);

	/// @brief �жϸ������ַ����Ƿ�ȫ��ΪСд��ĸ
	bool is_lower(const std::string& str);
	/// @brief �жϸ������ַ��Ƿ���Сд��ĸ
	inline bool is_lower(char c);
	/// @brief �жϸ������ַ����Ƿ�ȫ��Ϊ��д��ĸ
	bool is_upper(const std::string& str);
	/// @brief �жϸ������ַ��Ƿ��Ǵ�д��ĸ
	inline bool is_upper(char c);
	/// @brief �жϸ������ַ����Ƿ�ȫ��Ϊ��ĸ
	bool is_alpha(const std::string& str);
	/// @brief �жϸ������ַ��Ƿ�����ĸ
	inline bool is_alpha(char c);
	/// @brief �жϸ������ַ����Ƿ�ȫ��Ϊ����
	bool is_numeric(const std::string& str);
	/// @brief �жϸ������ַ��Ƿ�Ϊ����
	inline bool is_numeric(char c);
	/// @brief �жϸ������ַ����Ƿ�ȫΪ���ֻ���ĸ
	bool is_alnum(const std::string& str);
	/// @brief �жϸ������ַ��Ƿ�Ϊ���ֻ�����ĸ
	inline bool is_alnum(char c);
	/// @brief �жϸ������ַ��Ƿ��ǿհ��ַ�
	inline bool is_whitespace(char c);

	/// @brief ���ַ����͵�ת��ΪСд
	std::string& tolower(std::string& str);
	/// @brief ���ַ����͵�ת��ΪСд
	char* tolower(char* pstr);
	/// @brief ���ַ�ת��ΪСд
	inline char tolower(char c);
	/// @brief ���ַ����͵�ת��Ϊ��д
	std::string& toupper(std::string& str);
	/// @brief ���ַ�����Ϊ��д
	inline char toupper(char c);

	/// @brief ���ַ����͵�ת��ΪСд
	std::wstring& tolower(std::wstring& str);
	/// @brief ���ַ����͵�ת��ΪСд
	wchar_t* tolower(wchar_t* pstr);
	/// @brief ���ַ�ת��ΪСд
	inline wchar_t tolower(wchar_t c);
	/// @brief ���ַ����͵�ת��Ϊ��д
	std::wstring& toupper(std::wstring& str);
	/// @brief ���ַ�����Ϊ��д
	inline wchar_t toupper(wchar_t c);

	/// @brief �͵صߵ������ַ����Ĵ�Сд
	std::string& swapcase(std::string& str);
	/// @brief �ߵ������ַ��Ĵ�Сд
	inline char swapcase(char c);

	/// @brief �õ�str��module���ֵĴ���
	std::string::size_type count(const std::string& str, const std::string &module);
	/// @brief �ַ����ȽϺ���
	inline int32 compare(const char* str1, const char* str2);
	/// @brief �ַ����ȽϺ���
	inline int32 compare(const std::string& str1, const char* str2);
	/// @brief �ַ����ȽϺ���
	inline int32 compare(const std::string& str1, const std::string& str2);
	/// @brief �ַ����ȽϺ���
	inline int32 compare(const char* str1, const std::string& str2);
	/// @brief �ַ��ȽϺ���
	inline int32 compare(char c1, char c2);

	/// @brief �����ִ�Сд�ıȽϺ���
	int32 icompare(const std::string& str1, const std::string& str2);
	/// @brief �����ִ�Сд�ıȽϺ���
	inline int32 icompare(const char* str1, const char* str2);
	/// @brief �����ִ�Сд���ַ��ȽϺ���
	inline int32 icompare(char c1, char c2);

	/// @brief �ַ�����Ȼ�ȽϺ���
	/// @remark http://sourcefrog.net/projects/natsort/
	int32 compare_nature(const char *str1, const char *str2, bool case_sensitive = true);
	/// @brief �ַ�����Ȼ�ȽϺ���
	inline int32 compare_nature(const std::string& str1, const std::string& str2, bool case_sensitive = true);

	/// @brief �ַ���ͨ���ƥ���ж�
	/// @remark http://www.codeproject.com/KB/string/wildcmp.aspx
	bool match_wildcard(const char* str, const char* wildcard);

	/// @brief �жϸ������ַ����Ƿ�Ϊ��
	inline bool empty(const char* pstr);
	/// @brief �жϸ������ַ����Ƿ�Ϊ��
	inline bool empty(const std::string& str);
	/// @brief �жϸ������ַ��Ƿ�Ϊ��
	inline bool empty(char c);

	/// @brief ���ַ�����Hash��
	uint32 String2ID(const char* str);

	/// @brief �ļ�·�����ò���, ����pathΪ"c:\\temp/aaa.tmp"
	//@{
	/// �������һ��·���ָ�����λ��
	char* get_last_slash(const std::string& path);
	/// ��ʽ��·���ָ���Ϊ'\\'(c:\\temp\\aaa.tmp)
	std::string& slash_to_backslash(std::string& path);
	/// ��ʽ��·���ָ���Ϊ'/'(c:/temp/aaa.tmp)
	std::string&  backslash_to_slash(std::string& path);
	std::wstring& backslash_to_slash(std::wstring& path);
	/// ȡ���ļ���(aaa.tmp)
	std::string extract_filename(const std::string& path);
	/// ȡ����չ��(tmp)
	std::string extract_extention(const std::string& path);
	/// ȡ����Ŀ¼��(temp)
	std::string extract_parent_dirname(const std::string& path);
	/// ȡ����Ŀ¼(c:\\temp)
	std::string extract_parent_dir(const std::string& path);
	/// ����Ƿ�Ϊָ����չ��(ext = tmp)
	bool check_extention(const std::string& path, const std::string& ext);
	/// ����ļ�ͨ����Ƿ�ƥ��(pattern = c:\\te?p/*.tmp")
	bool match_pattern(const string& path, const string& pattern);
	//@}

	/// @brief �༭����
	/// @define (1) The smallest number of insertions, deletions, and substitutions required to change
	///		one string or tree into another. (2) A ��(m �� n) algorithm to compute the distance between
	///		strings, where m and n are the lengths of the strings
	/// @remark �ο�	http://www.merriampark.com/ld.htm
	///					http://www.merriampark.com/ldcpp.htm
	///
	/// @other Manhattan Distance �����پ���
	/// @define The distance between two points measured along axes at right angles.
	///		In a plane with p1 at (x1, y1) and p2 at (x2, y2), it is |x1 - x2| + |y1 - y2|.
	///
	/// @other Hamming Distance ��������
	/// @define The number of bits which differ between two binary strings.
	///		More formally, the distance between two strings A and B is �� | Ai - Bi |.
	///
	/// @other Euclidean Distance ŷ����¾���
	/// @define The straight line distance between two points.
	///		In a plane with p1 at (x1, y1) and p2 at (x2, y2), it is ��((x1 - x2)? + (y1 - y2)?).
	int32 LevenshteinDistance(const std::string& source, const std::string& target);

	/// @brief ��һ��wstringת��Ϊstring
	std::string& convert(std::string& dest, const std::wstring& src);
	/// @brief ��һ��stringת��Ϊwstring
	std::wstring& convert(std::wstring& dest, const std::string& src);

	/// @brief �жϸ����ַ��Ƿ�Ϊȫ���ַ�
	bool is_sbc(uint32 nChar);
	int32 find_sbc_on_utf8(const char* str);

	inline std::string vformat(const char *fmt, va_list argptr, int32 default_size/* = DEFAULT_BUFFER_SIZE*/)
	{
		std::string resultOUT;
		return vformat(resultOUT, fmt, argptr, default_size);
	}
	inline bool format_index(std::string& resultOUT, const char* fmt)
	{
		return format_index(resultOUT, fmt, std::vector<std::string>());
	}
	template<typename Arg0>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0)
	{
		std::vector<std::string> args(1);
		tostring(args[0], arg0);
		return format_index(resultOUT, fmt, args);
	}
	template<typename Arg0, typename Arg1>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1)
	{
		std::vector<std::string> args(2);
		tostring(args[0], arg0);
		tostring(args[1], arg1);
		return format_index(resultOUT, fmt, args);
	}
	template<typename Arg0, typename Arg1, typename Arg2>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2)
	{
		std::vector<std::string> args(3);
		tostring(args[0], arg0);
		tostring(args[1], arg1);
		tostring(args[2], arg2);
		return format_index(resultOUT, fmt, args);
	}
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3)
	{
		std::vector<std::string> args(4);
		tostring(args[0], arg0);
		tostring(args[1], arg1);
		tostring(args[2], arg2);
		tostring(args[3], arg3);
		return format_index(resultOUT, fmt, args);
	}
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
	{
		std::vector<std::string> args(5);
		tostring(args[0], arg0);
		tostring(args[1], arg1);
		tostring(args[2], arg2);
		tostring(args[3], arg3);
		tostring(args[4], arg4);
		return format_index(resultOUT, fmt, args);
	}
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
	{
		std::vector<std::string> args(6);
		tostring(args[0], arg0);
		tostring(args[1], arg1);
		tostring(args[2], arg2);
		tostring(args[3], arg3);
		tostring(args[4], arg4);
		tostring(args[5], arg5);
		return format_index(resultOUT, fmt, args);
	}
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
	{
		std::vector<std::string> args(7);
		tostring(args[0], arg0);
		tostring(args[1], arg1);
		tostring(args[2], arg2);
		tostring(args[3], arg3);
		tostring(args[4], arg4);
		tostring(args[5], arg5);
		tostring(args[6], arg6);
		return format_index(resultOUT, fmt, args);
	}
	template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	bool format_index(std::string& resultOUT, const char* fmt, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7)
	{
		std::vector<std::string> args(8);
		tostring(args[0], arg0);
		tostring(args[1], arg1);
		tostring(args[2], arg2);
		tostring(args[3], arg3);
		tostring(args[4], arg4);
		tostring(args[5], arg5);
		tostring(args[6], arg6);
		tostring(args[7], arg7);
		return format_index(resultOUT, fmt, args);
	}

	inline std::string& tostring(std::string& resultOUT, char c)
	{
		resultOUT = c;
		return resultOUT;
	}
	inline std::string& tostring(std::string& resultOUT, const char* pstr)
	{
		resultOUT = pstr == NULL ? "" : pstr;
		return resultOUT;
	}
	inline std::string& tostring(std::string& resultOUT, const std::string& str)
	{
		resultOUT = str;
		return resultOUT;
	}
	inline std::string& tostring(std::string& resultOUT, bool value)
	{
		resultOUT = value ? "true" : "false";
		return resultOUT;
	}
	template<typename ValueType>
	inline std::string& tostring(std::string& resultOUT, ValueType value)
	{
		std::stringstream oss;
		oss << value;
		resultOUT = oss.str();
		return resultOUT;
	}
	inline std::string& append(std::string& str, const std::string& cat)
	{
		str += cat;
		return str;
	}
	inline std::string& append(std::string& str, char c)
	{
		str += c;
		return str;
	}
	inline std::string& append(std::string& str, bool value)
	{
		str += value ? "true" : "false";
		return str;
	}
	template<typename ValueType>
	inline std::string& append(std::string& str, ValueType value)
	{
		std::stringstream oss;
		oss << value;
		str += oss.str();
		return str;
	}
	inline std::string trimbegin(std::string& str, const std::string& whiteSpace/* = WHITE_SPACE*/)
	{
		return str.erase(0,str.find_first_not_of(whiteSpace));
	}
	inline std::string trimend(std::string& str, const std::string& whiteSpace/* = WHITE_SPACE*/)
	{
		return str.erase(str.find_last_not_of(whiteSpace)+1);
	}
	inline std::string trim(std::string& str, const std::string& whiteSpace/* = WHITE_SPACE*/)
	{
		str.erase(str.find_last_not_of(whiteSpace)+1);
		return str.erase(0,str.find_first_not_of(whiteSpace));
	}

	inline bool contains(const std::wstring& str, const std::wstring& tosearch)
	{
		return str.find(tosearch) != std::wstring::npos;
	}
	inline bool contains(const std::wstring& str, const wchar_t* tosearch)
	{
		if (tosearch == NULL)
			return false;
		return str.find(tosearch) != std::wstring::npos;
	}

	inline bool contains(const std::string& str, const std::string& tosearch)
	{
		return str.find(tosearch) != std::string::npos;
	}
	inline bool contains(const std::string& str, const char* tosearch)
	{
		if (tosearch == NULL)
			return false;
		return str.find(tosearch) != std::string::npos;
	}
	inline bool startwith(const std::string& str, const std::string& needle)
	{
		return str.compare(0, needle.size(), needle) == 0;
	}
	inline bool endwith(const std::string& str, const std::string& needle)
	{
		std::string::size_type size = needle.size();
		return str.compare(str.size() - size, size, needle) == 0;
	}
	inline bool endwith(const std::string& str, char needle)
	{
		if (str.empty())
			return false;
		return (needle == str[str.length() - 1]);
	}
	inline bool endwith(const std::wstring& str, wchar_t needle)
	{
		if (str.empty())
			return false;
		return (needle == str[str.length() - 1]);
	}
	inline std::string& padleft(std::string& str, std::string::size_type size, std::string::value_type paddingchar/* = ' '*/)
	{
		if (size > str.size())
			str.insert(0, size - str.size(), paddingchar);
		return str;
	}
	inline std::string& padright(std::string& str, std::string::size_type size, std::string::value_type paddingchar/* = ' '*/)
	{
		if (size > str.size())
			str.resize(size, paddingchar);
		return str;
	}
	template <typename StringListContainerFrom, typename StringListContainerTo>
	void unique(const StringListContainerFrom& strList, StringListContainerTo& resultOUT)
	{
		resultOUT.clear();
		resultOUT.reserve(strList.size());
		for (typename StringListContainerFrom::const_iterator it_from = strList.begin();
			it_from != strList.end(); ++it_from)
		{
			bool repeat = false;
			for (typename StringListContainerFrom::const_iterator it_to = resultOUT.begin();
				it_to != resultOUT.end(); ++it_to)
			{
				if (it_from->compare(*it_to) == 0)
				{
					repeat = true;
					break;
				}
			}
			if (repeat == false)
				resultOUT.push_back(*it_from);
		}
	}
	template<typename StringListContainer>
	void unique(StringListContainer& strList)
	{
		StringListContainer result;
		sqr::unique(strList, result);
		if (strList.size() != result.size())
			strList.assign(result.begin(), result.end());
	}

	template<typename StringListContainer>
	void split(const std::string& str, const StringListContainer& separatorList, std::vector<std::string>& resultOUT, size_t resultLimit /*= -1*/)
	{
		typedef std::vector<std::string> container;

		resultOUT.clear();
		if (resultLimit == 0)
			return;
		// �ָ�ģ��ȥ��
		std::vector<std::string> splitSeparatorList;
		sqr::unique(separatorList, splitSeparatorList);
		if (separatorList.empty())
		{
			resultOUT.push_back(str);
			return;
		}

		// ���зָ����
		std::string::size_type begin = 0;
		for (std::string::size_type i = 0; i < str.size(); i++)
		{
			for (container::const_iterator it = splitSeparatorList.begin(); it != splitSeparatorList.end(); ++it)
			{
				if (it->compare(0, it->size(), str, i, it->size()) == 0)
				{
					if (i != begin)
					{
						resultOUT.push_back(str.substr(begin, i - begin));
						if (resultOUT.size() == resultLimit)
							return;
					}
					begin = i + it->size();
					i = begin - 1;// -1��Ϊ�˵������forѭ�������i++
					break;
				}
			}
		}
		if (begin < str.size() && resultOUT.size() < resultLimit)
			resultOUT.push_back(str.substr(begin));
		Ast(resultOUT.size() <= resultLimit);
	}
	template <class InputIterator>
	inline std::string& join(const std::string &glue, InputIterator first, InputIterator last, std::string& resultOUT)
	{
		resultOUT.clear();
		if (first == last)
			return resultOUT;
		for (; first != last; first++)
		{
			resultOUT.append(*first);
			resultOUT.append(glue);
		}
		resultOUT.erase(resultOUT.size() - glue.size());
		return resultOUT;
	}
	template <class InputIterator>
	inline std::string join(const std::string &glue, InputIterator first, InputIterator last)
	{
		std::string resultOUT;
		return join(glue, first, last);
	}
	inline std::string& erase(std::string& str, const std::string& to_remove)
	{
		return replace(str, to_remove, "");
	}
	inline std::wstring& erase(std::wstring& wstr, const std::wstring& to_remove)
	{
		return replace(wstr, to_remove, L"");
	}
	inline std::string repeat(const std::string& str, std::string::size_type times)
	{
		std::string resultOUT;
		return repeat(str, times, resultOUT);
	}
	inline bool is_lower(char c)
	{
		return c >= 'a' && c <= 'z';
	}
	inline bool is_lower(wchar_t c)
	{
		return c >= L'a' && c <= L'z';
	}
	inline bool is_upper(char c)
	{
		return c >= 'A' && c <= 'Z';
	}
	inline bool is_upper(wchar_t c)
	{
		return c >= L'A' && c <= L'Z';
	}
	inline bool is_alpha(char c)
	{
		return is_lower(c) || is_upper(c);
	}
	inline bool is_alpha(wchar_t c)
	{
		return is_lower(c) || is_upper(c);
	}
	inline bool is_numeric(char c)
	{
		return c >= '0' && c <= '9';
	}
	inline bool is_numeric(wchar_t c)
	{
		return c >= L'0' && c <= L'9';
	}
	inline bool is_alnum(char c)
	{
		return is_alpha(c) || is_numeric(c);
	}
	inline bool is_alnum(wchar_t c)
	{
		return is_alpha(c) || is_numeric(c);
	}
	inline bool is_whitespace(char c)
	{
		return WHITE_SPACE.find(c) != std::string::npos;
	}
	inline char tolower(char c)
	{
		return is_upper(c) ? c - 'A' + 'a' : c;
	}
	inline wchar_t tolower(wchar_t c)
	{
		return is_upper(c) ? c - L'A' + L'a' : c;
	}
	inline char toupper(char c)
	{
		return is_lower(c) ? c - 'a' + 'A' : c;
	}
	inline wchar_t toupper(wchar_t c)
	{
		return is_lower(c) ? c - L'a' + L'A' : c;
	}
	inline char swapcase(char c)
	{
		if (is_upper(c))
			return c - 'A' + 'a';
		if (is_lower(c))
			return c - 'a' + 'A';
		return c;
	}
	inline int32 compare(const char* str1, const char* str2)
	{
		if (str1 == NULL)
		{
			if (str2 == NULL)
				return 0;
			else
				return -1;
		}
		else
		{
			if (str2 == NULL)
				return 1;
			else
				return strcmp(str1, str2);
		}
	}
	inline int32 compare(const std::string& str1, const char* str2)
	{
		if (str2 == NULL)
			return 1;
		else
			return str1.compare(str2);
	}
	inline int32 compare(const std::string& str1, const std::string& str2)
	{
		return str1.compare(str2);
	}
	inline int32 compare(const char* str1, const std::string& str2)
	{
		return -compare(str2, str1);
	}
	inline int32 compare(char c1, char c2)
	{
		if (c1 > c2)
			return 1;
		else if (c1 < c2)
			return -1;
		else
			return 0;
	}
	inline int32 icompare(const char* str1, const char* str2)
	{
		if (str1 == str2)
			return 0;
		if (str1 == NULL)
			return -1;
		if (str2 == NULL)
			return 1;
#ifdef unix
		return strcasecmp(str1, str2);
#else
		return _stricmp(str1, str2);
#endif
	}
	inline int32 icompare(char c1, char c2)
	{
		if (c1 == c2)
			return 0;
		if (is_alpha(c1))
			c1 = tolower(c1);
		if (is_alpha(c2))
			c2 = tolower(c2);
		if (c1 > c2)
			return 1;
		else// if(c1 < c2)
			return -1;
	}
	inline int32 compare_nature(const std::string& str1, const std::string& str2, bool case_sensitive /*= true*/)
	{
		return compare_nature(str1.c_str(), str2.c_str(), case_sensitive);
	}

	inline bool empty(const char* pstr)
	{
		return pstr == NULL || pstr[0] == '\0';
	}
	inline bool empty(const std::string& str)
	{
		return str.empty();
	}
	inline bool empty(char c)
	{
		return c == '\0';
	}

	//------------------------------------------------------------------------------
	inline std::string
	extract_filename( const std::string& path )
	{
		string pathString;
		const char* lastSlash = get_last_slash(path);
		if (lastSlash)
		{
			pathString = &(lastSlash[1]);
		}
		else
		{
			pathString = path;
		}
		return pathString;
	}

	//------------------------------------------------------------------------------
	inline std::string
	extract_extention( const std::string& path )
	{
		const char* str = path.c_str();
		const char* ext = strrchr(str, '.');
		if (ext)
		{
			ext++;
			return string(ext);
		}
		return string("");
	}

	//------------------------------------------------------------------------------
	inline std::string
	extract_parent_dirname( const std::string& path )
	{
		string pathString(path);
		char* lastSlash = get_last_slash(pathString);

		// special case if path ends with a slash
		if (lastSlash)
		{
			if (0 == lastSlash[1])
			{
				*lastSlash = 0;
				lastSlash = get_last_slash(pathString);
			}

			char* secLastSlash = 0;
			if (0 != lastSlash)
			{
				*lastSlash = 0; // cut filename
				secLastSlash = get_last_slash(pathString);
				if (secLastSlash)
				{
					*secLastSlash = 0;
					return string(secLastSlash+1);
				}
			}
		}
		return "";
	}

	std::string  extract_parent_dir( const std::string& path );
	std::wstring extract_parent_dir( const std::wstring& path );

	//------------------------------------------------------------------------------
	inline bool
	check_extention( const std::string& path, const std::string& ext )
	{
		return (ext == extract_extention(path));
	}

	//------------------------------------------------------------------------------
	inline std::string&
	slash_to_backslash( std::string& path )
	{
		replace(path, "/", "\\");
		replace(path, "\\\\", "\\");
		return path;
	}

	//------------------------------------------------------------------------------
	inline std::wstring&
		slash_to_backslash( std::wstring& path )
	{
		replace(path, L"/", L"\\");
		replace(path, L"\\\\", L"\\");
		return path;
	}

	//------------------------------------------------------------------------------
	inline std::string&
	backslash_to_slash( std::string& path )
	{
		replace(path, "\\", "/");
		replace(path, "//", "/");
		return path;
	}

	//------------------------------------------------------------------------------
	inline std::wstring&
		backslash_to_slash( std::wstring& path )
	{
		replace(path, L"\\", L"/");
		replace(path, L"//", L"/");
		return path;
	}

	char*	 get_last_slash( const std::string& path );
	wchar_t* get_last_slash( const std::wstring& path );

	inline bool tobool(const std::string& s){
		return s=="true" ? true : false;
	}

	inline bool tobool(const char* ch){
		return tobool(std::string(ch));
	}

	//�ؼ��ֹ��˸���utf-8, ���ĳЩ�����ֱ�������˵�����
	int StringFindBaseOnUTF8(const char* str, const char* subStr, int beginIndex);	

	int GetCharCount(const char* str);
}

