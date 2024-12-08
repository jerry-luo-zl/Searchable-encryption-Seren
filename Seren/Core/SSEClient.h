//
// Created by shangqi on 2020/11/18.
//

#ifndef AURA_SSECLIENT_H
#define AURA_SSECLIENT_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include "../gen-cpp/SSEService.h"
#include "rocksdb/db.h"
#include <tuple>
#include <ctime>
#include <random>
#include <vector>
#include "GGMTree.h"
#include "BloomFilter.h"
enum OP {
    INS, DEL
};

using namespace server;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace std;

class SSEClient {
private:
    uint8_t *key = (unsigned char*) "0123456789123456";
    uint8_t *iv = (unsigned char*) "0123456789123456";

    GGMTree *tree;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> delete_bf;
    unordered_map<string, int> C;       // search time

    shared_ptr<TTransport> transport;
    int st;
    default_random_engine e;
    SSEServiceClient *server;

public:
    SSEClient();
    ~SSEClient();

    void update(OP op, const string& keyword, const vector<int>& inds);
    void search(vector<int>& res, const string& keyword);

};


#endif //AURA_SSECLIENT_H
