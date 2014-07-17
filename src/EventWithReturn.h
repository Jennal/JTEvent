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

#ifndef  __JT_EVENT_WITH_RETURN_H__
#define __JT_EVENT_WITH_RETURN_H__

#include "macro.h"
#include "Func.h"
#include "IDisconnectable.h"
#include "EventTarget.h"
#include <vector>

#ifndef EVENT_WITH_RETURN_SLOT
/*
 * 事件绑定方法
 *
 * @param func 函数名
 * @param paramType 函数参数类型
 */
#define EVENT_WITH_RETURN_SLOT(func, paramType) (JT::EventWithReturn<paramType>::TargetFunc)(&func)
#define EVENT_WITH_RETURN_TEMPLATE_SLOT(func, paramType) (typename JT::EventWithReturn<paramType>::TargetFunc)(&func)
#endif

NS_JT_BEGIN

/*
 * 事件，仿函数
 *
 * @typename ParamType  参数类型
 */
template<typename ParamType> class EventWithReturn : public EventTarget, public IDisconnectable
{
public:
	typedef ParamType (EventTarget::*TargetFunc)(ParamType);
	typedef Func<EventTarget, ParamType, ParamType> ActionType;
	typedef std::vector<ActionType> ActionsVector;

	EventWithReturn(void){}
	virtual ~EventWithReturn(void) { disconnectAll(); }

	virtual inline ParamType operator()(ParamType p)
	{
		if (m_actions.size() <= 0)
		{
			return p;
		}

		/* 防止循环过程中对m_actions做改变 */
		ActionsVector loopActions(m_actions);
		for (typename ActionsVector::iterator iter=loopActions.begin(); iter!=loopActions.end(); ++iter)
		{
			p = (*iter)(p);
		}

		return p;
	}

	/*
	 * 注册事件
	 * 
	 * @param T* 事件监听类
	 * @param TargetFunc   事件触发函数
	 * 
	 * @example
	 *     trigger->onDone.connect(target, EVENT_WITH_RETURN_SLOT(Target::onDone, EventTrigger*));
	 */
	template<typename T> EventWithReturn& connect(T* t, TargetFunc tf)
	{
		EventTarget* target = dynamic_cast<EventTarget*>(t);
		JTASSERT(target, "target is not a sub class of EventTarget");

		ActionType at(target, (void*)t, tf);
		return connect(at);
	}

	inline EventWithReturn& connect(EventWithReturn<ParamType>& event)
	{
		return connect(&event, EVENT_WITH_RETURN_SLOT(EventWithReturn<ParamType>::operator(), ParamType));
	}

	inline EventWithReturn& connect(ActionType& a)
	{
		a.getTarget()->addConnection(this);
		m_actions.push_back(a);
		return *this;
	}

	/*
	 * 反注册事件
	 *
	 * @param EventTarget* 事件监听类
	 */
	virtual IDisconnectable& disconnect(EventTarget* t)
	{
		for (typename ActionsVector::iterator iter=m_actions.begin(); iter!=m_actions.end(); )
		{
			ActionType* a = dynamic_cast<ActionType*>(&(*iter));
			if (a == NULL) { ++iter; continue; }
			if ( ! a->isTarget(t)) { ++iter; continue; }

			a->getTarget()->removeConnection(this);
			iter = m_actions.erase(iter);
		}

		return *this;
	}

	virtual void disconnectAll(void) 
	{
		for (typename ActionsVector::iterator iter=m_actions.begin(); iter!=m_actions.end(); )
		{
			ActionType* a = dynamic_cast<ActionType*>(&(*iter));
			if (a == NULL) { ++iter; continue; }

			a->getTarget()->removeConnection(this);
			iter = m_actions.erase(iter);
		}

		m_actions.clear();
	}

protected:
	ActionsVector m_actions;
};

NS_JT_END

#endif /* __JT_EVENT_WITH_RETURN_H__ */