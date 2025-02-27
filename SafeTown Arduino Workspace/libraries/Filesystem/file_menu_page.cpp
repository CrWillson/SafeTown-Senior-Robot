#include "file_menu_page.hpp"
#include "menu.hpp"

FileMenuPage::FileMenuPage(const std::string& lbl, const std::string& parentLbl)
    : MenuPage(lbl), parentMenuLbl(parentLbl) 
{
    refreshPage();

    eventManager = &EventManager::getInstance();
    eventManager->subscribe<Event::FileCreatedEvent>([this](const auto& event) {
        this->refreshPage();
    });
    eventManager->subscribe<Event::FileDeletedEvent>([this](const auto& event) {
        this->refreshPage();
    }); 
}


void FileMenuPage::generateFilePage(const std::string& fileName)
{
    auto fileOptPage = new FileOptMenuPg("FileOption", label, fileName);
    parentMenu->addPage(fileOptPage);
    parentMenu->setCurrentPage(fileOptPage->label);
}


void FileMenuPage::refreshPage()
{
    Serial.println("Refreshing file list...");
    lines.clear();
    addLine(new ButtonMenuLine("Back", [this]{
        this->parentMenu->setCurrentPage(parentMenuLbl);
    }));
    addLine(new ButtonMenuLine("Refresh List", [this]{
        this->refreshPage();
    }));
    addLine(new TextMenuLine("----------------"));
    addFileLines();
}


void FileMenuPage::addFileLines()
{
    auto dir = LittleFS.openDir("/");
    while (dir.next()) {
        auto fileName = dir.fileName();
        addLine(new ButtonMenuLine(fileName.c_str(), [this, fileName]{
            Serial.print("File ");
            Serial.print(fileName.c_str());
            Serial.println(" selected");
            generateFilePage(fileName.c_str());
        }));
    }
}
