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
    lines.clear();
    numLines = 0;

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
            generateFilePage(fileName.c_str());
        }));
    }
}
