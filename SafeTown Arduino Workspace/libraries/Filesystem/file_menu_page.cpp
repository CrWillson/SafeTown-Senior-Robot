#include "file_menu_page.hpp"
#include "menu.hpp"

FileMenuPage::FileMenuPage(const std::string& lbl, const std::string& parentLbl)
    : MenuPage(lbl), parentMenuLbl(parentLbl) 
{
    currentPath = "/";
}


void FileMenuPage::generateFilePage(const std::string& fileName)
{
    auto fileOptPage = new FileOptMenuPg("FileOption", label, fileName);
    parentMenu->addPage(fileOptPage);
    parentMenu->setCurrentPage(fileOptPage->label);
}


void FileMenuPage::onPageLoad()
{
    addDirectoryLines();
    addFileLines();
}

void FileMenuPage::onPageExit()
{   
    clearLines();

    std::string displayPath = (currentPath == "/") ? "root/" : currentPath.substr(currentPath.find_last_of('/', currentPath.length() - 2) + 1);
    addLine(new TextMenuLine(displayPath.c_str()));
    addLine(new SpacerMenuLine());

    addLine(new ButtonMenuLine("Back", [this]{
        if (currentPath != "/") {
            size_t pos = currentPath.find_last_of('/', currentPath.length() - 2);
            currentPath = currentPath.substr(0, pos + 1);
            this->parentMenu->setCurrentPage(label);    // set the current page to itself to refresh the page
        } else {
            this->parentMenu->setCurrentPage(parentMenuLbl);
        }
    }));
    addLine(new SpacerMenuLine());
}

void FileMenuPage::addDirectoryLines()
{
    auto dir = LittleFS.openDir(currentPath.c_str());
    while (dir.next()) {
        if (dir.isDirectory()) {
            auto dirName = dir.fileName();
            addLine(new ButtonMenuLine((std::string(dirName.c_str()) + "/").c_str(), [this, dirName]{
                currentPath += std::string(dirName.c_str()) + "/";
                this->parentMenu->setCurrentPage(label);    // set the current page to itself to refresh the page
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
