//
// Created by 张宇轩 on 2021/12/13.
//

#ifndef BLOCKCHAINWORK_SM2_H
#define BLOCKCHAINWORK_SM2_H

#include <string>

namespace CryptoPP {
    // 声明类，头文件在CPP文件中引入
    class Integer;
    class ECP;
    class ECPPoint;

    // 借助CryptoPP开源库 实现 SM2算法
    class SM2 {
    public:
        // 签名：输入的消息和输出的签名数据  均为字节格式，签名是由(r,s)各32字节拼接得到的
        void Sign(const std::string &message, std::string &signature);

        // 指定临时私钥k值， 测试用
        void Sign(const std::string &message, std::string &signature, Integer k);

        // 验证：输入的消息和签名均为字节格式，输出为bool值
        bool Verify(const std::string &message, const std::string &signature);

        // 公钥恢复算法，输入消息，签名（r,s），y值是否为奇数， 输出pubKey 字节格式，由(x,y) 各32字节拼接而成
        void RecoverPublicKey(const std::string &message, bool isOdd,
                                     const Integer &r, const Integer &s, std::string &pubKey);

        // 设置私钥
        void SetPrivateKey(Integer d);

        // 设置生成元 与 对应的阶数
        void SetGenerator(Integer xg, Integer yg, Integer n);

        // 默认构造函数，使用官方参数
        SM2();

        // 定制椭圆曲线参数
        SM2(Integer p, Integer a, Integer b);

        // TODO: SM2算法 - 密钥交换协议
        // TODO: SM2算法的加密与解密

    private:

        std::shared_ptr<ECP> ecp;                   // SM2 对应的椭圆曲线
        std::shared_ptr<Integer> p;
        std::shared_ptr<Integer> a;
        std::shared_ptr<Integer> b;
        std::shared_ptr<Integer> privateKey;        // 私钥
        std::shared_ptr<ECPPoint> publicKey;        // 公钥，以Point表示
        std::shared_ptr<ECPPoint> generator;        // 生成元 点
        std::shared_ptr<Integer> n;                 // 生成元 对应的阶数
        std::string ZA;                             // 杂凑值，字节形式 ：ZA = H256(ENTL_A ∥ ID_A ∥ a ∥ b ∥ xG ∥ yG ∥ xA ∥ yA)

        // 初始化曲线参数，使用官方参数
        void init();

        // 计算杂凑值 ZA
        void ComputeZA();

    };
}


#endif //BLOCKCHAINWORK_SM2_H
