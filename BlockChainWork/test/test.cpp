//
// Created by 张宇轩 on 2021/10/27.
//

#include "DataBase.h"
#include "wallet/SimpleWallet.h"
#include "test.h"
#include "MerkleTree.h"
#include "wallet/WalletKeys.h"
#include "transaction/Transaction.h"
#include "transaction/TxIn.h"
#include "transaction/TxOut.h"
#include "util.h"
#include <string>
#include <vector>
#include <iostream>
#include <block/Miner.h>
#include <unordered_map>
#include <hex.h>

using namespace std;
using namespace blockchain;

namespace blockchain{
    void Test::TestAll() {
        // -----------  实验1测试  -----------
        // 测试6个交易生成默克尔树
        TestMerkleTreeSixTx();
        // 测试9个交易生成默克尔树
        TestMerkleTreeNineTx();

        // -----------  实验2测试  -----------
        // 测试钱包密钥的正确性
        TestWalletKeys();

        // -----------  实验3测试  -----------
        // 测试签名与验证
        TestSignAndVerify();

        // -----------  实验4测试  -----------
        // 测试交易的序列化与交易hash生成
        TestTransactionSerializeAndHash();
        // 测试交易生成，锁定脚本，解锁脚本等
        TestGenerateTransaction();
        // 上一个只测试了单个输入，单个输出的交易逻辑，这个测试多个输入，多个输出的交易生成
        TestGenerateComplexTransaction();
        // 测试1000个交易生成
        TestGenerate1000Tx();

        // -----------  实验5测试  -----------
        // 测试矿工挖矿
        TestMiner();
        // 测试 模拟挖矿，挖出10个区块
        Miner10Block();
    }

    bool Test::IsTestTransaction = false;

