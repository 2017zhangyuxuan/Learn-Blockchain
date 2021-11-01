//
// Created by 张宇轩 on 2021/10/27.
//

#ifndef BLOCKCHAINWORK_TEST_H
#define BLOCKCHAINWORK_TEST_H

#include "MerkleTree.h"
#include "wallet/SimpleWallet.h"

namespace blockchain{
    // 以后可以学习使用GTest 来进行单测
    class Test {
    public:
        //执行所有测试
        void TestAll();

        // 测试6个交易生成默克尔树
        void TestMerkleTreeSixTx();
        // 测试9个交易生成默克尔树
        void TestMerkleTreeNineTx();

        // 测试钱包密钥的正确性
        void TestWalletKeys();
        // 测试签名与验证
        void TestSignAndVerify();

        // 测试交易的序列化与交易hash生成
        void TestTransactionSerializeAndHash();

        // 生成测试用环境 - 1 ; 用于测试单个交易的生成
        void InitDataBaseOne();

        // 测试交易生成，锁定脚本，解锁脚本等
        void TestGenerateTransaction();

        static bool IsTestTransaction;

        // 生成测试用环境 - 2 ; 用于测试多输入多输出交易
        void InitDataBaseTwo();

        // 上一个只测试了单个输入，单个输出的交易逻辑，这个测试多个输入，多个输出的交易生成
        void TestGenerateComplexTransaction();

        // 生成测试用环境 - 3 ; 用于测试1000个交易生成
        void InitDataBaseThree(SimpleWallet& wallet);

        // 测试1000个交易生成
        void TestGenerate1000Tx();

        // 测试矿工挖矿
        void TestMiner();

        // 生成测试用环境 - 4 ; 用于测试创造区块
        void InitDataBaseFour();

        // 测试 模拟挖矿，挖出10个区块
        void Miner10Block();
    };
}


#endif //BLOCKCHAINWORK_TEST_H
