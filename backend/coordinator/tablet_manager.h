#ifndef TABLET_MANAGER_H
#define TABLET_MANAGER_H

#include "../common/types.h"
#include <vector>
#include <unordered_map>
#include <mutex>

class TabletManager {
public:
    TabletManager(size_t numTablets);
    Value get(const Key& key);
    void put(const Key& key, const Value& value);
    void remove(const Key& key);

private:
    TabletID getTabletId(const Key& key);
    std::vector<std::unique_ptr<Tablet>> tablets;
    std::vector<std::mutex> tabletLocks;
};

#endif // TABLET_MANAGER_H