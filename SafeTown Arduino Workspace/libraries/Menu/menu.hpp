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
class Menu {
public:
    Menu() = default;
    virtual ~Menu() = default;

    void initMenu(EventManager* manager);

    void addPage(const std::string& label);
    void setCurrentPage(const std::string& label);

    void onScrollUp(const Event::Event& e);
    void onScrollDown(const Event::Event& e);
    void onSelect(const Event::Event& e);
    void onValueChange(const Event::ValueChangedEvent& e);

    std::shared_ptr<MenuPage> currentPage;

protected:
    //---------------------------------------------------------------------------
    // Creating a custom menu requires overriding this method in a child class
    //---------------------------------------------------------------------------
    virtual void buildMenu() = 0;

    EventManager* eventManager;

    std::unordered_map<std::string, std::shared_ptr<MenuPage>> allPages;

    friend class MenuPage;
    friend class MenuLine;

};