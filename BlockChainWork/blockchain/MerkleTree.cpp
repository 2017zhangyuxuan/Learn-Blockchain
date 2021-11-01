//
// Created by 张宇轩 on 2021/10/26.
//

#include "MerkleTree.h"
#include "util.h"
using namespace std;


namespace blockchain {
    // 构造函数
    MerkleTree::MerkleTree(std::string v, MerkleTree *left = nullptr, MerkleTree *right = nullptr) {
        hash = v;
        leftChild = left;
        rightChild = right;
    }

    void MerkleTree::DeleteRoot(MerkleTree *root) {
        if (root->leftChild == nullptr && root->rightChild == nullptr) {
            delete (root);
            return;
        }
        if (root->leftChild != nullptr) {
            DeleteRoot(root->leftChild);
        }
        if (root->rightChild != nullptr) {
            DeleteRoot(root->rightChild);
        }
        delete (root);
    }

    MerkleTree::~MerkleTree() {

    }

    MerkleTree *MerkleTree::MakeMerkleTree(const std::vector<std::string> &vec) {
        queue<string> q;
        for (const string &s: vec) {
            q.push(s);
        }
        return MakeMerkleTree(q);
    }

    MerkleTree *MerkleTree::MakeMerkleTree(const std::string pStr[], int len) {
        queue<string> q;
        for (int i = 0; i < len; i++) {
            q.push(pStr[i]);
        }
        return MakeMerkleTree(q);
    }

    MerkleTree *MerkleTree::MakeMerkleTree(std::queue<std::string> &q) {
        queue<MerkleTree *> merkle;

        // 将交易哈希 转换成叶子节点
        while (!q.empty()) {
            merkle.push(new MerkleTree(q.front()));
            q.pop();
        }

        MerkleTree *p1,*p2;   // 临时变量

        // 迭代地进行哈希
        while (merkle.size() > 1) {
            int size = merkle.size();
            int round = size / 2;   // 每层需要迭代的轮数
            bool flag = false;      // 特殊标记，当交易数为奇数时设置了true，表明最后一个元素需要进行复制
            if ((size & 1) > 0) {
                flag = true;
            }

            for (int i = 0; i < round; i++){
                p1 = merkle.front();
                merkle.pop();
                p2 = merkle.front();
                merkle.pop();

                // 拼接hash值，进行两次SHA256，生成新的节点
                merkle.push(new MerkleTree(Util::DoubleSHA256(p1->hash + p2->hash), p1, p2));
            }
            // 交易数为奇数，对最后一个元素进行复制
            if (flag) {
                p1 = merkle.front();
                merkle.pop();
                p2 = new MerkleTree(p1->hash);      // 只需要拷贝hash值即可
                merkle.push(new MerkleTree(Util::DoubleSHA256(p1->hash + p2->hash), p1, p2));
            }
        }

        // 最顶端的就是MerkleTree Root
        return merkle.front();
    }
}