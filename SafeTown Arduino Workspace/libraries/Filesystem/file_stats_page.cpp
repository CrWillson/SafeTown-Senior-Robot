#include "file_stats_page.hpp"
#include <LittleFS.h>


void FileStatPage::onPageLoad()
{
    FSInfo fs_info;
    LittleFS.info(fs_info);

    addLine("Total Bytes: " + std::to_string(fs_info.totalBytes));
    addLine("Used Bytes: " + std::to_string(fs_info.usedBytes));
    addLine("Available Bytes: " + std::to_string(fs_info.totalBytes - fs_info.usedBytes));

    addLine(new SpacerMenuLine());
    addLine(new ButtonMenuLine("Refresh", [this]{
        this->parentMenu->setCurrentPage(label);
    }));
    addLine(new ButtonMenuLine("Back", [this]{
        this->parentMenu->setCurrentPage(parentMenuLbl);
    }));
    addLine(new SpacerMenuLine());
}

void FileStatPage::onPageExit()
{
    clearLines();

    addLine(new TextMenuLine("File System Stats"));
    addLine(new SpacerMenuLine());
}
