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

```cpp
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

	int getValue(void)
	{
		return m_val;
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

	int getValue(void)
	{
		return m_val;
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
	Silder* slider = new Slider;
	Number* number = new Number;

	/* test connected events */
	slider->onChange.connect(number, EVENT_SLOT(Number::onSliderChangedSlot, int));
	number->onChange.connect(slider, EVENT_SLOT(Slider::onSliderChangedSlot, int));

	slider->setValue(10);
	std::cout << "slider value: " << slider->getValue() << std::endl;
	std::cout << "number value: " << slider->getValue() << std::endl;

	number->setValue(20);
	std::cout << "slider value: " << slider->getValue() << std::endl;
	std::cout << "number value: " << slider->getValue() << std::endl;

	/* automatic disconnect while EventTarget deleted */
	delete number;
	slider->setValue(10);

	delete slider;

	return 0;
}

```