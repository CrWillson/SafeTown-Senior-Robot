#include "menu_line.hpp"

//--------------------------------------------------------------
// Text Menu Line
//--------------------------------------------------------------

std::string TextMenuLine::getText(bool selected) const
{
    std::string pre = (selected) ? "> " : "  ";
    return pre + label;
}


//--------------------------------------------------------------
// Button Menu Line
//--------------------------------------------------------------

std::string ButtonMenuLine::getText(bool selected) const
{
    std::string pre = (selected) ? "> " : "  ";
    return pre + label;
}

void ButtonMenuLine::onSelect()
{
    Serial.println("Running line action");
    this->action();
}

//--------------------------------------------------------------
// Value Menu Line
//--------------------------------------------------------------

std::string ValueMenuLine::getText(bool selected) const
{
    std::string pre = (selected) ? "> " : "  ";
    return pre + label + " " + value;
}


//--------------------------------------------------------------
// SLider Menu Line
//--------------------------------------------------------------

std::string SliderMenuLine::getText(bool selected) const
{
    std::string pre = (selected) ? ">" : " ";
    std::string edit = (editing) ? ">" : " ";
    return pre + edit + label + " " + std::to_string(value);
}
