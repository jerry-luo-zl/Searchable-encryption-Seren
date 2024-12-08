//
// Created by shangqi on 2020/6/17.
//

#include "SSEServerHandler.h"

template <typename T>
void printVector(vector<T> arr) {
    for (auto a : arr) {
        cout << a << " ";
    }
    cout << endl;
}

SSEServerHandler::~SSEServerHandler() {
    delete dbTag;
    delete dbCt;
    delete dbPf;
    delete dbS;
}

SSEServerHandler::SSEServerHandler() {
    //std::cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() 
    //        << " : " << "Create SSESeverHandler!" << std::endl;
    // open dbs
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, "./tagDB", &dbTag);
    assert(status.ok());

    status = rocksdb::DB::Open(options, "./CtDB", &dbCt);
    assert(status.ok());

    status = rocksdb::DB::Open(options, "./PfDB", &dbPf);
    assert(status.ok());

    status = rocksdb::DB::Open(options, "./sDB", &dbS);
    assert(status.ok());
}


void SSEServerHandler::add_entries(const string& key, const string& val, const int& flag) {
    rocksdb::Status status;
    // std::cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() 
      //      << " : " << "SSEServerHandler addentry!" << std::endl;
    switch (flag) {
        case 1:
            // add S
            status = dbS->Put(rocksdb::WriteOptions(), key, val);
            assert(status.ok());
            break;
        case 2:
            // add S
            status = dbTag->Put(rocksdb::WriteOptions(), key, val);
            assert(status.ok());
            break;
        case 3:
            // add ct
            status = dbCt->Put(rocksdb::WriteOptions(), key, val);
            assert(status.ok());
            break;
        case 4:
            // add pf
            status = dbPf->Put(rocksdb::WriteOptions(), key, val);
            assert(status.ok());
            break;
    }
}

