#include "file_opt_menu_page.hpp"
#include "menu.hpp"

FileOptMenuPg::FileOptMenuPg(const std::string& lbl, const std::string& parentLbl, const std::string& file)
    : MenuPage(lbl), parentMenuLbl(parentLbl), selectedFile(file) 
{
    std::string labelText = "File: " + selectedFile;
    addLine(new TextMenuLine(labelText));
    addLine(new ButtonMenuLine(" Cancel", [this]{
        this->parentMenu->setCurrentPage(parentMenuLbl);
    }));
    addLine(new ButtonMenuLine(" Print", [this]{
        this->onFilePrint();
    }));
    addLine(new ButtonMenuLine(" Delete", [this]{
        this->onFileDelete();
    }));
} 