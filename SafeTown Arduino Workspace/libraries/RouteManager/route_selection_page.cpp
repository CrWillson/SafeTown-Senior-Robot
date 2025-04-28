#include "route_selection_page.hpp"
#include "route_manager.hpp"

RouteSelectPage::RouteSelectPage(const std::string &lbl, const std::string &parentLbl)
    : MenuPage(lbl), parentMenuLbl(parentLbl) 
{
        
}

void RouteSelectPage::onPageLoad()
{
    addLine<TextMenuLine>("Route Selection");
    addLine<SpacerMenuLine>();

    auto& routeMan = RouteManager::getInstance();

    for (const auto& [label, route] : routeMan.getAllRoutes()) {
        
    }
}
