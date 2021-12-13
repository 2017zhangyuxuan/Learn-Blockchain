//
// Created by 张宇轩 on 2021/10/31.
//

#include "TestCrypto.h"
#include<integer.h>//使用Integer
#include<iostream>//使用cout、cin
#include<osrng.h>//使用AutoSeededRandomPool
#include<eccrypto.h>
#include <oids.h>
#include <files.h>
#include <filters.h>
#include <filesystem>
#include <hex.h>
#include <base32.h>
#include <ripemd.h>
#include <dsa.h>
#include <wallet/WalletKeys.h>
#include <sm3.h>
#include <SM2.h>
#include "test.h"
#include "ECDSAKeys.h"
#include "util.h"

using namespace std;
using namespace CryptoPP;//CryptoPP是CryptoPP库的命名空间

// ----------------------------   下面是第三库的使用测试  ——————————————————————————————

// 测试交易ID hash 生成
void TestCrypto::Test_Hash(){
    // 验证这笔交易hash的产生
    // https://www.blockchain.com/btc/tx/0627052b6f28912f2703066a912ea577f2ce4da4caa5a5fbd8a57286c345c2f2
    SHA256 sha1,sha2; //定义一个SHA256的类对象
    byte msg[] = "0100000001186f9f998a5aa6f048e51dd8419a14d8a0f1a8a2836dd734d2804fe65fa35779000000008b483045022100884d142d86652a3f47ba4746ec719bbfbd040a570b1deccbb6498c75c4ae24cb02204b9f039ff08df09cbe9f6addac960298cad530a863ea8f53982c09db8f6e381301410484ecc0d46f1918b30928fa0e4ed99f16a0fb4fde0735e7ade8416ab9fe423cc5412336376789d172787ec3457eee41c04f4938de5cc17b4a10fa336a8d752adfffffffff0260e31600000000001976a914ab68025513c3dbd2f7b92a94e0581f5d50f654e788acd0ef8000000000001976a9147f9b1a7fb68d60c536c2fd8aeaa53a8f3cc025a888ac00000000";

    // 使用pipeline范式
    SecByteBlock tmp(msg, sizeof(msg)-1);
    string r;
    StringSource s1 (tmp, tmp.size(),true,
                     new HexDecoder(new HashFilter(sha1,new HashFilter(sha2,
                                                                       new HexEncoder(
                                                                               new StringSink(r)))))
    );
    cout<< "len:" << r.size() << " " << r << endl;

    // 最后展示要用大端展示（之前运算都是用小端模式）
    cout << Util::ReverseByteOrder(r) << endl;
}

void TestCrypto::ECDSA_Generate()
{
    /*
    // 指定 private exponent  32字节随机数，生成对应的私钥
    string exp = "E4A6CFB431471CFCAE491FD566D19C87082CF9FA7722D7FA24B2B3F5669DBEFB";

    HexDecoder decoder;
    decoder.Put((byte*)&exp[0], exp.size());
    decoder.MessageEnd();

    Integer x;
    x.Decode(decoder, decoder.MaxRetrievable());

    privateKey.Initialize(ASN1::secp256r1(), x);
     */

    AutoSeededRandomPool prng;
    ECDSA<ECP, SHA256>::PrivateKey privateKey;

    privateKey.Initialize( prng, ASN1::secp256k1() );

    /* 使用ByteQueue 方便将公私钥存储在内存中，如果要持久化到磁盘，可以使用FileSink
    ByteQueue queue;
    privateKey.Save(queue);
    privateKey.Load(queue);
    */

    // 验证密钥强度
    bool result = privateKey.Validate( prng, 3 );
    cout << boolalpha << result << endl;

    Integer p = privateKey.GetPrivateExponent();
    cout<< "len:" << p.BitCount() << " Private Key:" << p << endl;
    string priHexKey;
    HexEncoder encoder(new StringSink(priHexKey));
    p.Encode(encoder, 32);
    cout << "priHexKey:"<<priHexKey.size() << "  " << priHexKey << endl;

    // Save private key in PKCS #8 format
    FileSink fs1( "../../keys/private.ec.der", true /*binary*/ );
    privateKey.Save( fs1 );

    // Generate publicKey
    ECDSA<ECP, CryptoPP::SHA256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);
    const ECP::Point& q = publicKey.GetPublicElement();
    const Integer& qx = q.x;
    const Integer& qy = q.y;
    string qxHex, qyHex;
    HexEncoder encoderx(new StringSink(qxHex));
    HexEncoder encodery(new StringSink(qyHex));
    qx.Encode(encoderx, 32);
    qy.Encode(encodery, 32);
    cout << "len:" << qx.BitCount() << " Public Point x:" << qx << endl;
    cout << "qxHex:" << qxHex << endl;
    cout << "len:" << qy.BitCount() << " Public Point y:" << qy << endl;
    cout << "qyHex:" << qyHex << endl;

    // Save public key in X.509 format
    FileSink fs2( "../../keys/public.ec.der", true /*binary*/ );
    publicKey.Save( fs2 );

}

