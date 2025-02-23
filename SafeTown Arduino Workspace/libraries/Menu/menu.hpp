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

    void init();

    void addPage(MenuPage* page);
    void setCurrentPage(const std::string& label);
    void setCurrentPage(MenuPage* page);

    void onScrollUp(const Event::Event& e);
    void onScrollDown(const Event::Event& e);
    void onSelect(const Event::Event& e);
    void onValueChange(const Event::ValueChangedEvent& e);
    void onValueRequest(const Event::ValueRequestEvent& e);

    MenuPage* currentPage;

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