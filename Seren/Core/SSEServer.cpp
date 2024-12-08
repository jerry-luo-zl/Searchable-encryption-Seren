//
// Created by shangqi on 2020/11/18.
//

#include "SSEServer.h"

SSEServer::SSEServer() {
  // std::cout <<
  // duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  //  << " : " << "Create SSEServer!" << std::endl;
  this->handler = new SSEServerHandler();
}

void SSEServer::add_entries(const string &key, const string &val,
                            const int32_t flag) {
  // std::cout <<
  // duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  //   << " : " << "SSEServer addentry!" << std::endl;
  handler->add_entries(key, val, flag);
}

void SSEServer::search(vector<int32_t> &_return, const int32_t st,
                       const string &Kw,
                       const vector<SerialisedNode> &node_list,
                       const int32_t level) {

  // convert serialised node to GGM nodes
  vector<GGMNode> received_nodes;
  received_nodes.reserve(node_list.size());
  for (const SerialisedNode &node : node_list) {
    received_nodes.emplace_back(node.index, node.level,
                                (uint8_t *)node.key.c_str());
  }
  // search for the result
  //    cout <<
  //    duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  //    << endl;
  handler->search(_return, st, Kw, received_nodes, level);
  //    cout <<
  //    duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  //    << endl;
}
void SSEServer::verify(vector<int32_t> &_return, const string &D,
                       const string &Kw) {
  handler->verify(_return, D, Kw);
}

int main() {
  int a = 1;
  // change to remote server
  TSimpleServer server(
      make_shared<SSEServiceProcessor>(make_shared<SSEServer>()),
      make_shared<TServerSocket>("localhost", 9090),
      make_shared<TFramedTransportFactory>(),
      make_shared<TBinaryProtocolFactory>());
  cout << "Server listening to localhost:9090..." << endl;
  // start service
  server.serve();
}