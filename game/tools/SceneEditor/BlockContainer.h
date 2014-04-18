#pragma once
#include "stdafx.h"
#include "DataType.h"
#include "MyDefine.h"


//��������ֿ������ݵ�����, (���� �� ������ ��ʹ�÷����� STL��������)
//�뾡��ʹ��ָ������ʵ��������ģ��, (���ⲻ��Ҫ�����ݸ��ƿ���,����������Ĳ����ٶ�)
template <typename DataType>
class TBlockContainer
{
public:
	typedef pair <int, int>										BlockMap_Key;
	typedef pair <float, float>									DataMap_Key;
	typedef map<DataMap_Key, DataType>							Data_Map;
	typedef map<BlockMap_Key, Data_Map*>						Block_Map;
	typedef typename Data_Map::iterator							DataMap_Iter;
	typedef typename Block_Map::iterator						BlockMap_Iter;
	
	template <typename> class _iterator;
	typedef	_iterator<DataType>									iterator;
	friend iterator;



	template <typename DataType>//��Ȼ����������� DataType �� ����� DataType ����ͬһ����, 
								//��ʹ��ʱ��ȴ��ͬһ����,����ȡ�˸���ͬ������,���˼ά��ת�鷳
	class _iterator
	{
	public:
		typedef	typename TBlockContainer<DataType>				Owner_Type;
		typedef _iterator<DataType>								My_Type;
		typedef	typename Owner_Type::BlockMap_Iter					BlockMap_Iter;
		typedef typename Owner_Type::DataMap_Iter					DataMap_Iter;

		friend Owner_Type;

		_iterator(){}
		_iterator(const Owner_Type* pOwner, const BlockMap_Iter& blockIter, const DataMap_Iter& dataIter)
		{
			m_curBlockIter = blockIter;
			m_curDataIter = dataIter;
			m_pOwnerContainer = pOwner;
		}
		~_iterator(){}

		My_Type& operator =(const My_Type& another)
		{
			m_curBlockIter = another.m_curBlockIter;
			m_curDataIter = another.m_curDataIter;
			m_pOwnerContainer = another.m_pOwnerContainer;
			return *this;
		}

		DataType& operator *()const
		{
			return m_curDataIter->second;
		}

		DataType* operator ->()const
		{
			return &(m_curDataIter->second);
		}
		
		//����ǰ׺++
		My_Type& operator ++()
		{
			if (isBlockMapEnd())
			{
				GenErr("iterator �Ѵ��� end λ��,�޷�ʹ�� ++");
			}
			m_curDataIter++;
			if (isDataMapEnd())
			{
				m_curBlockIter++;
				if (!isBlockMapEnd())
				{
					m_curDataIter = m_curBlockIter->second->begin();
				}
			}
			return *this;
		}

		//���غ�׺++
		My_Type operator ++(int)
		{
			My_Type old = *this;
			++(*this);
			return old;
		}

		bool operator ==(const My_Type& another) const
		{
			if ( m_curBlockIter == another.m_curBlockIter
				&& (isBlockMapEnd()	//MapIter Ϊendʱ����Ҫ�Ƚ�DataIter(��ʱ��������)
					|| m_curDataIter == another.m_curDataIter))
			{
				return true;
			}
			return false;
		}
		
		bool operator !=(const My_Type& another) const
		{
			return !(*this == another);
		}

		inline void GetBlockPos(OUT Pos& pos)
		{
			pos.x = m_curBlockIter->first.first;
			pos.y = m_curBlockIter->first.second;
		}

		inline void GetDataPos(OUT FPos& fPos)
		{
			fPos.x = m_curDataIter->first.first;
			fPos.y = m_curDataIter->first.second;
		}

	private:
		inline bool isBlockMapEnd() const
		{
			return m_curBlockIter == m_pOwnerContainer->m_mapBlock.end();
		}
		inline bool isDataMapEnd() const
		{
			return m_curDataIter == m_curBlockIter->second->end();
		}
	private:
		BlockMap_Iter											m_curBlockIter;
		DataMap_Iter											m_curDataIter;
		const Owner_Type *										m_pOwnerContainer;
	};




	TBlockContainer(int nBlockSize = 1):m_nBlockSize(nBlockSize),m_nSize(0){}
	~TBlockContainer(void) { clear(); }



	bool insert(float fX, float fY,const DataType& Data)
	{
		if (find(fX,fY) != end())
		{
			return false;
		}
		int iX, iY;
		GetBlockPos(iX, iY, fX, fY);
		DataMap_Key	dataKey(fX, fY);
		BlockMap_Key blockKey(iX,iY);
		BlockMap_Iter blockIter = m_mapBlock.find(blockKey);
		if (blockIter == m_mapBlock.end())
		{
			Data_Map*  pDataMap = new Data_Map();
			pDataMap->insert(make_pair(dataKey, Data));
			m_mapBlock.insert(make_pair(blockKey, pDataMap));
		}
		else
		{
			Data_Map*  pDataMap = blockIter->second;
			pDataMap->insert(make_pair(dataKey, Data));
		}
		m_nSize++;
		return true;
	}


