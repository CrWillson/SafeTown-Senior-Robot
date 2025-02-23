#pragma once

#include <LittleFS.h>
#include "menu_page.hpp"
#include "menu.hpp"
#include "fs_manager.hpp"

class FileMenuPage;

class FileOptMenuPg : public MenuPage {
public:
    FileOptMenuPg(const std::string& lbl, const std::string& parentLbl, const std::string& file);
    ~FileOptMenuPg() = default;

    void onFileDelete();
    void onFilePrint();

private:
    std::string selectedFile;
    std::string parentMenuLbl;

    FSManager* fsManager;

    friend class FileMenuPage;
};