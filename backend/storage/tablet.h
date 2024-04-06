#ifndef TABLET_H
#define TABLET_H

#include "../common/types.h"
#include <unordered_map>

class Tablet {
public:
    Tablet(TabletID id);
    void put(const Key& key, const Value& value);
    Value get(const Key& key);
    void remove(const Key& key);

private:
    TabletID id;
    std::unordered_map<Key, Value> data;
};

#endif // TABLET_H