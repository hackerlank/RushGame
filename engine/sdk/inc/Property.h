/*
 * =====================================================================================
 *
 *       Filename:  Property.h
 *
 *    Description:  �ṩ����C#��Property�﷨����
 *
 *        Version:  1.0
 *        Created:  2008��08��03�� 18ʱ41��30��
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ����� (Comet), liaoxinwei@linekong.com
 *        Company:  ThreeOGCMan
 *
 * =====================================================================================
 */

#include "ArgType.h"

namespace sqr
{

/*
 * =====================================================================================
 *        Class:  Property
 *  Description:  Property����
 *  TemplateArg:  GetRetTypeTag:Get�������ͣ�ֵ�����õ� SetArgTypeTag:Set�����Ĳ������ͣ�ֵ�����õ� AccessTag:����Ȩ��
 * =====================================================================================
 */
template < class T, class AccessTag = TagReadWrite, class GetRetTypeTag = TagConstRef, class SetArgTypeTag = TagConstRef>
class Property
{
	typedef typename ArgType<T, SetArgTypeTag>::type SetArgType;
	typedef typename ArgType<T, GetRetTypeTag>::type GetRetType;
public:

	/* ====================  LIFECYCLE     ======================= */
	Property ()                             /* constructor */
	{
	}

	template <class T1>
	explicit Property(T1 value):value(value)
	{
	}


	/* ====================  ACCESS        ======================== */
	void Set(SetArgType value)
	{
		DoSet(value, AccessTag());
	}

	template <class T1>
	void Set(T1 value)
	{
		DoSet(value, AccessTag());
	}

	/* ====================  INQUIRY       ======================== */
	GetRetType Get() const
	{
		return DoGet(AccessTag());
	}

private:
	inline void DoSet(SetArgType value, TagWrite)
	{
		this->value = value;
	}

	template <class T1>
	inline void DoSet(T1 value, TagWrite)
	{
		this->value = value;
	}

	inline GetRetType DoGet(TagRead) const
	{
		return value;
	}
	T value;
}; /* ----------  end of template class  Property  ---------- */

//Get�����������͵�Property
template < class T, class AccessTag = TagReadWrite>
class RefProperty : public Property<T, AccessTag, TagRef, TagConstRef>
{
};

//Get����ֵ���͵�Property
template < class T, class AccessTag = TagReadWrite>
class ValueProperty : public Property<T, AccessTag, TagValue, TagValue>
{
};
}
