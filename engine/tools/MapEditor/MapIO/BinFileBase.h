#pragma once
#include "DataChunk.h"

namespace IO
{
SmartPointer(Stream);
SmartPointer(BinarySerializer);
class VersionMapChunk;
typedef std::vector<DataChunk*> ChunkList;

/// �������ļ�����.
/// �������ļ��İ汾�ź͸�Chunk�İ汾��
class BinFileBase
{
public:
	BinFileBase(uint version);
	virtual ~BinFileBase();

	/// ����
	bool Open(const StreamPtr& stream);
	/// �ر���
	void Close();

	uint GetVersion() const;

	/// �����м����ļ�
	virtual void Load();
	/// ���浽��
	virtual void Save();

	/// ���һ��Chunk
	void AddChunk(DataChunk* chunk);
	/// ����ָ��Chunk
	DataChunk* FindChunk(const Util::FourCC& fourcc);

protected:
	/// ����Ӧ
	virtual void OnOpen();
	/// �ر���Ӧ
	virtual void OnClose();
	/// ��ʼ����Ӧ
	virtual void OnInit();

protected:
	ChunkList chunkList;
	BinarySerializerPtr serializer;
};

}// namespace IO

