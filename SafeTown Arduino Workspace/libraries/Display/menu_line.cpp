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


//--------------------------------------------------------------
// Value Menu Line
//--------------------------------------------------------------

std::string ValueMenuLine::getText(bool selected) const
{
    std::string pre = (selected) ? "> " : "  ";
    return pre + label + " " + value;
}
