#include "stdafx.h"
#include "CRowTableFileKey.h"
#include "CXmlConfig.inl"
#include "TSqrAllocator.inl"
#include "CRowTableFileImp.h"
#include <algorithm>
#include "CMemPool.h"

const char* DEFUALT_KEY = "DefaultKey";
const char* RANDOM_KEY = "RandomKey";
const char* CHILD_INDEX = "Index";
const char* INDEX_KEY_NAME = "IndexKey";
const char* FIELD_COL = "Col";
const char* FIELD_NAME = "Name";
const char* FIELD_TYPE = "Type";

void* CRowTableFileKey::operator new(size_t stSize, CMemPool* pPool)
{
	return pPool->Alloc(stSize);
}

typedef pair<const char*, CRowTableFileKey*>	PairIndex2Key_t;

inline bool LessID(const PairIndex2Key_t& Left, const PairIndex2Key_t& Right)
{
	return (size_t)Left.first < (size_t)Right.first;
}

CRowTableFileTreeKey::CRowTableFileTreeKey(TiXmlNode* pNode, CRowTableFileImp* pImp, CMemPool* pPool)
: m_pImp(pImp)
{
	TiXmlNode* pChildNode = pNode->FirstChild(CHILD_INDEX);
	//����tree������˵���������ӽڵ㣬��Ȼ����
	Ast(pChildNode);	

	//��ȡ���ӽڵ��IndexKey Name
	const char* szKeyName = pChildNode->ToElement()->Attribute(INDEX_KEY_NAME);
	if(strcmp(szKeyName, DEFUALT_KEY) == 0)
	{
		m_eIndexKeyType = eTFIKT_Default;
	}
	else if(strcmp(szKeyName, RANDOM_KEY) == 0)
	{
		m_eIndexKeyType = eTFIKT_Random;
	}
	else
	{
		m_eIndexKeyType = eTFIKT_Name;
	}
	
	m_pVecIndex2Key = new (pPool->Alloc(sizeof(VecIndex2Key_t)))
		VecIndex2Key_t(pPool);

	switch(m_eIndexKeyType)
	{
	case eTFIKT_Name:
		{
			set<const char*>	setIndex;

			while (pChildNode)
			{
				CRowTableFileKey* pKey = CreateSubKey(pChildNode, pPool);

				szKeyName = pChildNode->ToElement()->Attribute(INDEX_KEY_NAME);
				const char* szIndex = m_pImp->GetIndexByName(szKeyName, m_pImp->GetXmlIndexPool(), true);
				bool bRet = setIndex.insert(szIndex).second;

				if (!bRet)
				{
					GenErr("XML�ļ���ʽ�����������ظ��ؼ��ֶ�����", szKeyName);
				}

				m_pVecIndex2Key->push_back(make_pair(szIndex, pKey));

				pChildNode = pChildNode->NextSibling();
			}

			sort(m_pVecIndex2Key->begin(), m_pVecIndex2Key->end(), LessID);
		}
		break;
	case eTFIKT_Default:
		{
			//�����Ĭ����������ôֻ������һ���ӽڵ�
			Ast(!pChildNode->NextSibling());

			//�����Ĭ����������ô����ӽڵ�һ����leaf�ڵ㣬��Ϊ����ֻ����������������Ĭ������
			CRowTableFileKey* pKey  = new (pPool)CRowTableFileLeafKey(pChildNode, m_pImp, pPool);
		
			const char* szIndex = m_pImp->GetIndexByName(szKeyName, m_pImp->GetXmlIndexPool(), true);
			m_pVecIndex2Key->push_back(make_pair(szIndex, pKey));
		}
		break;
	case eTFIKT_Random:
		{
			//�����������������ôֻ������һ���ӽڵ�
			Ast(!pChildNode->NextSibling());

			CRowTableFileKey* pKey = CreateSubKey(pChildNode, pPool);

			const char* szIndex = m_pImp->GetIndexByName(szKeyName, m_pImp->GetXmlIndexPool(), true);	
			m_pVecIndex2Key->push_back(make_pair(szIndex, pKey));
		}
		break;
	}
}

CRowTableFileTreeKey::~CRowTableFileTreeKey()
{
	//���ǲ���Ҫȥ�ݹ����ɾ���ӽڵ㣬��Ϊ���е�key������һ���ڴ���������
	//���ǽ������֮���ͳһɾ��

}

CRowTableFileKey* CRowTableFileTreeKey::CreateSubKey(TiXmlNode* pNode, CMemPool* pPool)
{
	//���Ȳ鿴����ӽڵ��ǲ���tree���͵Ľڵ�
	TiXmlNode* pChildNode = pNode->FirstChild(CHILD_INDEX);
	CRowTableFileKey* pKey = NULL;
	if (pChildNode)
	{
		pKey = new (pPool)CRowTableFileTreeKey(pNode, m_pImp, pPool);
	}
	else
	{
		pKey = new (pPool)CRowTableFileLeafKey(pNode, m_pImp, pPool);
	}

	return pKey;
}

