JTEvent
=======

Overview
-------------

Qt Event like, without Qt essential support.

Features
-------------

* Fast for coding, fast for performance
* Object method callback
* Strong type constraints
* Support for compilers as vs/g++/llvm/clang
* Automatic disconnect, no need to concern about disconnect

Usage
-------------

<pre><code>
#include &lt;iostream&gt;
#include "JTEvent.h"

class Slider : public JT::EventTarget {
public:
	Slider() : m_val(0) {}
	~Slider() {}

	void setValue(int newVal)
	{
		if (newVal == m_val) return;

		m_val = newVal;
		onChange(m_val);
	}

	int getValue(void)
	{
		return m_val;
	}

	void onNumberChangedSlot(int newVal)
	{
		std::cout &lt;&lt; "Slider::onNumberChangedSlot: " &lt;&lt; newVal &lt;&lt; std::endl;
		setValue(newVal);
	}

	JT::Event&lt;int&gt; onChange;

private:
	int m_val;
};

class Number : public JT::EventTarget {
public:
	Number() : m_val(0) {}
	~Number() {}

	void setValue(int newVal)
	{
		if (newVal == m_val) return;

		m_val = newVal;
		onChange(m_val);
	}

	int getValue(void)
	{
		return m_val;
	}

	void onSliderChangedSlot(int newVal)
	{
		std::cout &lt;&lt; "Number::onSliderChangedSlot: " &lt;&lt; newVal &lt;&lt; std::endl;
		setValue(newVal);
	}

	JT::Event&lt;int&gt; onChange;

private:
	int m_val;
};

int main() {
	Silder* slider = new Slider;
	Number* number = new Number;

	/* test connected events */
	slider-&gt;onChange.connect(number, EVENT_SLOT(Number::onSliderChangedSlot, int));
	number-&gt;onChange.connect(slider, EVENT_SLOT(Slider::onSliderChangedSlot, int));

	slider->setValue(10);
	std::cout &lt;&lt; "slider value: " &lt;&lt; slider-&gt;getValue() &lt;&lt; std::endl;
	std::cout &lt;&lt; "number value: " &lt;&lt; slider-&gt;getValue() &lt;&lt; std::endl;

	number->setValue(20);
	std::cout &lt;&lt; "slider value: " &lt;&lt; slider-&gt;getValue() &lt;&lt; std::endl;
	std::cout &lt;&lt; "number value: " &lt;&lt; slider-&gt;getValue() &lt;&lt; std::endl;

	/* automatic disconnect while EventTarget deleted */
	delete number;
	slider->setValue(10);

	delete slider;

	return 0;
}

</code></pre>