#include <Core/SSL/RSA.h>

using namespace Upp;

static const String PrivateKey =
R"(-----BEGIN PRIVATE KEY-----
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
uzzK4dHQ/EzuAnhodKY0bN04Uqc4nw==
-----END PRIVATE KEY-----)";

static const String PublicKey =
R"(-----BEGIN RSA PUBLIC KEY-----
MIICCgKCAgEAnwynqiITmJ2Ce/X5IVViY/qn1OnX+RLahPZhwcF6jDIv+PQyopRt
EoLdALlUNYfCqlnfdxY5PgiD1VnvZVU/vHlPPpYorJxmB+QjlVNL5pU4oBoJm4fS
OonUB1mpV/yCgVB58Q1hhS9EZsxY12UcHs/MluJCKHFQoaKuqJFilviYnLlyomAx
jL47s65cuaukPwi6xi4rX05Y2i8TV5mC7LR6rvmSl3OJBK6kNjLewCh19UUGt7Y/
v0riOCap7cUBojRvtCJM8gyX6KJHQoEolPYd9fkv2XTu5XrUfzjJiYJUhybMpf5P
feH1cio6efwtC/ZHcCmkGMdCuHU9VPURPFTCaFO7G7vTI+JFlt6TYaEF/Rad1jMQ
N5mky92viAYjtO5Plc0S9zec0aubw5CJEnZom0ebPVq8HDnu0CG8xQD9Y6CqPGNC
bwlbISjw7F9AwSNfAUaFDZqFyhCCXfGOpKXCivoSKp09pK4UCgrMbTWJtkxTDm/L
8aeUfadTDz9gwq9xe+7Ay3/1mehwbZ2ifuDvUE+J5juYPun8En0vgCe2Io56mciB
Fb2xssDFdcL95It4forEamwBVQmkvK6gPEPl+fti+LrlGsi5eAF2eBnVpBkAnjIJ
XFuZd1c2EdJnctB6B4kRx8ExKqrGgktPU82LYhJb+gB45YnDle+7t2ECAwEAAQ==
-----END RSA PUBLIC KEY-----)";

static void LoadSignVerify()
{
	// load keys
	RSAPrivateKey priv(PrivateKey);
	ASSERT(priv);

	RSAPublicKey pub(PublicKey);
	ASSERT(pub);

	// sign message
	String message = "hello world";
	String signature = priv.SignRS256(message);

	// verify signature
	ASSERT(pub.VerifyRS256(message, signature));

	// modify message and fail verify
	String message2;
	message2 << message << "x";
	ASSERT(!pub.VerifyRS256(message2, signature));
}

static void GenerateSignVerify()
{
	// generate key
	RSAPrivateKey priv;
	ASSERT(!priv);
	priv.Generate(2048);
	ASSERT(priv);
	// sign message
	String message = "hello world again";
	String signature = priv.SignRS256(message);
	// extract public key and verfify
	RSAPublicKey pub = priv.GetPublicKey();
	ASSERT(pub);
	ASSERT(pub.VerifyRS256(message, signature));
}

static void GenerateExportLoadSignVerify()
{
	// generate key
	RSAPrivateKey priv;
	ASSERT(!priv);
	priv.Generate(2048);
	ASSERT(priv);
	// extract public key
	RSAPublicKey pub = priv.GetPublicKey();
	ASSERT(pub);
	// get pem string keys
	String privPem = priv.GetKey();
	ASSERT(!privPem.IsVoid());
	String pubPem = pub.GetKey();
	ASSERT(!pubPem.IsVoid());
	ASSERT(privPem != pubPem);
	// load pem strings into new keys
	RSAPrivateKey priv2(privPem);
	ASSERT(priv2);
	RSAPublicKey pub2(pubPem);
	ASSERT(pub2);
	// use them to sign/verify message
	String message = "hello world again again";
	String signature = priv2.SignRS256(message);
	ASSERT(pub2.VerifyRS256(message, signature));
}

CONSOLE_APP_MAIN
{
	LoadSignVerify();
	GenerateSignVerify();
	GenerateExportLoadSignVerify();
}
