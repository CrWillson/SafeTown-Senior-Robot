#pragma once

#include <string>
#include "menu_page.hpp"
#include "menu_line.hpp"

class RouteLine : public MenuLine {
public:
    RouteLine(const std::string& text)
      : MenuLine(text) {}

    std::string getText(bool selected) const override;
    void onSelect() override;;
private:

    friend class MenuPage;
};


class RouteSelectPage : public MenuPage {
public:
    
    RouteSelectPage(const std::string& lbl, const std::string& parentLbl);
    ~RouteSelectPage() = default;

private:

    void onPageLoad() override;
    void onPageExit() override;

    std::string parentMenuLbl;
};