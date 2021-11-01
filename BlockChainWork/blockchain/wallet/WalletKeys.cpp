//
// Created by 张宇轩 on 2021/10/27.
//

#include "WalletKeys.h"
#include "util.h"
namespace blockchain {
    // 为了避免模块耦合，把格式之间转换逻辑的实现放在了Util类中

    WalletKeys::WalletKeys() {
        GenAddr();
    }
    WalletKeys::WalletKeys(std::string exponent) : keys(exponent) {
        GenAddr();
    }

    void WalletKeys::GenAddr() {
        address = Util::RIPEMD160(                    // 3. 进行RIPEMD160 哈希
                  Util::SHA256(                 // 2. 在进行SHA256 哈希
                        Util::HexDecode(        // 1. 先对进行hex解码
                                GetHexPublicKey())));
        compressedAddress = Util::RIPEMD160(
                Util::SHA256(
                        Util::HexDecode(
                                GetCompressedHexPublicKey())));
    }

    // 获取字节形式的密钥
    std::string WalletKeys::GetRawPrivateKey() {
        return keys.GetPrivateKey();
    }

    // 获取Hex编码形式的密钥
    std::string WalletKeys::GetHexPrivateKey() {
        return Util::HexEncode(keys.GetPrivateKey());
    }

    // 获取WIF格式的私钥
    std::string WalletKeys::GetWIFPrivateKey() {
        std::string prefix;         // WIF格式私钥的版本前缀
        prefix.resize(1);
        prefix[0] = 128;            // 对应0x80
        return Util::EncodeBase58Check(prefix + keys.GetPrivateKey());
    }

    // 获取WIF-Compressed 格式的私钥
    std::string WalletKeys::GetWIFCompressedPrivateKey() {
        std::string prefix;         // WIF格式私钥的版本前缀
        prefix.resize(1);
        prefix[0] = 128;            // 对应0x80
        std::string suffix;         // WIF-Compressed格式私钥的后缀
        suffix.resize(1);
        suffix[0] = 1;              // 对应0x01
        return Util::EncodeBase58Check(prefix + keys.GetPrivateKey() + suffix);
    }

    // 获取字节形式公钥（64字节）
    std::string WalletKeys::GetRawPublicKey() {
        return keys.GetPublicKey();
    }

    // 获取Hex编码格式公钥（带上前缀04）
    std::string WalletKeys::GetHexPublicKey() {
        std::string hex = Util::HexEncode(keys.GetPublicKey());
        return "04" + hex;              // 带上前缀0x04，表示未压缩公钥
    }

    // 获取压缩格式公钥（前缀02或03）
    std::string WalletKeys::GetCompressedHexPublicKey() {
        // 截取一半，获取x坐标对应字节
        std::string hex = Util::HexEncode(keys.GetPublicKey().substr(0, 32));
        std::string prefix;
        if (keys.QyIsEven()) {
            prefix = "02";              // 02表示 y坐标为偶数
        } else {
            prefix = "03";              // 03表示 y坐标为奇数
        }
        return prefix + hex;
    }

    // 获取使用非压缩公钥产生的比特币地址（字节形式）
    std::string WalletKeys::GetRawAddress() {
        return address;
    }

    // 获取使用非压缩公钥产生的比特币地址（Hex 编码）
    std::string WalletKeys::GetHexRawAddress() {
        return Util::HexEncode(address);
    }

    // 获取使用非压缩公钥产生的比特币地址（Base58check 编码）
    std::string WalletKeys::GetBase58CheckAddress() {
        std::string prefix;
        prefix.resize(1);
        prefix[0] = 0;              // 前缀0x00，对应比特币地址前缀
        return Util::EncodeBase58Check(prefix + address);
    }

    // 获取使用压缩公钥产生的比特币地址（字节形式）
    std::string WalletKeys::GetCompressedRawAddress() {
        return compressedAddress;
    }

    // 获取使用压缩公钥产生的比特币地址（Hex形式）
    std::string WalletKeys::GetHexCompressedRawAddress() {
        return Util::HexEncode(compressedAddress);
    }

    // 获取使用压缩公钥产生的比特币地址（Base58check 编码）
    std::string WalletKeys::GetCompressedBase58CheckAddress() {
        std::string prefix;
        prefix.resize(1);
        prefix[0] = 0;              // 前缀0x00，对应比特币地址前缀
        return Util::EncodeBase58Check(prefix + compressedAddress);
    }

    // 获取测试网压缩公钥产生的比特币地址（Base58check 编码）
    std::string WalletKeys::GetTestCompBase58CheckAddress() {
        std::string prefix;
        prefix.resize(1);
        prefix[0] = 0x6f;              // 前缀0x6f，对应比特币测试地址前缀
        return Util::EncodeBase58Check(prefix + compressedAddress);
    }

    // 解码比特币地址，返回Hex编码
    std::string WalletKeys::DecodeBase58check(std::string base58Addr) {
        // 去掉第一个字节版本签前缀
        std::string addr = Util::DecodeBase58Check(base58Addr).substr(1);
       return Util::HexEncode(addr);
    }

    // 进行签名，字节形式（R值和S值的拼接）
    std::string WalletKeys::Sign(const std::string& message) {
        return keys.Sign(message);
    }

    // 签名结果以DER编码形式返回
    std::string WalletKeys::SignWithDER(const std::string& message) {
        return keys.SignWithDER(message);
    }

    // 验证签名，签名需要是原生的拼接格式，而不是DER格式
    bool WalletKeys::VerifyMessage(
            const std::string& message, const std::string& signature) {
        return keys.VerifyMessage(message, signature);
    }

} // namespace blockchain