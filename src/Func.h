/************************************************************
The MIT License (MIT)

Copyright (c) 2014 Jennal (jennalcn@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************/

#ifndef  __JT_FUNC_H__
#define __JT_FUNC_H__

#include "macro.h"
#include "Action.h"
#include "EventTarget.h"

NS_JT_BEGIN

/*
 * 封装有返回值, 带一个参数的函数指针
 */
template<typename TargetType, typename ParamType, typename ReturnType> class Func
{
public:
	typedef ReturnType (EventTarget::*TargetFunc)(ParamType);

	/*
	 * 构造函数
	 *
	 * @param TargetType* 类的实例指针
	 * @param void*       类的指针的原始地址，防止原始指针在vs编译器做类型转换的过程中发生偏移
	 * @param TargetFunc  成员函数指针
	 */
	Func(TargetType* t, void* op, TargetFunc tf) : m_target(t), m_originPtr(op), m_targetFunc(tf) {}
	virtual ~Func(void){}

	virtual inline ReturnType operator()(ParamType p) const
	{
#ifdef _MSC_VER
		return (((TargetType*)m_originPtr)->*m_targetFunc)(p);
#else
		return (m_target->*m_targetFunc)(p);
#endif // _MSC_VER
		
	}

	virtual inline bool isTarget(const TargetType* t) const
	{
		return t == m_target;
	}

	virtual inline TargetType* getTarget(void) const
	{
		return m_target;
	}

protected:
	__weak_reference TargetFunc  m_targetFunc;
	__weak_reference TargetType* m_target;
	__weak_reference void*       m_originPtr;
};

NS_JT_END

#endif /* __JT_FUNC_H__ */