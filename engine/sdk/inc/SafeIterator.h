#pragma once

namespace sqr
{
	/// @brief �Ƚϵ�����ʱ��Ư�Ʒ���
	enum EIteratorCompareShift
	{
		EIteratorCompareShift_None,	///< �����κη�ʽ�ƶ��Ƚϣ������κ�һ����Ч�򲻵�
		EIteratorCompareShift_Raw,	///< ����ԭʼ�ķ�ʽ�Ƚ�
		EIteratorCompareShift_Add,	///< �����ƶ��Ƚϣ������operator++
		EIteratorCompareShift_Sub,	///< �����ƶ��Ƚϣ������operator--
	};
	/// @brief ȷ��ָ����Чʱ�ĵ�������
	enum EIteratorCheckShift
	{
		EIteratorCheckShift_None,	///< �����κε���
		EIteratorCheckShift_Add,	///< ���ҵ����������operator++
		EIteratorCheckShift_Sub,	///< ��������������operator--
	};

	/// @brief ˳��ȫ��������ʵ��
	template<typename TSafeContainer>
	class CSafeIterator
	{
	protected:
		typedef TSafeContainer									host_type;
		typedef CSafeIterator<host_type>						this_type;
		typedef typename host_type::base_iterator				base_iterator;
		typedef this_type iterator;
	public:
		typedef bidirectional_iterator_tag						iterator_category;
		typedef typename base_iterator::value_type				value_type;
		typedef typename base_iterator::difference_type			difference_type;
		typedef typename base_iterator::pointer					pointer;
		typedef typename base_iterator::reference				reference;
	private:
		base_iterator	m_baseIt;
		host_type&		m_parent;
	public:
		CSafeIterator(const base_iterator& it, host_type& parent);
		CSafeIterator(const iterator& other);
		~CSafeIterator();
		base_iterator get_base_iterator() const;
		bool is_valid()const;
		iterator& check_valid(EIteratorCheckShift shift);
		iterator& operator=(const iterator& other);
		iterator& operator++(); // preincrement
		iterator operator++(int); // postdecrement
		iterator& operator--(); // predecrement
		iterator operator--(int); // postdecrement
		bool operator!=(const iterator& other)const;
		bool operator==(const iterator& other)const;
		bool equal(const iterator& other, EIteratorCompareShift shift)const;
		pointer operator->()const;
		reference operator*()const;
	};

	/// @brief ���ü����ļ�ά��
	class CSafeIteratorRefCount
	{
	private:
		int m_referenceCount;
	protected:
		CSafeIteratorRefCount();
		~CSafeIteratorRefCount();
		void add_ref();
		void sub_ref();
		int get_ref()const;
		void reset_ref();
	};

}