void SSEServerHandler::search(vector<int> & searchRes, const int& st, const string& Kw, const vector<GGMNode>& node_list, 
                            int level) {

    // pre-search, derive all keys
    keys.clear();
    compute_leaf_keys(node_list, level);
    
    vector< vector<int> > addInds;
    vector< vector<string> > addTags;
    vector< vector<string> > delTags;
    vector< int > sts;

    int st1 = st;

    while (st1 != 0) { // 直到st为0为止
        
        //cout << "SeverHandler search st : " << st1 << endl;
        rocksdb::Status status;

        // retrieve S
        // h1(stc)
        uint8_t h1[DIGEST_SIZE];
        sha256_digest((uint8_t*)&st1, sizeof(int), h1);

        string idxs_str((char*)h1, DIGEST_SIZE);
        string S;
        status = dbS->Get(rocksdb::ReadOptions(), idxs_str, &S);
        assert(status.ok());
        //cout << "SeverHandler masked S: " << S << endl;

        // depart st_{c-1} and |in_c|
        //cout << "SeverHandler h: ";
        //arrPrint((char*)h1, DIGEST_SIZE);

        uint8_t sizeAndSt1[DIGEST_SIZE];
        strXor(sizeAndSt1, (uint8_t*)S.c_str(), h1, DIGEST_SIZE);

        //cout << "SeverHandler sizeAndst: ";
        //arrPrint((const char*)sizeAndSt1, DIGEST_SIZE);

        uint8_t st0[sizeof(int)];
        memcpy(st0, sizeAndSt1, sizeof(int));
        uint8_t size[sizeof(int)];
        memcpy(size, sizeAndSt1 + sizeof(int), sizeof(int));
        int sizeCt = convertArrayToInt(size, sizeof(int));
        // cout << "SeverHandler pre st: " <<convertArrayToInt(st0, sizeof(int)) << endl;
        // cout << "SeverHandler size: " << sizeCt << endl;

        
        vector<int> addIndc;
        vector<string> addTagc;
        vector<string> delTagc;
        
        // (find (ct1,....,ct_q), t)
        for (int i = 0; i < sizeCt; i++) {
            
            // compute idx
            // gen idxct=h(stc,i)
            uint8_t idxct[DIGEST_SIZE];
            uint8_t idxctTmp[2 * sizeof(int)];
            memcpy(idxctTmp, (uint8_t*)&st1, sizeof(int));
            memcpy(idxctTmp + sizeof(int), (uint8_t*)&i ,sizeof(int));
            sha256_digest(idxctTmp, 2 * sizeof(int), idxct);
            string idxct_str((char*)idxct, DIGEST_SIZE);

            // get the result
            string cipher;
            status = dbCt->Get(rocksdb::ReadOptions(), idxct_str, &cipher);
            assert(status.ok());
            //cout << "SeverHandler cipher: ";
            //arrPrint(cipher.c_str(), cipher.size());

            // spilt ciphertexts
            vector<string> ciphertext_list;
            for (int j = 0; j < cipher.size() / (AES_BLOCK_SIZE + sizeof(int)); j++) {
                string tmp = cipher.substr((AES_BLOCK_SIZE + sizeof(int)) * j, AES_BLOCK_SIZE + sizeof(int));
                ciphertext_list.push_back(tmp);
            }

            // fetch tag
            string tag;
            status = dbTag->Get(rocksdb::ReadOptions(), idxct_str, &tag);
            assert(status.ok());
    
            // get the insert position of the tag
            vector<long> search_pos = BloomFilter<32, GGM_SIZE, HASH_SIZE>::get_index((uint8_t*) tag.c_str());
            sort(search_pos.begin(), search_pos.end());
            // derive the key from search position and decrypt the id
            // min(search_pos.size(), ciphertext_list.size())
            for (int j = 0; j < 1; ++j) {
                uint8_t res[4];
                if(keys[search_pos[j]] == nullptr) {
                    delTagc.emplace_back(tag);
                    break;
                }
                
                aes_decrypt((uint8_t *) (ciphertext_list[j].c_str() + AES_BLOCK_SIZE),
                        ciphertext_list[j].size() - AES_BLOCK_SIZE, keys[search_pos[j]], 
                        (uint8_t *) ciphertext_list[j].c_str(), res);

                if(*((int*) res) >= 0) {
                    //cout << "res : " << *((int*) res) << endl;
                    //arrPrint((char*)res, 4);
                    addIndc.emplace_back(*((int*) res));
                    searchRes.emplace_back(*((int*) res));
                    addTagc.emplace_back(tag);
                }
                break;
            }
        }
        //cout << "st : " << st1 << ", inds : ";
        //printVector<int>(addIndc);
        
        sts.emplace_back(st1);
        addInds.emplace_back(addIndc);
        addTags.emplace_back(addTagc);
        delTags.emplace_back(delTagc);
        st1 = convertArrayToInt(st0, sizeof(int)); // st<-st_{c-1}
       
    }
    ret.sts = sts;
    ret.addInds = addInds;
    ret.addTags = addTags;
    ret.delTags = delTags;
    return;
}

void SSEServerHandler::verify(vector<int>& res, const string& D, const string& Kw) {
    
    //cout << "Search result : ";
    //printVector<int>(res);
}

void SSEServerHandler::compute_leaf_keys(const vector<GGMNode>& node_list, int level) {
    for(GGMNode node : node_list) {
        for (int i = 0; i < pow(2, level - node.level); ++i) {
            int offset = ((node.index) << (level - node.level)) + i;
            uint8_t derive_key[AES_BLOCK_SIZE];
            memcpy(derive_key, node.key, AES_BLOCK_SIZE);
            GGMTree::derive_key_from_tree(derive_key,  offset, level - node.level, 0);
            if(keys.find(offset) == keys.end()) {
                keys[offset] = (uint8_t*) malloc(AES_BLOCK_SIZE);
                memcpy(keys[offset], derive_key, AES_BLOCK_SIZE);
            }
        }
    }
}
