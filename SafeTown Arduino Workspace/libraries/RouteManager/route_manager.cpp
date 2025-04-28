#include "route_manager.hpp"
#include "debug_logger.hpp"
#include "event_manager.hpp"

void RouteManager::init(std::string routedir)
{
    _routeDir = routedir;

    _allRoutes[_defaultRouteLabel] = _defaultRoute;
    _currRoute = _allRoutes.find(_defaultRouteLabel);
    _currStep = 0;
    
    File dir = LittleFS.open(_routeDir.c_str(), "r");
    if (!dir || !dir.isDirectory()) {
        LOGLN("No routes directory found. Only default route loaded.");
        return;
    }

    File file = dir.openNextFile();
    while (file) {
        String filename = file.name();

        if (!file.isDirectory() && filename.endsWith(".tdr")) {
            String line = file.readStringUntil('\n');
            int sepIndex = line.indexOf(':');

            if (sepIndex > 0) {
                String name = line.substring(0, sepIndex);
                String steps = line.substring(sepIndex+1);

                route_t r;
                for (char c : steps) {
                    auto turn = charToDir(c);
                    if (turn) r.push_back(turn.value());
                }

                if (!r.empty()) {
                    _allRoutes[name.c_str()] = r;
                }
            }
        }
    }

}

RouteManager::DIR RouteManager::getCurrTurn() const
{
    return _currRoute->second.at(_currStep);
}

RouteManager::route_t RouteManager::getCurrentRoute() const
{
    return _currRoute->second;
}

std::optional<RouteManager::route_t> RouteManager::getRoute(std::string label) const
{
    if (auto search = _allRoutes.find(label); search != _allRoutes.end()) {
        return search->second;
    }
    return std::nullopt;
}

void RouteManager::incRouteStep()
{
    _currRouteStep++;
    if (_currRouteStep >= getCurrentRoute().size()) {
        _currRouteStep = 0;
    }

    publishCurrentStep();
}

void RouteManager::resetRoute()
{
    _currRouteStep = 0;
    publishCurrentStep();
}

bool RouteManager::setCurrentRoute(std::string label)
{
    if (_allRoutes.find(label) == _allRoutes.end()) {
        return false;
    }

    _currRouteLabel = label;
    _currRouteStep = 0;
    
    return true;
}

void RouteManager::publishCurrentStep()
{
    auto currTurn = dirToArrow(getCurrTurn()).value();

    auto newDirEvent = Event::ValueChangedEvent("currTurn", currTurn);
    EventManager::getInstance().publish(newDirEvent);
}
