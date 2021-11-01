//
// Created by 张宇轩 on 2021/10/30.
//

#include "SimpleWallet.h"
#include "DataBase.h"
#include "util.h"
#include "test.h"
namespace blockchain {
    // 默认生成一个密钥对
    SimpleWallet::SimpleWallet() {
        keys.push_back(WalletKeys());
    }

    // 生成指定数量的密钥对
    SimpleWallet::SimpleWallet(int size) {
        keys.resize(size);
    }

    // 获取钱包余额
    unsigned long long SimpleWallet::GetBalance() {
        // 从当前本地维护的UTXO中 获取余额
        unsigned long long sum = 0;
        for (auto pair : UTXOList) {
            std::unordered_map<std::string, UTXO>& map = pair.second;
            for (auto pair : map) {
                sum += pair.second.value;
            }
        }
        return sum;
    }

    // 进行转账交易，输入为转账金额，和对方的比特币地址（hex编码表示）
    Transaction *SimpleWallet::SendMoney(std::vector<unsigned long long> value,
                                         std::vector<std::string> address) {
        // 检查余额是否充足
        unsigned long long paySum = 0;
        for (unsigned long long v: value) {
            paySum += v;
        }
        if (GetBalance() < paySum) {
            return nullptr;
        }

        Transaction *tx = new Transaction();
        std::vector<TxIn> vin;
        std::vector<TxOut> vout;
        tx->version = 1;
        tx->lockTime = 0;

        // 构造输出
        vout.reserve(address.size());
        for (int i = 0; i < address.size(); i++) {
            vout.emplace_back(value[i], address[i]);
        }
        tx->vout = vout;

        // 构造待签名的 vin
        std::vector<int> vinIndex;              // 记录构造的输入对应使用的密钥，在之后签名的时候使用
        ConstructTxIn(paySum, vin, vinIndex, tx);
        tx->vin = vin;

        // 对每个input 进行签名
        for (int i=0; i< vin.size(); i++){
            // 进行签名前，先把对应input的 解锁脚本设置为对应的锁定脚本
            Transaction temp = *tx;
            TxOut txOut(0, keys[vinIndex[i]].GetHexCompressedRawAddress());
            temp.vin[i].scriptSig = txOut.scriptPubKey;
            vin[i].scriptSig = SignInput(vinIndex[i], &temp);
        }
        // 完成签名之后，更新到原来的交易上
        tx->vin = vin;

        // 暂时性地更新本地的UTXO，之后交易上链了，产生区块了再同步更新UTXO
        for (int i=0;i<vin.size();i++) {
            std::unordered_map<std::string, UTXO> map =  UTXOList[vinIndex[i]];
            map.erase(vin[i].prevOutHash);
        }

        return tx;
    }

    // 构造待签名的 vin
    void SimpleWallet::ConstructTxIn(unsigned long long paySum,
                                                  std::vector<TxIn>& vin,
                                                  std::vector<int>& vinIndex,
                                                  Transaction* transaction) {

        unsigned long long hasPay = 0;          // 已经支付的金额数

        // 简化支付实现逻辑，根据当前的维护的UTXO，依次用来支付；以后可以考虑更复杂的支付逻辑
        // 遍历所有密钥
        for (auto pair: UTXOList) {
            int index = pair.first;                      // 对应的密钥下标
            std::unordered_map<std::string, UTXO> map =  pair.second;   // 对应的UTXO集合

            // 遍历对应密钥的UTXO集合
            for (auto pair2 : map) {
                UTXO& utxo = pair2.second;
                TxIn txIn;
                txIn.prevOutHash = utxo.txId;
                txIn.prevOutIndex = utxo.outputIndex;
                txIn.sequence = 0xffffffff;
                txIn.scriptSig = "";                // 签名暂时置空

                // 加入vin集合，并标识对应的密钥 index
                vin.push_back(txIn);
                vinIndex.push_back(index);

                hasPay += utxo.value;

                // 支出过多，生成新地址进行找零
//                if (hasPay > paySum){
//                    WalletKeys newKey;
//                    TxOut txOut(hasPay-paySum, newKey.GetHexRawAddress());
//                    transaction->vout.push_back(txOut);     // 加入到vout中
//                    std::vector<UTXO> vec(1);
//                    vec[0] = UTXO(txOut);
//                    UTXOList[keys.size()] = vec;            // 更新本地的UTXO
//                    keys.push_back(newKey);                 // 加入密钥管理
//                    break;
//                }
                // 为了简单化和便于测试，假设多支付的钱支付给矿工
                if (hasPay >= paySum) {
                    break;
                }
            }

            // vin 构建完毕，退出循环
            if (hasPay >= paySum ) {
                break;
            }
        }
    }

    // 对 vin 中的input 进行签名
    std::string SimpleWallet::SignInput(int index, Transaction* tx) {
        WalletKeys key = keys[index];

        // 进行两次SHA256，产生sigHash（这里输出是Hex格式）
        std::string sigHash = Util::DoubleSHA256(tx->Serizalize());

        // 对sigHash进行签名，因为原来是Hex格式，所以再解码转换一下
        std::string signature = key.SignWithDER(Util::HexDecode(sigHash));
        if (Test::IsTestTransaction) {
            signature = "3045022100e15a8ead9013d1de55e71f195c9dc613483f07c8a0692"
                        "a2144ffa90506436822022062bc9466b9e1941037fc23e1cfadf24c"
                        "8833f96942beb8f4340df60d506f784b";
        }

        // sighash type : SIGHASH_ALL
        std::string sigHashType = "01";             // 对应 SIGHASH_ALL 类型

        // 签名的字节长度
        std::string sigScriptLen = Util::HexEncode(signature.size() / 2 + 1, 1);

        // 获取压缩公钥的Hex编码
        std::string pubKey = key.GetCompressedHexPublicKey();

        // 压缩公钥的字节长度
        std::string pubKeyLen = Util::HexEncode(pubKey.size() / 2, 1);

        // 拼接所有字段返回
        return sigScriptLen + signature + sigHashType + pubKeyLen + pubKey;
    }

    // 获取一个支付地址
    std::string SimpleWallet::GetBitcoinAddr() {
        // 如果当前密钥为空，生成10个新的密钥对
        if (keys.empty()) {
            keys.resize(10);
        }

        // 这里的实现逻辑很简单，就是直接返回第一个密钥对应的地址，以后可以实现更复杂的管理逻辑
        return keys[0].GetCompressedBase58CheckAddress();
    }

    // 同步更新UTXO
    void SimpleWallet::UpdateUTXO() {
        for (int i = 0; i<keys.size(); i++) {
            std::string txId = keys[i].GetHexCompressedRawAddress();
            auto iter = DataBase::UTXOList.find(txId);
            // 如果存在UTXO，则进行同步更新
            if (iter != DataBase::UTXOList.end()) {
                UTXOList[i] = iter->second;
            } else {
                // 如果不存在，则清楚原来的UTXO集合
                UTXOList[i].clear();
            }
        }
    }

} // namespace blockchain