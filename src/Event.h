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

#ifndef  __JT_EVENT_H__
#define __JT_EVENT_H__

#include "macro.h"
#include "Action.h"
#include "Func.h"
#include "IDisconnectable.h"
#include "EventTarget.h"
#include <vector>
#include <cstddef>

#ifndef EVENT_SLOT
/*
 * 事件绑定方法
 *
 * @param func 函数名
 * @param paramType 函数参数类型
 */
#define EVENT_SLOT(func, paramType) (JT::Event<paramType>::TargetFunc)(&func)
#define EVENT_TEMPLATE_SLOT(func, paramType) (typename JT::Event<paramType>::TargetFunc)(&func)
#endif

NS_JT_BEGIN

template<typename TargetType, typename ParamType> class EventAction;

/*
 * 事件，仿函数
 *
 * @typename ParamType 参数类型
 */
template<typename ParamType> class Event : public EventTarget, public IDisconnectable
{
public:
	typedef void (EventTarget::*TargetFunc)(ParamType);
	typedef Action<EventTarget, ParamType> ActionType;
	typedef EventAction<EventTarget, ParamType> EventActionType;
	typedef std::vector<ActionType*> ActionsVector;

	Event(void){}
	virtual ~Event(void){ disconnectAll(); }

	virtual inline void operator()(ParamType p)
	{
		if (m_actions.size() <= 0)
		{
			return;
		}

		/* 防止循环过程中对m_actions做改变 */
		ActionsVector loopActions(m_actions);
		for (typename ActionsVector::iterator iter=loopActions.begin(); iter!=loopActions.end(); ++iter)
		{
			(**iter)(p);
		}
	}

	/*
	 * 注册事件
	 * 
	 * @param T* 事件监听类
	 * @param TargetFunc   事件触发函数
	 * 
	 * @example
	 *     trigger->onDone.connect(target, EVENT_SLOT(Target::onDone, EventTrigger*));
	 */
	template<typename T> Event& connect(T* t, TargetFunc tf)
	{
		EventTarget* target = dynamic_cast<EventTarget*>(t);
		JTASSERT(target, "target is not a sub class of EventTarget");

		ActionType* at = new ActionType(target, (void*)t, tf);
		return connect(at);
	}

	template<typename T> Event& connect(T* t, Event<ParamType>& event)
	{
		EventTarget* target = dynamic_cast<EventTarget*>(t);
		JTASSERT(target, "target is not a sub class of EventTarget");

		EventActionType* eat = new EventActionType(target, &event);
		return connect(eat);
	}

	inline Event& connect(ActionType* a)
	{
		a->getTarget()->addConnection(this);
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
			ActionType* a = dynamic_cast<ActionType*>(*iter);
			if (a == NULL) { ++iter; continue; }
			if ( ! a->isTarget(t)) { ++iter; continue; }

			a->getTarget()->removeConnection(this);
			delete *iter;

			iter = m_actions.erase(iter);
		}

		return *this;
	}

	virtual void disconnectAll(void) 
	{
		for (typename ActionsVector::iterator iter=m_actions.begin(); iter!=m_actions.end(); )
		{
			ActionType* a = dynamic_cast<ActionType*>(*iter);
			if (a == NULL) { ++iter; continue; }

			a->getTarget()->removeConnection(this);
			delete *iter;

			iter = m_actions.erase(iter);
		}

		m_actions.clear();
	}

protected:
	ActionsVector m_actions;
};

/*
 * 封装无返回值, 带一个参数的类函数成员指针
 */
template<typename TargetType, typename ParamType> class EventAction : public Action<TargetType, ParamType>
{
public:
	typedef void (EventTarget::*TargetFunc)(ParamType);
	/*
	 * 构造函数
	 *
	 * @param TargetType* 类的实例指针
	 * @param void*       类的指针的原始地址，防止原始指针在类型转换的过程中发生偏移
	 * @param TargetFunc  成员函数指针
	 */
	EventAction(TargetType* t, Event<ParamType>* e)
		: Action<TargetType, ParamType>(t, (void*)t, NULL), m_event(e)
	{
	}

	virtual ~EventAction(void)
	{}

	virtual inline void operator()(ParamType param) const
	{
		(*m_event)(param);
	}

protected:
	Event<ParamType>* m_event;
};

NS_JT_END

#endif /* __JT_EVENT_H__ */