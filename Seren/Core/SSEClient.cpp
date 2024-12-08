// Created by shangqi on 2020/11/18.

#include "SSEClient.h"
#include <chrono>
#include <iostream>
#include <random>
using namespace chrono;
using namespace std;

SSEClient::SSEClient() {
  // std::cout <<
  // duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  //         << " : " << "Create SSEClient!" << std::endl;
  //  init ST
  st = 0;
  e.seed(time(0));
  // init the GGM Tree
  tree = new GGMTree(GGM_SIZE);
  // establish connection with the server
  // if deploy in remote server this should be changed
  shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  transport = shared_ptr<TTransport>(new TFramedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  server = new SSEServiceClient(protocol);
  transport->open();
}

SSEClient::~SSEClient() {
  st = 0;
  delete_bf.reset();
  // delete SSE handler and shutdown the connection
  delete server;
  transport->close();
}

void SSEClient::update(OP op, const string &keyword, const vector<int> &inds) {

  int st1;
  uint8_t h1[DIGEST_SIZE];
  uint8_t Kw[DIGEST_SIZE];

  auto t1 = high_resolution_clock::now();
  // compute K_w=F(K,W)
  hmac_digest((uint8_t *)keyword.c_str(), keyword.size(), key, AES_BLOCK_SIZE,
              Kw);
  uint8_t S[DIGEST_SIZE];

  // proof
  uint8_t proof[DIGEST_SIZE];
  memset(proof, 0, DIGEST_SIZE);
  if (op == INS) {
    // generate st_{c}
    st1 = e();
    // cout << "Client st1: " << st1 << endl;
    //  h(stc)
    // auto t3 = high_resolution_clock::now();
    sha256_digest((uint8_t *)&st1, sizeof(int), h1);
    // auto t4 = high_resolution_clock::now();

    // cout << "Client h: ";
    // arrPrint((char*)h1, DIGEST_SIZE);

    // generate S, S = h(stc) oplus (stc-1 || |in|)
    memcpy(S, h1, DIGEST_SIZE);

    int size = inds.size();
    uint8_t sizeAndSt1[DIGEST_SIZE];
    memset(sizeAndSt1, 0, DIGEST_SIZE);

    memcpy(sizeAndSt1, (uint8_t *)&st, sizeof(int));
    memcpy(sizeAndSt1 + sizeof(int), (uint8_t *)&size, sizeof(int));
    strXor(S, S, sizeAndSt1, DIGEST_SIZE);

    // cout << "Client sizeAndst: ";
    // arrPrint((char*)sizeAndSt1, DIGEST_SIZE);

    string idxs((char *)h1, DIGEST_SIZE);
    string strS((char *)S, DIGEST_SIZE);

    // cout << "Client masked S: " << strS << endl;
    // auto t5 = high_resolution_clock::now();
    server->add_entries(idxs, strS, 1);
    // auto t6 = high_resolution_clock::now();

    // auto d3 = duration_cast<microseconds>(t6 - t5).count();
    // cout << "upload time : " << d3 << endl;

    st = st1;
  }
  auto t2 = high_resolution_clock::now();
  // cout << "gen S and new state : " << duration_cast<microseconds>(t2 -
  // t1).count() << endl;

  // Gen idxct, ct
  for (int i = 0; i < inds.size(); i++) {
    // generate tag t=h(K_w||ind)
    uint8_t tagTmp[DIGEST_SIZE + sizeof(int)];
    memcpy(tagTmp, Kw, DIGEST_SIZE);
    memcpy(tagTmp + DIGEST_SIZE, (uint8_t *)&inds[i], sizeof(int));
    uint8_t tag[DIGEST_SIZE];
    sha256_digest(tagTmp, DIGEST_SIZE + sizeof(int), tag);

    // auto t8 = high_resolution_clock::now();
    if (op == INS) {

      // gen idxct=h(stc,i)
      uint8_t idxct[DIGEST_SIZE];
      uint8_t idxctTmp[2 * sizeof(int)];
      memcpy(idxctTmp, (uint8_t *)&st1, sizeof(int));
      memcpy(idxctTmp + sizeof(int), (uint8_t *)&i, sizeof(int));
      sha256_digest(idxctTmp, 2 * sizeof(int), idxct);

      // get all offsets in BF

      vector<long> indexes =
          BloomFilter<32, GGM_SIZE, HASH_SIZE>::get_index(tag);
      sort(indexes.begin(), indexes.end());

      // get SRE ciphertext list
      string ciphertexts = "";
      // cout << "index size : " << indexes.size() << endl;
      // for(long index : indexes) {
      //  derive a key from the offset
      uint8_t derived_key[AES_BLOCK_SIZE];
      memcpy(derived_key, key, AES_BLOCK_SIZE);
      // auto t3 = high_resolution_clock::now();
      GGMTree::derive_key_from_tree(derived_key, indexes[0], tree->get_level(),
                                    0);
      // auto t4 = high_resolution_clock::now();
      // cout << "GGMtree time : " << duration_cast<microseconds>(t4 -
      // t3).count() << endl;

      // use the key to encrypt the id
      uint8_t encrypted_id[AES_BLOCK_SIZE + sizeof(int)];
      memcpy(encrypted_id, iv, AES_BLOCK_SIZE);
      aes_encrypt((uint8_t *)&(inds[i]), sizeof(int), derived_key, encrypted_id,
                  encrypted_id + AES_BLOCK_SIZE);
      // save the encrypted id in the list
      ciphertexts += string((char *)encrypted_id, AES_BLOCK_SIZE + sizeof(int));

      // cout << "Client cipher : ";
      // arrPrint(ciphertexts.c_str(), ciphertexts.size());
      //  convert tag/label to string
      string tag_str((char *)tag, DIGEST_SIZE);
      string idxct_str((char *)idxct, DIGEST_SIZE);
      // save the list on the server forward-add
      // (ct1,ct2,......,ctn), t
      server->add_entries(idxct_str, tag_str, 2);
      server->add_entries(idxct_str, ciphertexts, 3);
    } else {
      // insert the tag into BF
      delete_bf.add_tag(tag);
    }
  }
  auto t3 = high_resolution_clock::now();
  // cout << "gen cipher time : " << duration_cast<microseconds>(t3 -
  // t2).count() << endl;
  //  output
  // cout << "Client update st : " << st << endl;

  // auto t12 = high_resolution_clock::now();
  // auto d6 = duration_cast<microseconds>(t12 - t1).count();
  // cout << "total time : " << d6 << endl;
  return;
}

void SSEClient::search(vector<int> &res, const string &keyword) {
  // std::cout <<
  // duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  // << " : "
  //  << "SSEClient search!" << std::endl;

  // gen Kw
  uint8_t Kw[DIGEST_SIZE];
  hmac_digest((uint8_t *)keyword.c_str(), keyword.size(), key, AES_BLOCK_SIZE,
              Kw);

  // search all deleted positions
  vector<long> bf_pos;
  bf_pos.reserve(GGM_SIZE);
  for (int i = 0; i < GGM_SIZE; ++i) {
    bf_pos.emplace_back(i);
  }
  vector<long> delete_pos = delete_bf.search();
  vector<long> remain_pos;
  set_difference(bf_pos.begin(), bf_pos.end(), delete_pos.begin(),
                 delete_pos.end(), inserter(remain_pos, remain_pos.begin()));

  // generate GGM Node for the remain position
  vector<GGMNode> node_list;
  node_list.reserve(remain_pos.size());
  for (long pos : remain_pos) {
    node_list.emplace_back(GGMNode(pos, tree->get_level()));
  }
  vector<GGMNode> remain_node = tree->min_coverage(node_list);
  vector<SerialisedNode> searialised_list;
  searialised_list.reserve(remain_node.size());

  // compute the key set, serialise it and send to the server
  for (auto &i : remain_node) {
    memcpy(i.key, key, AES_BLOCK_SIZE);
    GGMTree::derive_key_from_tree(i.key, i.index, i.level, 0);
    SerialisedNode sn;
    sn.level = i.level;
    sn.index = i.index;
    sn.key = string((char *)i.key, AES_BLOCK_SIZE);
    searialised_list.emplace_back(sn);
  }
  // give all results to the server for search

  // std::cout << "Stab 1" << std::endl;
  //     cout <<
  //     duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  //     << endl;
  server->search(res, st, string((char *)Kw, DIGEST_SIZE), searialised_list,
                 tree->get_level());

  // std::cout << "Stab 2" << std::endl;
  //     cout <<
  //     duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()
  //     << endl;

  // server->verify(res, delete_bf.to_string(), string((char*) Kw,
  // DIGEST_SIZE));
  return;
}