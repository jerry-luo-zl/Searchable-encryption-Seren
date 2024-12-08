//
// Created by shangqi on 2020/6/20.
//

#ifndef AURA_GGMTREE_H
#define AURA_GGMTREE_H

#include <bitset>
#include <cmath>
#include <cstring>
#include <vector>

#include "GGMNode.h"
extern "C" {
#include "CommonUtil.h"
}

using namespace std;

class GGMTree {
private:
    int level;

public:
    explicit GGMTree(long num_node);
    void static derive_key_from_tree(uint8_t *current_key, long offset, int start_level, int target_level);
    vector<GGMNode> min_coverage(vector<GGMNode> node_list);
    int get_level() const;
};


#endif //AURA_GGMTREE_H
