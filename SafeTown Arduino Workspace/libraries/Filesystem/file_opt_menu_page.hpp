#pragma once

#include <LittleFS.h>
#include "menu_page.hpp"
#include "menu.hpp"

class FileOptMenuPg : public MenuPage {
public:
    FileOptMenuPg(const std::string& lbl, const std::string& parentLbl, const std::string& file);
    ~FileOptMenuPg() override = default;

    void onFileDelete();
    void onFilePrint();

private:
    std::string selectedFile;
    std::string parentMenuLbl;
};