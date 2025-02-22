#include "file_menu_page.hpp"
#include "menu.hpp"

FileMenuPage::FileMenuPage(const std::string& lbl, const std::string& parentLbl)
    : MenuPage(lbl), parentMenuLbl(parentLbl) 
{
    eventManager->subscribe<Event::FileCreatedEvent>([this](const auto& event) {
        this->onFileCreated();
    });
    eventManager->subscribe<Event::FileDeletedEvent>([this](const auto& event) {
        this->onFileDeleted();
    }); 

    addLine(new ButtonMenuLine("Back", [this]{
        this->parentMenu->setCurrentPage(parentMenuLbl);
    }));
    addFileLines(); 
}

void FileMenuPage::addFileLines()
{
    lines.clear();

    auto dir = LittleFS.openDir("/");
    while (dir.next()) {
        addLine(new TextMenuLine(dir.fileName().c_str()));
    }
}

void FileMenuPage::onFileCreated()
{
}

void FileMenuPage::onFileDeleted()
{
}