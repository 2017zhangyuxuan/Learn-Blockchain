//
// Created by 张宇轩 on 2021/10/29.
//

#include "Wallet.h"
#include "WalletKeys.h"
namespace blockchain {
    // 进行转账交易，输入的地址用base58表示
    Transaction* Wallet::SendMoneyWithBase58(std::vector<unsigned long long> value,
                                             std::vector<std::string> address) {
        for (int i = 0; i< address.size(); i++) {
            address[i] = WalletKeys::DecodeBase58check(address[i]);
        }
        return SendMoney(value, address);
    }
} // namespace blockchain