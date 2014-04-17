#pragma once
#include "CGraphicMallocObject.h"
#include "TSingleton.h"

namespace sqr
{
	class CFontMgr
		: public Singleton<CFontMgr>
		, public CGraphicMallocObject
	{
	public:
		CFontMgr();

		void			AddFontInfo(const wchar_t* name, const char* file);
		void			ClearFontInfo();
		bool			LoadGDIFonts();
		void			UnloadGdiFonts();
		bool			SetLangIndex(int32 lang_index);
		int32			GetLangIndex() const;
		bool			SetOriLangFolder(const char* lang_folder);
		bool			SetLangFolder(const char* lang_folder);
		const char*		GetOriLangFolder();
		const char*		GetLangFolder();
		void			SetDisFontName(const char* name);
		const char*		GetDisFontName();
		const wchar_t*	GetGDIFontName(size_t nID);
		void			SetGDIDefFontName(const wchar_t* name);
		const wchar_t*  GetGDIDefFontName();

		typedef GVector<pair<GWString, GString> > GDIFontInfoList;

		static const int32 TEXTURE_FONT_ID		= 99998;
		static const int32 DISTANCE_FONT_ID		= 99999;
		static const int32 INVALID_FONT_ID		= -1;

	private:
		typedef GVector<HANDLE> FONTHandleList;

		FONTHandleList		m_hFontVec;			// ����ttf������Դ����б�
		GDIFontInfoList		m_GDIFontInfoList;	// ����ttf�������������ļ���ӳ���б�
		GString				m_szOriLangFolder;		// ԭʼ���԰����ڵ��ļ���
		GString				m_szLangFolder;		// ��ǰ���԰����ڵ��ļ���
		GString				m_szDisFontName;
		GWString			m_szDefFontName;
		int32				m_nLangIndex;
	};
}
