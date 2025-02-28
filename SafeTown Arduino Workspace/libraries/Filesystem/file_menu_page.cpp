#include "file_menu_page.hpp"
#include "menu.hpp"

FileMenuPage::FileMenuPage(const std::string& lbl, const std::string& parentLbl)
    : MenuPage(lbl), parentMenuLbl(parentLbl) 
{
    currentPath = "/";

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


void FileMenuPage::refreshPage(bool forceRefresh)
{
    clearLines();

    std::string displayPath = (currentPath == "/") ? "root/" : currentPath.substr(currentPath.find_last_of('/', currentPath.length() - 2) + 1);
    addLine(new TextMenuLine(displayPath.c_str()));
    addLine(new SpacerMenuLine());

    addLine(new ButtonMenuLine("Back", [this]{
        if (currentPath != "/") {
            size_t pos = currentPath.find_last_of('/', currentPath.length() - 2);
            currentPath = currentPath.substr(0, pos + 1);
            refreshPage(true);
        } else {
            this->parentMenu->setCurrentPage(parentMenuLbl);
        }
    }));
    addLine(new ButtonMenuLine("Refresh List", [this]{
        this->refreshPage();
    }));
    addLine(new SpacerMenuLine());
    addDirectoryLines();
    addFileLines();

    if (forceRefresh) {
        EventManager::getInstance().publish(Event::PageChangedEvent(getVisibleText()));
    }
}


void FileMenuPage::addDirectoryLines()
{
    auto dir = LittleFS.openDir(currentPath.c_str());
    while (dir.next()) {
        if (dir.isDirectory()) {
            auto dirName = dir.fileName();
            addLine(new ButtonMenuLine((std::string(dirName.c_str()) + "/").c_str(), [this, dirName]{
                currentPath += std::string(dirName.c_str()) + "/";
                refreshPage(true);
            }));
        }
    }
}

void FileMenuPage::addFileLines()
{
    auto dir = LittleFS.openDir(currentPath.c_str());
    while (dir.next()) {
        if (!dir.isDirectory()) {
            auto fileName = dir.fileName();
            addLine(new ButtonMenuLine(fileName.c_str(), [this, fileName]{
                generateFilePage(currentPath + std::string(fileName.c_str()));
            }));
        }
    }
}
