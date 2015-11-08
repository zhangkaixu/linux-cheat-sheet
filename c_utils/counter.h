#pragma once
#include <cstdio>
#include <string>
#include <set>
#include <map>
#include <vector>

namespace counter {

using namespace std;

template<class T>
class Counter {
    typedef map<T, size_t> Dict;
    Dict _map;

public:
    void clear() {
        _map.clear();
    }
    void update(const T& key, size_t n = 1) {
        auto iter = _map.find(key);
        if (iter == _map.end()) {
            _map[key] = n;
        } else {
            iter->second += n;
        }
    }
    void dump() {
        for (auto iter = _map.begin(); iter != _map.end(); ++iter) {
            cout<<iter->first<<"\t"<<iter->second<<endl;
        }
    }
};


}
