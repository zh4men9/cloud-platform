#include "tablet_manager.h"
#include <cstdint>

bool Tablet::storeFile(const File& file) {
    for (const auto& f : files) {
        if (f.name == file.name) {
            return false; // File with the same name already exists
        }
    }
    files.push_back(file);
    return true;
}

File Tablet::getFile(const std::string& name) {
    for (const auto& file : files) {
        if (file.name == name) {
            return file;
        }
    }
    return File(); // Return an empty file if not found
}

bool Tablet::deleteFile(const std::string& name) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        if (it->name == name) {
            files.erase(it);
            return true;
        }
    }
    return false; // File not found
}

TabletManager::TabletManager(size_t numTablets) {
    for (size_t i = 0; i < numTablets; i++) {
        tablets.emplace_back(std::make_unique<Tablet>(i));
    }
}

bool TabletManager::storeFile(const File& file) {
    size_t tabletIndex = getTabletIndex(file.name);
    return tablets[tabletIndex]->storeFile(file);
}

File TabletManager::getFile(const std::string& name) {
    size_t tabletIndex = getTabletIndex(name);
    return tablets[tabletIndex]->getFile(name);
}

bool TabletManager::deleteFile(const std::string& name) {
    size_t tabletIndex = getTabletIndex(name);
    return tablets[tabletIndex]->deleteFile(name);
}

size_t TabletManager::getTabletIndex(const std::string& fileName) {
    // Implement a hash-based algorithm to determine the tablet index
    return std::hash<std::string>()(fileName) % tablets.size();
}