#pragma once

class CCfgSkillReplaceCheck
{
public:
	static bool Check(const TCHAR* cfgFile);
	static void	EndCheck();

public:
	typedef multimap<string, CCfgSkillReplaceCheck*> MultiMapCSkillReplace;
	const string&	GetName()				{ return m_sSkillName; }
	const string&	GetStateName()			{ return m_sStateName; }
	const string&	GetReplaceSkillName()	{ return m_sReplaceSkillName; }
private:
	string 		m_sSkillName;			//��������	
	string		m_sStateName;			//����״̬��
	string		m_sReplaceSkillName;	//�滻������
public:
	static	MultiMapCSkillReplace	ms_multimapSkillReplace;
};
