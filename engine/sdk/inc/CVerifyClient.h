#pragma once

#include "CMd5Algorithm.h"
#include "CPathMgr.h"
#include "StringHelper.h"


#include "IAsyncUpdateHandler.h"
#include "CVerifyClient.h"
namespace sqr
{

	class IAsyncUpdateHandler;
	class CVerifyClient
	{
	public:
		CVerifyClient() : BUFF_LENGTH(64*1024)
		{
		}
		~CVerifyClient()
		{
			Clear();
		}

		// ɨ��sDirectoryAbsNameĿ¼,���ɾ���·����ΪsVerificationFileAbsName����֤�ļ�
		// ·��������  c:\Ӷ������  (ע��:����ܴ�"\")
		void MakeVerificationFile( wstring sDirectoryAbsName, wstring sVerificationFileAbsName )
		{
			Clear();

			m_strRootDirectoryAbsPath = sDirectoryAbsName;	

			CollectAllFileNameRecursive(m_strRootDirectoryAbsPath);
			GenerateFileNameToMd5Map();
			SaveVerificationToFile(sVerificationFileAbsName);

		}


		// �����֤�������ļ���������,sVerificationFileAbsName�Ǿ���·���ļ���
		bool CheckIntegrity(wstring sVerificationFileAbsName)
		{
			FILE* f= _wfopen(sVerificationFileAbsName.c_str(), L"rb");
			if (f == NULL)
			{
				// �ļ��򲻿�����
				return false;
			}

			fseek(f, 0, SEEK_END);
			size_t nByteLen = ftell(f);
			fseek(f, 0, SEEK_SET);
			if (nByteLen <=32) 
			{//����ļ�̫С
				fclose(f);

				return false;
			}


			char* pBuffer = new char[nByteLen];
			fread(pBuffer, sizeof(char), nByteLen, f);
			fclose(f);

			for (size_t i=0; i<nByteLen; i++)
			{ 
				pBuffer[i] ^= 0xFF;	
			}

			char* pCurr = pBuffer;

			char buffMD5[33];
			memset(buffMD5, 0, sizeof(buffMD5));
			memcpy(buffMD5, pCurr, 32);
			pCurr += 32;
			string strMd5Org(buffMD5);

			memset(buffMD5, 0, sizeof(buffMD5));
			CMd5Algorithm md5;
			md5.Begin();
			md5.Append((uint8*)pCurr, (uint)(pBuffer + (uint)nByteLen - pCurr));
			md5.GetMD5Base16Str(buffMD5);
			string strMd5(buffMD5);
			if (pBuffer)
			{
				delete[] pBuffer;
				pBuffer = NULL;
			}

			return strMd5Org == strMd5;

		}

		// ������ΪsVerificationFileAbsName����֤�ļ�����֤Ŀ¼sDirectoryAbsName
		// ����ֵ  true��ʾͨ����֤    false��ʾ��֤�����ļ����𻵻�ʧ
		// ·��������  c:\Ӷ������  (ע��:����ܴ�"\")
		bool VerifyDirectory(sqr::IAsyncUpdateHandler* handler, wstring sDirectoryAbsName, wstring sVerificationFileAbsName)
		{
			Clear();

			LoadVerificationFile(sVerificationFileAbsName);

			map<wstring, string>::iterator ite = m_mapFileToMd5.end();
			int i=0;
			for (map<wstring, string>::iterator it = m_mapFileToMd5.begin(); it != ite; it++)
			{
				if (handler)
				{
					handler->SetSetupState((int)(i*90/m_mapFileToMd5.size()));
				}
				i++;

				wstring strCurrTemp = sDirectoryAbsName + it->first;

				if (-1 == _waccess(strCurrTemp.c_str(), 04))
				{
					//m_vLostList.push_back(strCurrTemp);
					m_vLostList.push_back(it->first);
					continue;
				}

				string strNewMd5 = GenerateMd5OfFile(strCurrTemp);
				if (strNewMd5 != it->second)
				{
					// ���ļ�MD5����
					//m_vErrorList.push_back(strCurrTemp);
					m_vErrorList.push_back(it->first);
				}
			}
			if (handler)
			{
				handler->SetSetupState(100);
			}

			if (m_vErrorList.size() + m_vLostList.size())
			{
				return false;
			}
			return true;
		}

		// �õ�������ʾ�ַ���,���û�д�����ô���ؿմ�
		wstring FetchErrorMsg()
		{
			wstringstream ss;

			if (!m_vLostList.empty())
			{
				ss << L"����" << m_vLostList.size() << L"���ļ�ȱʧ: ";
			}
			vector<wstring>::iterator ite = m_vLostList.end();
			for (vector<wstring>::iterator it = m_vLostList.begin(); it != ite; it++)
			{
				ss << *it << L"  ";
			}
			ss << L"\n";

			if (!m_vErrorList.empty())
			{
				ss << L"����" << m_vErrorList.size() << L"���ļ���: ";		
			}

			ite = m_vErrorList.end();
			for (vector<wstring>::iterator it = m_vErrorList.begin(); it != ite; it++)
			{
				ss << *it << L"  ";
			}
			ss << L"\n";

			//ss << "����������������Ϸ�ͻ���.";

			return ss.str();
		}



