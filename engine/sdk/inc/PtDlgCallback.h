/*
 * =====================================================================================
 *
 *       Filename:  TPtDlgCallback.h
 *
 *    Description:  �ص�ģ�嶨��
 *
 *        Version:  1.0
 *        Created:  2008��08��04�� 09ʱ12��55��
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ����� (Comet), liaoxinwei@linekong.com
 *        Company:  ThreeOGCMan
 *
 * =====================================================================================
 */

#pragma once
#include "CRefObject.h"

namespace sqr
{

/*
 * =====================================================================================
 *        Class:  TPtDlgCallback
 *  Description:  TPtDlgCallback
 * =====================================================================================
 */
template < class RetType>
class TPtDlgCallback0 :public CRefObject
{
public:

	/* ====================  LIFECYCLE     ========================*/
	TPtDlgCallback0 () { }                 /* constructor */

	inline RetType operator()()
	{
		return this->exec();
	}
	virtual RetType exec() = 0;
protected:
	virtual ~TPtDlgCallback0() { }         /* destructor */
}; /* ----------  end of template class  Callback  ---------- */

template < class Param1, class RetType>
class TPtDlgCallback1 :public CRefObject
{
public:

	/* ====================  LIFECYCLE     ========================*/
	TPtDlgCallback1() { }                 /* constructor */

	inline RetType operator()(Param1 p1)
	{
		return this->exec(p1);
	}
	virtual RetType exec(Param1) = 0;
protected:
	virtual ~TPtDlgCallback1() { }         /* destructor */
}; /* ----------  end of template class  Callback  ---------- */

template < class Param1, class Param2, class RetType>
class TPtDlgCallback2 :public CRefObject
{
public:

	/* ====================  LIFECYCLE     ========================*/
	TPtDlgCallback2() { }                 /* constructor */

	inline RetType operator()(Param1 p1, Param2 p2)
	{
		return this->exec(p1, p2);
	}
	virtual RetType exec(Param1, Param2) = 0;
protected:
	virtual ~TPtDlgCallback2() { }         /* destructor */
}; /* ----------  end of template class  Callback  ---------- */

template < class Param1, class Param2, class Param3, class RetType>
class TPtDlgCallback3 :public CRefObject
{
public:

	/* ====================  LIFECYCLE     ========================*/
	TPtDlgCallback3() { }                 /* constructor */

	inline RetType operator()(Param1 p1, Param2 p2, Param3 p3)
	{
		return this->exec(p1, p2, p3);
	}
	virtual RetType exec(Param1, Param2, Param3) = 0;
protected:
	virtual ~TPtDlgCallback3() { }         /* destructor */
}; /* ----------  end of template class  Callback  ---------- */
template < class Param1, class Param2, class Param3, class Param4, class RetType>
class TPtDlgCallback4 :public CRefObject
{
public:

	/* ====================  LIFECYCLE     ========================*/
	TPtDlgCallback4() { }                 /* constructor */

	inline RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{
		return this->exec(p1, p2, p3, p4);
	}
	virtual RetType exec(Param1, Param2, Param3, Param4) = 0;
protected:
	virtual ~TPtDlgCallback4() { }         /* destructor */
}; /* ----------  end of template class  Callback  ---------- */
}
