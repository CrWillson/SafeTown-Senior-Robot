#include "file_opt_menu_page.hpp"
#include "menu.hpp"

FileOptMenuPg::FileOptMenuPg(const std::string& lbl, const std::string& parentLbl, const std::string& file)
    : MenuPage(lbl), parentMenuLbl(parentLbl), selectedFile(file) 
{
    addLine(new TextMenuLine(selectedFile));
    addLine(new SpacerMenuLine());

    File fileObj = LittleFS.open(selectedFile.c_str(), "r");
    uint16_t fileSize = fileObj.size();
    fileObj.close();
    addLine(new TextMenuLine("Size: " + std::to_string(fileSize) + " B"));
    addLine(new SpacerMenuLine());

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
        LOG("Failed to delete file!");
    }
    parentMenu->setCurrentPage(parentMenuLbl);
}

void FileOptMenuPg::onFilePrint()
{
    File file = LittleFS.open(selectedFile.c_str(), "r");
    if (!file) {
        LOG("Failed to open file for reading!");
        return;
    }

    
    SUPRESS_DEBUG_OUTPUT(true);

    Serial.println("FILE CONTENT START");
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
    Serial.println("FILE CONTENT END");

    SUPRESS_DEBUG_OUTPUT(false);
}