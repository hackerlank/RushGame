#pragma once
#include "DataChunk.h"

namespace IO
{

/// chunk�汾�ŵ�ӳ���ϵ.
class VersionMapChunk : public DataChunk
{
public:
	/// ����ʵ��.
	static VersionMapChunk* Instance();

	VersionMapChunk();
	virtual ~VersionMapChunk();
	/// ע��汾��
	void RegisterVersion(const Util::FourCC& id, uint ver);
	/// ���Ұ汾��
	uint FindVersion(const Util::FourCC& id);

	/// д����
	void Write();
	/// �����ж�ȡ
	void Read();

	/// ������ֵ
	VersionMapChunk& operator=(const VersionMapChunk& rhs);

private:
	static VersionMapChunk* instance;

	std::map<uint, uint> versionMap;
};

#define DeclareChunk(type) \
public: \
	static Util::FourCC FOURCC; \
	static uint VERSION; \
	static bool RegisterToVersionMap(); \
private:

#define ImplementChunk(type, fourcc, version) \
	Util::FourCC type::FOURCC = fourcc; \
	uint type::VERSION = version; \
	bool type##_registered = type::RegisterToVersionMap(); \
	bool type::RegisterToVersionMap() \
	{ \
		IO::VersionMapChunk::Instance()->RegisterVersion(type::FOURCC, type::VERSION); \
		return true; \
	}

}// namespace IO

