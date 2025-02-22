#pragma once

#include <LittleFS.h>
#include "menu_page.hpp"

class FileMenuPage : public MenuPage {
public:
    FileMenuPage(const std::string& lbl, const std::string& parentLbl);
    
    ~FileMenuPage() override = default;

    void addFileLines();
    void onFileCreated();
    void onFileDeleted();

private:

    std::string parentMenuLbl;
};