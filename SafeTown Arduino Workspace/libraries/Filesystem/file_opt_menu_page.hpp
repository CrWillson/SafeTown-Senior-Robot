#pragma once

#include <LittleFS.h>
#include "menu_page.hpp"
#include "menu.hpp"

class FileMenuPage;

class FileOptMenuPg : public MenuPage {
public:
    FileOptMenuPg(const std::string& lbl, const std::string& parentLbl, const std::string& file);
    ~FileOptMenuPg() = default;

    void initPage() override {}

    void onFileDelete();
    void onFilePrint();

private:
    std::string selectedFile;
    std::string parentMenuLbl;

    friend class FileMenuPage;
};