	//����fX,fY Ϊ���ʱ���ֵʱ�����ҵ�, �������ͨ����������õ���ֵһ�����Ҳ�����. ��ʹ�÷�Χ����
	iterator find(float fX, float fY)
	{
		int iX, iY;
		GetBlockPos(iX, iY, fX, fY);
		BlockMap_Iter blockIter = m_mapBlock.find(BlockMap_Key(iX,iY));
		if (blockIter == m_mapBlock.end())
		{
			return end();
		}
		else
		{
			DataMap_Iter dataIter = blockIter->second->find(DataMap_Key(fX,fY));
			if (dataIter == blockIter->second->end())
			{
				return end();
			}
			else
			{
				return iterator(this, blockIter, dataIter);
			}
		}
	}
	
	//�˲�����ʹ������ʧЧ
	bool erase(const iterator& iter)
	{
		if (iter.isBlockMapEnd())
		{
			GenErr("iterator �Ѵ��� end λ��,�޷�ɾ��");
		}
		iter.m_curBlockIter->second->erase(iter.m_curDataIter);
		if (iter.m_curBlockIter->second->size() == 0)
		{
			delete iter.m_curBlockIter->second;
			m_mapBlock.erase(iter.m_curBlockIter);
		}
		m_nSize --;
		return true;
	}

	void clear()
	{
		BlockMap_Iter blockIter = m_mapBlock.begin();
		while (blockIter != m_mapBlock.end())
		{
			delete blockIter->second;
			m_mapBlock.erase(blockIter);
			blockIter = m_mapBlock.begin();
		}
		m_nSize = 0;
	}

	iterator end()
	{
		return iterator(this, m_mapBlock.end(), DataMap_Iter());
	}
	iterator begin()
	{
		BlockMap_Iter blockIter = m_mapBlock.begin();
		if (blockIter == m_mapBlock.end())
		{
			return end();
		}
		else
		{
			return iterator(this, blockIter, blockIter->second->begin());
		}
	}
	
	//�˲�����ʹ������ʧЧ
	bool Move(const iterator& iter, float fDestX, float fDestY)
	{

		DataMap_Key pair = iter.m_curDataIter->first;
		float oldX = pair.first, oldY = pair.second;
		if(fDestX == oldX && fDestX == oldY)//Ŀ��λ��
		{
			return false;
		}
		if (find(fDestX,fDestY) != end())
		{
			return false;
		}
		DataType temp = *iter;
		if(erase(iter))
		{
			if(insert(fDestX, fDestY, temp))
			{
				return true;
			}
			else
			{
				Ast(insert(oldX, oldY, temp));//��ͼ�ָ�
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	//isClearVec �Ƿ�����vector
	//����ָ�������������ҵ���Ԫ�ظ���
	int GetElement(OUT vector<DataType>& vec,const FRect& frect, bool isClearVec = true)
	{
		COUT(TBlockContainer::GetElement)
		FRect rect = frect.Correct();
		if (isClearVec)
		{
			vec.clear();
		}
		int iStartX, iStartY, iEndX, iEndY;
		GetBlockPos(iStartX, iStartY, rect.left, rect.top);
		GetBlockPos(iEndX, iEndY, rect.right, rect.bottom);
		int nCount = 0;

		BlockMap_Iter	blockIter;
		BlockMap_Key	blockKey;
		DataMap_Iter	dataIter;
		DataMap_Iter	dataEndIter;
		DataMap_Key		dataKey;
		FPos			dataPos;
		int x, y;

		//����if-else ������������ѡ��
		if ((iEndX - iStartX) * (iEndY - iStartY) < (int)m_mapBlock.size())//������Χ��С,ͨ��������
		{
			for (x = iStartX; x <= iEndX; x++)
			{
				for (y = iStartY; y <= iEndY;y++)
				{
					blockKey.first = x;
					blockKey.second = y;
					blockIter = m_mapBlock.find(blockKey);
					if (blockIter != m_mapBlock.end())
					{
						dataIter = blockIter->second->begin();
						dataEndIter = blockIter->second->end();
						while (dataIter != dataEndIter)
						{
							dataPos.x = dataIter->first.first;
							dataPos.y = dataIter->first.second;
							//if (dataPos.isInRect(rect))
							if(rect.IsContainPos(dataPos))
							{
								vec.push_back(dataIter->second);
								nCount++;
							}
							dataIter++;
						}
					}
				}
			}
		}
		else //������Χ�ϴ�,ͨ��Map����
		{
			blockIter = m_mapBlock.begin();
			while (blockIter != m_mapBlock.end())
			{	
				x = blockIter->first.first;
				y = blockIter->first.second;
				if (x >= iStartX && x <= iEndX && y >= iStartY && y <= iEndY)
				{
					dataIter = blockIter->second->begin();
					dataEndIter = blockIter->second->end();
					while (dataIter != dataEndIter)
					{
						dataPos.x = dataIter->first.first;
						dataPos.y = dataIter->first.second;
						//if (dataPos.isInRect(rect))
						if(rect.IsContainPos(dataPos))
						{
							vec.push_back(dataIter->second);
							nCount++;
						}
						dataIter++;
					}
				}
				blockIter++;
			}
		}
		COUT(GetElement   end)
		return nCount;
	}
	
	inline int size() const {return m_nSize;}
	inline bool empty() const {return m_nSize == 0;}
	inline void GetBlockPos(OUT int& iX, OUT int& iY, float fX, float fY) const
	{
		iX = (int) (fX/m_nBlockSize);
		iY = (int) (fY/m_nBlockSize);
	}
private:
	Block_Map	m_mapBlock;					//���ݴ��MAP
	uint		m_nBlockSize;				//�ֿ��С
	int			m_nSize;					//���������ݸ���
};
