JTEvent
=======

Usage
-------------

<pre><code>
#include <iostream>
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

	void onNumberChangedSlot(int newVal)
	{
		std::cout << "Slider::onNumberChangedSlot: " << newVal << std::endl;
		setValue(newVal);
	}

	JT::Event<int> onChange;

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

	void onSliderChangedSlot(int newVal)
	{
		std::cout << "Number::onSliderChangedSlot: " << newVal << std::endl;
		setValue(newVal);
	}

	JT::Event<int> onChange;

private:
	int m_val;
};

int main() {
	//coming soon...

	return 0;
}

</code></pre>