		void AddFileName( const wchar_t* szBuffer  )
		{

			wstring strFileName(szBuffer);
			wstring strTemp = strFileName.substr(m_strRootDirectoryAbsPath.size());
			wstring strTempLow = strTemp;

			tolower(strTempLow);

			if (    strTempLow.find(L"wxsdlrevision.log")  == wstring::npos 
				&& strTempLow.find(L"ver.sys")  == wstring::npos
				&& strTempLow.find(L"cltax.xmf")  == wstring::npos
				&& strTempLow.find(L"var/")  == wstring::npos )
			{
				m_vFileList.push_back(strTemp);
				// �������ǰ���Ǵ��Ÿ�/
			}
		}

	protected:
		void Clear()
		{
			m_strRootDirectoryAbsPath.clear();
			m_vFileList.clear();
			m_mapFileToMd5.clear();

			m_vErrorList.clear();
			m_vLostList.clear();
		}
		string GenerateMd5OfFile(wstring sFileAbsName)
		{
			char buffMD5[35];
			memset(buffMD5, 0, sizeof(buffMD5));

			CMd5Algorithm md5;
			md5.Begin();
			FILE* f = _wfopen(sFileAbsName.c_str(), L"rb");
			if (f)
			{
				uint8* pDataBuffer = new uint8[BUFF_LENGTH];
				if (pDataBuffer == NULL)
				{
					//printf("MD5У��ʱ�ڴ�������\n");
					exit(0);
				}

				int nLenRead = 0;
				while (nLenRead = (int)fread(pDataBuffer, sizeof(uint8), BUFF_LENGTH, f))
				{
					md5.Append(pDataBuffer, nLenRead);
				}

				fclose(f);
				delete[] pDataBuffer;
				pDataBuffer = NULL;
			}
			md5.GetMD5Base16Str(buffMD5);
			return string(buffMD5);
		}

		void CollectAllFileNameRecursive(wstring sDirectory)
		{
			HANDLE hFind;
			WIN32_FIND_DATAW ffd;

			wstring szSearch = sDirectory + L"/*.*";

			ZeroMemory( &ffd, sizeof(WIN32_FIND_DATA) );
			hFind = FindFirstFileW( szSearch.c_str(), &ffd );

			if( hFind == INVALID_HANDLE_VALUE )
				return ;

			while( FindNextFileW( hFind, &ffd ) )
			{
				if ( !wcscmp( ffd.cFileName, L"." ) || !wcscmp( ffd.cFileName, L".." ) )
					continue;


				wstring sNewFileName = sDirectory + L"/" + ffd.cFileName;

				// ���Կͻ��˰汾Ψһ��ʶ�ļ�
				if ( !wcscmp(L"WxSDLRevision.log", ffd.cFileName))
					continue;
				if ( !wcscmp(L"ver.sys", ffd.cFileName))
					continue;


				if( ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ) // Ŀ¼
				{
					CollectAllFileNameRecursive( sNewFileName);
				}
				else // ��¼�ļ���Ϣ
				{
					if (sNewFileName.find(m_strRootDirectoryAbsPath, 0) == 0)
					{
						//wstring strTemp = sNewFileName.substr(m_strRootDirectoryAbsPath.size());
						//m_vFileList.push_back(strTemp);// �������ǰ���Ǵ��Ÿ�/
						AddFileName(sNewFileName.c_str());
					}
					else
					{
						//printf("�������ļ��б�ʱ�ļ��� %s ����\n", sNewFileName.c_str());
					}
				}
			}

			FindClose( hFind );


			//FileTreeWalk(sDirectory.c_str(), &GFileWalkCallback, this);

			return ;
		}

		size_t GenerateFileNameToMd5Map()
		{
			vector<wstring>::iterator ite = m_vFileList.end();
			vector<wstring>::iterator it = m_vFileList.begin();

			while (it != ite)
			{
				wstring strAbsName = m_strRootDirectoryAbsPath + *it;
				m_mapFileToMd5[*it] = GenerateMd5OfFile(strAbsName);
				it++;
			}
			return m_mapFileToMd5.size();
		}

