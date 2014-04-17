#pragma once

namespace sqr
{
	// ��Uint8תΪBase16, pBufΪ�շ���װ��char���ַ�����
	size_t Uint82Base16( const uint8* pUint8, size_t stSrcLen, char* pBase16, size_t sizeBuf );

	/**
	\brief
	���ַ���ת�����ڴ澵���16�����ַ������У���(����������)->(bebabcbcb3a1bde9c9dc),
	��������Ϊmbcs_to_MemH16,��ʶ�ú�����Ҫ������mbcs�ַ���ת��Ϊ��Ӧ���ڴ�16��������
	������������Ӧ��utf8��ʽ���ַ���
	*/
	std::string mbcs_to_MemH16(const std::string& src);


	/**
	\brief
	���ַ���ת�����ڴ澵���64�����ַ������У���(����������)->(vrq8vLOhvenJ3A),
	��������Ϊmbcs_to_MemH64,��ʶ�ú�����Ҫ������mbcs�ַ���ת��Ϊ��Ӧ���ڴ�64��������
	������������Ӧ��utf8��ʽ���ַ���
	*/
	std::string mbcs_to_MemH64(const std::string& src);
	std::string mbcs_to_MemH64(const char* src, uint32 length);

	/**
	\brief
	���ַ���ת���ڴ澵��64���Ʊ�ʶ���л�ԭΪmbcs�ַ�������(vrq8vLOhvenJ3A)->(����������),
	��������ΪMemH64_to_mbcs,��ʶ�ú�����Ҫ�������ڴ�64������������ԭΪ��Ӧ��mbcs�ַ���
	*/
	std::string MemH64_to_mbcs(const std::string& src);

	
	//������Щ�������ǲ��ܷŵ�moduleģ�����棬��Ϊ���ص���string����
	//�������dll������䵫����dll�����ͷţ��������ͷŵ�new��delete��ƥ��
	// ��utf8תΪunicode, pBufΪ�շ���װ��Unicode�ַ��ĸ���
	std::wstring& Utf8ToUCS2( std::wstring& dest, const std::string& src );
	std::wstring Utf8ToUCS2(const std::string& src);
	
	// ��UnicodeתΪutf8, pBufΪ�շ���װ��char���ַ�����
	std::string& UCS2ToUtf8( std::string& dest, const std::wstring& src );
	std::string UCS2ToUtf8(const std::wstring& src);

	/**
	\brief
		��utf8�����unicodeת����utf16����
	*/
	std::wstring& utf8_to_utf16(std::wstring& dest, const std::string& src);
	std::wstring  utf8_to_utf16(const std::string& src);

	/**
	\brief
		��utf16�����unicodeת����utf8����
	*/
	std::string& utf16_to_utf8(std::string& dest, const std::wstring& src);
	std::string  utf16_to_utf8(const std::wstring& src);

	/**
	\brief
		��mbcs������ַ���ת����utf16����
	*/
	bool			gbk_to_utf16(std::wstring& dest, const std::string& src);
	std::wstring	gbk_to_utf16(const std::string& src);

	/**
	\brief
		��utf16������ַ���ת����mbcs����
	*/
	bool		 utf16_to_gbk(std::string& dest, const std::wstring& src);
	std::string  utf16_to_gbk(const std::wstring& src);

	/**
	\brief
		��mbcs������ַ���ת����utf8����
	*/
	std::string gbk_to_utf8(const std::string& src);

	bool gbk_to_utf8(std::string& dest, const std::string& src);
	/**
	\brief
		��utf8������ַ���ת����mbcs����
	*/
	std::string utf8_to_gbk(const std::string& src);

	bool utf8_to_gbk(std::string& dest, const std::string& src);

	/**
	\brief
		����mbcs_to_u8
	*/
	const char* U8(const std::string& src);
	

}
