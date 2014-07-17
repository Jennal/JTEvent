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

#ifndef  __JT_EVENT_TARGET_H__
#define __JT_EVENT_TARGET_H__

#include "macro.h"
#include <set>
#include "IDisconnectable.h"

NS_JT_BEGIN

/*
 * 事件监听类基类，用于在析构时自动移除已注册的事件
 */
__abstract class EventTarget {
public:
	EventTarget(){}
	virtual ~EventTarget() { removeFromAllConnectedEvents(); }

	virtual void addConnection(IDisconnectable* a) 
	{
		m_connectedEvents.insert(a);
	}

	virtual void removeConnection(IDisconnectable* a) 
	{
		m_connectedEvents.erase(a);
	}

	virtual void disconnectTo(IDisconnectable* a)
	{
		a->disconnect(this);
	}

	virtual void removeFromAllConnectedEvents(void) 
	{
		for (EventSet::iterator iter=m_connectedEvents.begin(); iter!=m_connectedEvents.end();)
		{
			IDisconnectable* e = *iter;
			++iter;

			e->disconnect(this);
		}
	}

	inline int getConnectedEventCount(void)
	{
		return m_connectedEvents.size();
	}

protected:
	typedef std::set<IDisconnectable*> EventSet;

	EventSet m_connectedEvents;
};

NS_JT_END

#endif /* __JT_EVENT_TARGET_H__ */