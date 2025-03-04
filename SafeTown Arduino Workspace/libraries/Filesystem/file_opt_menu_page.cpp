#include "file_opt_menu_page.hpp"
#include "menu.hpp"

FileOptMenuPg::FileOptMenuPg(const std::string& lbl, const std::string& parentLbl, const std::string& file)
    : MenuPage(lbl), parentMenuLbl(parentLbl), selectedFile(file) 
{
    std::string labelText = "File: " + selectedFile;
    addLine(new TextMenuLine(labelText));
    addLine(new TextMenuLine("----------------"));
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


void FileOptMenuPg::onFileDelete()
{
    if (!LittleFS.remove(selectedFile.c_str())) {
        Serial.println("Failed to delete file!");
    }
    parentMenu->setCurrentPage(parentMenuLbl);
}

void FileOptMenuPg::onFilePrint()
{
    File file = LittleFS.open(selectedFile.c_str(), "r");
    if (!file) {
        Serial.println("Failed to open file for reading!");
        return;
    }

    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
    Serial.println("");
}