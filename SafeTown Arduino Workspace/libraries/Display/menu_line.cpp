#include "menu_line.hpp"

//--------------------------------------------------------------
// Text Menu Line
//--------------------------------------------------------------

std::string TextMenuLine::getText(bool selected) const
{
    return label;
}


//--------------------------------------------------------------
// Button Menu Line
//--------------------------------------------------------------

std::string ButtonMenuLine::getText(bool selected) const
{
    return label;
}


//--------------------------------------------------------------
// Value Menu Line
//--------------------------------------------------------------

std::string ValueMenuLine::getText(bool selected) const
{
    return std::string();
}

void ValueMenuLine::onEvent(const Event::ValueChangedEvent &e)
{
    if (e.valueId == valueLabel) {
        value = std::to_string(e.value);
        eventManager->publish(Event::PageChangedEvent{});
    }
}
