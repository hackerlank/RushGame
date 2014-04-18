#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include <boost/static_assert.hpp>
#include <boost/array.hpp>
#include <boost/operators.hpp>

#include "MathHelper.hpp"

namespace sqr
{
	template <typename T, int N>
	class Vector_T 
		: boost::addable<Vector_T<T, N>
		, boost::subtractable<Vector_T<T, N>
		, boost::multipliable<Vector_T<T, N>
		, boost::dividable<Vector_T<T, N>
		, boost::dividable2<Vector_T<T, N>, T
		, boost::multipliable2<Vector_T<T, N>, T> > > > > >
	{
		template <typename U, int M>
		friend class Vector_T;

		typedef boost::array<T, N>	DetailType;

	public:
		typedef typename DetailType::value_type			value_type;

		typedef value_type*								pointer;
		typedef value_type const *						const_pointer;

		typedef typename DetailType::reference			reference;
		typedef typename DetailType::const_reference	const_reference;

		typedef typename DetailType::iterator			iterator;
		typedef typename DetailType::const_iterator		const_iterator;

		typedef typename DetailType::size_type			size_type;
		typedef typename DetailType::difference_type	difference_type;

		enum { elem_num = N };

	public:
		Vector_T()
		{
		}
		explicit Vector_T(T const * rhs)
		{
			detail::vector_helper<T, N>::DoCopy(&vec_[0], rhs);
		}
		explicit Vector_T(T const & rhs)
		{
			detail::vector_helper<T, N>::DoAssign(&vec_[0], rhs);
		}
		Vector_T(Vector_T const & rhs)
		{
			detail::vector_helper<T, N>::DoCopy(&vec_[0], &rhs[0]);
		}
		template <typename U, int M>
		Vector_T(Vector_T<U, M> const & rhs)
		{
			BOOST_STATIC_ASSERT(M >= N);

			detail::vector_helper<T, N>::DoCopy(&vec_[0], &rhs[0]);
		}

		Vector_T(T const & x, T const & y)
		{
			BOOST_STATIC_ASSERT(2 == elem_num);

			this->x() = x;
			this->y() = y;
		}
		Vector_T(T const & x, T const & y, T const & z)
		{
			BOOST_STATIC_ASSERT(3 == elem_num);

			this->x() = x;
			this->y() = y;
			this->z() = z;
		}
		Vector_T(T const & x, T const & y, T const & z, T const & w)
		{
			BOOST_STATIC_ASSERT(4 == elem_num);

			this->x() = x;
			this->y() = y;
			this->z() = z;
			this->w() = w;
		}

		static size_t size()
		{
			return elem_num;
		}

		static Vector_T const & Zero()
		{
			static Vector_T<T, N> zero(T(0));
			return zero;
		}

		// ȡ����
		iterator begin()
		{
			return vec_.begin();
		}
		const_iterator begin() const
		{
			return vec_.begin();
		}
		iterator end()
		{
			return vec_.end();
		}
		const_iterator end() const
		{
			return vec_.end();
		}
		reference operator[](size_t index)
		{
			return vec_[index];
		}
		const_reference operator[](size_t index) const
		{
			return vec_[index];
		}

		reference x()
		{
			BOOST_STATIC_ASSERT(elem_num >= 1);
			return vec_[0];
		}
		const_reference x() const
		{
			BOOST_STATIC_ASSERT(elem_num >= 1);
			return vec_[0];
		}

		reference y()
		{
			BOOST_STATIC_ASSERT(elem_num >= 2);
			return vec_[1];
		}
		const_reference y() const
		{
			BOOST_STATIC_ASSERT(elem_num >= 2);
			return vec_[1];
		}

		reference z()
		{
			BOOST_STATIC_ASSERT(elem_num >= 3);
			return vec_[2];
		}
		const_reference z() const
		{
			BOOST_STATIC_ASSERT(elem_num >= 3);
			return vec_[2];
		}

		reference w()
		{
			BOOST_STATIC_ASSERT(elem_num >= 4);
			return vec_[3];
		}
		const_reference w() const
		{
			BOOST_STATIC_ASSERT(elem_num >= 4);
			return vec_[3];
		}

		// ��ֵ������
		template <typename U>
		Vector_T const & operator+=(Vector_T<U, N> const & rhs)
		{
			detail::vector_helper<T, N>::DoAdd(&vec_[0], &vec_[0], &rhs.vec_[0]);
			return *this;
		}
		template <typename U>
		Vector_T const & operator-=(Vector_T<U, N> const & rhs)
		{
			detail::vector_helper<T, N>::DoSub(&vec_[0], &vec_[0], &rhs.vec_[0]);
			return *this;
		}
		template <typename U>
		Vector_T const & operator*=(Vector_T<U, N> const & rhs)
		{
			detail::vector_helper<T, N>::DoMul(&vec_[0], &vec_[0], &rhs[0]);
			return *this;
		}
		template <typename U>
		Vector_T const & operator*=(U const & rhs)
		{
			detail::vector_helper<T, N>::DoScale(&vec_[0], &vec_[0], rhs);
			return *this;
		}
		template <typename U>
		Vector_T const & operator/=(Vector_T<U, N> const & rhs)
		{
			detail::vector_helper<T, N>::DoDiv(&vec_[0], &vec_[0], &rhs[0]);
			return *this;
		}
		template <typename U>
		Vector_T const & operator/=(U const & rhs)
		{
			return this->operator*=(1.0f / rhs);
		}

		Vector_T& operator=(Vector_T const & rhs)
		{
			if (this != &rhs)
			{
				vec_ = rhs.vec_;
			}
			return *this;
		}
		template <typename U, int M>
		Vector_T& operator=(Vector_T<U, M> const & rhs)
		{
			BOOST_STATIC_ASSERT(M >= N);

			detail::vector_helper<T, N>::DoCopy(&vec_[0], &rhs.vec_[0]);
			return *this;
		}

		// һԪ������
		Vector_T const operator+() const
		{ return *this; }
		Vector_T const operator-() const
		{
			Vector_T temp(*this);
			detail::vector_helper<T, N>::DoNegate(&temp.vec_[0], &vec_[0]);
			return temp;
		}

		void swap(Vector_T& rhs)
		{
			detail::vector_helper<T, N>::DoSwap(&vec_[0], &rhs.vec_[0]);
		}

		friend bool
			operator==(Vector_T const & lhs, Vector_T const & rhs)
		{
			return detail::vector_helper<T, N>::DoEqual(&lhs[0], &rhs[0]);
		}

	private:
		DetailType vec_;
	};

	template <typename T, int N>
	inline bool
		operator!=(Vector_T<T, N> const & lhs, Vector_T<T, N> const & rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T, int N>
	inline void swap(Vector_T<T, N>& lhs, Vector_T<T, N>& rhs)
	{
		lhs.swap(rhs);
	}

	typedef Vector_T<float, 1> float1;
	typedef Vector_T<float, 2> float2;
	typedef Vector_T<float, 3> float3;
	typedef Vector_T<float, 4> float4;
}

#endif			// _VECTOR_HPP