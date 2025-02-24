#pragma once

#include <LittleFS.h>
#include <string>
#include "menu_page.hpp"
#include "file_opt_menu_page.hpp"

class FileMenuPage : public MenuPage {
public:
    FileMenuPage(const std::string& lbl, const std::string& parentLbl);
    
    ~FileMenuPage() = default;

    void addFileLines();
    void refreshPage();

private:

    void generateFilePage(const std::string& fileName);

    std::string parentMenuLbl;

};