		void SaveVerificationToFile(wstring sVerificationFileAbsPathName)
		{	
			size_t nCount = 0;
			size_t nByteLen = 0;


			nByteLen += 32; // �������ݵ�md5��У��

			// m_mapFileToMd5.size()
			nByteLen += sizeof(size_t);

			map<wstring, string>::iterator ite = m_mapFileToMd5.end();
			for (map<wstring, string>::iterator it = m_mapFileToMd5.begin(); it!=ite; it++)
			{
				nCount++;
				nByteLen += sizeof(size_t);
				nByteLen += it->first.size() * sizeof(wstring::value_type);

				nByteLen += sizeof(size_t);
				nByteLen += it->second.size()* sizeof(string::value_type);
			}

			char* pBuffer = new char[nByteLen];
			char* pCurr = pBuffer;


			//��λ�ô�ź������ݵ�md5,�ȱ�������
			char* pMd5Self = pCurr;
			pCurr += 32;

			char* pContent = pCurr;

			memcpy(pCurr, &nCount, sizeof(nCount));
			pCurr += sizeof(nCount);

			ite = m_mapFileToMd5.end();
			for (map<wstring, string>::iterator it = m_mapFileToMd5.begin(); it!=ite; it++)
			{ 
				size_t nLen = it->first.size() * sizeof(wstring::value_type);
				memcpy(pCurr, &nLen, sizeof(nLen));
				pCurr += sizeof(nLen);

				memcpy(pCurr, it->first.c_str(), nLen);
				pCurr += nLen;


				nLen = it->second.size() * sizeof(string::value_type);
				memcpy(pCurr, &nLen, sizeof(nLen));
				pCurr += sizeof(nLen);

				memcpy(pCurr, it->second.c_str(), nLen);
				pCurr += nLen;
			}


			// ���ڴ�����У���md5
			char buffMD5[33];
			CMd5Algorithm md5;
			md5.Begin();
			md5.Append((uint8*)pContent, (uint)(pCurr - pContent));
			md5.GetMD5Base16Str(buffMD5);	
			memcpy(pMd5Self, buffMD5, 32);

			for (size_t i=0; i<nByteLen; i++)
			{ 
				pBuffer[i] ^= 0xFF;	
			}

			FILE* f = _wfopen(sVerificationFileAbsPathName.c_str(), L"wb");
			if (f == NULL)
			{
				// �ļ��򲻿�����
				return;
			}

			fwrite(pBuffer,sizeof(char), nByteLen, f);
			fclose(f);
			delete[] pBuffer;
			pBuffer = NULL;
		}
		size_t LoadVerificationFile(wstring sVerificationFileAbsPathName)
		{

			FILE* f= _wfopen(sVerificationFileAbsPathName.c_str(), L"rb");
			if (f == NULL)
			{
				// �ļ��򲻿�����
				return 0;
			}

			fseek(f, 0, SEEK_END);
			size_t nByteLen = ftell(f);
			fseek(f, 0, SEEK_SET);

			char* pBuffer = new char[nByteLen];
			fread(pBuffer, sizeof(char), nByteLen, f);
			fclose(f);

			for (size_t i=0; i<nByteLen; i++)
			{ 
				pBuffer[i] ^= 0xFF;	
			}

			char* pCurr = pBuffer;

			//��������md5У����
			pCurr += 32;

			size_t nCount = 0;
			memcpy(&nCount, pCurr, sizeof(nCount));
			pCurr += sizeof(nCount);

			wchar_t chPathTemp[MAX_PATH];
			char chMd5Temp[MAX_PATH];
			for (size_t i= 0; i<nCount; i++ )
			{
				size_t nLen = 0;
				memcpy(&nLen, pCurr, sizeof(nLen));
				pCurr += sizeof(nLen);

				memset(chPathTemp, 0, sizeof(chPathTemp));
				memcpy(chPathTemp, pCurr, nLen);
				pCurr += nLen;
				wstring strFileName(chPathTemp);

				memcpy(&nLen, pCurr, sizeof(nLen));
				pCurr += sizeof(nLen);

				memset(chMd5Temp, 0, sizeof(chMd5Temp));
				memcpy(chMd5Temp, pCurr, nLen);
				pCurr += nLen;
				string strMd5Code(chMd5Temp);

				m_mapFileToMd5[strFileName] = strMd5Code;
			}

			delete[] pBuffer;
			pBuffer = NULL;

			return m_mapFileToMd5.size();
		}


		std::wstring& tolower(std::wstring& str)
		{
			for (std::wstring::iterator i = str.begin(); i != str.end(); i++)
			{
				if (*i >= L'A' && *i <= L'Z')
					*i = (*i) + (L'a' - L'A');
			}
			return str;
		}


		wstring m_strRootDirectoryAbsPath;
		vector<wstring> m_vFileList;

		map<wstring, string> m_mapFileToMd5;


		vector<wstring> m_vErrorList;
		vector<wstring> m_vLostList;

		const size_t BUFF_LENGTH;

	};
}
