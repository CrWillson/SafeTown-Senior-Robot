#pragma once

#include <string>
#include "menu_page.hpp"

/**
 * @brief A special menu page type that displays information regarding the virtual file system
 * @details Displays the virtual file system's total space, used space, and avaialble space. 
 * @ingroup Menu
 */
class FileStatPage : public MenuPage {
public:
    /**
     * @brief Construct a new FileStatPage
     * 
     * @param lbl - The label to refer to this page as.
     * @param parentLbl - The page label to return to upon selecting the "back" option in this page.
     */
    FileStatPage(const std::string& lbl, const std::string& parentLbl)
        : MenuPage(lbl), parentMenuLbl(parentLbl) {}
    
    ~FileStatPage() = default;

protected:
    /**
     * @brief Called upon loading the page.
     * @details Automatically populates the page with the latest virtual file system information.
     * 
     */
    void onPageLoad() override;

    /**
     * @brief Called upon exiting the page
     * @details Automatically clears the previously shown information
     * 
     */
    void onPageExit() override;

private:
    std::string parentMenuLbl;
};