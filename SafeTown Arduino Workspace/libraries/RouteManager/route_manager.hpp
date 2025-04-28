#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <optional>
#include "FS.h"
#include <LittleFS.h>
#include "event_types.hpp"

class RouteManager {
public:

    enum class DIR : uint8_t { STRAIGHT, LEFT, RIGHT };

    using route_t = std::vector<DIR>;
    using routemap_t = std::unordered_map<std::string, route_t>;
    using routeiter_t = routemap_t::iterator;

    ~RouteManager() = default;
    RouteManager(const RouteManager&) = delete;
    RouteManager& operator=(const RouteManager&) = delete;

    /**
     * @brief Get the Meyer's singleton instance of the RouteManager
     * 
     * @return RouteManager& 
     */
    static RouteManager& getInstance() {
        static RouteManager instance;
        return instance;
    }

    void init(std::string routedir = "/routes/");

    DIR getCurrTurn() const;
    route_t getCurrentRoute() const;
    std::optional<route_t> getRoute(std::string label) const;
    const routemap_t& getAllRoutes() const {
        return _allRoutes;
    }

    void incRouteStep();
    void resetRoute();

    bool setCurrentRoute(std::string label);
    void addRoute(std::string label, route_t turns);

    std::optional<std::string> dirToArrow(DIR dir) {
        switch (dir) {
            case DIR::STRAIGHT: return "^";
            case DIR::LEFT: return "<";
            case DIR::RIGHT: return ">";
            default: return std::nullopt;
        }
    }

    std::optional<DIR> charToDir(char c) {
        switch (c) {
            case 'S': return DIR::STRAIGHT;
            case 'L': return DIR::LEFT;
            case 'R': return DIR::RIGHT;
            default: return std::nullopt;
        }
    }


private:

    RouteManager() {}

    void publishCurrentStep();

    void onValueChange(const Event::ValueChangedEvent& e);
    void onValueRequest(const Event::ValueRequestEvent& e);

    std::string _defaultRouteLabel = "D";
    route_t _defaultRoute = {DIR::LEFT, DIR::RIGHT, DIR::LEFT, DIR::STRAIGHT};

    routemap_t _allRoutes;
    routeiter_t _currRoute = _allRoutes.end();
    size_t _currStep = 0;

    std::string _currRouteLabel = _defaultRouteLabel;
    uint8_t _currRouteStep = 0;


    std::string _routeDir = "/routes/";

    friend class RouteMenuPage;
    friend class RouteMenuLine;
    friend class RouteOptPage;

};