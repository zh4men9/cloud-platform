#include "tablet.h"
#include <iostream>

Tablet::Tablet(TabletID id) : id(id) {}

void Tablet::put(const Key& key, const Value& value) {
    data[key] = value;
    std::cout << "Put key=" << key << ", value=" << value << " into tablet " << id << std::endl;
}

Value Tablet::get(const Key& key) {
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second;
    }
    return "";
}

void Tablet::remove(const Key& key) {
    data.erase(key);
    std::cout << "Removed key=" << key << " from tablet " << id << std::endl;
}