#pragma once

#include <string>
#include "menu_page.hpp"

class FileStatPage : public MenuPage {
public:
    FileStatPage(const std::string& lbl, const std::string& parentLbl)
        : MenuPage(lbl), parentMenuLbl(parentLbl) {}
    
    ~FileStatPage() = default;

    void onPageLoad() override;
    void onPageExit() override;

private:
    std::string parentMenuLbl;
};