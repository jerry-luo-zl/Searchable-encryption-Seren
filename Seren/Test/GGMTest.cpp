//
// Created by shangqi on 2020/11/12.
//

#include "GGMTree.h"

#define TREE_SIZE 8

int main() {
    GGMTree tree(TREE_SIZE);

    // add test nodes
    vector<GGMNode> node_list;
    node_list.emplace_back(GGMNode(0, tree.get_level()));
    node_list.emplace_back(GGMNode(1, tree.get_level()));
    node_list.emplace_back(GGMNode(3, tree.get_level()));
    node_list.emplace_back(GGMNode(5, tree.get_level()));

    // compute min coverage
    vector<GGMNode> coverage = tree.min_coverage(node_list);

    return 0;
}