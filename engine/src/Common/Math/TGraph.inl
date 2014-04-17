#include "TGraph.h"
#include "CGraphPath.h"
#include <algorithm>


#define  NORMAL_CAPACITY  256
#define  MAX_DIST  65532 // �Ժ���Ҫ��Ϊ����

template<typename T>
TGraph<T>::TGraph()
{
}

template<typename T>
TGraph<T>::~TGraph()
{
}

template<typename T>
void TGraph<T>::AddEdge(int start, int end, T weight)
{
	NBver<T> ver;
	ver.v = end;
	ver.w = weight;
	if ( start >= static_cast<int>(m_graph.size()) )
		m_graph.push_back(NBvList());

	if ( static_cast<int>(m_graph.size()) - 1 < start )
		m_graph.resize(start);
	m_graph.at(start).push_back(ver);
}


template<typename T>
void TGraph<T>::Relax(int u, int v, T w)
{	
	if ( m_heap.at(v) > ( m_heap.at(u) + w ) )
	{
		m_heap.at(v) = m_heap.at(u) + w;
		m_parent.at(v) = u;
	}
}

void swap(int *x, int *y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

template<typename T>
void TGraph<T>::PrintPath(int source, int dest)
{
	int* path = new int[m_parent.size()];
	int j = 0;
	if ( m_parent.at(dest) == -1 )
		printf("no path from %d to %d exists!", source, dest);

	while ( dest != -1 )
	{
		path[j] = dest; // ��¼·���еĶ���
		dest = m_parent[dest];
		++j;
	}

	for ( j=j-1; j>=1; --j )
		printf(" %d-->",path[j]);
	printf("%d:",path[j]);

	delete[] path;
}

// ��ӡ�����еĴ�source��������������·��
template<typename T>
void TGraph<T>::PrintAllPath(int source)
{
	int i = 0;
	printf("the minimum paths figured out by dijkstra algorithm are follows:\n");
	for ( i=0; i<static_cast<int>(m_heap.size()); ++i )
	{
		PrintPath(source, i);
		cout<<m_heap.at(i)<<endl;
	}
}

// �������������·������Ҫ���յ����·��
template<typename T>
CGraphPath* TGraph<T>::GetMinPath()
{
	CGraphPath* path = new CGraphPath();
	CGraphPath::GraphaPath& pathMap = path->GetPathMap();

	int i = 0;
	int dest = 0;
	int j = 0;
	pathMap.resize(m_heap.size());
	for ( i=0; i<static_cast<int>(m_heap.size()); ++i )
	{
		int* path = new int[m_parent.size()];
		dest = i;
		j = 0;
		if ( m_parent.at(dest) == -1 )
			printf("no path from %d to %d exists!", m_start, dest);

		while ( dest != -1 )
		{
			path[j] = dest; // ��¼·���еĶ���
			dest = m_parent[dest];
			++j;
		}

		for ( j=j-1; j>=0; --j )
			pathMap.at(i).push_back(path[j]);

		delete[] path;
	}

	return path;
}


// ������iΪ����������Ϊ��С��
template<typename T>
void TGraph<T>::MinHeapify(MathVector<int>& indexList, MathVector<T>& heap, int i)
{
	int right = ( i + 1 ) << 1; // i�ڵ�������ӽڵ�
	int left = right - 1;
	int smallest = 0;

	if ( left < m_heapSize && heap.at(indexList.at(left)) < heap.at(indexList.at(i)) )
		smallest = left;
	else
		smallest = i;

	if ( right < m_heapSize && heap.at(indexList.at(right)) < heap.at(indexList.at(smallest)) )
		smallest = right;

	if ( smallest != i ) // ��ǰ�ڵ�������ӽڵ�����ӽڵ����Ϲ���
	{
		swap(&indexList.at(i), &indexList.at(smallest));
		MinHeapify(indexList, heap, smallest); // ��ԭ�������ӽڵ���Ϊ�����ݹ�
	}
}

template<typename T>
int TGraph<T>::PopHeap(MathVector<int>& indexList, MathVector<T>& heap)
{
	int min = 0;
	min = indexList.at(0);
	indexList.at(0) = indexList.back(); // indexList.at(m_heapSize-1);
	indexList.pop_back();
	--m_heapSize;
	MinHeapify(indexList, heap, 0);
	return min;
}

template<typename T>
void TGraph<T>::SortHeap(MathVector<int>& indexList, MathVector<T>& heap)
{
	int i = m_heapSize / 2;
	for ( ; i>=0; --i ) // ����ÿһ����Ҷ�ӽ��Ϊ������
		MinHeapify(indexList, heap, i);
}

template<typename T>
void TGraph<T>::Dijkstra(int start)
{
	m_start = start;
	MathVector<int> UList; // δѰ�����·����
	int vertexNum = static_cast<int>(m_graph.size());
	int i = 0;
	for ( i=0; i<vertexNum; ++i )
	{
		UList.push_back(i);
		m_heap.push_back(MAX_DIST);
		m_parent.push_back(-1);
	}
	m_heap.at(start) = 0;
	m_heapSize = static_cast<int>(m_heap.size());

	SortHeap(UList, m_heap);

	int current = 0;
	while ( m_heapSize > 0 )
	{
		current = PopHeap(UList, m_heap);
		NBvList& neighbors = m_graph.at(current);
		typename NBvList::iterator it = neighbors.begin();
		for ( ; it != neighbors.end(); ++it )
			Relax(current, it->v, it->w);

		SortHeap(UList, m_heap);
	}
}

