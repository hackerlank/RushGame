#pragma once
#include "CFilterNode.h"

namespace sqr
{
	//�Ժ���Կ���д��ģ��
	class CRenderList
	{
	public:
		CRenderList(void);
		~CRenderList(void);

		typedef MapList::iterator	Key;
		typedef NodeList::iterator	iterator;
		
		bool	insert(FilterNode* pNode);
		void	erase(const FilterNode* pNode);
		//void	erase()
		
		iterator begin(void);
		iterator end(void);

		bool	empty(void) const;
		void	clear(void);
	protected:
		NodeList	TraverseList; 
		MapList		FindList;
	};
}