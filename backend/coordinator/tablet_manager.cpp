#include "tablet_manager.h"
#include "../storage/tablet.h"
#include <iostream>

TabletManager::TabletManager(size_t numTablets)
    : tablets(numTablets), tabletLocks(numTablets) {
    // Create the tablets and their corresponding locks
    for (size_t i = 0; i < numTablets; i++) {
        tablets[i] = std::make_unique<Tablet>(i);
    }
}

Value TabletManager::get(const Key& key) {
    TabletID tabletId = getTabletId(key);
    std::unique_lock<std::mutex> lock(tabletLocks[tabletId]);
    return tablets[tabletId]->get(key);
}

void TabletManager::put(const Key& key, const Value& value) {
    TabletID tabletId = getTabletId(key);
    std::unique_lock<std::mutex> lock(tabletLocks[tabletId]);
    tablets[tabletId]->put(key, value);
}

void TabletManager::remove(const Key& key) {
    TabletID tabletId = getTabletId(key);
    std::unique_lock<std::mutex> lock(tabletLocks[tabletId]);
    tablets[tabletId]->remove(key);
}

TabletID TabletManager::getTabletId(const Key& key) {
    return std::hash<std::string>{}(key) % tablets.size();
}