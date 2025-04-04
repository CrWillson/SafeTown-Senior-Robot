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

/**
 * @brief Abstract base menu type. Constructing a menu requires the user to inherit from this class
 * and override the buildMenu() method with the procedure for constructing their specific menu.
 * 
 */
class Menu {
public:
    Menu() = default;
    virtual ~Menu() = default;

    /**
     * @brief Initalize and construct the menu
     * 
     */
    void init();

    /**
     * @brief Set the current page to the one matching the label given. 
     * @details If no page is found with a maching label, nothing will happen. Changing the
     * current page will publish a PageChangedEvent to the event manager.
     * 
     * @param label - The page to switch to.
     */
    void setCurrentPage(const std::string& label);

    /**
     * @brief Add a page to the menu. Ownership of the MenuPage* is given to the menu object.
     * 
     * @param page - The page to add
     * @return MenuPage* - A pointer to the page just added
     */
    MenuPage* addPage(MenuPage* page);

protected:
    //---------------------------------------------------------------------------
    // Creating a custom menu requires overriding this method in a child class!
    //---------------------------------------------------------------------------
    virtual void buildMenu() = 0;
    //---------------------------------------------------------------------------

    std::unordered_map<std::string, std::shared_ptr<MenuPage>> allPages;

private:

    void onScrollUp(const Event::Event& e);
    void onScrollDown(const Event::Event& e);
    void onSelect(const Event::Event& e);
    void onValueChange(const Event::ValueChangedEvent& e);
    void onValueRequest(const Event::ValueRequestEvent& e);

    MenuPage* currentPage;

    EventManager* eventManager;

    friend class MenuPage;
    friend class MenuLine;

};