/*
 * =====================================================================================
 *
 *       Filename:  ArgType.h
 *
 *    Description:  ������������
 *
 *        Version:  1.0
 *        Created:  2008��08��03�� 18ʱ51��30��
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ����� (Comet), liaoxinwei@linekong.com
 *        Company:  ThreeOGCMan
 *
 * =====================================================================================
 */

#pragma once

#include "TagsDef.h"

namespace sqr
{
/*-----------------------------------------------------------------------------
 *  Arg Type Generator
 *-----------------------------------------------------------------------------*/
template < class T, class Tag = TagValue>
class ArgType
{
public:
	typedef T type;
};

template <class T>
class ArgType<T, TagRef>
{
public:
	typedef T& type;
};

template <class T>
class ArgType<T, TagConstRef>
{
public:
	typedef const T& type;
};

template <class T>
class ArgType<T, TagPtr>
{
public:
	typedef T* type;
};

template <class T>
class ArgType<T, TagConstPtr>
{
public:
	typedef const T* type;
};
}
