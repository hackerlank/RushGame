#pragma once
#include "TSingleton.h"

/**
	@class IoSystem

	IO ϵͳ�ĺ��Ĺ�����.
	��װ���õ��ļ�����.
	�ṩĿ¼���ƵĽ�����ӳ��, Ĭ���Ѿ�ӳ�����:
	bin:	��ִ���ļ�·��
	temp:	Windows��ʱĿ¼
	work:	��ǰ����Ŀ¼
 */
namespace sqr
{

class IoSystem : public TSingleton<IoSystem>
{
public:
	/// ����·����û�е�Ŀ¼
	bool CreateDirectory(const string& dir) const;
	/// ɾ��һ����Ŀ¼
	bool DeleteDirectory(const string& dir) const;
	/// Ŀ¼�Ƿ����
	bool DirectoryExists(const string& dir) const;

	/// �����ļ�
	bool CopyFile(const string& from, const string& to) const;
	/// ɾ���ļ�
	bool DeleteFile(const string& path) const;
	/// �ļ��Ƿ����
	bool FileExists(const string& path) const;
	/// �����ļ���ֻ������
	void SetReadOnly(const string& path, bool readOnly) const;
	/// �ļ��Ƿ�ֻ��
	bool IsReadOnly(const string& path) const;

	/// �г�Ŀ¼�µ��ļ�
	vector<string> ListFiles(const string& dir, const string& pattern) const;
	/// �г�Ŀ¼�µ���Ŀ¼
	vector<string> ListDirectories(const string& dir, const string& pattern) const;

	/// ����Ŀ¼ӳ��
	void RegisterDirMapping(const string& shortName, const string& dir, const bool bFull = true);
	/// ����ָ��·��
	string GetFullPath(const string& shortPath) const;
	string GetRegisterDir(const string& shortName) const;
	/// ���ù���Ŀ¼
	void SetWorkDirectory(const string& dir);
	void SetWorkSrcDirectory(const string& dir);
	void SetWorkTgtDirectory(const string& dir);

private:
	friend class TSingleton<IoSystem>;
	IoSystem();
	~IoSystem();
private:
	map<string, string> dirMappings;
};
}// namespace sqr
