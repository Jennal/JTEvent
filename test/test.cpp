#include <iostream>
#include "JTEvent.h"

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

class Slider : public Number {
public:
    Slider() {}
    ~Slider() {}

    void onNumberChangedSlot(int newVal)
    {
        std::cout << "Slider::onNumberChangedSlot: " << newVal << std::endl;
        setValue(newVal);
    }
};

int main() {
    Slider* slider = new Slider;
    Number* number = new Number;

    /* test connected events */
    slider->onChange.connect(number, EVENT_SLOT(Number::onSliderChangedSlot, int));
    number->onChange.connect(slider, EVENT_SLOT(Slider::onNumberChangedSlot, int));

    slider->setValue(10);
    std::cout << "slider value: " << slider->getValue() << std::endl;
    std::cout << "number value: " << number->getValue() << std::endl;

    number->setValue(20);
    std::cout << "slider value: " << slider->getValue() << std::endl;
    std::cout << "number value: " << number->getValue() << std::endl;

    /* automatic disconnect while EventTarget deleted */
    delete number;
    slider->setValue(10);

    delete slider;

    return 0;
}
