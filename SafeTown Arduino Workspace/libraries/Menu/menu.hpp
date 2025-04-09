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
 * @defgroup Menu
 * @brief A collection of utilities for building and navigating menus on the Pico's display
 * 
 */


/**
 * @brief Abstract base menu type. Constructing a menu requires the user to inherit from this class
 * and override the buildMenu() method with the procedure for constructing their specific menu.
 * @ingroup Menu
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
     * @brief Add a default menu page to the Menu
     * 
     * @overload
     * @param label - A label for the page by which it can be referred to later
     * 
     * @return MenuPage* - A raw pointer to the page that was just created
     */
    MenuPage* addPage(const std::string& label) {
        auto page = std::make_unique<MenuPage>(label);
        page->parentMenu = this;

        allPages[label] = std::move(page);

        if (allPages.size() == 1) {
            setCurrentPage(label);
        }

        return allPages[label].get();
    }


    /**
     * @brief Add any type of page to the Menu
     * 
     * @tparam Page - The page type to add. Must be a child class of the MenuPage type.
     * @param args - All of the parameters necessary to construct a page of type Page
     * 
     * @return MenuPage* - A raw pointer to the page that was just created.
     */
    template<typename Page, typename... Args, std::enable_if_t<std::is_base_of_v<MenuPage, Page>, int> = 0>
    MenuPage* addPage(Args&&... args) {
        auto page = std::make_unique<Page>(std::forward<Args>(args)...);
        page->parentMenu = this;

        auto label = page->getLabel();
        allPages[label] = std::move(page);

        if (allPages.size() == 1) {
            setCurrentPage(label);
        }

        return allPages[label].get();
    }

protected:
    //---------------------------------------------------------------------------
    // Creating a custom menu requires overriding this method in a child class!
    //---------------------------------------------------------------------------
    virtual void buildMenu() = 0;
    //---------------------------------------------------------------------------

    std::unordered_map<std::string, std::unique_ptr<MenuPage>> allPages;

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