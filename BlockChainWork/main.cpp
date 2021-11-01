#include "test.h"

using namespace blockchain;
int main() {
    blockchain::Test t;
//    t.TestAll();    // 执行一下所有测试

    // -----------  实验1 测试  -----------
    // 测试6个交易生成默克尔树
    t.TestMerkleTreeSixTx();
    // 测试9个交易生成默克尔树
    t.TestMerkleTreeNineTx();

    // -----------  实验2 测试  -----------
    // 测试钱包密钥的正确性
    t.TestWalletKeys();

    // -----------  实验3 测试  -----------
    // 测试签名与验证
    t.TestSignAndVerify();

    // -----------  实验4 测试  -----------
    // 测试交易的序列化与交易hash生成
    t.TestTransactionSerializeAndHash();
    // 测试交易生成，锁定脚本，解锁脚本等
    t.TestGenerateTransaction();
    // 上一个只测试了单个输入，单个输出的交易逻辑，这个测试多个输入，多个输出的交易生成
    t.TestGenerateComplexTransaction();
    // 测试1000个交易生成
    t.TestGenerate1000Tx();

    // -----------  实验5 测试  -----------
    // 测试矿工挖矿
    t.TestMiner();
    // 测试 模拟挖矿，挖出10个区块
    t.Miner10Block();

}


