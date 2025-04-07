#include "file_stats_page.hpp"
#include <LittleFS.h>
#include "menu.hpp"

void FileStatPage::onPageLoad()
{
    FSInfo fs_info;
    LittleFS.info(fs_info);

    clearLines();

    addLine<TextMenuLine>("File System Stats");
    addLine<SpacerMenuLine>();

    addLine<TextMenuLine>("Total Bytes: " + std::to_string(fs_info.totalBytes));
    addLine<TextMenuLine>("Used Bytes: " + std::to_string(fs_info.usedBytes));
    addLine<TextMenuLine>("Open Bytes: " + std::to_string(fs_info.totalBytes - fs_info.usedBytes));

    addLine<SpacerMenuLine>();
    addLine<ButtonMenuLine>("Refresh", [this]{
        this->parentMenu->setCurrentPage(label);
    });
    addLine<ButtonMenuLine>("Back", [this]{
        this->parentMenu->setCurrentPage(parentMenuLbl);
    });
}

void FileStatPage::onPageExit()
{
    
}
