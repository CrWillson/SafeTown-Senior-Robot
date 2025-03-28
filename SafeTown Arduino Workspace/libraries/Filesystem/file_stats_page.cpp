#include "file_stats_page.hpp"
#include <LittleFS.h>
#include "menu.hpp"

void FileStatPage::onPageLoad()
{
    FSInfo fs_info;
    LittleFS.info(fs_info);

    clearLines();

    addLine(new TextMenuLine("File System Stats"));
    addLine(new SpacerMenuLine());

    addLine(new TextMenuLine("Total Bytes: " + std::to_string(fs_info.totalBytes)));
    addLine(new TextMenuLine("Used Bytes: " + std::to_string(fs_info.usedBytes)));
    addLine(new TextMenuLine("Open Bytes: " + std::to_string(fs_info.totalBytes - fs_info.usedBytes)));

    addLine(new SpacerMenuLine());
    addLine(new ButtonMenuLine("Refresh", [this]{
        this->parentMenu->setCurrentPage(label);
    }));
    addLine(new ButtonMenuLine("Back", [this]{
        this->parentMenu->setCurrentPage(parentMenuLbl);
    }));
}

void FileStatPage::onPageExit()
{
    
}
