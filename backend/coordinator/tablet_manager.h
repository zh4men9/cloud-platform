#ifndef TABLET_MANAGER_H
#define TABLET_MANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "file.h"

class Tablet {
public:
    Tablet(int index) : index(index) {}
    bool storeFile(const File& file);
    File getFile(const std::string& name);
    bool deleteFile(const std::string& name);

private:
    int index;
    std::vector<File> files;
};

class TabletManager {
public:
    TabletManager(size_t numTablets);
    bool storeFile(const File& file);
    File getFile(const std::string& name);
    bool deleteFile(const std::string& name);

private:
    std::vector<std::unique_ptr<Tablet>> tablets;
    size_t getTabletIndex(const std::string& fileName);
};

#endif // TABLET_MANAGER_H