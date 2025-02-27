#include "menu_line.hpp"

//--------------------------------------------------------------
// Spacer Menu Line
//--------------------------------------------------------------

std::string SpacerMenuLine::getText(bool selected) const
{
    std::string pre = (selected) ? "> " : "  ";
    return pre + label;
}

//--------------------------------------------------------------
// Text Menu Line
//--------------------------------------------------------------

std::string TextMenuLine::getText(bool selected) const
{
    std::string pre = (selected) ? "> " : "  ";
    std::string displayLabel = label;
    if (displayLabel.length() > 19) {
        displayLabel = displayLabel.substr(0, 16) + "...";
    }
    return pre + displayLabel;
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
