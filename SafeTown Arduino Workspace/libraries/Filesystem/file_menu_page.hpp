#pragma once

#include <string>
#include "menu_page.hpp"

/**
 * @brief A special menu page for navigating the virtual file system
 * @details The contents of this page are automatically generated upon loading the page. No
 * setup beyond creating the page is necessary for the user.
 * @ingroup Menu
 */
class FileMenuPage : public MenuPage {
public:
    /**
     * @brief Construct a new FileMenuPage
     * 
     * @param lbl - The label to refer to this page as.
     * @param parentLbl - The page label to return to upon selecting the "back" option in this page.
     */
    FileMenuPage(const std::string& lbl, const std::string& parentLbl);
    
    ~FileMenuPage() = default;

private:
    void addFileLines();
    void addDirectoryLines();

    void onPageLoad() override;
    void onPageExit() override;
    
    /**
     * @brief Generate a FileOptMenuPg for the file that was just selected.
     * 
     * @param fileName - The filename to manipulate
     */
    void generateFilePage(const std::string& fileName);

    std::string parentMenuLbl;
    std::string currentPath;
};