bool CRowTableFileTreeKey::Equal(const CRowTableFileKey* pOther)
{
	const CRowTableFileTreeKey* pKey = dynamic_cast<const CRowTableFileTreeKey*>(pOther);
	if (!pKey)
	{
		return false;
	}

	if (m_eIndexKeyType != pKey->m_eIndexKeyType)
	{
		return false;
	}

	size_t stSize = m_pVecIndex2Key->size();

	if (stSize != pKey->m_pVecIndex2Key->size())
	{
		return false;
	}

	for (size_t i = 0; i < stSize; i++)
	{
		PairIndex2Key_t& PairData1 = m_pVecIndex2Key->at(i);
		PairIndex2Key_t& PairData2 = pKey->m_pVecIndex2Key->at(i);

		if (PairData1.first != PairData2.first)
		{
			return false;
		}

		bool bRet = PairData1.second->Equal(PairData2.second);
		if (!bRet)
		{
			return false;
		}

	}
	return true;

}

CRowTableFileKey* CRowTableFileTreeKey::GetSubKey(const char* szKeyName)
{
	if (m_eIndexKeyType == eTFIKT_Name)
	{
		const char* szIndex = m_pImp->GetIndexByName(szKeyName, m_pImp->GetXmlIndexPool(), true);
		VecIndex2Key_t::iterator iter = 
			lower_bound(m_pVecIndex2Key->begin(), m_pVecIndex2Key->end(), 
			PairIndex2Key_t(szIndex, NULL), LessID);
		if (iter != m_pVecIndex2Key->end())
		{
			if ((size_t)iter->first == (size_t)szIndex)
			{
				return iter->second;
			}
		}

		return NULL;
	}
	else
	{
		return m_pVecIndex2Key->at(0).second;
	}
}

CRowTableFileLeafKey::CRowTableFileLeafKey(TiXmlNode* pNode, CRowTableFileImp* pImp, CMemPool* pPool)
: m_Field(pNode, pImp, pPool)
{
		
}

CRowTableFileLeafKey::~CRowTableFileLeafKey()
{

}

bool CRowTableFileLeafKey::Equal(const CRowTableFileKey* pOther)
{
	const CRowTableFileLeafKey* pKey = dynamic_cast<const CRowTableFileLeafKey*>(pOther);
	if (!pKey)
	{
		return false;
	}

	return m_Field.Equal(pKey->m_Field);
}

CRowTableFileField::CRowTableFileField(TiXmlNode* pNode, CRowTableFileImp* pImp, CMemPool* pPool)
: m_vecFieldKey(pPool)
, m_vecFieldType(pPool)
, m_pImp(pImp)
{
	TiXmlNode* pChild = pNode->FirstChild(FIELD_COL);
	Ast(pChild);

	int32 nDataFieldNum = 0;
	while(pChild)
	{
		const char* szName = pChild->ToElement()->Attribute(FIELD_NAME);
		const char* szType = pChild->ToElement()->Attribute(FIELD_TYPE);
		
		Ast(szName);
		Ast(szType);

		m_vecFieldKey.push_back(m_pImp->GetIndexByName(szName, m_pImp->GetXmlIndexPool(), true));

		char cType = szType[0];
		if (cType == 'S' || cType == 's')
		{
			m_vecFieldType.push_back(eTFFT_String);
			nDataFieldNum++;
		}
		else if(cType == 'N' || cType == 'n')
		{
			m_vecFieldType.push_back(eTFFT_Number);
			nDataFieldNum++;
		}
		else if(cType == 'E' || cType == 'e')
		{
			m_vecFieldType.push_back(eTFFT_Empty);
		}
		else
		{
			ostringstream strm;
			strm << "Field Name: " << szName << " Field Type: " << szType << endl;
			GenErr("XML�����ļ����󣬲���ȷ������type", strm.str().c_str());
		}

		pChild = pChild->NextSibling();
	}

	if (nDataFieldNum == 0)
	{
		m_LeafNodeType = eRTNT_LeafNoData;
	}
	else if(nDataFieldNum == 1)
	{
		m_LeafNodeType = eRTNT_LeafOneData;
	}
	else
	{
		m_LeafNodeType = eRTNT_LeafMoreData;
	}

	m_nDataNum = nDataFieldNum;
}

///////////////////////////////////////////////////////

bool CRowTableFileField::Equal(const CRowTableFileField& Other)
{
	if (m_LeafNodeType != Other.m_LeafNodeType)
	{
		return false;
	}

	size_t stSize = m_vecFieldKey.size();

	if (stSize != Other.m_vecFieldKey.size())
	{
		return false;
	}

	for (size_t i = 0; i < stSize; i++)
	{
		if (m_vecFieldKey[i] != Other.m_vecFieldKey[i])
		{
			return false;
		}

		if (m_vecFieldType[i] != Other.m_vecFieldType[i])
		{
			return false;
		}
	}

	return true;
}