    void Test::TestMerkleTreeSixTx() {
        // 来源：https://live.blockcypher.com/btc/block/000000000000b0b8b4e8105d62300d63c8ec1a1df0af1c2cdbd943b156a8cd79/
        string merkleRoot = "a5c67f0b44c1a342dec9135a566fd93f0c728452d1dbf850f4f9a47af82aed77";
        string txs[6] = {
                "ab7ed423933fe5413dc51b1041a58fd8af0cd70491b1ce607cb41dddce74a92b",
                "c763e59a79c9f1e626ddf1c3e9f20f234959c457ff82918f7b24e9d18a12db99",
                "80ea3e647aeef92973f5414e0caa1721c7b42345b99ed161dd505acc41f5516b",
                "1b72eefd70ce0a362ec0cb48e2213274df3c55f9dabd5806cdc087a335498cd3",
                "23e13370f6d59e2d7c7a9ca604b872312de34a387bd7deca2c8f4486f7e66173",
                "149a098d6261b7f9359a572d797c4a41b62378836a14093912618b15644ba402"
        };
        for(int i=0; i < 6; i++){
            txs[i] = Util::ReverseByteOrder(txs[i]);     // 将显示的大端转为小端
        }

        MerkleTree* root = MerkleTree::MakeMerkleTree(txs, 6);
        Util::ToUpper(merkleRoot);        // 因为构造出来的默克尔树都是写的，这里将对照的hash值也转换成大写

        // 开始验证
        cout << "默克尔树构造验证（6个交易构造测试）：开始！" << endl;
        cout << "目标值：" << merkleRoot << endl;
        cout << "实际值：" << Util::ReverseByteOrder(root->hash) << endl;

        if (Util::ReverseByteOrder(root->hash) == merkleRoot) {     // 构造出来的root 是小端模式，所以将对照值也转换成小端
            cout << "默克尔树构造验证（6个交易构造测试）：成功！" << endl;
        } else {
            cout << "默克尔树构造验证（6个交易构造测试）：失败！" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 释放空间
        MerkleTree::DeleteRoot(root);
    }

    void Test::TestMerkleTreeNineTx() {
        // 来源：https://live.blockcypher.com/btc/block/0000000000013b8ab2cd513b0261a14096412195a72a0c4827d229dcc7e0f7af/
        string merkleRoot = "2fda58e5959b0ee53c5253da9b9f3c0c739422ae04946966991cf55895287552";
        vector<string> vec{
           "ef1d870d24c85b89d92ad50f4631026f585d6a34e972eaf427475e5d60acf3a3",
           "f9fc751cb7dc372406a9f8d738d5e6f8f63bab71986a39cf36ee70ee17036d07",
           "db60fb93d736894ed0b86cb92548920a3fe8310dd19b0da7ad97e48725e1e12e",
           "220ebc64e21abece964927322cba69180ed853bb187fbc6923bac7d010b9d87a",
           "71b3dbaca67e9f9189dad3617138c19725ab541ef0b49c05a94913e9f28e3f4e",
           "fe305e1ed08212d76161d853222048eea1f34af42ea0e197896a269fbf8dc2e0",
           "21d2eb195736af2a40d42107e6abd59c97eb6cffd4a5a7a7709e86590ae61987",
           "dd1fd2a6fc16404faf339881a90adbde7f4f728691ac62e8f168809cdfae1053",
           "74d681e0e03bafa802c8aa084379aa98d9fcd632ddc2ed9782b586ec87451f20",
        };
        for(int i=0; i < vec.size(); i++){
            vec[i] = Util::ReverseByteOrder(vec[i]);     // 将显示的大端转为小端
        }
        MerkleTree* root = MerkleTree::MakeMerkleTree(vec);
        Util::ToUpper(merkleRoot);

        // 开始验证
        cout << "默克尔树构造验证（9个交易构造测试）：开始！" << endl;
        cout << "目标值：" << merkleRoot << endl;
        cout << "实际值：" << Util::ReverseByteOrder(root->hash) << endl;
        if (Util::ReverseByteOrder(root->hash) == merkleRoot) {
            cout << "默克尔树构造验证（9个交易构造测试）：成功！" << endl;
        } else {
            cout << "默克尔树构造验证（9个交易构造测试）：失败！" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 释放空间
        MerkleTree::DeleteRoot(root);
    }

    void Test::TestWalletKeys() {
        // 测试数据来源：《精通比特币》 -> 用Python实现密钥和比特币地址
        // https://github.com/tianmingyun/MasterBitcoin2CN/blob/master/ch04.md#413-%E7%A7%81%E9%92%A5
        WalletKeys keys("3aba4162c7251c891207b747840551a71939b0de081f85c4e44cf7c13e41daa6");

        cout << "钱包密钥算法实现正确性测试：开始！" << endl;
        string hexPriKey = "3aba4162c7251c891207b747840551a71939b0de081f85c4e44cf7c13e41daa6";
        Util::ToUpper(hexPriKey);
        string wifPriKey = "5JG9hT3beGTJuUAmCQEmNaxAuMacCTfXuw1R3FCXig23RQHMr4K";
        string wifCompressedPriKey = "KyBsPXxTuVD82av65KZkrGrWi5qLMah5SdNq6uftawDbgKa2wv6S";
        string hexPubKey = "045c0de3b9c8ab18dd04e3511243ec2952002dbfadc864b9628910169d9b9b00ec243bcefdd4347074d44bd7356d6a53c495737dd96295e2a9374bf5f02ebfc176";
        Util::ToUpper(hexPubKey);
        string compHexPubKey = "025c0de3b9c8ab18dd04e3511243ec2952002dbfadc864b9628910169d9b9b00ec";
        Util::ToUpper(compHexPubKey);
        string address = "1thMirt546nngXqyPEz532S8fLwbozud8";
        string compAddress = "14cxpo3MBCYYWCgF74SWTdcmxipnGUsPw3";

        // 私钥 Hex编码测试
        cout << "Test Private Key (hex) is:" << endl;
        cout << hexPriKey << endl;
        cout << "Real Private Key (hex) is:" << endl;
        cout << keys.GetHexPrivateKey() << endl;
        if (hexPriKey == keys.GetHexPrivateKey()) {
            cout << "Private Key (hex) 测试：通过!" << endl;
        } else {
            cout << "Private Key (hex) 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 私钥 WIF格式 测试
        cout << "Test Private Key (WIF) is:" << endl;
        cout << wifPriKey << endl;
        cout << "Real Private Key (WIF) is:" << endl;
        cout << keys.GetWIFPrivateKey() << endl;
        if (wifPriKey == keys.GetWIFPrivateKey()) {
            cout << "Private Key (WIF) 测试：通过!" << endl;
        } else {
            cout << "Private Key (WIF) 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 私钥 WIF-Compressed 测试
        cout << "Test Private Key (WIF-Compressed) is:" << endl;
        cout << wifCompressedPriKey << endl;
        cout << "Real Private Key (WIF-Compressed) is:" << endl;
        cout << keys.GetWIFCompressedPrivateKey() << endl;
        if (wifCompressedPriKey == keys.GetWIFCompressedPrivateKey()) {
            cout << "Private Key (WIF-Compressed) 测试：通过!" << endl;
        } else {
            cout << "Private Key (WIF-Compressed) 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 未压缩公钥 Hex编码 测试
        cout << "Test Public Key (hex) is:" << endl;
        cout << hexPubKey << endl;
        cout << "Real Public Key (hex) is:" << endl;
        cout << keys.GetHexPublicKey() << endl;
        if (hexPubKey == keys.GetHexPublicKey()) {
            cout << "Public Key (hex) 测试：通过!" << endl;
        } else {
            cout << "Public Key (hex) 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 压缩公钥 Hex编码测试
        cout << "Test Compressed Public Key (hex) is:" << endl;
        cout << compHexPubKey << endl;
        cout << "Real Compressed Public Key (hex) is:" << endl;
        cout << keys.GetCompressedHexPublicKey() << endl;
        if (compHexPubKey == keys.GetCompressedHexPublicKey()) {
            cout << "Compressed Public Key (hex) 测试：通过!" << endl;
        } else {
            cout << "Compressed Public Key (hex) 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 未压缩比特币地址 Base58check 测试
        cout << "Test Bitcoin Address (b58check) is:" << endl;
        cout << address << endl;
        cout << "Real Bitcoin Address (b58check) is:" << endl;
        cout << keys.GetBase58CheckAddress() << endl;
        if (address == keys.GetBase58CheckAddress()) {
            cout << "Bitcoin Address (b58check) 测试：通过!" << endl;
        } else {
            cout << "Bitcoin Address (b58check) 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 压缩比特币地址 Base58check 测试
        cout << "Test Compressed Bitcoin Address (b58check) is:" << endl;
        cout << compAddress << endl;
        cout << "Real Compressed Bitcoin Address (b58check) is:" << endl;
        cout << keys.GetCompressedBase58CheckAddress() << endl;
        if (compAddress == keys.GetCompressedBase58CheckAddress()) {
            cout << "Compressed Bitcoin Address (b58check) 测试：通过!" << endl;
        } else {
            cout << "Compressed Bitcoin Address (b58check) 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 解码Base58check 测试
        cout << "Compressed Bitcoin Address (b58check) is" << endl;
        cout << compAddress << endl;
        cout << "Test Decode Compressed Bitcoin Address (Hex) is:" << endl;
        cout << keys.GetHexCompressedRawAddress() << endl;
        cout << "Real Decode Compressed Bitcoin Address (Hex) is:" << endl;
        std::string decode = keys.DecodeBase58check(compAddress);
        cout << decode << endl;
        if (keys.GetHexCompressedRawAddress() == decode) {
            cout << "解码Base58check 测试：通过!" << endl;
        } else {
            cout << "解码Base58check 测试：失败!" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        cout << "钱包密钥算法实现正确性测试：结束！" << endl;
        cout << "-----------------------------------------------" << endl;

    }

    void Test::TestSignAndVerify() {
        cout << "签名与验证测试：开始！" << endl;

        WalletKeys keys;
        string message = "blockchain-ss-2021";
        string signature = keys.Sign(message);
        cout << "message :" << message << endl;
        cout << "signature (Hex) :" << Util::HexEncode(signature) << endl;
        bool result = keys.VerifyMessage(message, signature);
        cout << "Verify result :" << boolalpha << result << endl;

        if (result) {
            cout << "签名与验证测试：通过！" << endl;
        } else {
            cout << "签名与验证测试：失败！" << endl;
        }
        cout << "-----------------------------------------------" << endl;
    }

    // 测试交易的序列化与交易hash生成
    void Test::TestTransactionSerializeAndHash() {
        cout << "测试交易的序列化 与 交易TxID 哈希生成：开始！" << endl;

        // 测试交易的数据来源：https://wiki.bitcoinsv.io/index.php/TXID
        blockchain::Transaction tx;
        tx.version = 1;
        tx.lockTime = 598793;
        vector<blockchain::TxIn>  vin;
        TxIn txIn;
        txIn.sequence = 4294967294;
        // 因为在程序中都以小端形式保存，所以这里再转换下字节序，因为是从网页上拷贝过来的
        txIn.prevOutHash = Util::ReverseByteOrder("b8ed28aa87b92328e26a20553ac49fcb21e1f68daeb6cf7bcf4536e40503ffa8");
        txIn.prevOutIndex = 0;
        txIn.scriptSig = "4830450221008824eee04a2fbe62d2c3ee330eb2523b2c0188240714bb1d893aced1c454fa9a02202d32dbccc2af1c4b830795f2fa8cd569a06ee70cb9d836bbd510f0b45a47711b4121028580686976c0e6a7e44a78387913e2d7508ff2344d5f48669ba111dcd04170a8";
        vin.push_back(txIn);
        vector<TxOut> vout;
        TxOut txOut1,txOut2;
        txOut1.scriptPubKey = "76a9146b0a9ed05da7223a1fe57e1a4d307556f7d6200788ac";
        txOut1.value = 1800;
        txOut2.scriptPubKey = "76a914b993e512cb186f3f1c3f556a09716a1580eb99a188ac";
        txOut2.value = 90000;
        vout.push_back(txOut1);
        vout.push_back(txOut2);
        tx.vin = vin;
        tx.vout = vout;
        // 可以打印输出看下交易的JSON格式
//        cout << tx.MarshalJson() << endl;

        string txHex = "0100000001a8ff0305e43645cf7bcfb6ae8df6e121cb9fc43a55206ae22823b987aa28edb8000000006b4830450221008824"
                       "eee04a2fbe62d2c3ee330eb2523b2c0188240714bb1d893aced1c454fa9a02202d32dbccc2af1c4b830795f2fa8cd569a06e"
                       "e70cb9d836bbd510f0b45a47711b4121028580686976c0e6a7e44a78387913e2d7508ff2344d5f48669ba111dcd04170a8fe"
                       "ffffff0208070000000000001976a9146b0a9ed05da7223a1fe57e1a4d307556f7d6200788ac905f0100000000001976a914"
                       "b993e512cb186f3f1c3f556a09716a1580eb99a188ac09230900";
        Util::ToUpper(txHex);
        cout << "Test Tx Hex:" <<endl;
        cout << txHex << endl;
        string realHex = tx.Serizalize();
        cout << "Real Tx Hex:" <<endl;
        cout << realHex << endl;

        if (txHex == realHex) {
            cout << "交易序列化测试：成功！" << endl;
        } else {
            cout << "交易序列化测试：失败！" << endl;
            // 看一下哪一位出错了
            for (int i=0;i<txHex.size();i++) {
                if (txHex[i] != realHex[i]) {
                    cout << "index:" << i << "  thHex[i]="<< txHex[i] << " realHex[i]=" << realHex[i] << endl;
                }
            }
        }
        cout << "-----------------------------------------------" << endl;

        string TXID = "d8c5c42cbd1df7e48acab76fe05f2c9e612a20996fd37f4ffd4dc251385b6ba3";
        Util::ToUpper(TXID);
        cout << "Test TXID:" << endl;
        cout << TXID << endl;
        cout << "Real TXID:" << endl;
        string realTXID = Util::ReverseByteOrder(Util::DoubleSHA256(tx.Serizalize()));
        cout << realTXID  << endl;
        if (TXID == realTXID) {
            cout << "交易TXID测试：成功！" << endl;
        } else {
            cout << "交易TXID测试：失败！" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        cout << "测试交易的序列化 与 交易TxID 哈希生成：结束！" << endl;
        cout << "-----------------------------------------------" << endl;

    }

    // 生成测试用环境
    void Test::InitDataBaseOne() {
        UTXO utxo;
        utxo.txId = Util::ReverseByteOrder("5e2383defe7efcbdc9fdd6dba55da148b206617bbb49e6bb93fce7bfbb459d44");
        utxo.outputIndex = 1;
        utxo.value = 130000000;
        utxo.scriptPubKey = "76a914a235bdde3bb2c326f291d9c281fdc3fe1e956fe088ac";
        string pubKey = "a235bdde3bb2c326f291d9c281fdc3fe1e956fe0";
        Util::ToUpper(pubKey);
        unordered_map<string, UTXO> map;
        map[utxo.txId] = utxo;
        DataBase::UTXOList[pubKey] = map;
    }

    // 测试交易生成，锁定脚本，解锁脚本等
    void Test::TestGenerateTransaction() {
        Test::InitDataBaseOne();
        // 开始测试开关，使得签名的结果为给定值
        IsTestTransaction = true;

        cout << "交易生成测试（锁定脚本、交易Hash）：开始！" << endl;
        // 数据来源：https://medium.com/@bitaps.com/exploring-bitcoin-signing-the-p2pkh-input-b8b4d5c4809c
        std::string pk= "cThjSL4HkRECuDxUTnfAmkXFBEg78cufVBy3ZfEhKoxZo6Q38R5L";
        std::string privateKey = Util::DecodeBase58Check(pk);       // 使用给出的私钥
        privateKey = privateKey.substr(1, 32);
        WalletKeys keys(Util::HexEncode(privateKey));
//        cout << keys.GetTestCompBase58CheckAddress() << endl;             // mvJe9AfPLrxpfHwjLNjDAiVsFSzwBGaMSP

        SimpleWallet wallet;

        wallet.keys.clear();
        wallet.keys.push_back(keys);        // 把指定的key放入钱包，以确保只使用该密钥
        wallet.UpdateUTXO();
        Transaction* tx = wallet.SendMoneyWithBase58({129000000},
                                                     {"n4AYuETorj4gYKendz2ndm9QhjUuruZnfk"});

        // 锁定脚本测试
        string sigScript = "483045022100e15a8ead9013d1de55e71f195c9dc613483f07c8a0692a2144ffa90506436822022062bc9466b9e1941037fc23e1cfadf24c8833f96942beb8f4340df60d506f784b012103969A4AC9B1521CFAE44A929A614193B0467A20E0A15973CAE9BA1EFB9627D830";
        cout << "Test sigScript:" << endl;
        cout << sigScript << endl;
        cout << "Real sigScript:" << endl;
        string realSig = tx->vin[0].scriptSig;
        cout << realSig << endl;
        if (sigScript == realSig) {
            cout << "锁定脚本测试：成功！" << endl;
        } else {
            cout << "锁定脚本测试：失败！" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        // 交易序列化测试
        string txHex = "0100000001449d45bbbfe7fc93bbe649bb7b6106b248a15da5dbd6fdc9bdfc7efede83235e010000006b483045022100e15a8ead9013d1de55e71f195c9dc613483f07c8a0692a2144ffa90506436822022062bc9466b9e1941037fc23e1cfadf24c8833f96942beb8f4340df60d506f784b012103969a4ac9b1521cfae44a929a614193b0467a20e0a15973cae9ba1efb9627d830ffffffff014062b007000000001976a914f86f0bc0a2232970ccdf4569815db500f126836188ac00000000";
        Util::ToUpper(txHex);
        cout << "Test Transaction Hex:" << endl;
        cout << txHex << endl;
        cout << "Real Transaction Hex:" << endl;
        string realHex= tx->Serizalize();
        cout << realHex << endl;
        if (txHex == realHex) {
            cout << "交易序列化（Hex）测试：成功！" << endl;
        } else {
            cout << "交易序列化（Hex）测试：失败！" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        string txHash = "4484ec8b4801ada92fc4d9a90bb7d9336d02058e9547d027fa0a5fc9d2c9cc77";
        string realHash = Util::ReverseByteOrder(tx->GetTxID());
        Util::ToLower(realHash);
        cout << "Test Transaction Hash:" << endl;
        cout << txHash << endl;
        cout << "Real Transaction Hash:" << endl;
        cout << realHash << endl;
        if (txHash == realHash) {
            cout << "交易Hash值（Hex）测试：成功！" << endl;
        } else {
            cout << "交易Hash值（Hex）测试：失败！" << endl;
        }
        cout << "-----------------------------------------------" << endl;

        cout << "生成交易JSON格式：" << endl;
        cout << tx->MarshalJson() << endl;
        cout << "交易生成测试（锁定脚本、交易Hash）：结束！" << endl;
        // 关闭特殊测试开关
        IsTestTransaction = false;
        cout << "-----------------------------------------------" << endl;
    }

    // 生成测试用环境 - 2 ; 用于测试多输入多输出交易
    void Test::InitDataBaseTwo() {
        // 参考数据来源：https://live.blockcypher.com/btc-testnet/tx/f0d9d482eb122535e32a3ae92809dd87839e63410d5fd52816fc9fc6215018cc/#spentby-1dda832890f85288fec616ef1f4113c0c86b7bf36b560ea244fd8a6ed12ada52-1
        UTXO utxo1;
        utxo1.txId = Util::ReverseByteOrder("1dda832890f85288fec616ef1f4113c0c86b7bf36b560ea244fd8a6ed12ada52");
        utxo1.outputIndex = 1;
        utxo1.value = 40000000;
        utxo1.scriptPubKey = "76a914a235bdde3bb2c326f291d9c281fdc3fe1e956fe088ac";
        string pubKey = "a235bdde3bb2c326f291d9c281fdc3fe1e956fe0";
        Util::ToUpper(pubKey);
        unordered_map<string, UTXO> map;
        map[utxo1.txId] = utxo1;

        UTXO utxo2;
        utxo2.txId = Util::ReverseByteOrder("24f284aed2b9dbc19f0d435b1fe1ee3b3ddc763f28ca28bad798d22b6bea0c66");
        utxo2.outputIndex = 1;
        utxo2.value = 110000000;
        utxo2.scriptPubKey = "76a914a235bdde3bb2c326f291d9c281fdc3fe1e956fe088ac";
        map[utxo2.txId] = utxo2;
        DataBase::UTXOList[pubKey] = map;
    }

    // 上一个只测试了单个输入，单个输出的交易逻辑，这个测试多个输入，多个输出的交易生成
    void Test::TestGenerateComplexTransaction() {
        InitDataBaseTwo();          // 对指定的公钥地址 构造UTXO

        cout << "复杂交易生成测试（多输入，多输出）：开始！" << endl;
        std::string pk= "cThjSL4HkRECuDxUTnfAmkXFBEg78cufVBy3ZfEhKoxZo6Q38R5L";
        std::string privateKey = Util::DecodeBase58Check(pk);       // 使用给出的私钥
        privateKey = privateKey.substr(1, 32);
        WalletKeys keys(Util::HexEncode(privateKey));

        SimpleWallet wallet;
        wallet.keys.clear();
        wallet.keys.push_back(keys);        // 把指定的key放入钱包，以确保使用该密钥 对应的UTXO
        wallet.UpdateUTXO();

        Transaction* tx = wallet.SendMoneyWithBase58({29910240,120000000},
                                                     {"mx3KrUjRzzqYTcsyyvWBiHBncLrrTPXnkV",
                                                              "mxqnGTekzKqnMqNFHKYi8FhV99WcvQGhfH"});

        cout << "生成交易JSON格式：" << endl;
        cout << tx->MarshalJson() << endl;
        cout << "复杂交易生成测试（多输入，多输出）：结束！" << endl;
        cout << "-----------------------------------------------" << endl;
    }

    // 生成测试用环境 - 3 ; 用于测试1000个交易生成
    void Test::InitDataBaseThree(SimpleWallet& wallet) {
       wallet.keys.clear();
       wallet.keys.resize(1000);        // 生成1000个密钥对，对应1000个UTXO
       DataBase::UTXOList.clear();
       for (int i=0; i<1000; i ++ ){
           string pubKey = wallet.keys[i].GetHexCompressedRawAddress();
           Util::ToUpper(pubKey);
           UTXO utxo(40000000, pubKey);
           string tmp;
           while (tmp.size() < 32) {
               tmp = Util::GetRandomBytes(32);
           }
           string prevHash = Util::HexEncode(tmp);      // 随机生成
           utxo.txId = prevHash;
           utxo.outputIndex = 0;
           unordered_map<string, UTXO>& m = DataBase::UTXOList[pubKey];
           m[utxo.txId] = utxo;
       }
    }

    // 测试1000个交易生成
    void Test::TestGenerate1000Tx() {
        vector<SimpleWallet> users(1000);             // 每个钱包类似于账户，生成100个钱包
        SimpleWallet wallet;                             // 转账钱包，由他来转账，生成交易
        InitDataBaseThree(wallet);                    // 生成UTXO，供交易使用
        wallet.UpdateUTXO();

        cout << "测试：开始生成1000个交易..." << endl;
        cout << "展示前5个生成交易：" << endl;
        for (int i = 0; i< 1000; i++ ) {
            Transaction* tx =
                wallet.SendMoneyWithBase58({40000000}, {users[i].GetBitcoinAddr()});
            string txId = tx->GetTxID();
            DataBase::mempool[txId] = tx;               // 加入内存池
            DataBase::hashToTx[txId] = tx;              // 维护全局交易数据

            if (i < 5) {
                cout << "第" << i+1 << "个生成交易" << "，交易TXID: " << endl;
                cout << txId << endl;
                cout << "交易的JSON格式：" << endl;
                cout << tx->MarshalJson() << endl;
            }
        }
        cout << "1000个交易生成完毕！" << endl;
        cout << "-----------------------------------------------" << endl;
    }

    // 测试矿工挖矿算法正确性
    void Test::TestMiner() {
        // 测试数据来源：https://en.bitcoin.it/wiki/Block_hashing_algorithm
        // 或者是：https://live.blockcypher.com/btc/block/00000000000000001e8d6829a8a21adc5d38d0a473b144b6765798e61f98bd1d/
        cout << "测试矿工挖矿算法正确性：开始！" << endl;
        Block block;
        block.version = 1;
        block.hashMerkleRoot = Util::ReverseByteOrder("2b12fcf1b09288fcaff797d71e950e71ae42b91e8bdb2304758dfcffc2b620e3");
        block.hashPrevBlock = Util::ReverseByteOrder("00000000000008a3a41b85b8b29ad444def299fee21793cd8b9e567eab02cd81");
        block.time = 1305998791;
        block.bits = 440711666;
        block.nonce = 2504433986;
        Miner miner;
        bool result = miner.proveBlock(block);
        cout << "挖矿正确性验证测试结果："  <<  boolalpha << result << endl;
        cout << "-----------------------------------------------" << endl;
    }

    // 生成测试用环境 - 4 ; 用于测试创造区块
    void Test::InitDataBaseFour() {
        DataBase::clearAll();
        SimpleWallet wallet;
        wallet.keys.clear();
        wallet.keys.resize(1000);        // 生成1000个密钥对，对应1000个UTXO
        DataBase::UTXOList.clear();
        for (int i=0; i<1000; i ++ ){
            string pubKey = wallet.keys[i].GetHexCompressedRawAddress();
            Util::ToUpper(pubKey);
            UTXO utxo(40000000, pubKey);

            // 虚构交易
            Transaction* prevTx = new Transaction();
            prevTx->version = 1;
            prevTx->lockTime = 0;
            vector<TxIn> vecIn;
            vecIn.emplace_back();
            prevTx->vin = vecIn;
            vector<TxOut> vecOut;
            vecOut.emplace_back(40000000, pubKey);
            prevTx->vout = vecOut;

            string prevHash = prevTx->GetTxID();
            utxo.txId = prevHash;
            utxo.outputIndex = 0;
            unordered_map<string, UTXO>& m = DataBase::UTXOList[pubKey];
            m[utxo.txId] = utxo;
            DataBase::hashToTx[prevHash] = prevTx;
        }
        wallet.UpdateUTXO();

        vector<SimpleWallet> users(1000);             // 每个钱包类似于账户，生成100个钱包
        for (int i = 0; i< 1000; i++ ) {                 // 生成1000个交易
            Transaction* tx =
                    wallet.SendMoneyWithBase58({40000000}, {users[i].GetBitcoinAddr()});
            string txId = tx->GetTxID();
            DataBase::mempool[txId] = tx;               // 加入内存池
            DataBase::hashToTx[txId] = tx;              // 维护全局交易数据
        }
        // 加入创世区块
        Block* genenis = new Block();
        std::string blockHash = "0000000000000000000000000000000000000000000000000000000000000000";
        DataBase::BlockHash.push_back(blockHash);
        DataBase::hashToBlock[blockHash] = genenis;
    }


    // 测试 模拟挖矿，挖出10个区块
    void Test::Miner10Block() {
        InitDataBaseFour();                         // 初始化测试环境
        cout << "创造10个区块测试：开始！" << endl;
        Miner miner;
        for (int i = 0; i< 10; i++) {
            Block* block = miner.Mine();
            cout << "挖出第" << i+1 << "个区块" << endl;
            miner.proveBlock(*block);               // 验证区块hash正确性
//            cout << block->MarshalJson() << endl;   // 输出JSON格式
        }

        cout << "创造10个区块测试：结束！"  <<  endl;
        cout << "-----------------------------------------------" << endl;
    }


} // namespace blockchain
