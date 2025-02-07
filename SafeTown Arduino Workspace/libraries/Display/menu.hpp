#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>
#include <type_traits>
#include <unordered_map>

#include "event_manager.hpp"
#include "menu_page.hpp"

// Forward declarations
class MenuPage;
class MenuLine;

class Menu {
public:
    void initMenu(EventManager* manager); // Where the whole menu structure is defined

    void addPage(const std::string& label);
    void setCurrentPage(const std::string& label);

    std::shared_ptr<MenuPage> currentPage;

private:
    EventManager* eventManager;

    std::unordered_map<std::string, std::shared_ptr<MenuPage>> allPages;

    friend class MenuPage;
    friend class MenuLine;

};