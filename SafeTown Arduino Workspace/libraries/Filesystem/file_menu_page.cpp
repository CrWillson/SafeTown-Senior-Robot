#include "file_menu_page.hpp"
#include "menu.hpp"

FileMenuPage::FileMenuPage(const std::string& lbl, const std::string& parentLbl)
    : MenuPage(lbl), parentMenuLbl(parentLbl) 
{
    generateFilePage();

    eventManager = &EventManager::getInstance();
    eventManager->subscribe<Event::FileCreatedEvent>([this](const auto& event) {
        this->generateFilePage();
    });
    eventManager->subscribe<Event::FileDeletedEvent>([this](const auto& event) {
        this->generateFilePage();
    }); 
}


void FileMenuPage::generateFileOptPage(const std::string& fileName)
{
    auto fileOptPage = new FileOptMenuPg("FileOption", label, fileName);
    parentMenu->addPage(fileOptPage);
    parentMenu->setCurrentPage(fileOptPage->label);
}


void FileMenuPage::generateFilePage()
{
    Serial.println("Generating file list...");
    lines.clear();
    addLine(new ButtonMenuLine("Back", [this]{
        if (currentPath != "/") {
            size_t pos = currentPath.find_last_of('/', currentPath.length() - 2);
            currentPath = currentPath.substr(0, pos + 1);
            generateFilePage();
        } else {
            this->parentMenu->setCurrentPage(parentMenuLbl);
        }
    }));
    addLine(new ButtonMenuLine("Refresh List", [this]{
        this->generateFilePage();
    }));
    addLine(new TextMenuLine("----------------"));
    addFileLines();
}


void FileMenuPage::addFileLines()
{
    auto dir = LittleFS.openDir(currentPath.c_str());
    while (dir.next()) {
        auto fileName = dir.fileName();
        if (dir.isDirectory()) {
            addLine(new ButtonMenuLine((fileName + "/").c_str(), [this, fileName]{
                Serial.print("Directory ");
                Serial.print(fileName.c_str());
                Serial.println(" selected");
                currentPath += std::string(fileName.c_str()) + "/";
                generateFilePage();
            }));
        } else {
            addLine(new ButtonMenuLine(fileName.c_str(), [this, fileName]{
                Serial.print("File ");
                Serial.print(fileName.c_str());
                Serial.println(" selected");
                generateFileOptPage(currentPath + std::string(fileName.c_str()));
            }));
        }
    }
}