//
// Created by 张宇轩 on 2021/10/26.
//

#ifndef BLOCKCHAINWORK_MERKLETREE_H
#define BLOCKCHAINWORK_MERKLETREE_H

#include <vector>
#include <queue>
#include <string>

namespace blockchain{
    class MerkleTree {
    public:
        std::string hash;           // 节点的hash值
        MerkleTree* leftChild;      // 左子树
        MerkleTree* rightChild;     // 右子树

        // 构造函数
        MerkleTree(std::string v, MerkleTree* left, MerkleTree* right);
        // 析构函数
        ~MerkleTree();
        // 释放内存
        static void DeleteRoot(MerkleTree* root);

        // 输入hash的集合，返回MerkleTree Root
        static MerkleTree* MakeMerkleTree(const std::vector<std::string>& vec);
        static MerkleTree* MakeMerkleTree(const std::string pStr[], int len);
    private:
        // 利用队列这一数据结构，来实现MerkleTree的构造
        static MerkleTree* MakeMerkleTree(std::queue<std::string>& q);
    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_MERKLETREE_H