void TestCrypto::ECDSA_LOAD(){
    AutoSeededRandomPool prng;
    FileSource fs1( "../../keys/private.ec.der", true /*pump all*/ );
    FileSource fs2( "../../keys/public.ec.der", true /*pump all*/ );
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    // 加载私钥， 私钥格式：PKCS #8
    privateKey.Load( fs1 );
    publicKey.Load(fs2);

    // 用私钥进行签名
    ECDSA<ECP, SHA256>::Signer signer(privateKey);
    string message = "Yoda said, Do or do not. There is no try.";
    string signature;


    StringSource s( message, true /*pump all*/,
                    new SignerFilter( prng,
                                      signer,
                                      new StringSink( signature )
                    ) // SignerFilter
    ); // StringSource
    cout << "signature len:" << signature.size() << endl;
    string hexSign;
    StringSource toHexoSign(signature, true, new HexEncoder(new StringSink(hexSign)));
    cout << "hex signature:" << hexSign << endl;

    // 将签名（包含R，S）转换成 DER 格式
    std::string derSign;
    // Make room for the ASN.1/DER encoding
    derSign.resize(3+3+3+2+signature.size());
    size_t converted_size = DSAConvertSignatureFormat(
            (byte*) (&derSign[0]), derSign.size(), DSA_DER,
            (const byte*) (signature.data()), signature.size(), DSA_P1363);
    derSign.resize(converted_size);
    cout << "DER len:" << derSign.size() << endl;
    string hexDER;
    StringSource toDER(derSign, true, new HexEncoder(new StringSink(hexDER)));
    cout << "DER hex:" << hexDER << endl;


    // 进行验证
    bool result;
    ECDSA<ECP, SHA256>::Verifier verifier(publicKey);
    StringSource ss( signature+message, true /*pump all*/,
                     new SignatureVerificationFilter(
                             verifier,
                             new ArraySink( (byte*)&result, sizeof(result) )
                     ) // SignatureVerificationFilter
    );

    // 传统的C 方式 - 函数调用形式
//    result = verifier.VerifyMessage( (const byte*)message.data(), message.size(), (const byte*)signature.data(), signature.size() );
    if(result)
        std::cout << "Verified signature on message" << std::endl;
    else
        std::cerr << "Failed to verify signature on message" << std::endl;

}


