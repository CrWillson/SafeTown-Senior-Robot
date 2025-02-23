#pragma once

#include <LittleFS.h>
#include <string>
#include "menu_page.hpp"
#include "file_opt_menu_page.hpp"

class FileMenuPage : public MenuPage {
public:
    FileMenuPage(const std::string& lbl, const std::string& parentLbl);
    ~FileMenuPage() = default;

private:
    
    void addFileLines();
    void generateFilePage();
    void generateFileOptPage(const std::string& fileName);

    std::string parentMenuLbl;
    std::string currentPath = "/";

};