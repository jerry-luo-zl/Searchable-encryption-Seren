//
// Created by shangqi on 2020/6/17.
//

#ifndef AURA_SSESERVERHANDLER_H
#define AURA_SSESERVERHANDLER_H

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "rocksdb/db.h"

#include "BloomFilter.h"
extern "C" {
#include "CommonUtil.h"
}
#include "GGMTree.h"


using namespace chrono;
using namespace std;

typedef struct {
    vector< vector<int> > addInds;
    vector< vector<string> > addTags;
    vector< vector<string> > delTags;
    vector< int > sts;
} RetStruct;

class SSEServerHandler {
private:
    rocksdb::DB* dbTag;
    rocksdb::DB* dbCt;
    rocksdb::DB* dbPf;
    rocksdb::DB* dbS;
    unordered_map<long, uint8_t*> keys;
    RetStruct ret;

    void compute_leaf_keys(const vector<GGMNode>& node_list, int level);
public:
    SSEServerHandler();
    ~SSEServerHandler();
    void add_entries(const string& key, const string& val, const int& flag);
    void search(vector<int>& res, const int& st, const string& Kw, const vector<GGMNode>& node_list, int level);
    void verify(vector<int>& res, const string& D, const string& Kw);
};

template <typename T>
void printVector(vector<T> arr);

#endif //AURA_SSESERVERHANDLER_H
