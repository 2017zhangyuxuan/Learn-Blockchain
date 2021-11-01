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
    FileSink fs1( "../../../keys/private.ec.der", true /*binary*/ );
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
    FileSink fs2( "../../../keys/public.ec.der", true /*binary*/ );
    publicKey.Save( fs2 );

}

void TestCrypto::ECDSA_LOAD(){
    AutoSeededRandomPool prng;
    FileSource fs1( "../../../keys/private.ec.der", true /*pump all*/ );
    FileSource fs2( "../../../keys/public.ec.der", true /*pump all*/ );
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
