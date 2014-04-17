/*
*	CPathMgr.h	�ļ�·����������������Դ·������
*/

#ifndef _VFS_CPATHMGR_H_
#define _VFS_CPATHMGR_H_


#ifndef _WIN32
#include <ftw.h>
#else
#include <Windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif


#define  PATHLEN  1024

namespace sqr
{
	enum FTW_FLAG
	{
#ifndef _WIN32
		_FTW_FILE = FTW_F,
		_FTW_DIR = FTW_D,
		_FTW_SL = FTW_SL,
		_FTW_NS = FTW_NS,
		_FTW_TMP,
#else
		_FTW_FILE = FILE_ATTRIBUTE_NORMAL,
		_FTW_DIR = FILE_ATTRIBUTE_DIRECTORY,
		_FTW_TMP = FILE_ATTRIBUTE_TEMPORARY,
		_FTW_SL,
		_FTW_NS,

#endif
	};

	enum FTW_RESULT
	{
		_FTW_STOP,
		_FTW_IGNORE,
		_FTW_CONTINUNE,
	};

#ifndef _WIN32
#	define _stat stat
#endif

	typedef FTW_RESULT(* FILE_PROC)( const wchar_t*, const struct _stat *, FTW_FLAG, void* );

	//����ע�⣬����������������κ���Ϊ".svn"Ŀ¼������
	int32		FileTreeWalk(const wchar_t* szDir, FILE_PROC pfnFileProc, void* pContext );
	bool		MakeDirectory( const wchar_t* szDirectory );
	bool		MakeDirectoryUTF8(const char * szDirectory);

	class CPathMgr
	{
	public:
		CPathMgr(const char* szPathName);
		CPathMgr(const wchar_t* szPathName = NULL);
		virtual ~CPathMgr();

		// ȡ�ñ�����ĸ�·��
		const wchar_t*	GetRootPath() const;
		
		// �ı����ĸ�·��
		void			ChangeRootPath(const wchar_t* RootPath);
		
		// �õ���ǰ·��
		const wchar_t*	GetCurPath() const;
		
		// ���õ�ǰ·��,�����������ڸ�Ŀ¼�����·��
		void			SetCurPath(const wchar_t* szPathName=NULL);

		//�ú������޸��ڲ���CurPath������
		const wchar_t* GetFullPath(const wchar_t* szPath);

	private:
		void			initialize(const wchar_t* szPathName);

	protected:
		wchar_t  m_szRootPath[PATHLEN];  // ����·��
		wchar_t  m_szCurrPath[PATHLEN];  // ��ǰ·��
	}; // End of class CPathMgr


}// End of namespace Vfs

#endif // End of #define _VFS_FILEPATHMGR_H_