void TestCrypto::Test_SimulateSign() {
    // 先模拟签名过程
    Integer k,e,r,s,a=0,b=7;
    k = Util::GetRandomInteger(4);          // 临时私钥
    // e 是消息散列
    e = Util::StringToInteger("9CE3A97A43618E606AD1FCD7926DE493E69E58CC0DD3139183A4E337F8E81A41");
    Integer prime = Util::StringToInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");

    ECP secp256k1(prime, a, b);
    Integer xg = Util::StringToInteger("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
    Integer yg = Util::StringToInteger("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
    ECP::Point G(xg,yg);        // 生成元
    Integer n = Util::StringToInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    Integer d = Util::StringToInteger("E4A6CFB431471CFCAE491FD566D19C87082CF9FA7722D7FA24B2B3F5669DBEFB");      //私钥
    ECP::Point Q = secp256k1.Multiply(d,G); //公钥

    cout << "——————————椭圆曲线参数——————————"<< endl;
    cout << "p:" << prime << endl;
    cout << "a:" << a << endl;
    cout << "b:" << b << endl;
    cout << "生成点G:" << endl;
    cout << "G.x:" << xg << endl;
    cout << "G.y:" << yg <<endl;
    cout << "n:" << n << endl;
    cout << "私钥d:" << d << endl;
    cout << "公钥Q:" << endl;
    cout << "Q.x:" << IntToString(Q.x) << endl;
    cout << "Q.y:" << IntToString(Q.y) << endl;


    // 签名算法
    ECP::Point R = secp256k1.Multiply(k, G);
    r = R.x.Modulo(n);
    Integer k_1 = k.InverseMod(n);
    s = (k_1 * (d * r + e)).Modulo(n);
    cout << "——————————签名过程——————————" << endl;
    cout << "r:" << r << endl;
    cout << "s:" << s << endl;

    // 验证签名
    Integer s_1 = s.InverseMod(n);
    Integer u_1 = (e * s_1).Modulo(n);
    Integer u_2 = (r * s_1).Modulo(n);
    ECP::Point tmp1 = secp256k1.Multiply(u_1,G);
    ECP::Point tmp2 = secp256k1.Multiply(u_2,Q);
    ECP::Point X = secp256k1.Add(tmp1, tmp2);
    cout << "——————————验证过程——————————" << endl;
    cout << "签名中的r:" <<r % n << endl;
    cout << "计算得到的x:"<<X.x % n<< endl;


    // 公钥恢复
    bool isOdd = R.y.IsOdd();
    Integer r_1 = r.InverseMod(n);
    Integer y_2 = (r * r * r + 7) ;

    // 模素数平方根的求解: 当  p = 4u+3, g = y^2, y= g^(u+1) % p
    Integer u = (prime-3).DividedBy(4);
    Integer y = a_exp_b_mod_c(y_2,u+1,prime);      // 返回  a^b % c

    if ((isOdd && y.IsEven()) || (!isOdd && y.IsOdd())) {       // 如果与原来的y坐标奇偶性相反
        y = prime - y;
    }

    ECP::Point tmpR(r,y);
    ECP::Point tmp3 = secp256k1.Multiply(s,tmpR);
    ECP::Point tmp4 = secp256k1.Multiply(e,G);
    tmp3 = secp256k1.Subtract(tmp3,tmp4);
    tmp4 = secp256k1.Multiply(r_1,tmp3);

    cout << "——————————公钥恢复——————————" << endl;
    cout << "计算的x:" << tmp4.x << endl;
    cout << "公钥Q.x:" <<Q.x << endl;
    cout << "计算的y:"<< tmp4.y << endl;
    cout << "公钥Q.y:"<<Q.y << endl;
}

// 测试 SM2 类
void TestCrypto::Test_SM2() {
    SM2 sm2;
    std::string message = "message digest";
    std::string signature;
    Integer k = Util::StringToInteger("59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21");
    sm2.Sign(message, signature, k);

    Integer r = Util::StringToInteger(Util::HexEncode(signature.substr(0,32)));
    Integer s = Util::StringToInteger(Util::HexEncode(signature.substr(32,32)));

    // 打印输出，对照真实数据
    cout << "——————————————测试SM2 签名算法——————————————" << endl;
    cout << "真实 r:" << "F5A03B0648D2C4630EEAC513E1BB81A15944DA3827D5B74143AC7EACEEE720B3" << endl;
    cout << "计算 r:" << Util::HexEncode(signature.substr(0,32)) << endl;
    cout << "真实 s:" << "B1B6AA29DF212FD8763182BC0D421CA1BB9038FD1F7F42D4840B69C485BBC1AA" << endl;
    cout << "计算 s:" << Util::HexEncode(signature.substr(32,32)) << endl;

    cout << "——————————————测试SM2 验证签名——————————————" << endl;
    cout << "message: " << message << endl;
    cout << "r: " << Util::IntegerToString(r) << endl;
    cout << "s: " << Util::IntegerToString(s) << endl;
    cout << "签名验证结果: " << boolalpha << sm2.Verify(message, signature ) << endl;


    cout << "——————————————测试SM2 公钥恢复算法——————————————" << endl;
    std::string pubKey;
    sm2.RecoverPublicKey(message,false, r, s,pubKey);
    ECP::Point P(Util::StringToInteger(Util::HexEncode(pubKey.substr(0,32))),
                 Util::StringToInteger(Util::HexEncode(pubKey.substr(32,32))));
    cout << "真实公钥 P.x:" << "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020" << endl;
    cout << "恢复公钥 P.x:" << Util::IntegerToString(P.x) << endl;
    cout << "真实公钥 P.y:" << "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13" << endl;
    cout << "恢复公钥 P.y:" << Util::IntegerToString(P.y) << endl;


}
