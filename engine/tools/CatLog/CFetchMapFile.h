#pragma once

//��ָ���ط���ȡmap file�ļ�����log��������

class CFetchMapFile
{
public:
	static CFetchMapFile& Inst();

	//���ݰ汾down����map file��list
	void Fetch(uint32 uVersion, bool bGac = true);
	
	void SetPath(const char* szPath);

private:
	CFetchMapFile();
	~CFetchMapFile();

private:
	string	m_strMapFilePath;
};

