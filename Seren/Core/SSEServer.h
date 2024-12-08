//
// Created by shangqi on 2020/11/18.
//

#ifndef AURA_SSESERVER_H
#define AURA_SSESERVER_H

#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include "SSEServerHandler.h"
#include "../GGM/GGMNode.h"
#include "../gen-cpp/SSEService.h"

using namespace server;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::server;
using namespace apache::thrift::transport;

class SSEServer : public SSEServiceIf {
private:
    SSEServerHandler *handler;

public:
    SSEServer();
    void add_entries(const string& key, const string& val, const int32_t flag) override;
    void search(vector<int32_t> & _return, const int32_t st, const string& Kw, const vector<SerialisedNode> & node_list,
                                     const int32_t level) override;
    void verify(vector<int32_t> & _return, const string& D, const string& Kw) override;
};


#endif //AURA_SSESERVER_H
